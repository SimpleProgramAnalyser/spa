/**
 * Implementation of an evaluator for a such that clause
 * describing a Parent relationship between design entities
 */
#include "ParentEvaluator.h"

#include <stdexcept>

#include "RelationshipsUtil.h"

class ParentEvaluator {
private:
    Reference leftRef;
    Reference rightRef;
    Boolean isStar;
    ResultsTable* resultsTable;
    Boolean (*pkbBothKnownFunction)(Integer, Integer);

    // case where left is known (integer), right is variable
    Void evaluateLeftKnown() const;
    // case where left is variable, right is known (integer)
    Void evaluateRightKnown() const;
    // case where both are variable
    Void evaluateBothAny() const;
    // case where both are known
    Void evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const;

public:
    ParentEvaluator(Reference leftRef, Reference rightRef, Boolean isStar, ResultsTable* resultsTable):
        leftRef(std::move(leftRef)), rightRef(std::move(rightRef)), isStar(isStar), resultsTable(resultsTable),
        pkbBothKnownFunction(isStar ? checkIfParentHoldsStar : checkIfParentHolds)
    {}
    Void evaluateParentClause();
};

Void evaluateParentClause(const Reference& leftRef, const Reference& rightRef, Boolean isStar,
                          ResultsTable* resultsTable)
{
    ParentEvaluator evaluator(leftRef, rightRef, isStar, resultsTable);
    evaluator.evaluateParentClause();
}

Void ParentEvaluator::evaluateLeftKnown() const
{
    Integer leftValue = std::stoi(leftRef.getValue());
    // get the exact DesignEntityType of the right operand
    // if it is wildcard, equivalent to any statement
    DesignEntityType rightSynonymType
        = rightRef.isWildCard() ? StmtType : resultsTable->getTypeOfSynonym(rightRef.getValue());
    Vector<Integer> tempResult
        = (isStar ? getAllChildStatementsStar : getAllChildStatements)(leftValue, mapToStatementType(rightSynonymType));
    resultsTable->storeResultsOne(rightRef, convertToClauseResult(tempResult));
}

Void ParentEvaluator::evaluateRightKnown() const
{
    Integer rightValue = std::stoi(rightRef.getValue());
    // get the DesignEntityType of the left operand
    DesignEntityType leftSynonymType
        = leftRef.isWildCard() ? StmtType : resultsTable->getTypeOfSynonym(leftRef.getValue());
    Vector<Integer> tempResult = (isStar ? getAllParentStatementsStar
                                         : getAllParentStatements)(rightValue, mapToStatementType(leftSynonymType));
    resultsTable->storeResultsOne(leftRef, convertToClauseResult(tempResult));
}


Void ParentEvaluator::evaluateBothAny() const
{
    // we can only get the DesignEntityType of both the left and right operands
    StatementType leftRefStmtType = leftRef.isWildCard()
                                    ? AnyStatement
                                    : mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
    StatementType rightRefStmtType = rightRef.isWildCard()
                                     ? AnyStatement
                                     : mapToStatementType(resultsTable->getTypeOfSynonym(rightRef.getValue()));
    ClauseResult leftResults;
    ClauseResult rightResults;
    PairedResult tuples;
    if (isStar) {
        leftResults = convertToClauseResult(getAllParentStatementsTypedStar(leftRefStmtType, rightRefStmtType));
        rightResults = convertToClauseResult(getAllChildStatementsTypedStar(leftRefStmtType, rightRefStmtType));
        tuples = convertToPairedResult(getAllParentTupleStar(leftRefStmtType, rightRefStmtType));
    } else {
        leftResults = convertToClauseResult(getAllParentStatementsTyped(leftRefStmtType, rightRefStmtType));
        rightResults = convertToClauseResult(getAllChildStatementsTyped(leftRefStmtType, rightRefStmtType));
        tuples = convertToPairedResult(getAllParentTuple(leftRefStmtType, rightRefStmtType));
    }
    resultsTable->storeResultsTwo(leftRef, leftResults, rightRef, rightResults, tuples);
}

Void ParentEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const
{
    Boolean parentHolds = pkbBothKnownFunction(leftRefVal, rightRefVal);
    std::vector<String> tempResult;
    if (parentHolds) {
        // we add a placeholder item to denote presence of results
        tempResult.emplace_back("trueParent");
    }
    resultsTable->storeResultsOne(leftRef, tempResult);
}

Void ParentEvaluator::evaluateParentClause()
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    if (leftRefType == IntegerRefType && canMatchMultiple(rightRefType)) {
        evaluateLeftKnown();
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        evaluateRightKnown();
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        Integer leftRefVal = std::stoi(leftRef.getValue());
        Integer rightRefVal = std::stoi(rightRef.getValue());
        evaluateBothKnown(leftRefVal, rightRefVal);
    } else if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // if left == right, for Parent this will always return empty
        resultsTable->storeResultsOne(leftRef, std::vector<String>());
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        evaluateBothAny();
    } else {
        throw std::runtime_error(
            "Error in ParentExtractor::evaluateParentClause: No synonyms or integers in Parent clause");
    }
}
