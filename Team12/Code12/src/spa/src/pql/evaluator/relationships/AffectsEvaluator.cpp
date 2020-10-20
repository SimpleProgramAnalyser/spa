/**
 * Implementation of an evaluator for a such that clause that has
 * Affects and Affect* relationships between design entities
 */
#include "AffectsEvaluator.h"

Void AffectsEvaluator::evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal) {}

Void AffectsEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) {}

Void AffectsEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal) {}

Void AffectsEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal) {}

AffectsEvaluator::AffectsEvaluator(ResultsTable& resultsTable): resultsTable(resultsTable) {}

Void AffectsEvaluator::evaluateAffectsClause(const Reference& leftRef, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateAffectsStarClause(const Reference& leftRef, const Reference& rightRef) {}
