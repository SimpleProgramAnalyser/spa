/**
 * Implementation of Such That Evaluator for Query
 * Evaluator, used to evaluate Such That clauses in
 * queries given to SIMPLE Program Analyser.
 */
#include "SuchThatEvaluator.h"

#include <stdexcept>

#include "CallsEvaluator.h"
#include "FollowsEvaluator.h"
#include "ModifiesEvaluator.h"
#include "ParentEvaluator.h"
#include "UsesEvaluator.h"
#include "affects/AffectsEvaluator.h"
#include "next/NextEvaluator.h"

void evaluateAffectsNormal(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getAffectsEvaluator()->evaluateAffectsClause(leftRef, rightRef);
}

void evaluateAffectsTransitive(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getAffectsEvaluator()->evaluateAffectsStarClause(leftRef, rightRef);
}

void evaluateCallsNormal(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    evaluateCallsClause(leftRef, rightRef, false, resultsTable);
}

void evaluateCallsTransitive(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    evaluateCallsClause(leftRef, rightRef, true, resultsTable);
}

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

void evaluateNextNormal(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getNextEvaluator()->evaluateNextClause(leftRef, rightRef);
}

void evaluateNextTransitive(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getNextEvaluator()->evaluateNextStarClause(leftRef, rightRef);
}

// ======================================== Branch Into Procedure (BIP) ========================================

void evaluateAffectsBip(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getAffectsBipEvaluator()->evaluateAffectsClause(leftRef, rightRef);
}

void evaluateAffectsBipTransitive(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getAffectsBipEvaluator()->evaluateAffectsStarClause(leftRef, rightRef);
}

void evaluateNextBip(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getNextBipEvaluator()->evaluateNextClause(leftRef, rightRef);
}

void evaluateNextBipTransitive(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    resultsTable->getNextBipEvaluator()->evaluateNextStarClause(leftRef, rightRef);
}

std::unordered_map<RelationshipType, auto (*)(const Reference&, const Reference&, ResultsTable*)->void>
getSuchThatEvaluatorMap()
{
    return std::unordered_map<RelationshipType, auto (*)(const Reference&, const Reference&, ResultsTable*)->void>(
        {{AffectsType, evaluateAffectsNormal},
         {AffectsStarType, evaluateAffectsTransitive},
         {CallsType, evaluateCallsNormal},
         {CallsStarType, evaluateCallsTransitive},
         {FollowsType, evaluateFollowsNormal},
         {FollowsStarType, evaluateFollowsTransitive},
         {ParentType, evaluateParentNormal},
         {ParentStarType, evaluateParentTransitive},
         {UsesType, evaluateUsesClause},
         {UsesProcedureType, evaluateUsesClause},
         {UsesStatementType, evaluateUsesClause},
         {ModifiesType, evaluateModifiesClause},
         {ModifiesStatementType, evaluateModifiesClause},
         {ModifiesProcedureType, evaluateModifiesClause},
         {NextType, evaluateNextNormal},
         {NextStarType, evaluateNextTransitive},
         // branch into procedures (BIP)
         {AffectsBipType, evaluateAffectsBip},
         {AffectsBipStarType, evaluateAffectsBipTransitive},
         {NextBipType, evaluateNextBip},
         {NextBipStarType, evaluateNextBipTransitive}});
}

Void evaluateSuchThat(SuchThatClause* stClause, ResultsTable* resultsTable)
{
    RelationshipType relRefType = stClause->getRelationship().getType();
    std::unordered_map<RelationshipType, auto (*)(const Reference&, const Reference&, ResultsTable*)->void> evaluatorMap
        = getSuchThatEvaluatorMap();
    auto mapEntry = evaluatorMap.find(relRefType);
    if (mapEntry == evaluatorMap.end()) {
        throw std::runtime_error("Unknown relationship type in evaluateSuchThat");
    } else {
        mapEntry->second(stClause->getRelationship().getLeftRef(), stClause->getRelationship().getRightRef(),
                         resultsTable);
    }
}
