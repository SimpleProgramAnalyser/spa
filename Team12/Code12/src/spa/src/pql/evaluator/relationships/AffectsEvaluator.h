/**
 * Method to evaluate such that clauses
 * with Affects relationships
 */
#ifndef SPA_PQL_AFFECTS_EVALUATOR_H
#define SPA_PQL_AFFECTS_EVALUATOR_H

#include "pql/evaluator/ResultsTable.h"

class AffectsEvaluator {
private:
    ResultsTable& resultsTable;
    // case where left is known (integer), right is variable
    Void evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef) const;
    // case where left is variable, right is known (integer)
    Void evaluateRightKnown(const Reference& leftRef, Integer rightRefVal) const;
    // case where both are variable
    Void evaluateBothAny(const Reference& leftRef, const Reference& rightRef) const;                                                                                    tRef) const;
    // case where both are known
    Void evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const;

public:
    explicit AffectsEvaluator(ResultsTable& resultsTable);
    Void evaluateAffectsClause(const Reference& leftRef, const Reference& rightRef);
    Void evaluateAffectsStarClause(const Reference& leftRef, const Reference& rightRef);
};

#endif // SPA_PQL_AFFECTS_EVALUATOR_H
