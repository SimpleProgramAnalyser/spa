/**
 * Cache set class for Query Evaluator.
 * Guarantees amortised constant time access and insertion.
 */

#ifndef SPA_PQL_CACHE_SET_H
#define SPA_PQL_CACHE_SET_H

#include "../EvaluatorUtils.h"

class CacheSet {
private:
    std::unordered_set<StatementNumber> set;

public:
    CacheSet() = default;

    explicit CacheSet(std::unordered_set<StatementNumber> unorderedSet);

    /**
     * Uses the non star version of the relationship
     * to initialize the CacheSet.
     *
     * E.g. The initial state of Next*(1, _) will be
     * the results of getAllNextStatements(1, AnyType).
     * It is important that the second argument is
     * AnyType, so that we cache all Next* statements
     * without the filtering of the Statement Type.
     */
    explicit CacheSet(const Vector<StatementNumber>& nonStarRelationshipResults);

    /**
     * Inserts a StatementNumber into the set.
     * Duplicates will not be added again.
     *
     * @param stmtNum StatementNumber to be inserted.
     */
    Void insert(StatementNumber stmtNum);

    /**
     * Adds another CacheSet to the current CacheSet,
     * mutating the original CacheSet.
     *
     * @param otherSet Other CacheSet to combine with.
     */
    Void combine(CacheSet otherSet);

    /**
     * Converts the encapsulated set into a
     * ClauseResult.
     *
     * @return ClauseResult format of the set.
     */
    ClauseResult toClauseResult() const;

    // Checks if StatementNumber is already cached
    Boolean isCached(StatementNumber stmtNum) const;

    // Returns a ClauseResult with the StatementNumber filtered for stmtType
    ClauseResult filterStatementType(StatementType stmtType) const;

    // Converts CacheSet into a Vector of StatementNumbers.
    Vector<StatementNumber> toList() const;

    // Returns true if CacheSet is empty.
    Boolean empty() const;
};

#endif // SPA_PQL_CACHE_SET_H
