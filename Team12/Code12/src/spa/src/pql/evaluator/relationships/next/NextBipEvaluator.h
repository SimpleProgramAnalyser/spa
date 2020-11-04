/**
 * Methods to evaluate NextBip and NextBip* relationships.
 */

#ifndef SPA_PQL_NEXT_BIP_EVALUATOR_H
#define SPA_PQL_NEXT_BIP_EVALUATOR_H

#include "NextBipFacade.h"
#include "NextEvaluator.h"

class NextBipEvaluator: public NextEvaluator {
private:
    // Methods for NextBip*
    Void evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef) override;
    Void evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal) override;
    Void evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef) override;
    Void evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal) override;

public:
    NextBipEvaluator(NextBipEvaluator&&) = default;
    NextBipEvaluator(const NextBipEvaluator&) = delete;
    ~NextBipEvaluator() override = default;
    NextBipEvaluator& operator=(const NextBipEvaluator&) = delete;
    NextBipEvaluator& operator=(NextBipEvaluator&&) = delete;

    /**
     * Constructs a new NextBipEvaluator.
     *
     * @param resultsTable The results table for the Evaluator to
     *                     store results in after evaluation.
     * @param facade The facade used by NextBipEvaluator to access
     *               other components of SIMPLE program analyser.
     *               Note that this pointer will be managed by Next BIP
     *               Evaluator (the Facade will be deleted at the end
     *               of the lifetime of the parent NextBipEvaluator).
     */
    explicit NextBipEvaluator(ResultsTable& resultsTable, NextEvaluatorFacade* facade);

    /**
     * Filters out invalid transitive closure NextBip* relationships.
     *
     * @param procName  ProcedureName of the containing procedure
     *                  known reference.
     * @param isCaller  True if known reference's containing
     *                  procedure is the caller.
     * @param list      Original list of NextBip* results with
     *                  transitive closure.
     */
    Vector<StatementNumber> filterValidResults(ProcedureName procName, Boolean isCaller,
                                                                 Vector<StatementNumber> list);
};

#endif // SPA_PQL_NEXT_BIP_EVALUATOR_H
