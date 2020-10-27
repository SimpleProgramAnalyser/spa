/**
 * Implementation of PQL query optimiser.
 */

#include "Optimiser.h"

Void optimiseQuery(AbstractQuery& abstractQuery)
{
    substituteWithValues(abstractQuery);
    deleteDuplicateClauses(abstractQuery);
    groupQueryClauses(abstractQuery);
    sortWithinEachClause(abstractQuery);
    sortClauses(abstractQuery);
}

Void substituteWithValues(AbstractQuery& abstractQuery) {}
Void deleteDuplicateClauses(AbstractQuery& abstractQuery) {}
Void groupQueryClauses(AbstractQuery& abstractQuery) {}
Void sortWithinEachClause(AbstractQuery& abstractQuery) {}
Void sortClauses(AbstractQuery& abstractQuery) {}
