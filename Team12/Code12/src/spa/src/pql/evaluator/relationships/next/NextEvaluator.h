/**
 * Method to evaluate such that clauses
 * with Next relationships
 */
#ifndef SPA_PQL_NEXT_EVALUATOR_H
#define SPA_PQL_NEXT_EVALUATOR_H

#include "NextEvaluatorFacade.h"
#include "pql/evaluator/ResultsTable.h"
#include "pql/evaluator/relationships/CacheSet.h"
#include "pql/evaluator/relationships/CacheTable.h"

class NextEvaluator {
private:
    // caching of intermediate Next* clauses results
    CacheTable cacheNextStarTable;
    CacheTable cachePrevStarTable;
    CacheSet exploredNextStatements;
    CacheSet exploredPrevStatements;

    // case where left is known (integer), right is variable
    Void evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef) const;
    // case where left is variable, right is known (integer)
    Void evaluateRightKnown(const Reference& leftRef, Integer rightRefVal) const;
    // case where both are variable
    Void evaluateBothAny(const Reference& leftRef, const Reference& rightRef) const;
    // case where both are known
    Void evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const;

    // DFS method to search and cache all Next* relationship for stmtNum.
    // If currentWhileStmtNum is set to -1, that means the function is
    // currently not operating on a statement in a while statement list.
    CacheSet getCacheNextStatement(StatementNumber stmtNum);
    CacheSet getCachePrevStatement(StatementNumber stmtNum);

protected:
    ResultsTable& resultsTable;
    // The facade which this Next Evaluator uses to interact
    // with components outside of Query Processor (i.e. PKB)
    std::unique_ptr<NextEvaluatorFacade> facade;

    // Allow AffectsBipEvaluator to call internal methods
    friend class AffectsBipEvaluator;

    virtual Void evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef);
    virtual Void evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal);
    virtual Void evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef);
    virtual Void evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal);

public:
    NextEvaluator(NextEvaluator&&) = default;
    NextEvaluator(const NextEvaluator&) = delete;
    virtual ~NextEvaluator() = default;
    NextEvaluator& operator=(const NextEvaluator&) = delete;
    NextEvaluator& operator=(NextEvaluator&&) = delete;

    /**
     * Constructs a new NextEvaluator.
     *
     * @param resultsTable The results table for the Evaluator to
     *                     store results in after evaluation.
     * @param facade The facade used by NextEvaluator to access
     *               other components of SIMPLE program analyser.
     *               Note that this pointer will be managed by Next
     *               Evaluator (the Facade will be deleted at the end
     *               of the lifetime of the parent NextEvaluator)
     */
    explicit NextEvaluator(ResultsTable& resultsTable, NextEvaluatorFacade* facade);
    Void evaluateNextClause(const Reference& leftRef, const Reference& rightRef);
    Void evaluateNextStarClause(const Reference& leftRef, const Reference& rightRef);
};

#endif // SPA_PQL_NEXT_EVALUATOR_H
