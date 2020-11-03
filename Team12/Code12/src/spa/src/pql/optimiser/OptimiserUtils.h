#ifndef SPA_OPTIMISERUTILS_H
#define SPA_OPTIMISERUTILS_H
#include <set>

#include "GroupedClauses.h"

/**
 * Accepts an EntRef and decide whether it is a value, ie INT or IDENT.
 *
 * @param reference
 * @return
 */
inline bool isValue(const Reference& reference);

/**
 * Given a
 *
 * @param reference
 * @return
 */
inline bool hasSynonym(const Reference& reference);
bool hasSynonym(Clause* clause);
uint countSynonym(Clause* clause);
bool shareSynonym(Clause* clause1, Clause* clause2);
#endif // SPA_OPTIMISERUTILS_H
