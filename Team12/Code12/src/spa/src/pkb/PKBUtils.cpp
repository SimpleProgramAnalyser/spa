#include <pkb/PKBUtils.h>

/**
 * (Internal) Given a relationship and the respective collection table (and de-duplicating sets), add the relationship
 * into the collection table. It is:
 * 1. Idempotent; values can be added multiple times.
 * 2. Respecting of the StatementType given; any type given is assumed to be not AnyStatement, and the information
 *   is automatically duplicated in the AnyStatement section.
 *
 * This is the version whose primary key is a StatementNumber, secondary key a StatementType, and value a
 * StatementNumber.
 *
 * @param primaryKey
 * @param secondaryKey
 * @param value
 * @param collectionTable
 * @param deduplicatingTable
 */
void tryAdd(StatementType primaryKey, StatementType secondaryKey, Integer value, ArrayArrayList& collectionTable,
            ArrayArraySet& deduplicatingTable)
{
    deduplicatedAdd(value, collectionTable[primaryKey].byType[secondaryKey],
                    deduplicatingTable[primaryKey].byType[secondaryKey]);
    deduplicatedAdd(value, collectionTable[primaryKey].byType[AnyStatement],
                    deduplicatingTable[primaryKey].byType[AnyStatement]);
    deduplicatedAdd(value, collectionTable[AnyStatement].byType[AnyStatement],
                    deduplicatingTable[AnyStatement].byType[AnyStatement]);
    deduplicatedAdd(value, collectionTable[AnyStatement].byType[secondaryKey],
                    deduplicatingTable[AnyStatement].byType[secondaryKey]);
}

/**
 * (Internal) Given a relationship and the respective collection table (and de-duplicating sets), add the relationship
 * into the collection table. It is:
 * 1. Idempotent; values can be added multiple times.
 * 2. Respecting of the StatementType given; any type given is assumed to be not AnyStatement, and the information
 *   is automatically duplicated in the AnyStatement section.
 *
 * This is the version whose primary key is a StatementNumber, secondary key a StatementType, and value a
 * StatementNumber.
 *
 * @param primaryKey
 * @param secondaryKey
 * @param value
 * @param collectionTable
 * @param deduplicatingTable
 */
void tryAdd(StatementNumber primaryKey, StatementType secondaryKey, StatementNumber value,
            HashMap<StatementNumber, StatementNumVectorsByType>& collectionTable,
            HashMap<StatementNumber, StatementNumSetsByType>& deduplicatingTable)
{
    deduplicatedAdd(value, collectionTable[primaryKey].byType[secondaryKey],
                    deduplicatingTable[primaryKey].byType[secondaryKey]);
    deduplicatedAdd(value, collectionTable[primaryKey].byType[AnyStatement],
                    deduplicatingTable[primaryKey].byType[AnyStatement]);
}
