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
    ResultsTable& resultsTable;
    // The facade which this Affects Evaluator uses to interact
    // with components outside of Query Processor (i.e. PKB)
    std::unique_ptr<AffectsEvaluatorFacade> facade;

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
    CacheTable partiallyCacheUserStarTable;
    CacheTable partiallyCacheModifierStarTable;
    CacheSet exploredUserStarAssigns;
    CacheSet exploredModifierStarAssigns;
    CacheSet visitedUserStarAssigns;
    CacheSet visitedModifierStarAssigns;

    // Helper methods for Affects
    const CfgNode* affectsSearch(const CfgNode* cfg,
                                 std::unordered_map<String, std::unordered_set<Integer>>& affectsMap,
                                 AffectsTuple& resultsLists);
    Void cacheModifierAssigns(Integer leftRefVal);
    Void cacheUserAssigns(Integer rightRefVal, Vector<String> usedFromPkb);
    Void cacheAll();

    // Helper methods for Affects*
    CacheSet getCacheModifierStarStatement(StatementNumber stmtNum, StatementNumber prevStmtNum);
    CacheSet getCacheUserStarStatement(StatementNumber stmtNum, StatementNumber prevStmtNum);

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
