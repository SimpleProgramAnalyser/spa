/**
 * Handles reordering the clauses for faster evaluation.
 */

#ifndef SPA_PQL_OPTIMISER_H
#define SPA_PQL_OPTIMISER_H

#include "Types.h"
#include "pql/preprocessor/AqTypes.h"

/**
 * Sorts the clauses in an AbstractQuery for faster evaluation.
 *
 * @param query The PQL query to sort.
 */
Void optimiseQuery(AbstractQuery& abstractQuery);

#endif // SPA_PQL_OPTIMISER_H

Void substituteWithValues(AbstractQuery& abstractQuery);
Void deleteDuplicateClauses(AbstractQuery& abstractQuery);
Void groupQueryClauses(AbstractQuery& abstractQuery);
Void sortWithinEachClause(AbstractQuery& abstractQuery);
Void sortClauses(AbstractQuery& abstractQuery);
