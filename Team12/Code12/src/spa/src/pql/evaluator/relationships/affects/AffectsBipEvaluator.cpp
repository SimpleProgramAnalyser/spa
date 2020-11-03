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

// Helper class to represent the exact manifestation of
// a statement number within the CFG BIP. There can be
// multiple statement positions for one statement, if
// the CFG BIP is duplicated by multiple calls.
class StatementPositionInCfg {
private:
    CfgNode* nodePosition;
    std::size_t statementIndex;

    friend class StatementPositionHasher;

public:
    /**
     * Constructor for a StatementPosition representing
     * the exact position of a statement instance in the
     * Control Flow Graph with branching into procedures.
     *
     * @param node Pointer to the CFG BIP node. If this
     *             is nullptr, it may indicate that the
     *             StatementPosition is invalid.
     * @param index Index of the statement within node.
     *              If this is negative, it may indicate
     *              that the StatementPosition is invalid.
     */
    StatementPositionInCfg(CfgNode* node, Integer index): nodePosition(node), statementIndex(index) {}

    /**
     * Constructor for a StatementPosition with a pair of
     * the node location and the statement index.
     */
    explicit StatementPositionInCfg(Pair<CfgNode*, Integer> positionPair):
        nodePosition(positionPair.first), statementIndex(positionPair.second)
    {}

    bool operator==(const StatementPositionInCfg& sp) const
    {
        return this->nodePosition == sp.nodePosition && this->statementIndex == sp.statementIndex;
    }

    CfgNode* getNodePosition() const
    {
        return nodePosition;
    }

    Integer getStatementIndex() const
    {
        return statementIndex;
    }

    Integer getStatementNumber() const
    {
        return nodePosition->statementNodes->at(statementIndex)->getStatementNumber();
    }
};

// Hash function for a StatementPosition.
class StatementPositionHasher {
public:
    std::size_t operator()(const StatementPositionInCfg& sp) const
    {
        std::size_t hashedPointer = std::hash<CfgNode*>()(sp.nodePosition);
        return (hashedPointer
                ^ (std::hash<Integer>()(sp.statementIndex) + uint32_t(2654435769) + (hashedPointer * 64)
                   + (hashedPointer / 4)));
    }
};

/**
 * Tries to find the statement with number specified
 * in the CFG BIP, given a pointer to the starting
 * node where the traversal is to begin from, and
 * the index of the statement in the node to begin.
 *
 * Ignores the statement specified at startingIndex,
 * so the method works for self-influencing statements.
 * To search all statements in the CfgNode, use a
 * value of -1 for startingIndex.
 *
 * @param startingPosition Starting node of the CFG BIP, with
 *                         the index to begin searching the AST
 *                         statement nodes of the CFG starting node.
 * @param statement The statement to be found.
 * @param visitedCfgNodes The set of visitedCfgNodes, to prevent
 *                        infinite recursion over a while loop.
 *
 * @return Pair of pointer to the CFG node with the index
 *         of the statement in the CFG, if found.
 *         Otherwise, if not found, nullptr and -1.
 *         The pair is represented as a StatementPosition.
 */
StatementPositionInCfg findCorrespondingNode(StatementPositionInCfg startingPosition, Integer statementToFind,
                                             std::unordered_set<CfgNode*>& visitedCfgNodes)
{
    CfgNode* startingNode = startingPosition.getNodePosition();
    Integer startingIndex = startingPosition.getStatementIndex();

    // if visited before, we could not find the statementToFind here
    if (visitedCfgNodes.find(startingNode) != visitedCfgNodes.end()) {
        return {nullptr, -1};
    }

    // first, try to search the untraversed statements
    Integer maxLength = startingNode->statementNodes->size();
    for (Integer i = startingIndex + 1; i < maxLength; i++) {
        if (startingNode->statementNodes->at(i)->getStatementNumber() == statementToFind) {
            return {startingNode, i};
        }
    }

    // else, search the children recursively
    for (CfgNode* child : *startingNode->childrenNodes) {
        StatementPositionInCfg resultsFromChild = findCorrespondingNode({child, -1}, statementToFind, visitedCfgNodes);
        if (resultsFromChild.getNodePosition() != nullptr) {
            return resultsFromChild;
        }
    }

    // we searched all paths and could not find anything
    return {nullptr, -1};
}

/**
 * Given a statement number, find all positions of that
 * statement in the CFG BIP that exist.
 *
 * @param statementToFind The statement to find.
 * @return Every position of that statement in the CFG BIP.
 */
Vector<StatementPositionInCfg> AffectsBipEvaluator::findAllCorrespondingPositions(Integer statementToFind)
{
    // first, get all procedures that possibly have CFG BIPs with statement
    Vector<String> directContainerProcedures = bipFacade->getProcedure(statementToFind);
    std::unordered_set<String> indirectContainerProcedures;
    for (const String& procedure : directContainerProcedures) {
        for (const String& indirectProcedures : bipFacade->getCallersStar(procedure)) {
            indirectContainerProcedures.insert(indirectProcedures);
        }
        indirectContainerProcedures.insert(procedure);
    }

    // next, find the matching positions of those CFG BIPs
    std::unordered_set<StatementPositionInCfg, StatementPositionHasher> matchingPositions;
    for (const String& procedure : indirectContainerProcedures) {
        CfgNode* startingNode = bipFacade->getCfg(procedure);
        if (startingNode == nullptr) {
            continue;
        }
        std::unordered_set<CfgNode*> uniqueStatementsVisited;
        StatementPositionInCfg positionInProcedure
            = findCorrespondingNode({startingNode, -1}, statementToFind, uniqueStatementsVisited);
        if (positionInProcedure.getNodePosition() != nullptr) {
            matchingPositions.insert(positionInProcedure);
        }
    }
    return Vector<StatementPositionInCfg>(matchingPositions.begin(), matchingPositions.end());
}

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
    AffectsEvaluator::cacheModifierAssigns(startingStmtNum);
    Vector<Integer> affectedBipStatements = AffectsEvaluator::getModifierAssigns(startingStmtNum).toVector();
    // Terminate when endValue is found
    Boolean foundEndValue = false;
    for (Integer affectedStatement : affectedBipStatements) {
        std::unordered_set<CfgNode*> uniqueStatementsVisited;
        StatementPositionInCfg correspondingPosition
            = findCorrespondingNode(startingPosition, affectedStatement, uniqueStatementsVisited);
        Boolean foundCorrespondingNode = correspondingPosition.getNodePosition() != nullptr;

        if (foundCorrespondingNode && endValue.isMatched(affectedStatement)) {
            // we found the end value and it is reachable
            foundEndValue = true;
            break;
        }

        if (foundCorrespondingNode) {
            // did not find end value, but there are still nodes we can search
            foundEndValue = affectsBipSearch(affectedStatement, correspondingPosition, false, visitedAssigns, endValue);
            if (foundEndValue) {
                break;
            }
        }
    }
    return foundEndValue;
}

Void AffectsBipEvaluator::cacheModifierBipStarAssigns(Integer leftRefVal)
{
    // find all positions of leftRefVal
    Vector<StatementPositionInCfg> allPositions = findAllCorrespondingPositions(leftRefVal);
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
}

Void AffectsBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    if (facade->getType(leftRefVal) != AssignmentStatement || !isAffectable(rightRef)) {
        resultsTable.storeResultsZero(false);
        return;
    }

    if (!exploredModifierBipStarAssigns.isCached(leftRefVal)) {
        cacheModifierBipStarAssigns(leftRefVal);
    }

    resultsTable.storeResultsOne(rightRef, cacheModifierBipStarTable.get(leftRefVal).toClauseResult());
}

Void AffectsBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    AffectsEvaluator::evaluateRightKnownStar(leftRef, rightRefVal);
}

Void AffectsBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    AffectsEvaluator::evaluateBothAnyStar(leftRef, rightRef);
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
    Vector<StatementPositionInCfg> allPositions = findAllCorrespondingPositions(leftRefVal);
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

AffectsBipEvaluator::AffectsBipEvaluator(ResultsTable& resultsTable, AffectsBipFacade* facade):
    AffectsEvaluator(resultsTable, facade), bipFacade(facade), cacheModifierBipStarTable(),
    exploredModifierBipStarAssigns(), allModifierBipStarAssigns(), allUserBipStarAssigns(), allAffectsBipStarTuples(),
    bipStarCacheFullyPopulated(false)
{}
