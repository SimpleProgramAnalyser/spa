#include "Preprocessor.h"

#include "frontend/parser/Parser.h"

StringVector splitByFirstConsecutiveWhitespace(const String& str);
Boolean containsOpenParentheses(const String& str);
StringPair splitDeclarationAndSelectClause(const String& query);
std::pair<Boolean, Integer> countNumOfOpenParentheses(const String& token, Integer previousNumOfOpenParentheses);
std::pair<Boolean, Vector<ResultSynonym>> processSelectResultString(String selectResultString,
                                                                    DeclarationTable& declarationTable);
std::pair<Boolean, ResultSynonym> processResultSynonym(String resultSynonymString, DeclarationTable& declarationTable);
StringVector splitResultAndClauses(String& s);

/**
 * Processes a given PQL query into an AbstractQuery.
 * The returned AbstractQuery will contain the selected
 * synonym, all the declarations in a DeclarationTable
 * and all the clauses contained separately in a
 * ClauseVector.
 *
 * If the PQL query is syntactically or semantically
 * incorrect, an invalid AbstractQuery will be returned.
 *
 * @param query PQL query string to be processed
 * @return      AbstractQuery that breaks the PQL query into
 *              abstract objects that can be evaluated.
 */
AbstractQuery Preprocessor::processQuery(const String& query)
{
    // TODO: Improve splitting of Declarations and Select Clauses - iterate every char from the front until ; Select
    StringPair splitQuery = splitDeclarationAndSelectClause(query);

    String declarationString = trimWhitespace(splitQuery.first);
    String synonymAndClausesString = trimWhitespace(splitQuery.second);

    // Process declarations into declaration table
    DeclarationTable declarationTable = processDeclarations(declarationString);
    if (declarationTable.isInvalid()) {
        return AbstractQuery(true);
    }

    // Extract select keyword
    StringVector selectAndClausesVector = splitByFirstConsecutiveWhitespace(synonymAndClausesString);

    // No select clause
    if (selectAndClausesVector.size() < 2) {
        return AbstractQuery(true);
    }

    String possibleSelectKeyword = selectAndClausesVector.at(0);
    if (possibleSelectKeyword != "Select") {
        return AbstractQuery(true);
    }

    String synonymAndClauses = selectAndClausesVector.at(1);
    StringVector synonymAndClausesVector = splitResultAndClauses(synonymAndClauses);

    // No synonym
    if (selectAndClausesVector.empty()) {
        return AbstractQuery(true);
    }

    String selectResultString = synonymAndClausesVector.at(0);

    std::pair<Boolean, Vector<ResultSynonym>> processedResults
        = processSelectResultString(selectResultString, declarationTable);

    if (processedResults.first) {
        return AbstractQuery(true);
    }

    Vector<ResultSynonym> resultSynonyms = processedResults.second;

    // Only select a synonym, with no other clauses
    if (synonymAndClausesVector.size() == 1) {
        AbstractQuery abstractQuery(resultSynonyms, declarationTable);
        return abstractQuery;
    }

    // Process clauses into ClauseList
    String clausesString = synonymAndClausesVector.at(1);
    ClauseVector clausesVector = processClauses(clausesString, declarationTable);
    if (clausesVector.isInvalid()) {
        return AbstractQuery(true);
    }

    AbstractQuery abstractQuery(resultSynonyms, declarationTable, clausesVector);
    return abstractQuery;
}

std::pair<Boolean, Vector<ResultSynonym>> processSelectResultString(String selectResultString,
                                                                    DeclarationTable& declarationTable)
{
    // BOOLEAN result
    if (selectResultString == "BOOLEAN") {
        if (!declarationTable.hasSynonym(selectResultString)) {
            // Empty Vector means result is of type BOOLEAN
            return std::make_pair(false, Vector<ResultSynonym>());
        }

        return std::make_pair(false, Vector<ResultSynonym>{ ResultSynonym{selectResultString} });
    }

    // Single Synonym result
    if (selectResultString.at(0) != '<') {
        std::pair<Boolean, ResultSynonym> processedResultSynonym
            = processResultSynonym(selectResultString, declarationTable);
        if (processedResultSynonym.first) {
            return std::make_pair(true, Vector<ResultSynonym>{});
        }

        return std::make_pair(false, Vector<ResultSynonym>{ processedResultSynonym.second });
    }

    // Tuple result
    String removeTupleString = selectResultString.substr(1);
    StringVector resultSynonymStrings = splitByDelimiter(removeTupleString, ",");
    Vector<ResultSynonym> resultSynonyms;
    for (auto& resultSynonymString : resultSynonymStrings) {
        String trimmedResultSynonymString = trimWhitespace(resultSynonymString);
        std::pair<Boolean, ResultSynonym> processedResultSynonym
            = processResultSynonym(trimmedResultSynonymString, declarationTable);
        if (processedResultSynonym.first) {
            return std::make_pair(true, Vector<ResultSynonym>{});
        }

        resultSynonyms.push_back(processedResultSynonym.second);
    }

    return std::make_pair(false, resultSynonyms);
}

std::pair<Boolean, ResultSynonym> processResultSynonym(String resultSynonymString, DeclarationTable& declarationTable)
{
    StringVector splitByFullStop = splitByDelimiter(resultSynonymString, ".");
    std::pair<Boolean, ResultSynonym> invalidResult = std::make_pair(true, ResultSynonym(true));
    if (splitByFullStop.size() > 2) {
        return invalidResult;
    }

    // Synonym without attributes
    if (splitByFullStop.size() == 1) {
        if (!isValidSynonym(resultSynonymString) || !declarationTable.hasSynonym(resultSynonymString)) {
            return invalidResult;
        }

        return std::make_pair(false, ResultSynonym(resultSynonymString));
    }
    // TODO: Consider s . stmt # (s.stmt#)
    String synonym = splitByFullStop[0];
    String attribute = splitByFullStop[1];

    if (!isValidSynonym(synonym) || declarationTable.hasSynonym(synonym)) {
        return invalidResult;
    }

    ResultSynonym resultSynonym(synonym, attribute);
    if (resultSynonym.isInvalid()) {
        return invalidResult;
    }

    return std::make_pair(false, resultSynonym);
}

/**
 * Splits the selected synonym, tuple or BOOLEAN
 * from the rest of the clauses.
 * @param s     String to be split
 * @return      StringVector that contains either one or two strings.
 *              If StringVector only contains one string, that means
 *              that are no constraint clauses.
 */
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

/**
 * Processes the clauses component of the PQL query, and
 * split them up accordingly into separate clauses stored
 * in a clause vector. The different clauses include
 * SuchThatClause and PatternClause.
 *
 * @param clausesString String of clauses to be processed. String
 *                      starts from after the selected synonym.
 * @return              ClauseVector of all clauses in the
 *                      clausesString.
 */
ClauseVector Preprocessor::processClauses(const String& clausesString, DeclarationTable& declarationTable)
{
    ClauseVector clauseVector;
    String currentClauseConstraint;
    Boolean hasCurrentClause = false;
    Boolean isPreviousTokenSuch = false; // is the previous token "such"
    ClauseType currentClauseType = NonExistentClauseType;
    Integer numOfOpenedParentheses = 0;
    Boolean hasOpenParentheses = false;
    Boolean isInProcessOfCreatingClause = false;

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

    for (auto& token : splitStringList) {
        if (token == "and" && !hasCurrentClause && currentClauseType != NonExistentClauseType) {
            hasCurrentClause = true;
            isInProcessOfCreatingClause = true;
            continue;
        }

        if (!hasCurrentClause) {
            if (token != "such" && token != "pattern") {
                return ClauseVector::invalidClauseVector();
            }

            if (token == "such") {
                isPreviousTokenSuch = true;
            } else if (token == "pattern") {
                currentClauseType = PatternClauseType;
            }

            isInProcessOfCreatingClause = true;
            hasCurrentClause = true;
        } else {
            if (isPreviousTokenSuch) {
                if (token != "that") {
                    return ClauseVector::invalidClauseVector();
                }

                isPreviousTokenSuch = false;
                currentClauseType = SuchThatClauseType;
            } else {
                currentClauseConstraint.append(token);

                std::pair<Boolean, Integer> result = countNumOfOpenParentheses(token, numOfOpenedParentheses);
                if (!result.first) {
                    return ClauseVector::invalidClauseVector();
                }

                numOfOpenedParentheses = result.second;

                if (!hasOpenParentheses) {
                    hasOpenParentheses = containsOpenParentheses(token);
                }

                if (numOfOpenedParentheses < 0) {
                    return ClauseVector::invalidClauseVector();
                } else if (numOfOpenedParentheses == 0) {
                    if (!hasOpenParentheses) {
                        if (token != "*") {
                            numOfTokensWithoutOpenParentheses++;
                        }

                        if (numOfTokensWithoutOpenParentheses >= 2) {
                            return ClauseVector::invalidClauseVector();
                        }
                    } else {
                        Clause* clause;

                        if (currentClauseType == SuchThatClauseType) {
                            clause = SuchThatClause::createSuchThatClause(currentClauseConstraint, declarationTable);
                        } else { // PatternClause
                            clause = PatternClause::createPatternClause(currentClauseConstraint, declarationTable);
                        }

                        if (clause->isInvalid()) {
                            return ClauseVector::invalidClauseVector();
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
        }
    }

    if (!currentClauseConstraint.empty() || isInProcessOfCreatingClause) {
        return ClauseVector::invalidClauseVector();
    }

    return clauseVector;
}

/**
 * Processes the declaration component of the PQL query.
 * A DeclarationTable will be created with the Synonym as
 * the key and the DesignEntity of the Synonym as the
 * value.
 *
 * If any of the declarations is invalid, or a syntax
 * error is encountered, an invalid DeclarationTable will
 * be returned.
 *
 * @param declarationsString    The substring of the PQL query with all
 *                              the declarations.
 * @return                      A DeclarationTable that stores all stated
 *                              declarations in the declarationsString.
 */
DeclarationTable Preprocessor::processDeclarations(const String& declarationsString)
{
    DeclarationTable newDeclarations;

    StringVector tokenizedStringList = splitProgram(declarationsString);
    DesignEntity currentDesignEntity;
    bool hasCurrentDesignEntity = false;
    bool isPreviousTokenASynonym = false;

    for (std::string& token : tokenizedStringList) {
        if (!hasCurrentDesignEntity) {
            currentDesignEntity = DesignEntity(token);
            if (currentDesignEntity.getType() == NonExistentType) {
                return DeclarationTable::invalidDeclarationTable();
            }

            hasCurrentDesignEntity = true;
        } else {
            if (token == ";") {
                if (!isPreviousTokenASynonym) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                hasCurrentDesignEntity = false;
                isPreviousTokenASynonym = false;
                continue;
            } else if (token == ",") {
                if (!isPreviousTokenASynonym) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                isPreviousTokenASynonym = false;
                continue;
            } else if (isPreviousTokenASynonym) {
                // Syntax error e.g. while w w1;
                return DeclarationTable::invalidDeclarationTable();
            } else {
                if (!isValidSynonym(token) || newDeclarations.hasSynonym(token)) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                newDeclarations.addDeclaration(token, currentDesignEntity);
                isPreviousTokenASynonym = true;
            }
        }
    }
    return newDeclarations;
}

// Utils

/**
 * Returns a pair of Strings, split by the
 * last appearance of the semicolon. If the
 * semicolon does not exist, a pair of empty
 * strings will be returned.
 *
 * @param query String to be split.
 * @return Pair of Strings split by delimiter.
 */
StringPair splitDeclarationAndSelectClause(const String& query)
{
    StringPair stringVector;
    std::size_t indexOfLastDelimiter = query.find_last_of(';', query.size() - 1);
    if (indexOfLastDelimiter == static_cast<std::size_t>(-1)) {
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

Boolean hasChar(const String& str, char charToLookFor)
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

Boolean containsOpenParentheses(const String& str)
{
    return hasChar(str, '(');
}

/**
 * Counts the number of opened parentheses in the
 * newly appended clause constraint. Opened
 * parentheses here means open parentheses that
 * are not yet closed by a close parentheses.
 *
 * It uses the number of opened parentheses in the
 * token, and add it to the count of number of
 * opened parentheses before the current token was
 * appended.
 *
 * If a close parentheses in this token that closes
 * the first open parentheses in the newly appended
 * clause constraint exist, any further parentheses
 * encountered in this token would deem the clause
 * invalid.
 *
 * If there ever a point where the number of opened
 * parentheses is negative, which means there are
 * more close than open parentheses, the clause
 * would be deemed invalid.
 *
 * @param token                         Current token being processed
 * @param previousNumOfOpenParentheses  Previous number of opened parentheses
 *                                      from the clause constraint before it
 *                                      was appended with token.
 * @return                              Current number of opened parentheses
 */
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