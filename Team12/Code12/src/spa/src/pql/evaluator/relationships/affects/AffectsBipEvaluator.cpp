/**
 * Implementation of an evaluator for a such that clause that has
 * AffectsBip and AffectsBip* relationships between design entities.
 */

#include "AffectsBipEvaluator.h"

#include "AffectsUtils.h"
#include "pql/evaluator/relationships/next/NextEvaluator.h"

// Helper class to express an optional statement number
// to terminate early, when searching for AffectsBip.
class MaybeStatementNumber {
private:
    Boolean isNothing;
    Integer statementNumber;
    MaybeStatementNumber(): isNothing(true), statementNumber(0) {}
    explicit MaybeStatementNumber(Integer stmtNum): isNothing(false), statementNumber(stmtNum) {}

public:
    static MaybeStatementNumber just(Integer stmtNum)
    {
        return MaybeStatementNumber(stmtNum);
    }

    static MaybeStatementNumber nothing()
    {
        return {};
    }

    Boolean isMatched(Integer stmtNum) const
    {
        return !isNothing && stmtNum == statementNumber;
    }
};

/**
 * Searches the CFG BIP for the statement with number
 * endValue, and returns true if the value is found.
 * The search will recursively go through nodes that
 * have an AffectsBip(starting, _) relationship.
 *
 * @param startingStmtNum The initial statement number, that
 *                        refers to a statement represented
 *                        in the CFG at startingNode.
 * @param startingPosition A pair of the node of CFG BIP and
 *                         the index of the initial startingStmtNum
 *                         in the list of statements of the CFG
 *                         BIP node, at which to start the
 *                         search for endValue.
 * @param ignoreStarting Whether the starting position is to be
 *                       ignored by the visited set, such that
 *                       the search will be able to go back to
 *                       the starting position (and only the
 *                       starting position, nothing else). Set
 *                       to false to disable re-visiting.
 * @param visitedAssigns Set of all assignment statements visited
 *                       along the search, to prevent infinite loop.
 * @param endValue The statement number to terminate the search.
 *                 If this number is nothing, this method only
 *                 terminates when all CFG BIP paths have been
 *                 searched, and always return false.
 * @return True, if the endValue is found.
 */
Boolean AffectsBipEvaluator::affectsBipSearch(
    Integer startingStmtNum, StatementPositionInCfg startingPosition, Boolean ignoreStarting,
    std::unordered_set<StatementPositionInCfg, StatementPositionHasher>& visitedAssigns, MaybeStatementNumber endValue)
{
    // ignore, if we have searched this position before
    if (visitedAssigns.find(startingPosition) != visitedAssigns.end()) {
        return false;
    }
    if (!ignoreStarting) {
        visitedAssigns.insert(startingPosition);
    }

    // Find all s that match AffectsBip(starting, s)
    cacheAll();
    Vector<Integer> affectedBipStatements = getModifierAssigns(startingStmtNum).toVector();
    // Terminate when endValue is found
    Boolean foundEndValue = false;
    for (Integer affectedStatement : affectedBipStatements) {
        std::unordered_set<StatementPositionInCfg, StatementPositionHasher> uniqueStatementsVisited;
        Vector<StatementPositionInCfg> correspondingPositions
            = findCorrespondingNodes(startingPosition, affectedStatement, uniqueStatementsVisited);
        Boolean foundCorrespondingNode = !correspondingPositions.empty();

        if (foundCorrespondingNode && endValue.isMatched(affectedStatement)) {
            // we found the end value and it is reachable
            foundEndValue = true;
            break;
        }

        if (foundCorrespondingNode) {
            // did not find end value, but there are still nodes we can search
            for (StatementPositionInCfg position : correspondingPositions) {
                foundEndValue = affectsBipSearch(affectedStatement, position, false, visitedAssigns, endValue);
                if (foundEndValue) {
                    break;
                }
            }
            if (foundEndValue) {
                break;
            }
        }
    }
    return foundEndValue;
}

/**
 * Caches all statements that match s in AffectsBip*(leftRefVal, s),
 * and returns a vector of all such statements.
 */
Vector<Integer> AffectsBipEvaluator::cacheModifierBipStarAssigns(Integer leftRefVal)
{
    // find all positions of leftRefVal
    Vector<StatementPositionInCfg> allPositions = findAllCorrespondingPositions(leftRefVal, *bipFacade);
    std::unordered_set<Integer> matchingStatements;
    for (StatementPositionInCfg position : allPositions) {
        std::unordered_set<StatementPositionInCfg, StatementPositionHasher> visitedAssignsWithinProcedure;
        affectsBipSearch(leftRefVal, position, true, visitedAssignsWithinProcedure, MaybeStatementNumber::nothing());
        for (const StatementPositionInCfg& visitedAssigns : visitedAssignsWithinProcedure) {
            matchingStatements.insert(visitedAssigns.getStatementNumber());
        }
    }
    cacheModifierBipStarTable.insert(leftRefVal, CacheSet(matchingStatements));
    exploredModifierBipStarAssigns.insert(leftRefVal);
    return Vector<Integer>(matchingStatements.begin(), matchingStatements.end());
}

/**
 * Caches all statements that match <s1, s2> in AffectsBip*(s1, s2).
 */
Void AffectsBipEvaluator::cacheAllBipStar()
{
    std::unordered_set<Integer> uniqueAffectedUsers;
    Vector<Integer> allAssigns = facade->getAssigns();
    for (Integer assignStmt : allAssigns) {
        Vector<Integer> resultsForThisAssign = cacheModifierBipStarAssigns(assignStmt);
        if (!resultsForThisAssign.empty()) {
            allModifierBipStarAssigns.push_back(assignStmt);
            for (Integer affectedUser : resultsForThisAssign) {
                uniqueAffectedUsers.insert(affectedUser);
                allAffectsBipStarTuples.emplace_back(assignStmt, affectedUser);
            }
        }
    }
    bipStarCacheFullyPopulated = true;
}

Void AffectsBipEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal)
{
    cacheAll();
    CacheSet resultsForLeft = getModifierAssigns(leftRefVal);
    resultsTable.storeResultsZero(resultsForLeft.hasCached(rightRefVal));
}

Void AffectsBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    if (facade->getType(leftRefVal) != AssignmentStatement || !isAffectable(rightRef)) {
        resultsTable.storeResultsZero(false);
        return;
    }

    if (!exploredModifierBipStarAssigns.hasCached(leftRefVal)) {
        cacheModifierBipStarAssigns(leftRefVal);
    }

    resultsTable.storeResultsOne(rightRef, cacheModifierBipStarTable.get(leftRefVal).toClauseResult());
}

Void AffectsBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    // hard to do this without finding all results, since
    // the CFG cannot be traversed backwards currently
    cacheAllBipStar();
    std::unordered_set<Integer> affectedUsers;
    for (const Pair<Integer, Integer>& affectsBipStarTuple : allAffectsBipStarTuples) {
        if (affectsBipStarTuple.second == rightRefVal) {
            affectedUsers.insert(affectsBipStarTuple.first);
        }
    }
    resultsTable.storeResultsOne(leftRef,
                                 convertToClauseResult(Vector<Integer>(affectedUsers.begin(), affectedUsers.end())));
}

Void AffectsBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    cacheAllBipStar();
    resultsTable.storeResultsTwo(leftRef, convertToClauseResult(allModifierBipStarAssigns), rightRef,
                                 convertToClauseResult(allUserBipStarAssigns),
                                 convertToPairedResult(allAffectsBipStarTuples));
}

Void AffectsBipEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    if (facade->getType(leftRefVal) != AssignmentStatement || facade->getType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
        return;
    }
    // check cache
    if (bipStarCacheFullyPopulated) {
        resultsTable.storeResultsZero(cacheModifierBipStarTable.check(leftRefVal, rightRefVal));
        return;
    }
    if (cacheModifierBipStarTable.check(leftRefVal, rightRefVal)) {
        resultsTable.storeResultsZero(true);
        return;
    }
    // find all positions of leftRefVal
    Vector<StatementPositionInCfg> allPositions = findAllCorrespondingPositions(leftRefVal, *bipFacade);
    Boolean matchedRightRef = false;
    for (StatementPositionInCfg position : allPositions) {
        std::unordered_set<StatementPositionInCfg, StatementPositionHasher> visitedAssignsWithinProcedure;
        if (affectsBipSearch(leftRefVal, position, true, visitedAssignsWithinProcedure,
                             MaybeStatementNumber::just(rightRefVal))) {
            matchedRightRef = true;
            break;
        }
    }
    if (matchedRightRef) {
        cacheModifierBipStarTable.insertPartial(rightRefVal, leftRefVal);
    }
    resultsTable.storeResultsZero(matchedRightRef);
}

Void AffectsBipEvaluator::cacheModifierAssigns(Integer)
{
    // For Affects BIP, we don't use the Next BIP table as we cannot
    // tell which exact call path the statement number belongs to.
    //
    // Instead we traverse the CFG BIP manually to find all results.
    cacheAll();
}

Void AffectsBipEvaluator::cacheUserAssigns(Integer, Vector<String>)
{
    // For Affects BIP, we don't use the Next BIP table as we cannot
    // tell which exact call path the statement number belongs to.
    //
    // Instead we traverse the CFG BIP manually to find all results.
    cacheAll();
}

AffectsBipEvaluator::AffectsBipEvaluator(ResultsTable& resultsTable, AffectsBipFacade* facade):
    AffectsEvaluator(resultsTable, facade), bipFacade(facade), cacheModifierBipStarTable(),
    exploredModifierBipStarAssigns(), allModifierBipStarAssigns(), allUserBipStarAssigns(), allAffectsBipStarTuples(),
    bipStarCacheFullyPopulated(false)
{}
