/**
 * Implementation of an evaluator for a such that clause
 * describing a Parent relationship between design entities
 */
#include "ParentEvaluator.h"

#include <utility>

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
    Void evaluateBothAnyConstrained() const;
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
    resultsTable->filterTable(rightRef, convertToClauseResult(tempResult));
}

Void ParentEvaluator::evaluateRightKnown() const
{
    Integer rightValue = std::stoi(rightRef.getValue());
    // get the DesignEntityType of the left operand
    DesignEntityType leftSynonymType
        = leftRef.isWildCard() ? StmtType : resultsTable->getTypeOfSynonym(leftRef.getValue());
    Vector<Integer> tempResult = (isStar ? getAllParentStatementsStar
                                         : getAllParentStatements)(rightValue, mapToStatementType(leftSynonymType));
    resultsTable->filterTable(leftRef, convertToClauseResult(tempResult));
}

Void ParentEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const
{
    Boolean parentHolds = pkbBothKnownFunction(leftRefVal, rightRefVal);
    if (parentHolds) {
        std::vector<String> tempResult{"trueParent"};
        resultsTable->filterTable(leftRef, tempResult);
    } else {
        // we store an empty list to denote a lack of results
        resultsTable->filterTable(leftRef, std::vector<String>());
    }
}

Void ParentEvaluator::evaluateBothAnyConstrained() const
{
    std::vector<String> tempResultForLeft;
    std::vector<String> tempResultForRight;
    std::vector<std::pair<String, String>> tempResultForRelationships;
    Boolean leftAndRightRelated = refsHaveRelationship(leftRef, rightRef, resultsTable);
    // check if variables have relations determined by previous clauses
    if (leftAndRightRelated) {
        // refsHaveRelationship guarantees both left and right are synonyms
        std::vector<std::pair<String, String>> relationsList
            = resultsTable->getRelationships(leftRef.getValue(), rightRef.getValue());
        for (const std::pair<String, String>& relation : relationsList) {
            // for Parent, both are guaranteed to be integer
            Boolean parentHolds = pkbBothKnownFunction(std::stoi(relation.first), std::stoi(relation.second));
            if (parentHolds) {
                tempResultForLeft.push_back(relation.first);
                tempResultForRight.push_back(relation.second);
                tempResultForRelationships.emplace_back(relation.first, relation.second);
            }
        }
    } else {
        ClauseResult previousResultsForLeft = resultsTable->get(leftRef.getValue());
        ClauseResult previousResultsForRight = resultsTable->get(rightRef.getValue());
        // do a Cartesian product of both result lists and check each pair
        for (const String& strLeft : previousResultsForLeft) {
            for (const String& strRight : previousResultsForRight) {
                // for Parent, both are guaranteed to be integer
                Boolean parentHolds = pkbBothKnownFunction(std::stoi(strLeft), std::stoi(strRight));
                if (parentHolds) {
                    tempResultForLeft.push_back(strLeft);
                    tempResultForRight.push_back(strRight);
                    tempResultForRelationships.emplace_back(strLeft, strRight);
                }
            }
        }
    }
    resultsTable->filterTable(leftRef, tempResultForLeft);
    resultsTable->filterTable(rightRef, tempResultForRight);
    resultsTable->associateRelationships(tempResultForRelationships, leftRef, rightRef);
}

Void ParentEvaluator::evaluateBothAny() const
{
    Boolean leftHasConstraints = refHasConstraints(leftRef, resultsTable);
    Boolean rightHasConstraints = refHasConstraints(rightRef, resultsTable);
    if (leftHasConstraints || rightHasConstraints) {
        evaluateBothAnyConstrained();
    } else {
        // In this case, we can only get the DesignEntityType of both the left and right operands
        StatementType leftRefStmtType = leftRef.isWildCard()
                                            ? AnyStatement
                                            : mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
        StatementType rightRefStmtType = rightRef.isWildCard()
                                             ? AnyStatement
                                             : mapToStatementType(resultsTable->getTypeOfSynonym(rightRef.getValue()));
        if (isStar) {
            resultsTable->filterTable(
                leftRef, convertToClauseResult(getAllParentStatementsTypedStar(leftRefStmtType, rightRefStmtType)));
            resultsTable->filterTable(
                rightRef, convertToClauseResult(getAllChildStatementsTypedStar(leftRefStmtType, rightRefStmtType)));
            resultsTable->associateRelationships(getAllParentTupleStar(leftRefStmtType, rightRefStmtType), leftRef,
                                                 rightRef);
        } else {
            resultsTable->filterTable(
                leftRef, convertToClauseResult(getAllParentStatementsTyped(leftRefStmtType, rightRefStmtType)));
            resultsTable->filterTable(
                rightRef, convertToClauseResult(getAllChildStatementsTyped(leftRefStmtType, rightRefStmtType)));
            resultsTable->associateRelationships(getAllParentTuple(leftRefStmtType, rightRefStmtType), leftRef,
                                                 rightRef);
        }
    }
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
        resultsTable->filterTable(leftRef, std::vector<String>());
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        evaluateBothAny();
    } else {
        throw std::runtime_error(
            "Error in ParentExtractor::evaluateParentClause: No synonyms or integers in Parent clause");
    }
}
