#include "Preprocessor.h"

#include "frontend/parser/Parser.h"

StringPair splitByFirstDelimiter(const String& str, char c);
StringVector splitByFirstConsecutiveWhitespace(String str);
Boolean containsOpenParentheses(String str);
Boolean isEnclosedWith(String str, char leftEnd, char rightEnd);
StringPair splitDeclarationAndSelectClause(const String& query);
std::pair<Boolean, Integer> countNumOfOpenParentheses(String token, Integer previousNumOfOpenParentheses);

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
AbstractQuery Preprocessor::processQuery(String query)
{
    StringPair splitQuery = splitDeclarationAndSelectClause(query);

    String declarationString = trimWhitespace(splitQuery.first);
    String synonymAndClausesString = trimWhitespace(splitQuery.second);

    // Process declarations into declaration table
    DeclarationTable processedDeclarations = processDeclarations(declarationString);
    if (processedDeclarations.isInvalid()) {
        return AbstractQuery::invalidAbstractQuery();
    }
    this->declarationTable = processedDeclarations;

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
    if (!isValidSynonym(selectSynonym) || !processedDeclarations.hasSynonym(selectSynonym)) {
        return AbstractQuery::invalidAbstractQuery();
    }

    // Only select a synonym, with no other clauses
    if (synonymAndClausesVector.size() == 1) {
        AbstractQuery abstractQuery(selectSynonym, processedDeclarations);
        return abstractQuery;
    }

    // Process clauses into ClauseList
    String clausesString = synonymAndClausesVector.at(1);
    ClauseVector clausesVector = processClauses(clausesString);
    if (clausesVector.isInvalid()) {
        return AbstractQuery::invalidAbstractQuery();
    }

    AbstractQuery aq(selectSynonym, processedDeclarations, clausesVector);
    return aq;
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
ClauseVector Preprocessor::processClauses(const String& clausesString)
{
    ClauseVector clauseVector;
    String currentClauseConstraint;
    Boolean hasCurrentClause = false;
    Boolean isPreviousTokenSuch = false; // is the previous token "such"
    ClauseType currentClauseType;
    Integer numOfOpenedParentheses = 0;
    Boolean hasOpenParentheses = false;

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

    StringList* splitStringList = splitByWhitespace(clausesString);

    for (auto& token : *splitStringList) {
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
                currentClauseConstraint.append(*token);

                std::pair<Boolean, Integer> result = countNumOfOpenParentheses(*token, numOfOpenedParentheses);
                if (!result.first) {
                    return ClauseVector::invalidClauseVector();
                }

                numOfOpenedParentheses = result.second;

                if (!hasOpenParentheses) {
                    hasOpenParentheses = containsOpenParentheses(*token);
                }

                if (numOfOpenedParentheses < 0) {
                    return ClauseVector::invalidClauseVector();
                } else if (numOfOpenedParentheses == 0) {
                    if (!hasOpenParentheses) {
                        numOfTokensWithoutOpenParentheses++;

                        if (numOfTokensWithoutOpenParentheses >= 2) {
                            return ClauseVector::invalidClauseVector();
                        }
                    } else {
                        Clause* clause;

                        if (currentClauseType == SuchThatClauseType) {
                            clause = processSuchThatClause(currentClauseConstraint);
                        } else { // PatternClause
                            clause = processPatternClause(currentClauseConstraint);
                        }

                        if (clause->isInvalid()) {
                            return ClauseVector::invalidClauseVector();
                        }

                        clauseVector.add(clause);

                        hasCurrentClause = false;
                        numOfOpenedParentheses = 0;
                        numOfTokensWithoutOpenParentheses = 0;
                        currentClauseConstraint.clear();
                        hasOpenParentheses = false;
                    }
                }
                // if numOfOpenedParentheses is positive, continue looping
            }
        }
    }

    if (!currentClauseConstraint.empty()) {
        return ClauseVector::invalidClauseVector();
    }

    delete splitStringList;

    return clauseVector;
}

/**
 * Processes the clause constraint string for a
 * SuchThatClause, by abstracting it into its relevant
 * Relationship and References.
 *
 * @param clauseConstraint  String of the clause constraint with all
 *                          whitespaces removed.
 * @return                  A Clause pointer of the SuchThatClause
 *                          that was constructed.
 */
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

    return new SuchThatClause(relationship);
}

/**
 * Processes the clause constraint string for a
 * PatternClause, by abstracting it into its relevant
 * PatternTypeStatement, Reference and ExpressionSpec.
 *
 * @param clauseConstraint  String of the clause constraint with all
 *                          whitespaces removed.
 * @return                  A Clause pointer of the PatternClause
 *                          that was constructed.
 */
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

    return new PatternClause(patternSynonym, AssignPatternType, leftReference, std::move(rightExpressionSpec));
}

/**
 * Creates an ExpressionSpec of based on the given
 * exprSpecString. It will determine the ExpressionSpecType
 * and call the parser from frontend to parse the expression
 * string into an Expression.
 *
 * If the exprSpecString is an invalid form of
 * ExpressionSpec, an invalid ExpressionSpec will be
 * returned.
 *
 * @param exprSpecString    The string to be parsed into an ExpressionSpec.
 * @return                  The ExpressionSpec constructed using exprSpectString.
 */
ExpressionSpec Preprocessor::createExpressionSpec(String exprSpecString)
{
    if (exprSpecString == "_") {
        ExpressionSpec expressionSpec{WildcardExpressionType};
        return expressionSpec;
    }

    if (isEnclosedWith(exprSpecString, '_', '_')) {
        String possibleLiteral = util::removeCharFromBothEnds(exprSpecString);
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

    if (!isEnclosedWith(exprSpecString, '\"', '\"')) {
        return ExpressionSpec::invalidExpressionSpec();
    }

    String expressionString = util::removeCharFromBothEnds(exprSpecString);
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

/**
 * Creates a Reference using the given ref String. It will
 * determine the ReferenceType based on the ref given.
 * If the ref is a synonym, the design entity of the synonym
 * will be stored in the Reference object.
 *
 * If the ref is an invalid form of a Reference, an invalid
 * Reference will be returned.
 *
 * @param ref   String of the reference to be constructed.
 * @return      A Reference based on ref.
 */
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

    StringList* tokenizedStringList = splitProgram(declarationsString);
    DesignEntity currentDesignEntity;
    bool hasCurrentDesignEntity = false;
    bool isPreviousTokenASynonym = false;

    for (std::unique_ptr<std::string>& token : *tokenizedStringList) {
        if (!hasCurrentDesignEntity) {
            currentDesignEntity = DesignEntity(*token);
            if (currentDesignEntity.getType() == NonExistentType) {
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
                if (!isValidSynonym(*token) || newDeclarations.hasSynonym(*token)) {
                    return DeclarationTable::invalidDeclarationTable();
                }

                newDeclarations.addDeclaration(*token, currentDesignEntity);
                isPreviousTokenASynonym = true;
            }
        }
    }
    delete tokenizedStringList;
    return newDeclarations;
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
StringPair splitByFirstDelimiter(const String& str, char c)
{
    StringPair stringVector;
    const char* currentChar = str.c_str();
    String currentToken;

    // Find first delimiter
    while (*currentChar != c && *currentChar != '\0') {
        currentToken.push_back(*currentChar);
        currentChar++;
    }

    String leftString = currentToken;
    currentToken.clear();

    if (*currentChar != '\0') {
        currentChar++; // Skips '('
    }

    while (*currentChar != '\0') {
        currentToken.push_back(*currentChar);
        currentChar++;
    }

    return std::make_pair(leftString, currentToken);
}

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
StringVector splitByFirstConsecutiveWhitespace(String str)
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
std::pair<Boolean, Integer> countNumOfOpenParentheses(String token, Integer previousNumOfOpenParentheses)
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