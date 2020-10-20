/**
 * Method to evaluate such that clauses
 * with Affects relationships
 */
#ifndef SPA_PQL_AFFECTS_EVALUATOR_H
#define SPA_PQL_AFFECTS_EVALUATOR_H

#include "CacheTable.h"
#include "pql/evaluator/ResultsTable.h"

class AffectsEvaluator {
private:
    ResultsTable& resultsTable;

    // Cache for Affects(modifier, user)
    CacheTable cacheUserTable;
    CacheTable cacheModifierTable;
    CacheSet exploredUserAssigns;
    CacheSet exploredModifierAssigns;

    // Cache for Affects*(modifierStar, userStar)
    CacheTable cacheUserStarTable;
    CacheTable cacheModifierStarTable;
    CacheSet exploredUserStarAssigns;
    CacheSet exploredModifierStarAssigns;

    // Methods for Affects
    Void evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef);
    Void evaluateRightKnown(const Reference& leftRef, Integer rightRefVal);
    Void evaluateBothAny(const Reference& leftRef, const Reference& rightRef);
    Void evaluateBothKnown(Integer leftRefVal, Integer rightRefVal);

    // Methods for Affects*
    Void evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef);
    Void evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal);
    Void evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef);
    Void evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal);

public:
    explicit AffectsEvaluator(ResultsTable& resultsTable);
    Void evaluateAffectsClause(const Reference& leftRef, const Reference& rightRef);
    Void evaluateAffectsStarClause(const Reference& leftRef, const Reference& rightRef);
};

#endif // SPA_PQL_AFFECTS_EVALUATOR_H
