//
// Created by Chester Sim on 6/9/20.
//

#include "PreProcessor.h"

#include <iostream>

#include "../lexer/Lexer.h"

StringPair splitByFirstDelimiter(String str, char c);
StringVector splitByFirstConsecutiveWhitespace(String str);
Boolean containsOpenParentheses(String str);
Boolean containsCloseParenthesesAsLastChar(String str);

// TODO: Every class that can be invalid to inherit from an Error class
// Todo: Error class to have `hasError` member and `isInvalid` member function

AbstractQuery PreProcessor::processQuery(String query)
{
    // Split between declarations and clauses
    StringList* splitBySelectList = splitByDelimiter(query, "Select");
    // Validate both strings exists.
    if (splitBySelectList->size() != 2) {
        return AbstractQuery::invalidAbstractQuery();
    }
    String declarationString = trimWhitespace(*(splitBySelectList->at(0)));
    String synonymAndClausesString = trimWhitespace(*(splitBySelectList->at(1)));

    // Process declarations into declaration table
    DeclarationTable declarationTable = processDeclarations(declarationString);
    if (declarationTable.isInvalid()) {
        return AbstractQuery::invalidAbstractQuery();
    }
    this->declarationTable = declarationTable;

    // Extract select synonym
    StringVector synonymAndClausesVector = splitByFirstConsecutiveWhitespace(synonymAndClausesString);
    // TODO: consider to split clauses string by whitespace, remove first element (synonym), use rest of the elements as
    // argument for processClauses Select without any synonym or clauses
    if (synonymAndClausesVector.size() == 0) {
        return AbstractQuery::invalidAbstractQuery();
    }

    Synonym selectSynonym = synonymAndClausesVector.at(0);
    if (!isValidSynonym(selectSynonym)) {
        return AbstractQuery::invalidAbstractQuery();
    }

    // Only select a synonym, with no other clauses
    if (synonymAndClausesVector.size() == 1) {
        AbstractQuery abstractQuery(selectSynonym, declarationTable, *(new ClauseVector()));
        return abstractQuery;
    }

    // Process clauses into ClauseList
    String clausesString = synonymAndClausesVector.at(1);
    ClauseVector clausesVector = processClauses(clausesString);
    if (clausesVector.isInvalid()) {
        return AbstractQuery::invalidAbstractQuery();
    }

    // Encapsulate into AbstractQuery and return

    AbstractQuery aq(selectSynonym, declarationTable, clausesVector);
    return aq;
}

ClauseVector PreProcessor::processClauses(String clausesString)
{
    ClauseVector clauseVector;
    String* currentClauseConstraint = new String();
    Boolean hasCurrentClause = false;
    Boolean isPreviousTokenSuch = false; // is the previous token "such"
    ClauseType currentClauseType;
    Boolean hasOpenParentheses = false;
    // Syntactically incorrect if number of tokens after clause identifier
    // that does not have open parentheses >= 2
    int numOfTokensWithoutOpenParentheses = 0;

    StringList* splitByWhitespaceList = splitByWhitespace(clausesString);

    for (auto& token : *splitByWhitespaceList) {
        if (!hasCurrentClause) {
            if (*token != "such" && *token != "pattern") {
                return ClauseVector::invalidClauseVector();
            }

            if (*token == "such") {
                isPreviousTokenSuch = true;
            } else if (*token == "pattern") {
                currentClauseType = PatternClauseType;
            }

            hasCurrentClause = true;
        } else {
            if (isPreviousTokenSuch) {
                if (*token != "that") {
                    return ClauseVector::invalidClauseVector();
                }

                isPreviousTokenSuch = false;
                currentClauseType = SuchThatClauseType;
            } else {
                currentClauseConstraint->append(*token);

                if (!hasOpenParentheses) {
                    if (containsOpenParentheses(*token)) {
                        hasOpenParentheses = true;
                    } else {
                        numOfTokensWithoutOpenParentheses++;

                        if (numOfTokensWithoutOpenParentheses >= 2) {
                            // At most 2 tokens after clause identifier should
                            return ClauseVector::invalidClauseVector();
                        }
                    }
                }

                // Important to still check for close parentheses for
                // tokens with open parentheses already e.g. "Follows(s,a)"
                if (containsCloseParenthesesAsLastChar(*token)) {
                    if (!hasOpenParentheses) {
                        return ClauseVector::invalidClauseVector();
                    }

                    Clause* clause;

                    if (currentClauseType == SuchThatClauseType) {
                        // create and add such that
                        clause = processSuchThatClause(*currentClauseConstraint);
                    } else { // PatternClause
                        clause = processPatternClause(*currentClauseConstraint);
                    }

                    if (clause->isInvalid()) {
                        return ClauseVector::invalidClauseVector();
                    }

                    clauseVector.add(clause);
                    //                    SuchThatClause& suchThatClause = dynamic_cast<SuchThatClause&>(*clause);

                    hasCurrentClause = false;
                    hasOpenParentheses = false;
                    currentClauseConstraint = new String();
                }
            }
        }
    }

    if (!currentClauseConstraint->empty()) {
        return ClauseVector::invalidClauseVector();
    }

    return clauseVector;
}

Clause* PreProcessor::processSuchThatClause(String clauseConstraint)
{
    StringPair pair = splitByFirstDelimiter(clauseConstraint, '(');

    String relRef = pair.first;
    if (!util::isRelationshipReference(relRef)) {
        return Clause::invalidClause(SuchThatClauseType);
    }

    String references
        = pair.second.substr(0, pair.second.size() - 1); // remove the last char which is a close parentheses
    StringPair refPair = splitByFirstDelimiter(references, ',');
    String leftRefString = refPair.first;
    String rightRefString = refPair.second;

    Reference leftReference = createReference(leftRefString);
    Reference rightReference = createReference(rightRefString);

    if (leftReference.isInvalid() || rightReference.isInvalid()) {
        return Clause::invalidClause(SuchThatClauseType);
    }

    Relationship relationship(relRef, leftReference, rightReference);
    if (relationship.isInvalid()) {
        return Clause::invalidClause(SuchThatClauseType);
    }

    auto* suchThatClause = new SuchThatClause(relationship);
    return suchThatClause;
}

Clause* PreProcessor::processPatternClause(String clauseConstraint) {}

Reference PreProcessor::createReference(String ref)
{
    if (ref == "_" || declarationTable.hasSynonym(ref)) {
        Reference reference(AnyRefType, ref, isSynonymOfProcedureType(ref));
        return reference;
    }

    if (util::isPossibleConstant(ref)) {
        Reference reference(StatementRefType, ref);
        return reference;
    }

    if (util::isLiteralIdent(ref)) {
        Reference reference(EntityRefType, ref);
        return reference;
    }

    return Reference::invalidReference();
}

Boolean PreProcessor::isSynonymOfProcedureType(Synonym s)
{
    return declarationTable.getDesignEntityOfSynonym(s).getType() == ProcedureType;
}

DeclarationTable PreProcessor::processDeclarations(String declarationsString)
{
    DeclarationTable declarationTable;

    StringList* splitByWhiteSpacesList = splitByWhitespace(declarationsString);
    DesignEntity* currentDesignEntityPtr;
    bool hasCurrentDesignEntity = false;

    for (auto& token : *splitByWhiteSpacesList) {
        if (!hasCurrentDesignEntity) {
            currentDesignEntityPtr = new DesignEntity(*token);
            if (currentDesignEntityPtr->getType() == NonExistentType) {
                return DeclarationTable::invalidDeclarationTable();
            }

            hasCurrentDesignEntity = true;
        } else {
            char lastChar = token->at(token->size() - 1);
            if (lastChar == ';' || lastChar == ',') {
                String synonymToken = (*token).substr(0, token->size() - 1);

                if (!isValidSynonym(synonymToken) || declarationTable.hasSynonym(synonymToken)) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                declarationTable.addDeclaration(synonymToken, *currentDesignEntityPtr);

                if (lastChar == ';') {
                    hasCurrentDesignEntity = false;
                }

            } else {
                return DeclarationTable::invalidDeclarationTable();
            }
        }
    }

    return declarationTable;
}

// Utils

/**
 * Returns a pair of Strings, split by the
 * first appearance of the delimiter. If the
 * delimiter does not exist, a pair of empty
 * strings will be returned.
 *
 * @param str String to be split.
 * @param c Delimiter.
 * @return Pair of Strings split by delimiter.
 */
StringPair splitByFirstDelimiter(String str, char c)
{
    StringPair stringVector;
    const char* currentChar = str.c_str();
    String* currentToken = new String();

    // Find first delimiter
    while (*currentChar != c && *currentChar != '\0') {
        currentToken->push_back(*currentChar);
        currentChar++;
    }

    String leftString = *currentToken;
    currentToken = new String();

    if (*currentChar != '\0') {
        currentChar++; // Skips '('
    }

    while (*currentChar != '\0') {
        currentToken->push_back(*currentChar);
        currentChar++;
    }

    return std::make_pair(leftString, *currentToken);
}

/**
 * Splits up the given string by the first
 * consecutive whitespace, into two substrings.
 * Given string should be trimmed.
 *
 * @param str String to be split
 * @return vector of 2 strings
 */
StringVector splitByFirstConsecutiveWhitespace(String str)
{
    const char* currentChar = str.c_str();
    String* currentToken = new String();
    StringVector splitByFirstWhitespaceVector;

    // Find first whitespace
    while (!isWhitespace(currentChar) && *currentChar != '\0') {
        currentToken->push_back(*currentChar);
        currentChar++;
    }

    splitByFirstWhitespaceVector.push_back(*currentToken);
    currentToken = new String();

    // Skip past all whitespaces
    while (isWhitespace(currentChar)) {
        currentChar++;
    }

    while (*currentChar != '\0') {
        currentToken->push_back(*currentChar);
        currentChar++;
    }

    splitByFirstWhitespaceVector.push_back(*currentToken);
    return splitByFirstWhitespaceVector;
}

Boolean hasChar(String str, char charToLookFor)
{
    const char* currentChar = str.c_str();
    while (*currentChar != '\0') {
        if (*currentChar == charToLookFor) {
            return true;
        }
        currentChar++;
    }

    return false;
}

Boolean containsOpenParentheses(String str)
{
    return hasChar(str, '(');
}

Boolean containsCloseParenthesesAsLastChar(String str)
{
    return str.at(str.size() - 1) == ')';
}
