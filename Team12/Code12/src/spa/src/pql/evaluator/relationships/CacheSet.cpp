/**
 * Implementation of CacheSet class for Query Evaluator.
 */
#include "CacheSet.h"

#include <utility>

#include "pkb/PKB.h"

CacheSet::CacheSet(std::unordered_set<StatementNumber> unorderedSet): set(std::move(unorderedSet)) {}

CacheSet::CacheSet(const Vector<StatementNumber>& nonStarRelationshipResults)
{
    for (auto stmtNum : nonStarRelationshipResults) {
        set.insert(stmtNum);
    }
}

Void CacheSet::insert(StatementNumber stmtNum)
{
    set.insert(stmtNum);
}

Void CacheSet::combine(CacheSet otherSet)
{
    set.insert(otherSet.set.begin(), otherSet.set.end());
}

ClauseResult CacheSet::toClauseResult() const
{
    ClauseResult strList;
    for (StatementNumber i : set) {
        strList.push_back(std::to_string(i));
    }
    return strList;
}

Vector<Integer> CacheSet::toVector() const
{
    return Vector<Integer>(set.begin(), set.end());
}

Boolean CacheSet::isCached(StatementNumber stmtNum) const
{
    return set.find(stmtNum) != set.end();
}

CacheSet CacheSet::filterStatementType(StatementType stmtType) const
{
    if (stmtType == AnyStatement) {
        return *this;
    }

    CacheSet filteredCacheSet;
    for (StatementNumber i : set) {
        StatementType currentType = getStatementType(i);
        if (currentType == stmtType) {
            filteredCacheSet.insert(i);
        }
    }
    return filteredCacheSet;
}

Vector<StatementNumber> CacheSet::toList() const
{
    Vector<StatementNumber> stmtNumList;
    for (StatementNumber i : set) {
        stmtNumList.push_back(i);
    }
    return stmtNumList;
}

Boolean CacheSet::empty() const
{
    return set.empty();
}

size_t CacheSet::size() const
{
    return set.size();
}

void CacheSet::remove(StatementNumber stmtNumToRemove)
{
    set.erase(stmtNumToRemove);
}