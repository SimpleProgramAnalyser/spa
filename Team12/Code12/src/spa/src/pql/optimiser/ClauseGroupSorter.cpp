#include "ClauseGroupSorter.h"

#include <map>
#include <set>
#include <stdexcept>

#include "GroupedClauses.h"
#include "OptimiserUtils.h"

/**
 * Global Variables
 */
std::unordered_map<unsigned int, unsigned int> weights;
DP dp;
AdjacencyList adj;

// digit at visited (LSD) should be 1. Mark it as 0.
bitmap markVisited(bitmap nodesLeft, unsigned int visited)
{
    return nodesLeft & ~(static_cast<unsigned int>(1) << visited);
}

// aim: update nodesReachable so it
// 1. is all reachable
// 2. does not contain currentNode
// 3. does not contain anything from the zeroes of nodesLeft
bitmap updateReachableBitmap(bitmap nodesLeft, unsigned int currentNode, bitmap nodesReachable)
{
    // 1. make sure everything is reachable.
    // for neighbours of current node, if it is yet to be visited (in nodesLeft), update nodesReachable
    for (auto neighbour : adj[currentNode]) {
        unsigned int u = neighbour;
        // the uth LSB in nodesLeft is set, which means it is to be visited
        bool notVisited = (static_cast<unsigned int>(1) << u) & nodesLeft;
        if (notVisited) {
            // set the uth bit in nodesReachable
            nodesReachable |= (static_cast<unsigned int>(1) << u);
        }
    }
    // 2. remove current node
    // finally remove currentNode by AND with 11111011111
    nodesReachable &= ~(static_cast<unsigned int>(1) << currentNode);

    // 3. remove nodesLeft
    // say nodesLeft is 1110001, just AND it with nodesReachable
    nodesReachable &= nodesLeft;
    return nodesReachable;
}

Vector<unsigned int> getReachableNodes(bitmap nodesReachable)
{
    Vector<unsigned int> toReturn;
    unsigned int count = startingNode;
    while (nodesReachable) {
        // if LSD is 1, push it into the vector
        if ((unsigned int)1 & nodesReachable) {
            toReturn.push_back(count);
        }
        nodesReachable = nodesReachable >> static_cast<unsigned int>(1);
        count++;
    }
    return toReturn;
}

Arrangement constructQueue(Arrangement currArrangement, bitmap nodesLeft)
{
    if (nodesLeft == 0) {
        return currArrangement;
    }
    currArrangement.push(dp[nodesLeft].next);
    return constructQueue(currArrangement, markVisited(nodesLeft, dp[nodesLeft].next));
}

// an arrangement is a permutation of the n nodes.
// this DP aims to MINIMIZE i * weights(p_i) where p_1, p_2, ... ,p_n represent the permutation
// arrangementQueue is GREEDY: it contains the currentNode when called.
std::pair<Arrangement, unsigned int> arrange(bitmap nodesLeft, bitmap nodesReachable, unsigned int multiplier,
                                             unsigned int currentWeight, const Arrangement& arrangementQueue)
{
    // Base case: no more node to visit
    if (nodesLeft == 0) {
        return std::make_pair(arrangementQueue, currentWeight);
    }

    // Optimization: memoization
    IntWithNext memoized = dp[nodesLeft];
    if (memoized.weight != INF) { // memoized before
        // memoized.next stores the next node to go, and memoized.weights stores the total weights of this path.
        // we cannot return directly but have to first construct the queue.
        return std::make_pair(constructQueue(arrangementQueue, nodesLeft), memoized.weight);
    }

    // Backtracking (or top-down DP)
    // explore the reachable ones.
    unsigned int minWeight = INF;
    unsigned int minWeightNode = -1;
    Arrangement bestQueue;
    for (auto reachable : getReachableNodes(nodesReachable)) {
        bitmap currNodesLeft = markVisited(nodesLeft, reachable);
        bitmap currReachable = updateReachableBitmap(nodesLeft, reachable, nodesReachable);
        Arrangement currQueue(arrangementQueue);
        currQueue.push(reachable);
        auto result
            = arrange(currNodesLeft, currReachable, multiplier - 1, currentWeight + weights[reachable], currQueue);
        if (result.second < minWeight) {
            minWeight = std::min(minWeight, result.second);
            minWeightNode = reachable;
            bestQueue = result.first;
        }
    }
    // Update DP Table
    dp[nodesLeft].weight = minWeight;
    dp[nodesLeft].next = minWeightNode;

    // return the minimum
    return std::make_pair(bestQueue, minWeight);
}

// weights of a clause is the estimated time it takes to evaluate.
unsigned int getWeight(Clause* clause)
{
    const unsigned int SYNONYM_COUNT_MULTIPLIER = 100, WITH = 0, FOLLOWS_MODIFIES = 2, PATTERN_REST = 4, AFFECTS = 100,
                       BIP = 150;
    unsigned int clauseWeight = 0;
    clauseWeight += SYNONYM_COUNT_MULTIPLIER * (countSynonym(clause) - 1);
    /**
     * with clause: +0
     * such that: follows/modifies: +2
     * pattern, such that: rest: +4
     * such that: affects(*): +100
     */
    switch (clause->getType()) {
    case WithClauseType: { // +0
        clauseWeight += WITH;
        break;
    }
    case SuchThatClauseType: {
        // NOLINTNEXTLINE
        SuchThatClause* suchThatClause = static_cast<SuchThatClause*>(clause);
        switch (suchThatClause->getRelationship().getType()) {
        case FollowsType:
        case ModifiesType: {
            clauseWeight += FOLLOWS_MODIFIES;
            break;
        }
        case FollowsStarType:
        case ParentType:
        case ParentStarType:
        case UsesType:
        case UsesStatementType:
        case UsesProcedureType:
        case ModifiesStatementType:
        case ModifiesProcedureType:
        case CallsType:
        case CallsStarType:
        case NextType: {
            clauseWeight += PATTERN_REST;
            break;
        }
        case NextStarType:
        case AffectsType:
        case AffectsStarType: {
            clauseWeight += AFFECTS;
            break;
        }
        case AffectsBipType:
        case AffectsBipStarType:
        case NextBipType:
        case NextBipStarType: {
            clauseWeight += BIP;
            break;
        }
        default:
            throw std::runtime_error("Error: unknown SuchThatClauseType in ClauseGroupSorter::getWeight");
        }
        break;
    }
    case PatternClauseType: {
        clauseWeight += PATTERN_REST;
        break;
    }
    case NonExistentClauseType: // unexpected
        break;
    }
    return clauseWeight;
}

/**
 * (Subjective) Sort the groups of clauses in the GroupedClauses object using the following principles, copied from
 * lecture notes:
 *
 * Prioritize clauses with one constant and one synonym
 * Prioritize clauses with less number of results: Follows, Modifies, etc.
 * Sort clauses such that at least one synonym has been computed in a previous clause
 * Prioritize with-clauses – more restrictive than such that clauses
 * Evaluating pattern-clauses – similar to any such that clause
 * Push Affects(*) clauses on the last positions in a group
 *
 * @param groupedClauses
 */
Void sortWithinEachGroup(GroupedClauses& groupedClauses)
{
    /**
     * Each clause is given a priority according to its 1. synonym count, and 2. clause type.
     * a graph is constructed where clauses are vertices and edge iff the clauses share at least one synonym.
     * we want to find a permutation of the clauses where the weighted sum,
     * \sum^n_{i=1}(n-1)w(v_{\sigma_i}) is minimized, where the vertices are v1, v2, ..., vn
     *
     * This is a NP-hard problem, and the proof is left as an exercise to the reader.
     *
     * We will be using a dynamic programming algorithm to find the optimal sort.
     */

    for (int i = 0; i < groupedClauses.size(); i++) {
        // we only sort the groups with synonym
        if (!groupedClauses.groupHasSynonym(i))
            continue;

        unsigned int groupSize = groupedClauses.groupSize(i);
        // create graph
        adj.clear();
        weights.clear();

        unsigned int minWeight = INF;
        std::unordered_multimap<unsigned int, unsigned int> weightNodes;
        for (std::size_t j = 0; j < groupSize; j++) {
            Clause* clause1 = groupedClauses.getClause(i, j);
            for (std::size_t k = j + 1; k < groupSize; k++) {
                Clause* clause2 = groupedClauses.getClause(i, k);
                if (shareSynonym(clause1, clause2)) {
                    adj[j].insert(k);
                    adj[k].insert(j);
                }
            }
            weights[j] = getWeight(clause1);
            minWeight = std::min(minWeight, weights[j]);
            weightNodes.insert(std::make_pair(weights[j], j));
        }

        // Instead of just DPing, we provide sensible starting points - start from the nodes where
        // weight is lowest.
        auto lowestIT = weightNodes.equal_range(minWeight);
        // take minimum across all starting points
        Arrangement arr;
        unsigned int minCost = INF;
        for (auto it = lowestIT.first; it != lowestIT.second; it++) {
            // nodesLeft is 1111111 as many 1s as groupSize, taken away the current node (which is it->second)
            unsigned int currentNode = it->second;
            // DO NOT SWAP THE TWO LINES BELOW. updateReachableBitmap assumes the currentNode is marked as visited
            unsigned int nodesLeft = markVisited((static_cast<unsigned int>(1) << groupSize) - 1, currentNode);
            unsigned int nodesReachable = updateReachableBitmap(nodesLeft, currentNode, 0);
            Arrangement currentArr;
            currentArr.push(it->second);
            auto results = arrange(nodesLeft, nodesReachable, groupSize - 1, it->first * groupSize, currentArr);
            if (results.second < minCost) {
                minCost = results.second;
                arr = std::move(results.first);
            }
        }
        groupedClauses.applyArrangementToGroup(arr, i);
    }
    // TODO: Move global variables into scope so they will be deleted automatically.
    dp.clear();
    weights.clear();
    adj.clear();
}