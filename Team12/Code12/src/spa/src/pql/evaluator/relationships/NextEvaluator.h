/**
 * Method to evaluate such that clauses
 * with Next relationships
 */
#ifndef SPA_PQL_NEXT_EVALUATOR_H
#define SPA_PQL_NEXT_EVALUATOR_H

#include "CacheSet.h"
#include "CacheTable.h"
#include "pql/evaluator/ResultsTable.h"

class NextEvaluator {
private:
    ResultsTable& resultsTable;
    // caching of intermediate Next* clauses results
    CacheTable cacheNextStarTable;
    CacheTable cachePrevStarTable;
    CacheSet visitedNextStatements;
    CacheSet visitedPrevStatements;

    // case where left is known (integer), right is variable
    Void evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef) const;
    // case where left is variable, right is known (integer)
    Void evaluateRightKnown(const Reference& leftRef, Integer rightRefVal) const;
    // case where both are variable
    Void evaluateBothAny(const Reference& leftRef, const Reference& rightRef) const;
    // case where both are known
    Void evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const;

    Void evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef);
    Void evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal);
    Void evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef) const;
    Void evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal) const;
    // DFS method to search and cache all Next* relationship for stmtNum
    CacheSet getCacheNextStatement(StatementNumber stmtNum);
    CacheSet getCachePrevStatement(StatementNumber stmtNum);
    Boolean isNextVisited(StatementNumber stmtNum) const;
    Boolean isPrevVisited(StatementNumber stmtNum) const;

public:
    explicit NextEvaluator(ResultsTable& resultsTable);
    Void evaluateNextClause(const Reference& leftRef, const Reference& rightRef);
    Void evaluateNextStarClause(const Reference& leftRef, const Reference& rightRef);
};

#endif // SPA_PQL_NEXT_EVALUATOR_H
