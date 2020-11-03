/**
 * Implementation of an evaluator for a such that clause that
 * has NextBip and NextBip* relationships between design entities.
 */

#include "NextBipEvaluator.h"

Void NextBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    // TODO
    NextEvaluator::evaluateLeftKnownStar(leftRefVal, rightRef);
}

Void NextBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    // TODO
    NextEvaluator::evaluateRightKnownStar(leftRef, rightRefVal);
}

Void NextBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    // TODO
    NextEvaluator::evaluateBothAnyStar(leftRef, rightRef);
}

Void NextBipEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    // TODO
    NextEvaluator::evaluateBothKnownStar(leftRefVal, rightRefVal);
}

NextBipEvaluator::NextBipEvaluator(ResultsTable& resultsTable, NextEvaluatorFacade* facade):
    NextEvaluator(resultsTable, facade)
{}
