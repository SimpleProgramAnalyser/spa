/**
 * Implementation of an evaluator for a such that clause that
 * has Next and Next* relationships between design entities
 */
#include "NextEvaluator.h"

Void NextEvaluator::evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef) const {}

Void NextEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal) const {}

Void NextEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef) const {}

Void NextEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const {}

NextEvaluator::NextEvaluator(ResultsTable& resultsTable): resultsTable(resultsTable) {}

Void NextEvaluator::evaluateNextClause(const Reference& leftRef, const Reference& rightRef) {}

Void NextEvaluator::evaluateNextStarClause(const Reference& leftRef, const Reference& rightRef) {}
