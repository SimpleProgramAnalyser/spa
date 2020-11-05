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
    if (table.find(stmtNum) == table.end()) {
        // return empty set if not found
        return CacheSet();
    }
    return table.find(stmtNum)->second;
}

CacheSet* CacheTable::getReference(StatementNumber stmtNum)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(table.find(stmtNum) != table.end());

    return &(table.find(stmtNum)->second);
}

Boolean CacheTable::check(StatementNumber key, StatementNumber value)
{
    return table.find(key) != table.end() && table[key].isCached(value);
}

Void CacheTable::remove(StatementNumber stmtNum)
{
    table.erase(stmtNum);
}

std::unordered_set<StatementNumber> CacheTable::keys() const
{
    std::unordered_set<StatementNumber> keys;
    for (auto k : table) {
        keys.insert(k.first);
    }

    return keys;
}
