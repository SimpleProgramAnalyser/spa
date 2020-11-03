/**
 * Methods to evaluate AffectsBip and AffectsBip* relationships.
 */

#ifndef SPA_PQL_AFFECTS_BIP_EVALUATOR_H
#define SPA_PQL_AFFECTS_BIP_EVALUATOR_H

#include "AffectsEvaluator.h"

class AffectsBipEvaluator: public AffectsEvaluator {
private:
    NextEvaluator& nextBipEvaluator;

    // Methods for AffectsBip*
    Void evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef) override;
    Void evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal) override;
    Void evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef) override;
    Void evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal) override;

public:
    AffectsBipEvaluator(AffectsBipEvaluator&&) = default;
    AffectsBipEvaluator(const AffectsBipEvaluator&) = delete;
    ~AffectsBipEvaluator() override = default;
    AffectsBipEvaluator& operator=(const AffectsBipEvaluator&) = delete;
    AffectsBipEvaluator& operator=(AffectsBipEvaluator&&) = delete;

    /**
     * Constructs a new AffectsBipEvaluator.
     *
     * @param resultsTable The results table for the Evaluator to
     *                     store results in after evaluation.
     * @param facade The facade used by AffectsBipEvaluator to access
     *               other components of SIMPLE program analyser.
     *               Note that this pointer will be managed by Affects BIP
     *               Evaluator (the Facade will be deleted at the end
     *               of the lifetime of the parent AffectsBipEvaluator).
     * @param nextBipEvaluator The evaluator used by AffectsBipEvaluator
     *                         to check whether NextBip* holds, as required
     *                         by the definition of AffectsBip*.
     */
    explicit AffectsBipEvaluator(ResultsTable& resultsTable, AffectsEvaluatorFacade* facade,
                                 NextEvaluator& nextBipEvaluator);
};

#endif // SPA_PQL_AFFECTS_BIP_EVALUATOR_H
