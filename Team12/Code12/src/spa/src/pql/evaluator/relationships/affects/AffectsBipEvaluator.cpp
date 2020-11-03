/**
 * Implementation of an evaluator for a such that clause that has
 * AffectsBip and AffectsBip* relationships between design entities.
 */

#include "AffectsBipEvaluator.h"

#include "pql/evaluator/relationships/next/NextEvaluator.h"

Void AffectsBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    AffectsEvaluator::evaluateLeftKnownStar(leftRefVal, rightRef);
    nextBipEvaluator.evaluateLeftKnownStar(leftRefVal, rightRef);
}

Void AffectsBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    AffectsEvaluator::evaluateRightKnownStar(leftRef, rightRefVal);
    nextBipEvaluator.evaluateRightKnownStar(leftRef, rightRefVal);
}

Void AffectsBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    AffectsEvaluator::evaluateBothAnyStar(leftRef, rightRef);
    nextBipEvaluator.evaluateBothAnyStar(leftRef, rightRef);
}

Void AffectsBipEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    AffectsEvaluator::evaluateBothKnownStar(leftRefVal, rightRefVal);
    nextBipEvaluator.evaluateBothKnownStar(leftRefVal, rightRefVal);
}

AffectsBipEvaluator::AffectsBipEvaluator(ResultsTable& resultsTable, AffectsEvaluatorFacade* facade,
                                         NextEvaluator& nextBipEvaluator):
    AffectsEvaluator(resultsTable, facade),
    nextBipEvaluator(nextBipEvaluator)
{}
