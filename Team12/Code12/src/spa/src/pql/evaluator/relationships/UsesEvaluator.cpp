/**
 * Implementation of an evaluator for a such that clause
 * describing a Uses relationship between design entities
 */
#include "UsesEvaluator.h"

#include "RelationshipsUtil.h"

Void evaluateUsesClause(Reference leftRef, Reference rightRef, ResultsTable* resultsTable)
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();

    if (canMatchOnlyOne(leftRefType) && canMatchMultiple(rightRefType)) {
        result = leftRefType == IntegerRefType ? getUsesVariablesFromStatement(std::stoi(leftRef.getValue()))
                                               : getUsesVariablesFromProcedure(leftRef.getValue());
    } else if (canMatchMultiple(leftRefType) && canMatchOnlyOne(rightRefType)) {
        if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            result = convertToClauseResult(getUsesStatements(
                rightRef.getValue(),
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else {
            // declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType() == ProcedureType
            result = getUsesProcedures(rightRef.getValue());
        }
    } else if (canMatchOnlyOne(leftRefType) && canMatchOnlyOne(rightRefType)) {
        Boolean usesHolds;
        if (leftRefType == IntegerRefType) {
            usesHolds = checkIfStatementUses(std::stoi(leftRef.getValue()), rightRef.getValue());
        } else {
            // leftRefType == LiteralRefType
            usesHolds = checkIfProcedureUses(leftRef.getValue(), rightRef.getValue());
        }
        if (usesHolds) {
            result.push_back("trueUses");
        }
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        if (leftRef.getValue() == synonym
            && isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select stmt
            result = convertToClauseResult(getAllUsesStatements(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else if (leftRef.getValue() == synonym) {
            // select procedure
            result = getAllUsesProcedures();
        } else if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select variable with statement
            result = getAllUsesVariables(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType()));
        } else {
            // select variable with procedure
            result = getAllUsesVariables(leftRef.getValue());
        }
    } else {
        throw std::runtime_error("Error in evaluateUsesClause: invalid arguments in Uses");
    }
    return result;
}
