/**
 * Implementation of an evaluator for a such that clause
 * describing a Follows relationship between design entities
 */
#include "FollowsEvaluator.h"

#include "RelationshipsUtil.h"

Void evaluateFollowsClause(Reference leftRef, Reference rightRef, Boolean isStar, ResultsTable* resultsTable)
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    /*
     * Here, we filter by reference types of the follows clause
     * operands, and there are a few possible combinations,
     * each of them corresponds to a particular group of
     * PKB API calls.
     *
     * The possible combinations are:
     * 1. Follows(3, s)
     *    Where the left operand is an integer
     *    (line number) and the right operand is
     *    a synonym.
     *
     * 2. Follows(s, 3)
     *    Where the left operand is a synonym type
     *    and the left operand is a integer (line
     *    number).
     *
     * 3. Follows(3, _)
     *    Where the left operand is an integer
     *    (line number) and the right operand
     *    is a wildcard.
     *
     * 4. Follows(_, 3)
     *    Where the left operand is a wildcard,
     *    and the right operand is a wildcard.
     *
     * 5. Follows(2, 3)
     *    Both operands are integers.
     *    (It is not hard to see that the left and right
     *    operands in this case, would never be related to
     *    the synonym (as per iteration 1 requirements)).
     *
     * 6. Follows(s1, s2)
     *    Where both operands are synonym types.
     *
     * 7. Follows(st, st)
     *    The left operand must be the same as the right
     *    operand. For Follows, this is impossible.
     */
    if (leftRefType == IntegerRefType && canMatchMultiple(rightRefType)) {
        Integer leftValue = std::stoi(leftRef.getValue());
        // get the DesignEntityType of the right operand
        // if it is wildcard, equivalent to any statement
        DesignEntityType rightSynonymType
            = rightRef.isWildCard() ? StmtType : resultsTable->getTypeOfSynonym(rightRef.getValue());
        /*
         * Here there are many combinations/possibilities, of what the
         * right DesignEntityType can be, however with a some mapping,
         * function, we can simplify things.
         *
         * The function, would map DesignEntityType (pql/preprocessor/AqTypes.h)
         * to StatementType (in Types.h). It is declared in EvaluatorUtils.cpp.
         *
         * Note: Here we make a critical assumption, that we can never
         * have invalid DesignEntityType which would hence get mapped
         * to spurious StatementType. This is guaranteed due to our
         * (robust) validation at the PQL Preprocessor side.
         */
        Vector<Integer> tempResult = (isStar ? getAllAfterStatementsStar
                                             : getAllAfterStatements)(leftValue, mapToStatementType(rightSynonymType));
        resultsTable->filterTable(rightRef, convertToClauseResult(tempResult));
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        // get the DesignEntityType of the left operand
        DesignEntityType leftSynonymType
            = leftRef.isWildCard() ? StmtType : resultsTable->getTypeOfSynonym(leftRef.getValue());
        Integer rightValue = std::stoi(rightRef.getValue());
        Vector<Integer> tempResult = (isStar ? getAllBeforeStatementsStar
                                             : getAllBeforeStatements)(rightValue, mapToStatementType(leftSynonymType));
        resultsTable->filterTable(leftRef, convertToClauseResult(tempResult));
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        /*
         * For this case, in iteration 1, the clause is perpetually not
         * related to the select synonym (because the wildcard to converted
         * to any StmtType, hence no checking needs to be done here (if
         * clause is related to select synonym).
         */
        Integer leftRefVal = std::stoi(leftRef.getValue());
        Integer rightRefVal = std::stoi(rightRef.getValue());
        Boolean followsHolds = (isStar ? checkIfFollowsHoldsStar : checkIfFollowsHolds)(leftRefVal, rightRefVal);
        if (followsHolds) {
            std::vector<String> tempResult{"trueFollows"};
            resultsTable->filterTable(leftRef, tempResult);
        } else {
            // we store an empty list to denote a lack of results
            resultsTable->filterTable(leftRef, std::vector<String>());
        }
    } else if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // Check if left == right, for Follows this will always return empty
        resultsTable->filterTable(leftRef, std::vector<String>());
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        // In this case, we can only get the DesignEntityType of both the left and right operands
        StatementType leftRefStmtType = leftRef.isWildCard()
                                            ? AnyStatement
                                            : mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
        StatementType rightRefStmtType = rightRef.isWildCard()
                                             ? AnyStatement
                                             : mapToStatementType(resultsTable->getTypeOfSynonym(rightRef.getValue()));
        if (isStar) {
            resultsTable->filterTable(
                leftRef, convertToClauseResult(getAllBeforeStatementsTypedStar(leftRefStmtType, rightRefStmtType)));
            resultsTable->filterTable(
                rightRef, convertToClauseResult(getAllAfterStatementsTypedStar(leftRefStmtType, rightRefStmtType)));
        } else {
            resultsTable->filterTable(
                leftRef, convertToClauseResult(getAllBeforeStatementsTyped(leftRefStmtType, rightRefStmtType)));
            resultsTable->filterTable(
                rightRef, convertToClauseResult(getAllAfterStatementsTyped(leftRefStmtType, rightRefStmtType)));
        }
    } else {
        throw std::runtime_error("Error in evaluateFollowsClause: No synonyms or integers in Follows clause");
    }
}
