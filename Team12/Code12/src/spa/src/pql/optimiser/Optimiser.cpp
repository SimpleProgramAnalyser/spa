/**
 * Implementation of PQL query optimiser.
 */

#include "Optimiser.h"

Void optimiseQuery(AbstractQuery& abstractQuery)
{
    // preprocessing
    substituteWithValues(abstractQuery);
    deleteDuplicateClauses(abstractQuery);

    // group and sort
    groupQueryClauses(abstractQuery);
    sortWithinEachClause(abstractQuery);
    sortClauses(abstractQuery);
}

Void substituteWithValues(AbstractQuery& abstractQuery) {}
Void deleteDuplicateClauses(AbstractQuery& abstractQuery) {}
Void groupQueryClauses(AbstractQuery& abstractQuery) {}

/**
 * 1. Least synonyms (1) first
 * 2. Least result (Modifies, Follows) first
 * 3. Prevent both cold synonyms
 * 4. With before such that
 *
 * @param abstractQuery
 * @return
 */
Void sortWithinEachClause(AbstractQuery& abstractQuery) {}

/**
 *
 * @param abstractQuery
 * @return
 */
Void sortClauses(AbstractQuery& abstractQuery) {}
