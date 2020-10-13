/**
 * Implementation of Such That Evaluator for Query
 * Evaluator, used to evaluate Such That clauses in
 * queries given to SIMPLE Program Analyser.
 */
#include "SuchThatEvaluator.h"

#include <stdexcept>
#include <typeindex>

#include "relationships/FollowsEvaluator.h"
#include "relationships/ModifiesEvaluator.h"
#include "relationships/ParentEvaluator.h"
#include "relationships/UsesEvaluator.h"

void evaluateFollowsNormal(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    evaluateFollowsClause(leftRef, rightRef, false, resultsTable);
}

void evaluateFollowsTransitive(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    evaluateFollowsClause(leftRef, rightRef, true, resultsTable);
}

void evaluateParentNormal(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    evaluateParentClause(leftRef, rightRef, false, resultsTable);
}

void evaluateParentTransitive(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    evaluateParentClause(leftRef, rightRef, true, resultsTable);
}

std::unordered_map<RelationshipReferenceType, auto (*)(const Reference&, const Reference&, ResultsTable*)->void>
getEvaluatorMap()
{
    return std::unordered_map<RelationshipReferenceType,
                              auto (*)(const Reference&, const Reference&, ResultsTable*)->void>(
        {{FollowsType, evaluateFollowsNormal},
         {FollowsStarType, evaluateFollowsTransitive},
         {ParentType, evaluateParentNormal},
         {ParentStarType, evaluateParentTransitive},
         {UsesType, evaluateUsesClause},
         {UsesProcedureType, evaluateUsesClause},
         {UsesStatementType, evaluateUsesClause},
         {ModifiesType, evaluateModifiesClause},
         {ModifiesStatementType, evaluateModifiesClause},
         {ModifiesProcedureType, evaluateModifiesClause}});
}

Void evaluateSuchThat(SuchThatClause* stClause, ResultsTable* resultsTable)
{
    RelationshipReferenceType relRefType = stClause->getRelationship().getType();
    std::unordered_map<RelationshipReferenceType, auto (*)(const Reference&, const Reference&, ResultsTable*)->void>
        evaluatorMap = getEvaluatorMap();
    auto mapEntry = evaluatorMap.find(relRefType);
    if (mapEntry == evaluatorMap.end()) {
        throw std::runtime_error("Unknown relationship type in evaluateSuchThat");
    } else {
        mapEntry->second(stClause->getRelationship().getLeftRef(), stClause->getRelationship().getRightRef(),
                         resultsTable);
    }
}
