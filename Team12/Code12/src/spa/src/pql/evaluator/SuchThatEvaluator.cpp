/**
 * Implementation of Such That Evaluator for Query
 * Evaluator, used to evaluate Such That clauses in
 * queries given to SIMPLE Program Analyser.
 */
#include "SuchThatEvaluator.h"

#include "relationships/FollowsEvaluator.h"
#include "relationships/ModifiesEvaluator.h"
#include "relationships/ParentEvaluator.h"
#include "relationships/UsesEvaluator.h"

Void evaluateSuchThat(SuchThatClause* stClause, ResultsTable* resultsTable)
{
    Relationship rel = stClause->getRelationship();
    RelationshipReferenceType relRefType = rel.getRelationship();
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
    switch (relRefType) {
    case FollowsType:
        return evaluateFollowsClause(leftRef, rightRef, false, resultsTable);
    case FollowsStarType:
        return evaluateFollowsClause(leftRef, rightRef, true, resultsTable);
    case ParentType:
        return evaluateParentClause(leftRef, rightRef, false, resultsTable);
    case ParentStarType:
        return evaluateParentClause(leftRef, rightRef, true, resultsTable);
    case UsesType:
    case UsesStatementType:
    case UsesProcedureType:
        return evaluateUsesClause(leftRef, rightRef, resultsTable);
    case ModifiesType:
    case ModifiesStatementType:
    case ModifiesProcedureType:
        return evaluateModifiesClause(leftRef, rightRef, resultsTable);
    default:
        throw std::runtime_error("Unknown relationship type in evaluateSuchThat");
    }
}
