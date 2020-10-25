/**
 * Implementation of CacheTable class for Query Evaluator.
 */
#include "CacheTable.h"

Boolean CacheTable::isCached(StatementNumber num) const
{
    return table.find(num) != table.end();
}

Void CacheTable::insert(StatementNumber stmtNum, const CacheSet& cacheSet)
{
    table.insert({stmtNum, cacheSet});
}

Void CacheTable::insertPartial(StatementNumber key, StatementNumber value)
{
    if (table.find(key) == table.end()) {
        table.insert({key, CacheSet()});
    }
    table[key].insert(value);
}

CacheSet CacheTable::get(StatementNumber stmtNum)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(table.find(stmtNum) != table.end());

    return table.find(stmtNum)->second;
}

Boolean CacheTable::check(StatementNumber key, StatementNumber value)
{
    return table.find(key) != table.end() && table[key].isCached(value);
}
