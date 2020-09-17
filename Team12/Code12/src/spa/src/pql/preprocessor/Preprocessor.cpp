#include "Preprocessor.h"

#include "frontend/parser/Parser.h"

StringPair splitByFirstDelimiter(String str, char c);
StringVector splitByFirstConsecutiveWhitespace(String str);
Boolean containsOpenParentheses(String str);
Boolean containsCloseParenthesesAsLastChar(String str);
Boolean isEnclosedWith(String str, char leftEnd, char rightEnd);
StringPair splitDeclarationAndSelectClause(String query);

// TODO: Every class that can be invalid to inherit from an Error class
// Todo: Error class to have `hasError` member and `isInvalid` member function

AbstractQuery Preprocessor::processQuery(String query)
{
    StringPair splitQuery = splitDeclarationAndSelectClause(query);

    String declarationString = trimWhitespace(splitQuery.first);
    String synonymAndClausesString = trimWhitespace(splitQuery.second);

    // Process declarations into declaration table
    DeclarationTable declarationTable = processDeclarations(declarationString);
    if (declarationTable.isInvalid()) {
        return AbstractQuery::invalidAbstractQuery();
    }
    this->declarationTable = declarationTable;

    // Extract select synonym
    StringVector selectAndClausesVector = splitByFirstConsecutiveWhitespace(synonymAndClausesString);

    // No select clause
    if (selectAndClausesVector.size() < 2) {
        return AbstractQuery::invalidAbstractQuery();
    }

    String possibleSelectKeyword = selectAndClausesVector.at(0);
    if (possibleSelectKeyword != "Select") {
        return AbstractQuery::invalidAbstractQuery();
    }

    String synonymAndClauses = selectAndClausesVector.at(1);
    StringVector synonymAndClausesVector = splitByFirstConsecutiveWhitespace(synonymAndClauses);

    // No synonym
    if (selectAndClausesVector.size() == 0) {
        return AbstractQuery::invalidAbstractQuery();
    }

    Synonym selectSynonym = synonymAndClausesVector.at(0);
    if (!isValidSynonym(selectSynonym) || !declarationTable.hasSynonym(selectSynonym)) {
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

    AbstractQuery aq(selectSynonym, declarationTable, clausesVector);
    return aq;
}

ClauseVector Preprocessor::processClauses(String clausesString)
{
    ClauseVector clauseVector;
    String* currentClauseConstraint = new String();
    Boolean hasCurrentClause = false;
    Boolean isPreviousTokenSuch = false; // is the previous token "such"
    ClauseType currentClauseType;
    Integer numOfOpenParentheses = 0;

    // Syntactically incorrect if number of tokens after clause identifier
    // that does not have open parentheses >= 2
    int numOfTokensWithoutOpenParentheses = 0;

    StringList* splitByWhitespaceList = splitProgram(clausesString);
    //    StringList* splitByWhitespaceList = splitByWhitespace(clausesString);

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

                if (containsOpenParentheses(*token)) {
                    numOfOpenParentheses++;
                }

                if (numOfOpenParentheses == 0) {
                    if (*token != "*") {
                        numOfTokensWithoutOpenParentheses++;
                    }

                    if (numOfTokensWithoutOpenParentheses >= 2) {
                        // At most 2 tokens after clause identifier should
                        return ClauseVector::invalidClauseVector();
                    }
                }

                // Important to still check for close parentheses for
                // tokens with open parentheses already e.g. "Follows(s,a)"
                if (containsCloseParenthesesAsLastChar(*token)) {
                    numOfOpenParentheses--;

                    if (numOfOpenParentheses < 0) {
                        return ClauseVector::invalidClauseVector();
                    }

                    if (numOfOpenParentheses > 0) {
                        continue;
                    }

                    Clause* clause;

                    if (currentClauseType == SuchThatClauseType) {
                        clause = processSuchThatClause(*currentClauseConstraint);
                    } else { // PatternClause
                        clause = processPatternClause(*currentClauseConstraint);
                    }

                    if (clause->isInvalid()) {
                        return ClauseVector::invalidClauseVector();
                    }

                    clauseVector.add(clause);

                    hasCurrentClause = false;
                    numOfOpenParentheses = 0;
                    numOfTokensWithoutOpenParentheses = 0;
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

Clause* Preprocessor::processSuchThatClause(String clauseConstraint)
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

Clause* Preprocessor::processPatternClause(String clauseConstraint)
{
    StringPair pair = splitByFirstDelimiter(clauseConstraint, '(');

    Synonym patternSynonym = pair.first;
    DesignEntity synonymDesignEntity = declarationTable.getDesignEntityOfSynonym(patternSynonym);
    if (synonymDesignEntity.getType() != AssignType) {
        return Clause::invalidClause(PatternClauseType);
    }

    String constraintVariablesString
        = pair.second.substr(0, pair.second.size() - 1); // remove the last char which is a close parentheses
    StringPair constraintVariablesPair = splitByFirstDelimiter(constraintVariablesString, ',');
    String leftConstraintString = constraintVariablesPair.first;
    String rightConstraintString = constraintVariablesPair.second;

    Reference leftReference = createReference(leftConstraintString);
    if (!leftReference.isValidEntityRef()) { // TODO: Find out other restrictions of the left reference in a pattern
        return Clause::invalidClause(PatternClauseType);
    }

    ExpressionSpec rightExpressionSpec = createExpressionSpec(rightConstraintString);
    if (rightExpressionSpec.isInvalid()) {
        return Clause::invalidClause(PatternClauseType);
    }

    PatternClause* patternClause
        = new PatternClause(patternSynonym, AssignPatternType, leftReference, rightExpressionSpec);
    return patternClause;
}

ExpressionSpec Preprocessor::createExpressionSpec(String ref)
{
    if (ref == "_") {
        ExpressionSpec expressionSpec{WildcardExpressionType};
        return expressionSpec;
    }

    if (isEnclosedWith(ref, '_', '_')) {
        String possibleLiteral = util::removeCharFromBothEnds(ref);
        if (!isEnclosedWith(possibleLiteral, '\"', '\"')) {
            return ExpressionSpec::invalidExpressionSpec();
        }

        Expression* expression = createExpression(util::removeCharFromBothEnds(possibleLiteral));
        if (!expression) {
            return ExpressionSpec::invalidExpressionSpec();
        }

        ExpressionSpec expressionSpec{expression, ExtendableLiteralExpressionType};
        return expressionSpec;
    }

    if (!isEnclosedWith(ref, '\"', '\"')) {
        return ExpressionSpec::invalidExpressionSpec();
    }

    String expressionString = util::removeCharFromBothEnds(ref);
    Expression* expression = createExpression(expressionString);
    if (!expression) {
        return ExpressionSpec::invalidExpressionSpec();
    }

    ExpressionSpec expressionSpec{expression, LiteralExpressionType};
    return expressionSpec;
}

Expression* Preprocessor::createExpression(String literal)
{
    StringList* splitString = splitProgram(literal);
    Expression* expression = parseExpression(splitString);
    return expression;
}

Reference Preprocessor::createReference(String ref)
{
    if (ref == "_") {
        Reference reference(WildcardRefType, ref);
        return reference;
    }

    if (util::isPossibleConstant(ref)) {
        Reference reference(IntegerRefType, ref);
        return reference;
    }

    if (util::isLiteralIdent(ref)) {
        // unquote the string literal
        Reference reference(LiteralRefType, util::removeCharFromBothEnds(ref));
        return reference;
    }

    if (declarationTable.hasSynonym(ref)) {
        Reference reference(SynonymRefType, ref, declarationTable.getDesignEntityOfSynonym(ref));
        return reference;
    }

    return Reference::invalidReference();
}

DeclarationTable Preprocessor::processDeclarations(String declarationsString)
{
    DeclarationTable declarationTable;

    StringList* tokenizedStringList = splitProgram(declarationsString);
    DesignEntity* currentDesignEntityPtr;
    bool hasCurrentDesignEntity = false;
    bool isPreviousTokenASynonym = false;

    for (auto& token : *tokenizedStringList) {
        if (!hasCurrentDesignEntity) {
            currentDesignEntityPtr = new DesignEntity(*token);
            if (currentDesignEntityPtr->getType() == NonExistentType) {
                return DeclarationTable::invalidDeclarationTable();
            }

            hasCurrentDesignEntity = true;
        } else {
            if (*token == ";") {
                if (!isPreviousTokenASynonym) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                hasCurrentDesignEntity = false;
                isPreviousTokenASynonym = false;
                continue;
            } else if (*token == ",") {
                if (!isPreviousTokenASynonym) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                isPreviousTokenASynonym = false;
                continue;
            } else if (isPreviousTokenASynonym) {
                // Syntax error e.g. while w w1;
                return DeclarationTable::invalidDeclarationTable();
            } else {
                if (!isValidSynonym(*token) || declarationTable.hasSynonym(*token)) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                declarationTable.addDeclaration(*token, *currentDesignEntityPtr);
                isPreviousTokenASynonym = true;
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
 * strings will be returned. The particular
 * delimiter will be removed.
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
 * Returns a pair of Strings, split by the
 * last appearance of the a semicolon. If the
 * delimiter does not exist, a pair of empty
 * strings will be returned.
 *
 * @param query String to be split.
 * @return Pair of Strings split by delimiter.
 */
StringPair splitDeclarationAndSelectClause(String query)
{
    StringPair stringVector;
    std::size_t indexOfLastDelimiter = query.find_last_of(';', query.size() - 1);
    if (indexOfLastDelimiter == -1) {
        return std::make_pair("", "");
    }

    String leftToken = query.substr(0, indexOfLastDelimiter + 1);

    if (indexOfLastDelimiter >= query.size()) {
        return std::make_pair(leftToken, "");
    }

    String rightToken = query.substr(indexOfLastDelimiter + 1, query.size() - indexOfLastDelimiter - 1);

    return std::make_pair(leftToken, rightToken);
}

/**
 * Splits up the given string by the first
 * consecutive whitespaces, into two substrings.
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

/**
 * Returns true if a non-empty substring is enclosed
 * by the given left end and right character.
 *
 * @param str String to be checked
 * @param leftEnd Character at left end
 * @param rightEnd Character at right end
 */
Boolean isEnclosedWith(String str, char leftEnd, char rightEnd)
{
    return str[0] == leftEnd && str.size() > 2 && str[str.size() - 1] == rightEnd;
}