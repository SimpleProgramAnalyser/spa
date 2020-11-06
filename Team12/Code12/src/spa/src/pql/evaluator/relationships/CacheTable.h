/**
 * Cache table class for Query Evaluator.
 * Guarantees amortised constant time access and insertion.
 */

#ifndef SPA_PQL_CACHE_TABLE_H
#define SPA_PQL_CACHE_TABLE_H

#include "../EvaluatorUtils.h"
#include "CacheSet.h"

class CacheTable {
private:
    std::unordered_map<StatementNumber, CacheSet> table;

public:
    // checks if evaluation of statement has already been cached
    Boolean hasCached(StatementNumber num) const;

    /**
     * Inserts multiple table entries stmtNum -> {v1, v2, ..., vn}
     * where cacheSet is the set of {v1, v2, ..., vn}.
     */
    Void insert(StatementNumber stmtNum, const CacheSet& cacheSet);

    /**
     * Inserts a single table entry key -> value.
     */
    Void insertPartial(StatementNumber key, StatementNumber value);

    CacheSet get(StatementNumber stmtNum);

    /**
     * Checks if key -> value is TRUE (both in the table). If
     * not in the table (either key or value), returns FALSE.
     */
    Boolean check(StatementNumber key, StatementNumber value);

    Void remove(StatementNumber stmtNum);
};

#endif // SPA_PQL_CACHE_TABLE_H
