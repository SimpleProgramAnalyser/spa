#include <pkb/PkbTypes.h>

#ifndef SPA_PKBUTILS_H
#define SPA_PKBUTILS_H

/**
 * Adds a value into an entry with a set that acts as de-duplication check. This method is idempotent.
 *
 * @param value
 * @param entry
 * @param set
 */
template <typename T>
void deduplicatedAdd(T value, Vector<T>& entry, HashSet<T>& set)
{
    if (set.find(value) == set.end()) {
        set.insert(value);
        entry.push_back(value);
    }
}

void tryAdd(StatementType primaryKey, StatementType secondaryKey, Integer value, ArrayArrayList& collectionTable,
            ArrayArraySet& deduplicatingTable);
void tryAdd(StatementNumber primaryKey, StatementType secondaryKey, StatementNumber value,
            HashMap<StatementNumber, StatementNumVectorsByType>& collectionTable,
            HashMap<StatementNumber, StatementNumSetsByType>& deduplicatingTable);

#endif // SPA_PKBUTILS_H
