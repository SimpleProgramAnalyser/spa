#include "Preprocessor.h"
#include "AqTypesUtils.h"

AbstractQuery Preprocessor::processQuery(const String& query)
{
    // TODO: Improve splitting of Declarations and Select Clauses - iterate every char from the front until ; Select
    StringPair splitQuery = splitDeclarationAndSelectClause(query);

    String declarationString = trimWhitespace(splitQuery.first);
    String synonymAndClausesString = trimWhitespace(splitQuery.second);

    DeclarationTable declarationTable;
    if (!declarationString.empty()) {
        // Process declarations into declaration table
        declarationTable = processDeclarations(declarationString);
        if (declarationTable.isSyntacticallyInvalid()) {
            return AbstractQuery(QuerySyntaxError, declarationTable.getErrorMessage());
        }
    }

    // Extract select keyword
    StringVector selectAndClausesVector = splitByFirstConsecutiveWhitespace(synonymAndClausesString);

    // No select clause
    if (selectAndClausesVector.size() < 2) {
        return AbstractQuery(QuerySyntaxError, "No Select Clause");
    }

    String possibleSelectKeyword = selectAndClausesVector.at(0);
    if (possibleSelectKeyword != "Select") {
        return AbstractQuery(QuerySyntaxError, "No Select keyword");
    }

    String synonymAndClauses = selectAndClausesVector.at(1);
    if (synonymAndClauses.empty()) {
        return AbstractQuery(QuerySyntaxError, "No Select synonym");
    }
    StringVector synonymAndClausesVector = splitResultAndClauses(synonymAndClauses);

    // No synonym
    if (selectAndClausesVector.empty()) {
        return AbstractQuery(QuerySyntaxError, "No Result Synonym");
    }

    String selectResultString = synonymAndClausesVector.at(0);

    ResultSynonymVector resultSynonymVector = processSelectResultString(selectResultString, declarationTable);

    if (resultSynonymVector.isInvalid()) {
        return AbstractQuery(resultSynonymVector.getErrorType(), resultSynonymVector.getErrorMessage());
    }

    // Return false if Declarations is semantically invalid and Select BOOLEAN
    if (declarationTable.isSemanticallyInvalid()) {
        return AbstractQuery(QuerySemanticsError, declarationTable.getErrorMessage(),
                             resultSynonymVector.isSelectBoolean());
    }

    // Only select a synonym, with no other clauses
    if (synonymAndClausesVector.size() == 1) {
        AbstractQuery abstractQuery(resultSynonymVector, declarationTable);
        return abstractQuery;
    }

    // Process clauses into ClauseList
    String clausesString = synonymAndClausesVector.at(1);
    ClauseVector clausesVector = processClauses(clausesString, declarationTable);
    if (clausesVector.isSyntacticallyInvalid()) {
        return AbstractQuery(QuerySyntaxError, clausesVector.getErrorMessage());
    } else if (clausesVector.isSemanticallyInvalid()) {
        // Return false if Declarations is semantically invalid and Select BOOLEAN
        return AbstractQuery(QuerySemanticsError, clausesVector.getErrorMessage(),
                             resultSynonymVector.isSelectBoolean());
    }

    AbstractQuery abstractQuery(resultSynonymVector, declarationTable, clausesVector);
    return abstractQuery;
}

ResultSynonymVector processSelectResultString(String selectResultString, DeclarationTable& declarationTable)
{
    // BOOLEAN result
    if (selectResultString == "BOOLEAN") {
        if (!declarationTable.hasSynonym(selectResultString)) {
            // Empty Vector means result is of type BOOLEAN
            return ResultSynonymVector();
        }

        return ResultSynonymVector{ResultSynonym{selectResultString}};
    }

    // Single Synonym result
    if (selectResultString.at(0) != '<') {
        ResultSynonym processedResultSynonym = processResultSynonym(selectResultString, declarationTable);
        if (processedResultSynonym.isInvalid()) {
            return ResultSynonymVector(processedResultSynonym.getErrorType(), processedResultSynonym.getErrorMessage());
        }

        return ResultSynonymVector{processedResultSynonym};
    }

    // Tuple result
    String removeTupleString = selectResultString.substr(1);
    StringVector resultSynonymStrings = splitByDelimiter(removeTupleString, ",");
    if (resultSynonymStrings.size() == 0
        || (resultSynonymStrings.size() == 1 && isAllWhitespaces(resultSynonymStrings.at(0)))) {
        return ResultSynonymVector(QuerySyntaxError, "Result Synonym tuple does not have any Synonym");
    }

    ResultSynonymVector resultSynonyms;
    for (auto& resultSynonymString : resultSynonymStrings) {
        String trimmedResultSynonymString = trimWhitespace(resultSynonymString);
        ResultSynonym processedResultSynonym = processResultSynonym(trimmedResultSynonymString, declarationTable);
        if (processedResultSynonym.isInvalid()) {
            return ResultSynonymVector(processedResultSynonym.getErrorType(), processedResultSynonym.getErrorMessage());
        }

        resultSynonyms.add(processedResultSynonym);
    }

    return resultSynonyms;
}

ResultSynonym processResultSynonym(const String& resultSynonymString, DeclarationTable& declarationTable)
{
    StringVector splitByFullStop = splitByDelimiter(resultSynonymString, ".");
    if (splitByFullStop.size() > 2) {
        return ResultSynonym(QuerySyntaxError, "More than one full stop in Synonym " + resultSynonymString);
    }

    // Synonym without attributes
    if (splitByFullStop.size() == 1) {
        if (!isValidSynonym(resultSynonymString)) {
            return ResultSynonym(QuerySyntaxError, ResultSynonym::INVALID_SYNONYM_MESSAGE + resultSynonymString);
        } else if (!declarationTable.hasSynonym(resultSynonymString)) {
            return ResultSynonym(QuerySemanticsError, "Synonym " + resultSynonymString + " is not declared");
        }

        return ResultSynonym(resultSynonymString);
    }

    Synonym synonym = splitByFullStop[0];
    String attribute = splitByFullStop[1];

    if (!isValidSynonym(synonym)) {
        return ResultSynonym(QuerySyntaxError, ResultSynonym::INVALID_SYNONYM_MESSAGE + resultSynonymString);
    } else if (!declarationTable.hasSynonym(synonym)) {
        return ResultSynonym(QuerySemanticsError, "Synonym " + synonym + " is not declared");
    }

    DesignEntity designEntityOfSynonym = declarationTable.getDesignEntityOfSynonym(synonym);
    ResultSynonym resultSynonym(synonym, attribute, designEntityOfSynonym);

    return resultSynonym;
}

StringVector splitResultAndClauses(String& s)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(s.length() > 0);

    char firstChar = s.at(0);

    // Normal single synonym or BOOLEAN
    if (firstChar != '<') {
        StringVector resultAndClauses = splitByFirstConsecutiveWhitespace(s);
        return resultAndClauses;
    }

    StringVector tupleResultAndClauses = splitByDelimiter(s, ">");
    if (tupleResultAndClauses.size() > 2) {
        return StringVector();
    }

    return tupleResultAndClauses;
}

ClauseVector processClauses(const String& clausesString, DeclarationTable& declarationTable)
{
    ClauseVector clauseVector;
    String currentClauseConstraint;
    Boolean hasCurrentClause = false;
    Boolean isPreviousTokenSuch = false; // is the previous token "such"
    ClauseType currentClauseType = NonExistentClauseType;
    Integer numOfOpenedParentheses = 0;
    Boolean hasOpenParentheses = false;
    Boolean isInProcessOfCreatingClause = false;
    Boolean hasEncounteredEqualsOperator = false;
    Boolean hasClosedQuotes = false;
    Boolean isPreviousTokenFullStop = false;

    /**
     * Syntactically incorrect if number of tokens after
     * clause identifier (e.g. such that, pattern), that
     * does not have open parentheses is at least 2.
     *
     * E.g.
     *      Select s such that Follows a (s, _)
     *
     * The Select string will be split as such:
     * "Select", "s", "such", "that", "Follows", "a",
     * "(", ...
     *
     * There are at least two tokens, after the "that"
     * token, that does not contain an open parentheses,
     * which is not possible for any form of valid clause,
     * except for SuchThatClauses that contains Follows*
     * and Parent* relationship as clause constraints.
     */
    int numOfTokensWithoutOpenParentheses = 0;

    StringVector splitStringList = splitProgram(clausesString);
    size_t length = splitStringList.size();
    for (size_t i = 0; i < length; i++) {
        auto& token = splitStringList.at(i);
        if (token == "and" && !hasCurrentClause && currentClauseType != NonExistentClauseType) {
            hasCurrentClause = true;
            isInProcessOfCreatingClause = true;
            continue;
        }

        if (!hasCurrentClause) {
            if (token != "such" && token != "pattern" && token != "with") {
                return ClauseVector(QuerySyntaxError, "Invalid Clause Type: " + token);
            }

            if (token == "such") {
                isPreviousTokenSuch = true;
            } else if (token == "pattern") {
                currentClauseType = PatternClauseType;
            } else if (token == "with") {
                currentClauseType = WithClauseType;
            }

            isInProcessOfCreatingClause = true;
            hasCurrentClause = true;
            continue;
        }

        if (isPreviousTokenSuch) {
            if (token != "that") {
                return ClauseVector(QuerySyntaxError, "Keyword such is not followed by keyword that");
            }

            isPreviousTokenSuch = false;
            currentClauseType = SuchThatClauseType;
            continue;
        }

        currentClauseConstraint.append(token);

        if (currentClauseType == WithClauseType) {
            if (hasEncounteredEqualsOperator) {
                if (token == "\"" && !hasClosedQuotes) {
                    continue;
                }

                if (token == ".") {
                    isPreviousTokenFullStop = true;
                    continue;
                }

                if (isPreviousTokenFullStop) {
                    isPreviousTokenFullStop = false;

                    // Expecting a '#' as the next token
                    if (token == "stmt") {
                        continue;
                    }
                } else if (i + 1 < length && !hasClosedQuotes) {
                    if (splitStringList.at(i + 1) == ".") {
                        continue;
                    }

                    if (splitStringList.at(i + 1) == "\"") {
                        hasClosedQuotes = true;
                        continue;
                    }
                }

                Clause* clause = WithClause::createWithClause(currentClauseConstraint, declarationTable);
                if (clause->isInvalid()) {
                    return ClauseVector(clause->getErrorType(), clause->getErrorMessage());
                }

                clauseVector.add(clause);

                hasEncounteredEqualsOperator = false;
                currentClauseConstraint.clear();
                hasCurrentClause = false;
                isInProcessOfCreatingClause = false;
                hasClosedQuotes = false;
                continue;
            }

            if (token == "=") {
                hasEncounteredEqualsOperator = true;
            }

            continue;
        }

        std::pair<Boolean, Integer> result = countNumOfOpenParentheses(token, numOfOpenedParentheses);
        if (!result.first) {
            return ClauseVector(QuerySyntaxError, "Incorrect number of parentheses");
        }

        numOfOpenedParentheses = result.second;

        if (!hasOpenParentheses) {
            hasOpenParentheses = containsOpenParentheses(token);
        }

        if (numOfOpenedParentheses < 0) {
            return ClauseVector(QuerySyntaxError, "Close parentheses appears before open parentheses");
        } else if (numOfOpenedParentheses == 0) {
            if (!hasOpenParentheses) {
                if (token != "*") {
                    numOfTokensWithoutOpenParentheses++;
                }

                if (numOfTokensWithoutOpenParentheses >= 2) {
                    return ClauseVector(QuerySyntaxError,
                                        "Too many tokens after parsing Relationship: " + currentClauseConstraint);
                }
            } else {
                Clause* clause;

                if (currentClauseType == SuchThatClauseType) {
                    clause = SuchThatClause::createSuchThatClause(currentClauseConstraint, declarationTable);
                } else { // PatternClause
                    clause = PatternClause::createPatternClause(currentClauseConstraint, declarationTable);
                }

                if (clause->isInvalid()) {
                    return ClauseVector(clause->getErrorType(), clause->getErrorMessage());
                }

                clauseVector.add(clause);

                currentClauseConstraint.clear();
                numOfOpenedParentheses = 0;
                numOfTokensWithoutOpenParentheses = 0;
                hasCurrentClause = false;
                hasOpenParentheses = false;
                isInProcessOfCreatingClause = false;
            }
        }
        // if numOfOpenedParentheses is positive, continue looping
    }

    if (!currentClauseConstraint.empty() || isInProcessOfCreatingClause) {
        return ClauseVector(QuerySyntaxError, "Extra incomplete tokens at end of query");
    }

    return clauseVector;
}

DeclarationTable processDeclarations(const String& declarationsString)
{
    DeclarationTable newDeclarations;

    StringVector tokenizedStringList = splitProgram(declarationsString);
    DesignEntity currentDesignEntity;
    Boolean hasCurrentDesignEntity = false;
    Boolean isPreviousTokenASynonym = false;
    Boolean isPotentialProgLineDesignEntity = false;
    Boolean isHighPotentialProgLineDesignEntity = false;

    for (std::string& token : tokenizedStringList) {
        if (!hasCurrentDesignEntity) {
            if (token == "prog") {
                isPotentialProgLineDesignEntity = true;
                continue;
            }

            if (token == "_") {
                if (!isPotentialProgLineDesignEntity) {
                    return DeclarationTable(QuerySyntaxError, DesignEntity::INVALID_DESIGN_ENTITY + "prog");
                }

                isPotentialProgLineDesignEntity = false;
                isHighPotentialProgLineDesignEntity = true;
                continue;
            }

            if (token == "line") {
                if (!isHighPotentialProgLineDesignEntity) {
                    return DeclarationTable(QuerySyntaxError, DesignEntity::INVALID_DESIGN_ENTITY + "prog_");
                }

                currentDesignEntity = DesignEntity(Prog_LineType);
                isHighPotentialProgLineDesignEntity = false;
                hasCurrentDesignEntity = true;
                continue;
            }

            currentDesignEntity = DesignEntity(token);
            if (currentDesignEntity.getType() == NonExistentType) {
                return DeclarationTable(QuerySyntaxError, DesignEntity::INVALID_DESIGN_ENTITY + token);
            }

            hasCurrentDesignEntity = true;
        } else {
            if (token == ";") {
                if (!isPreviousTokenASynonym) {
                    return DeclarationTable(QuerySyntaxError, DeclarationTable::INVALID_DECLARATION_SYNTAX);
                }

                hasCurrentDesignEntity = false;
                isPreviousTokenASynonym = false;
                continue;
            } else if (token == ",") {
                if (!isPreviousTokenASynonym) {
                    return DeclarationTable(QuerySyntaxError, DeclarationTable::INVALID_DECLARATION_SYNTAX);
                }

                isPreviousTokenASynonym = false;
                continue;
            } else if (isPreviousTokenASynonym) {
                // Syntax error e.g. while w w1;
                return DeclarationTable(QuerySyntaxError, DeclarationTable::INVALID_DECLARATION_SYNTAX);
            } else {
                if (!isValidSynonym(token)) {
                    return DeclarationTable(QuerySyntaxError, ResultSynonym::INVALID_SYNONYM_MESSAGE + token);
                } else if (newDeclarations.hasSynonym(token)) {
                    return DeclarationTable(QuerySemanticsError, "Synonym " + token + " has already been declared");
                }

                newDeclarations.addDeclaration(token, currentDesignEntity);
                isPreviousTokenASynonym = true;
            }
        }
    }
    return newDeclarations;
}

// Utils

StringPair splitDeclarationAndSelectClause(const String& query)
{
    StringPair stringVector;
    std::size_t indexOfLastDelimiter = query.find_last_of(';', query.size() - 1);
    if (indexOfLastDelimiter == String::npos) {
        // No declarations
        return std::make_pair("", query);
    }

    String leftToken = query.substr(0, indexOfLastDelimiter + 1);

    String rightToken = query.substr(indexOfLastDelimiter + 1, query.size() - indexOfLastDelimiter - 1);

    return std::make_pair(leftToken, rightToken);
}

StringVector splitByFirstConsecutiveWhitespace(const String& str)
{
    const char* currentChar = str.c_str();
    String currentToken;
    StringVector splitByFirstWhitespaceVector;

    // Find first whitespace
    while (!isWhitespace(currentChar) && *currentChar != '\0') {
        currentToken.push_back(*currentChar);
        currentChar++;
    }

    splitByFirstWhitespaceVector.push_back(currentToken);
    currentToken.clear();

    // Skip past all whitespaces
    while (isWhitespace(currentChar)) {
        currentChar++;
    }

    while (*currentChar != '\0') {
        currentToken.push_back(*currentChar);
        currentChar++;
    }

    splitByFirstWhitespaceVector.push_back(currentToken);
    return splitByFirstWhitespaceVector;
}

Boolean containsOpenParentheses(const String& str)
{
    const char* currentChar = str.c_str();
    while (*currentChar != '\0') {
        if (*currentChar == '(') {
            return true;
        }
        currentChar++;
    }

    return false;
}

std::pair<Boolean, Integer> countNumOfOpenParentheses(const String& token, Integer previousNumOfOpenParentheses)
{
    const char* currentChar = token.c_str();
    Integer numOfOpenParentheses = previousNumOfOpenParentheses;
    Boolean closedBefore = false;

    while (*currentChar != '\0') {
        if (*currentChar == '(') {
            if (closedBefore) {
                return std::make_pair(false, 0);
            }

            numOfOpenParentheses++;
        } else if (*currentChar == ')') {
            if (numOfOpenParentheses == 0) {
                return std::make_pair(false, 0);
            }

            numOfOpenParentheses--;

            if (numOfOpenParentheses == 0) {
                closedBefore = true;
            }
        }

        currentChar++;
    }

    return std::make_pair(true, numOfOpenParentheses);
}