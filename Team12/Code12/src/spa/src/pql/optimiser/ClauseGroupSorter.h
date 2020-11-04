/**
 * A class that handles sorting of clauses within
 * a small group of clauses.
 */

#ifndef SPA_PQL_CLAUSE_GROUP_SORTER_H
#define SPA_PQL_CLAUSE_GROUP_SORTER_H

#include <algorithm>
#include <queue>

#include "GroupedClauses.h"
#include "Types.h"
#include "pql/preprocessor/AqTypes.h"

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
void sortWithinEachGroup(GroupedClauses& groupedClauses);

/**
 * Constant declarations
 */
const unsigned int INF = 2e9;
const uint startingNode = 0;

/**
 * Type/struct declarations
 */
struct IntWithNext {
    unsigned int weight = INF;
    unsigned int next = -1;
};
typedef std::queue<unsigned int> Arrangement;
// DP is a matrix with rows a bitmap, columns the weights.
// Each cells save the best ending weights along with the next node to visit
typedef std::unordered_map<unsigned long long, IntWithNext> DP;
typedef std::unordered_map<unsigned int, std::unordered_set<uint>> AdjacencyList;
typedef unsigned long long bitmap;

#endif // SPA_PQL_CLAUSE_GROUP_SORTER_H
