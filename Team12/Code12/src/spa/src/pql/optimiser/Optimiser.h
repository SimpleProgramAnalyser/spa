/**
 * Handles reordering the clauses for faster evaluation.
 */

#ifndef SPA_PQL_OPTIMISER_H
#define SPA_PQL_OPTIMISER_H

#include <algorithm>

#include "GroupedClauses.h"
#include "Types.h"
#include "pql/preprocessor/AqTypes.h"

/**
 * Sorts the clauses in an AbstractQuery for faster evaluation.
 *
 * @param query The PQL query to sort.
 */
Void optimiseQuery(AbstractQuery& abstractQuery);

#endif // SPA_PQL_OPTIMISER_H

/**
 * For with clauses such as s.stmt# = 5, substitute all occurences of the synonym "s" with the value 5.
 *
 * @param abstractQuery The PQL query to substitute in.
 */
Void substituteWithValues(AbstractQuery& abstractQuery);

/**
 * Remove clauses that are exactly the same. Eg. Next*(s1, s2) and Next*(s1, s2).
 *
 * @param abstractQuery
 * @return
 */
Void deleteDuplicateClauses(AbstractQuery& abstractQuery);

/**
 * Group the clauses into a few groups, using the following principles:
 * 1. Clauses without any synonym in one group.
 * Consider the the remaining clauses as vertices in a graph, and add an edge between
 * two vertices if and only if the clauses share at least one synonym.
 * 2. Every connected component should belong to one and only one group.
 *
 *
 * @param groupedClauses The abstract data type of grouped clauses.
 */
GroupedClauses groupQueryClauses(AbstractQuery& abstractQuery);

/**
 * Start with clauses without synonyms, and prioritize groups with synonyms that do not return result.
 *
 * @param groupedClauses
 * @return
 */
Void sortGroups(GroupedClauses& groupedClauses);

/**
 * Sort the clauses in abstractQuery based on the order in GroupedClauses.
 *
 * @param groupedClauses
 * @param abstractQuery
 * @return
 */
Void sortClauses(GroupedClauses& groupedClauses, AbstractQuery& abstractQuery);
