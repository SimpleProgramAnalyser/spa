//
// Created by Chester Sim on 17/10/20.
//

#ifndef AUTOTESTER_CACHETABLE_H
#define AUTOTESTER_CACHETABLE_H

#include "../EvaluatorUtils.h"
#include "CacheSet.h"

class CacheTable {
private:
    std::unordered_map<StatementNumber, CacheSet> table;

public:
    // checks if evaluation of statement has already been cached
    Boolean isCached(StatementNumber num) const;
    Void insert(StatementNumber stmtNum, const CacheSet& cacheSet);
    CacheSet get(StatementNumber stmtNum);
};

#endif // AUTOTESTER_CACHETABLE_H
