//
// Created by Chester Sim on 17/10/20.
//

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

Boolean CacheSet::isCached(StatementNumber stmtNum) const
{
    return set.find(stmtNum) != set.end();
}

ClauseResult CacheSet::filterStatementType(StatementType stmtType) const
{
    if (stmtType == AnyStatement) {
        return this->toClauseResult();
    }

    ClauseResult filteredStrList;
    for (StatementNumber i : set) {
        StatementType currentType = getStatementType(i);
        if (currentType == stmtType) {
            filteredStrList.push_back(std::to_string(i));
        }
    }
    return filteredStrList;
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
