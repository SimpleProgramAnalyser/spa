/**
 * Implementation of the FollowsTable Class.
 */

#include "Follows.h"

void FollowsTable::tryAddBefore(Integer before, StatementType beforeType, StatementType afterType,
                                ArrayArrayList& aal, ArrayArraySet& aas)
{
    // store the before statement into (lists of statements before type afterStmtType)
    auto setBeforeType = &aas[afterType].byType[beforeType];
    // returns true if new object is created (emplace is successful)
    if (setBeforeType->find(before) == setBeforeType->end()) {
        setBeforeType->insert(before);
        aal[afterType].byType[beforeType].push_back(before);
    }
}
void FollowsTable::tryAddAfter(Integer after, StatementType beforeType, StatementType afterType,
                               ArrayArrayList& aal, ArrayArraySet& aas)
{
    // store the after statement into (lists of statements after type afterStmtType)
    auto setAfterType = &aas[beforeType].byType[afterType];
    // returns true if new object is created (emplace is successful)
    if (setAfterType->find(after) == setAfterType->end()) {
        setAfterType->insert(after);
        aal[beforeType].byType[afterType].push_back(after);
    }
}
// important private helper function
void FollowsTable::typedShenanigans(Integer before, StatementType beforeStmtType, Integer after,
                                    StatementType afterStmtType)
{
    tryAddBefore(before, beforeStmtType, afterStmtType, stmtBeforeType, stmtBeforeTypeSet);
    tryAddBefore(before, beforeStmtType, AnyStatement, stmtBeforeType, stmtBeforeTypeSet);
    tryAddBefore(before, AnyStatement, AnyStatement, stmtBeforeType, stmtBeforeTypeSet);

    tryAddAfter(after, afterStmtType, afterStmtType, stmtAfterType, stmtAfterTypeSet);
    tryAddAfter(after, afterStmtType, AnyStatement, stmtAfterType, stmtAfterTypeSet);
    tryAddAfter(after, AnyStatement, AnyStatement, stmtAfterType, stmtAfterTypeSet);
}
// important private helper function
void FollowsTable::typedShenanigansStar(Integer before, StatementType beforeStmtType, Integer after,
                                        StatementType afterStmtType)
{
    tryAddBefore(before, beforeStmtType, afterStmtType, stmtBeforeStarType, stmtBeforeStarTypeSet);
    tryAddBefore(before, beforeStmtType, AnyStatement, stmtBeforeStarType, stmtBeforeStarTypeSet);
    tryAddBefore(before, AnyStatement, AnyStatement, stmtBeforeStarType, stmtBeforeStarTypeSet);

    tryAddAfter(after, afterStmtType, afterStmtType, stmtAfterStarType, stmtAfterStarTypeSet);
    tryAddAfter(after, afterStmtType, AnyStatement, stmtAfterStarType, stmtAfterStarTypeSet);
    tryAddAfter(after, AnyStatement, AnyStatement, stmtAfterStarType, stmtAfterStarTypeSet);
}

void FollowsTable::addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after,
                                           StatementType afterStmtType)
{
    // store statements before it
    stmtBeforeMap.insert(std::make_pair(after, before));
    // store statements after it
    stmtAfterMap.insert(std::make_pair(before, after));

    typedShenanigans(before, beforeStmtType, after, afterStmtType);
}

void FollowsTable::addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                               const Vector<Pair<Integer, StatementType>>& afterStmttypePairs)
{
    for (auto stmtTypePair : afterStmttypePairs) {
        Integer after = stmtTypePair.first;
        StatementType afterStmtType = stmtTypePair.second;
        // set and list
        stmtBeforestarsetMap[after].insert(before);
        stmtBeforestarlistMap[after].byType[beforeStmtType].push_back(before);
        stmtBeforestarlistMap[after].byType[AnyStatement].push_back(before);

        stmtAfterstarsetMap[before].insert(after);
        stmtAfterstarlistMap[before].byType[afterStmtType].push_back(after);
        stmtAfterstarlistMap[before].byType[AnyStatement].push_back(after);

        typedShenanigansStar(before, beforeStmtType, after, afterStmtType);
    }
}

// reading
Boolean FollowsTable::checkIfFollowsHolds(Integer before, Integer after)
{
    return stmtAfterMap[before] == after;
}

Boolean FollowsTable::checkIfFollowsHoldsStar(Integer before, Integer after)
{
    auto stmtSet = stmtAfterstarsetMap[before];
    return stmtSet.find(after) != stmtSet.end();
}

Vector<Integer> FollowsTable::getAfterStatement(Integer before)
{
    Vector<Integer> toReturn;
    if (stmtAfterMap[before] != 0) {
        toReturn.push_back(stmtAfterMap[before]);
    }
    return toReturn;
}
Vector<Integer> FollowsTable::getBeforeStatement(Integer after)
{
    Vector<Integer> toReturn;
    if (stmtBeforeMap[after] != 0) {
        toReturn.push_back(stmtBeforeMap[after]);
    }
    return toReturn;
}
Vector<Integer> FollowsTable::getAllAfterStatementsStar(Integer before, StatementType stmtType)
{
    return stmtAfterstarlistMap[before].byType[stmtType];
}
Vector<Integer> FollowsTable::getAllBeforeStatementsStar(Integer after, StatementType stmtType)
{
    return stmtBeforestarlistMap[after].byType[stmtType];
}

Vector<Integer> FollowsTable::getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return stmtBeforeType[stmtTypeOfAfter].byType[stmtTypeOfBefore];
}
Vector<Integer> FollowsTable::getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore,
                                                              StatementType stmtTypeOfAfter)
{
    return stmtBeforeStarType[stmtTypeOfAfter].byType[stmtTypeOfBefore];
}
Vector<Integer> FollowsTable::getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return stmtAfterType[stmtTypeOfBefore].byType[stmtTypeOfAfter];
}
Vector<Integer> FollowsTable::getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore,
                                                             StatementType stmtTypeOfAfter)
{
    return stmtAfterStarType[stmtTypeOfBefore].byType[stmtTypeOfAfter];
}
