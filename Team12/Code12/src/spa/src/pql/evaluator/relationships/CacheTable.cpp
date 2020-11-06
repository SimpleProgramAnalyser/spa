/**
 * Implementation of CacheTable class for Query Evaluator.
 */
#include "CacheTable.h"

Boolean CacheTable::hasCached(StatementNumber num) const
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
    if (table.find(stmtNum) == table.end()) {
        // return empty set if not found
        return CacheSet();
    }
    return table.find(stmtNum)->second;
}

Boolean CacheTable::check(StatementNumber key, StatementNumber value)
{
    return table.find(key) != table.end() && table[key].hasCached(value);
}

Void CacheTable::remove(StatementNumber stmtNum)
{
    table.erase(stmtNum);
}
