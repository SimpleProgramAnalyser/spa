//
// Created by Chester Sim on 17/10/20.
//

#include "CacheTable.h"


Boolean CacheTable::isCached(StatementNumber num) const
{
    return table.find(num) != table.end();
}

Void CacheTable::insert(StatementNumber stmtNum, const CacheSet& cacheSet)
{
    table.insert({stmtNum, cacheSet});
}

CacheSet CacheTable::get(StatementNumber stmtNum)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(table.find(stmtNum) != table.end());

    return table.find(stmtNum)->second;
}