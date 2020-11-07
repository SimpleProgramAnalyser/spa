/**
 * Utility methods for use in Query Optimiser.
 */

#ifndef SPA_PQL_OPTIMISER_UTILS_H
#define SPA_PQL_OPTIMISER_UTILS_H

#include <set>

#include "GroupedClauses.h"

/**
 * Accepts an EntRef and decide whether it is a value, ie INT or IDENT.
 *
 * @param reference
 * @return
 */
inline bool isValue(const Reference& reference)
{
    return reference.getReferenceType() == LiteralRefType || reference.getReferenceType() == IntegerRefType;
}

/**
 * Given a
 *
 * @param reference
 * @return
 */
inline bool hasSynonym(const Reference& reference)
{
    return reference.getReferenceType() == AttributeRefType || reference.getReferenceType() == SynonymRefType;
}

bool canInitiateSubstitution(Reference& reference);
bool canBeSubstituted(Reference& reference);
bool hasSynonym(Clause* clause);
unsigned int countSynonym(Clause* clause);
bool shareSynonym(Clause* clause1, Clause* clause2);

#endif // SPA_PQL_OPTIMISER_UTILS_H
