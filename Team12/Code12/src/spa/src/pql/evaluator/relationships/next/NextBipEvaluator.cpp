/**
 * Implementation of an evaluator for a such that clause that
 * has NextBip and NextBip* relationships between design entities.
 */

#include "NextBipEvaluator.h"

Void NextBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    NextEvaluator::evaluateLeftKnownStar(leftRefVal, rightRef);
}

Void NextBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    NextEvaluator::evaluateRightKnownStar(leftRef, rightRefVal);
}

Void NextBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    NextEvaluator::evaluateBothAnyStar(leftRef, rightRef);
}

Void NextBipEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    NextEvaluator::evaluateBothKnownStar(leftRefVal, rightRefVal);
}

NextBipEvaluator::NextBipEvaluator(ResultsTable& resultsTable, NextEvaluatorFacade* facade):
    NextEvaluator(resultsTable, facade)
{}
