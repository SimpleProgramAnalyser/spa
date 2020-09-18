/**
 * Implementation of an evaluator for a such that clause
 * describing a Parent relationship between design entities
 */
#include "ParentEvaluator.h"

#include "RelationshipsUtil.h"

Void evaluateParentClause(Reference leftRef, Reference rightRef, Boolean isStar, ResultsTable* resultsTable)
{
    ClauseResult result;
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    if (leftRefType == IntegerRefType && canMatchMultiple(rightRefType)) {
        Integer leftValue = std::stoi(leftRef.getValue());
        DesignEntityType rightSynonymType
            = rightRef.isWildCard() ? StmtType : declarations.getDesignEntityOfSynonym(rightRef.getValue()).getType();
        Vector<Integer> tempResult = (isStar ? getAllChildStatementsStar
                                             : getAllChildStatements)(leftValue, mapToStatementType(rightSynonymType));
        result = convertToClauseResult(tempResult);
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        DesignEntityType leftSynonymType
            = leftRef.isWildCard() ? StmtType : declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType();
        Integer rightValue = std::stoi(rightRef.getValue());
        Vector<Integer> tempResult = (isStar ? getAllParentStatementsStar
                                             : getAllParentStatements)(rightValue, mapToStatementType(leftSynonymType));
        result = convertToClauseResult(tempResult);
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        Integer leftRefVal = std::stoi(leftRef.getValue());
        Integer rightRefVal = std::stoi(rightRef.getValue());
        Boolean followsHolds = (isStar ? checkIfParentHoldsStar : checkIfParentHolds)(leftRefVal, rightRefVal);
        if (followsHolds) {
            result.push_back("trueParent");
        }
    } else if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // Check if left == right, for Parent this will always return empty
        return result;
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        StatementType leftRefStmtType
            = leftRef.isWildCard()
                  ? AnyStatement
                  : mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType());
        StatementType rightRefStmtType
            = rightRef.isWildCard()
                  ? AnyStatement
                  : mapToStatementType(declarations.getDesignEntityOfSynonym(rightRef.getValue()).getType());
        std::vector<Integer> (*lookupPkbFunction)(StatementType, StatementType);
        if (leftRef.getValue() == synonym) {
            lookupPkbFunction = isStar ? getAllParentStatementsTypedStar : getAllParentStatementsTyped;
        } else {
            lookupPkbFunction = isStar ? getAllChildStatementsTypedStar : getAllChildStatementsTyped;
        }
        result = convertToClauseResult(lookupPkbFunction(leftRefStmtType, rightRefStmtType));
    } else {
        throw std::runtime_error("Error in evaluateParentClause: No synonyms or integers in Parent clause");
    }
    return result;
}
