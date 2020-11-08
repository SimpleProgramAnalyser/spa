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
const unsigned int startingNode = 0;

/**
 * Type/struct declarations
 */
/**
 * IntWithNext is the the type of entries in the DP table, which is default initializable, and acts similarly as a pair
 * of unsigned integers.
 */
struct IntWithNext {
    unsigned int weight = INF;
    unsigned int next = -1;
};
/**
 * An arrangement(or permutation) of objects can be described as a queue of the objects. Here each clause is represented
 * as an unsigned int, which is its index in GroupedClauses.
 */
typedef std::queue<unsigned int> Arrangement;
/**
 * DP is a matrix with rows a bitmap, columns the weights.
 * Each cell saves the best ending weights along with the next node to visit
 */
typedef std::unordered_map<unsigned long long, IntWithNext> DP;
/**
 * An adjacency list for unweighted edges.
 */
typedef std::unordered_map<unsigned int, std::unordered_set<unsigned int>> AdjacencyList;
/**
 * A bitmap is used to represent the state of the clauses chosen, using 1s and 0s.
 */
typedef unsigned long long bitmap;

#endif // SPA_PQL_CLAUSE_GROUP_SORTER_H
