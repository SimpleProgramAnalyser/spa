/**
 * Method to evaluate such that clauses
 * with Affects relationships
 */
#ifndef SPA_PQL_AFFECTS_EVALUATOR_H
#define SPA_PQL_AFFECTS_EVALUATOR_H

#include "AffectsEvaluatorFacade.h"
#include "cfg/CfgTypes.h"
#include "pql/evaluator/ResultsTable.h"
#include "pql/evaluator/relationships/CacheTable.h"

/**
 * A class to hold result lists for Affects.
 */
class AffectsTuple {
private:
    std::unordered_set<Integer> modifyingStatementResults;
    std::unordered_set<Integer> usingStatementResults;
    Vector<Pair<Integer, Integer>> affectsResults;

public:
    /**
     * Adds an Affects relationship to the results lists.
     */
    Void addAffects(Integer modifyingStmt, Integer usingStmt);

    /**
     * Gets the list of results for modifying statements
     * (left reference in the Affects relationship).
     */
    Vector<Integer> getModifyingStatements() const;

    /**
     * Gets the list of results for using statements
     * (right reference in the Affects relationship).
     */
    Vector<Integer> getUsingStatements() const;

    /**
     * Gets the list of results for Affects relationships.
     */
    Vector<Pair<Integer, Integer>> getAffects() const;
};

class AffectsEvaluator {
private:
    // Cache for Affects(modifier, user)
    CacheTable cacheUserTable;          // cache tables to store individual Affects(a, b) for known a, b
    CacheTable cacheModifierTable;      //
    CacheSet exploredUserAssigns;       // cache set to tell if ALL Affects(a, ?) is stored in table for known a
    CacheSet exploredModifierAssigns;   // same for Affects(?, b)
    Vector<Integer> allModifierAssigns; // vectors to store every result for Affects(?, ?)
    Vector<Integer> allUserAssigns;
    Vector<Pair<Integer, Integer>> allAffectsTuples;
    bool cacheFullyPopulated = false;

    // Cache for Affects*(modifierStar, userStar)
    CacheTable cacheUserStarTable;
    CacheTable cacheModifierStarTable;
    CacheSet partiallyCacheUserStarSet;
    CacheSet partiallyCacheModifierStarSet;
    CacheSet exploredUserStarAssigns;
    CacheSet exploredModifierStarAssigns;
    CacheSet visitedUserStarAssigns;
    CacheSet visitedModifierStarAssigns;

    // Helper methods for Affects
    const CfgNode* affectsSearch(const CfgNode* cfg,
                                 std::unordered_map<String, std::unordered_set<Integer>>& affectsMap,
                                 AffectsTuple& resultsLists);

    // Helper methods for Affects*
    CacheSet getCacheModifierStarStatement(StatementNumber stmtNum);
    CacheSet getCacheUserStarStatement(StatementNumber stmtNum);
    CacheSet evaluateModifierStar(StatementNumber stmtNum);
    CacheSet evaluateUserStar(StatementNumber stmtNum);
    static void cleanup(CacheSet& partiallyCacheSet, CacheTable& cacheTable);

protected:
    ResultsTable& resultsTable;
    // The facade which this Affects Evaluator uses to interact
    // with components outside of Query Processor (i.e. PKB)
    std::unique_ptr<AffectsEvaluatorFacade> facade;

    // Methods for Affects
    virtual Void evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef);
    virtual Void evaluateRightKnown(const Reference& leftRef, Integer rightRefVal);
    virtual Void evaluateBothAny(const Reference& leftRef, const Reference& rightRef);
    virtual Void evaluateBothKnown(Integer leftRefVal, Integer rightRefVal);

    // Methods for Affects*
    virtual Void evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef);
    virtual Void evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal);
    virtual Void evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef);
    virtual Void evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal);

    /**
     * Runs the Affects search through the Control Flow Graph,
     * storing all results for Affects in the cache. If this
     * method is called more than once, calls after the first
     * will be ignored because the cache is already populated.
     *
     * In other words, this method caches all the modifiers
     * and users in the entire SIMPLE source program. This
     * caching is only done a maximum of 1 time per query.
     */
    Void cacheAll();

    /**
     * Caches all the statements that the
     * given statement affects.
     */
    virtual Void cacheModifierAssigns(Integer leftRefVal);

    /**
     * Caches all the statements that the
     * given statement is affected by.
     *
     * @param usedFromPkb List of variables used by the assignment
     *                    statement with number rightRefVal.
     */
    virtual Void cacheUserAssigns(Integer rightRefVal, Vector<String> usedFromPkb);

    /**
     * Gets the unique set of statements that
     * match Affects(stmtNum, _) from the cache.
     */
    CacheSet getModifierAssigns(Integer stmtNum);

public:
    AffectsEvaluator(AffectsEvaluator&&) = default;
    AffectsEvaluator(const AffectsEvaluator&) = delete;
    virtual ~AffectsEvaluator() = default;
    AffectsEvaluator& operator=(const AffectsEvaluator&) = delete;
    AffectsEvaluator& operator=(AffectsEvaluator&&) = delete;

    /**
     * Constructs a new AffectsEvaluator.
     *
     * @param resultsTable The results table for the Evaluator to
     *                     store results in after evaluation.
     * @param facade The facade used by AffectsEvaluator to access
     *               other components of SIMPLE program analyser.
     *               Note that this pointer will be managed by Affects
     *               Evaluator (the Facade will be deleted at the end
     *               of the lifetime of the parent AffectsEvaluator)
     */
    explicit AffectsEvaluator(ResultsTable& resultsTable, AffectsEvaluatorFacade* facade);
    Void evaluateAffectsClause(const Reference& leftRef, const Reference& rightRef);
    Void evaluateAffectsStarClause(const Reference& leftRef, const Reference& rightRef);
};

#endif // SPA_PQL_AFFECTS_EVALUATOR_H
