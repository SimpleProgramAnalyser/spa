/**
 * Implementation of an evaluator for a such that clause that has
 * Affects and Affect* relationships between design entities
 */
#include "AffectsEvaluator.h"

Void AffectsEvaluator::evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef) const {}

Void AffectsEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal) const {}

Void AffectsEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef) const {}

Void AffectsEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const {}

AffectsEvaluator::AffectsEvaluator(ResultsTable& resultsTable): resultsTable(resultsTable) {}

Void AffectsEvaluator::evaluateAffectsClause(const Reference& leftRef, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateAffectsStarClause(const Reference& leftRef, const Reference& rightRef) {}
