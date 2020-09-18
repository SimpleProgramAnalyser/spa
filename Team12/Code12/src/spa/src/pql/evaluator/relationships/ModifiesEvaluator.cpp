/**
 * Implementation of an evaluator for a such that clause
 * describing a Modifies relationship between design entities
 */
#include "ModifiesEvaluator.h"

#include "RelationshipsUtil.h"

/**
 * Processes a single Modifies clause in a PQL query.
 *
 * @param synonym The name that is to be selected.
 * @param stClause The Modifies clause to evaluate.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @return Results for the synonym in the Modifies clause.
 */
Void evaluateModifiesClause(Reference leftRef, Reference rightRef, ResultsTable* resultsTable)
{
    ClauseResult result;
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();

    if (canMatchOnlyOne(leftRefType) && canMatchMultiple(rightRefType)) {
        result = leftRefType == IntegerRefType ? getModifiesVariablesFromStatement(std::stoi(leftRef.getValue()))
                                               : getModifiesVariablesFromProcedure(leftRef.getValue());
    } else if (canMatchMultiple(leftRefType) && canMatchOnlyOne(rightRefType)) {
        if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            result = convertToClauseResult(getModifiesStatements(
                rightRef.getValue(),
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else {
            // declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType() == ProcedureType
            result = getModifiesProcedures(rightRef.getValue());
        }
    } else if (canMatchOnlyOne(leftRefType) && canMatchOnlyOne(rightRefType)) {
        Boolean modifies;
        if (leftRefType == IntegerRefType) {
            modifies = checkIfStatementModifies(std::stoi(leftRef.getValue()), rightRef.getValue());
        } else {
            // leftRefType == LiteralRefType
            modifies = checkIfProcedureModifies(leftRef.getValue(), rightRef.getValue());
        }
        if (modifies) {
            result.push_back("trueModifies");
        }
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        if (leftRef.getValue() == synonym
            && isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select stmt
            result = convertToClauseResult(getAllModifiesStatements(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else if (leftRef.getValue() == synonym) {
            // select procedure
            result = getAllModifiesProcedures();
        } else if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select variable with statement
            result = getAllModifiesVariables(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType()));
        } else {
            // select variable with procedure
            result = getAllModifiesVariables(leftRef.getValue());
        }
    } else {
        throw std::runtime_error("Error in evaluateModifiesClause: invalid arguments in Modifies");
    }
    return result;
}
