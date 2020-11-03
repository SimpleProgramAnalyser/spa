/**
 * Methods to evaluate AffectsBip and AffectsBip* relationships.
 */

#ifndef SPA_PQL_AFFECTS_BIP_EVALUATOR_H
#define SPA_PQL_AFFECTS_BIP_EVALUATOR_H

#include "AffectsBipFacade.h"
#include "AffectsEvaluator.h"

// Forward declaration of an optional statement number
class MaybeStatementNumber;

// A class representing the exact position of a statement
// number within the CFG with branching into procedures
class StatementPositionInCfg;

// Hash function for a StatementPosition
class StatementPositionHasher;

class AffectsBipEvaluator: public AffectsEvaluator {
private:
    AffectsBipFacade* bipFacade;
    // cache for AffectsBip*(modifier, user)
    CacheTable cacheModifierBipStarTable;      // cache table to store AffectsBip*(a, _) for known a
    CacheSet exploredModifierBipStarAssigns;   // cache set to tell if ALL AffectsBip*(a, _) is stored in table
    Vector<Integer> allModifierBipStarAssigns; // vectors to store every result for AffectsBip*(_, _)
    Vector<Integer> allUserBipStarAssigns;
    Vector<Pair<Integer, Integer>> allAffectsBipStarTuples;
    bool bipStarCacheFullyPopulated;

    // Helper methods for AffectsBip*
    Vector<StatementPositionInCfg> findAllCorrespondingPositions(Integer statementToFind);
    Boolean affectsBipSearch(Integer startingStmtNum, StatementPositionInCfg startingPosition, Boolean ignoreStarting,
                             std::unordered_set<StatementPositionInCfg, StatementPositionHasher>& visitedAssigns,
                             MaybeStatementNumber endValue);
    Void cacheModifierBipStarAssigns(Integer leftRefVal);

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
     */
    explicit AffectsBipEvaluator(ResultsTable& resultsTable, AffectsBipFacade* facade);
};

#endif // SPA_PQL_AFFECTS_BIP_EVALUATOR_H
