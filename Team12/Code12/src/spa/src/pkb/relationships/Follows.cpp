/**
 * Implementation of the FollowsTable Class.
 */

#include "Follows.h"

// important private helper function
void FollowsTable::typedShenanigans(Integer before, StatementType beforeStmtType, Integer after,
                                    StatementType afterStmtType)
{
    // store the before statement into (lists of statements before type afterStmtType)
    auto setBeforeType = &stmtBeforeTypeSet[afterStmtType].byType[beforeStmtType];
    // returns true if new object is created (emplace is successful)
    if (setBeforeType->emplace(before).second) {
        stmtBeforeType[afterStmtType].byType[beforeStmtType].push_back(before);
    }

    // store the after statement into (lists of statements after type beforeStmtType)
    auto setAfterType = &stmtAfterTypeSet[beforeStmtType].byType[afterStmtType];
    // returns true if new object is created (emplace is successful)
    if (setAfterType->emplace(after).second) {
        stmtAfterType[beforeStmtType].byType[afterStmtType].push_back(after);
    }
}
// important private helper function
void FollowsTable::typedShenanigansStar(Integer before, StatementType beforeStmtType, Integer after,
                                        StatementType afterStmtType)
{
    // store the before statement into (lists of statements before type afterStmtType)
    auto setBeforeType = &stmtBeforeStarTypeSet[afterStmtType].byType[beforeStmtType];
    // returns true if new object is created (emplace is successful)
    if (setBeforeType->emplace(before).second) {
        stmtBeforeType[afterStmtType].byType[beforeStmtType].push_back(before);
    }

    // store the after statement into (lists of statements after type beforeStmtType)
    auto setAfterType = &stmtAfterStarTypeSet[beforeStmtType].byType[afterStmtType];
    // returns true if new object is created (emplace is successful)
    if (setAfterType->emplace(after).second) {
        stmtAfterType[beforeStmtType].byType[afterStmtType].push_back(after);
    }
}
void FollowsTable::addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after,
                                           StatementType afterStmtType)
{
    // store statements before it
    stmtBeforeMap.insert(std::pair<Integer, Integer>(after, before));
    // store statements after it
    stmtAfterMap.insert(std::pair<Integer, Integer>(before, after));
    typedShenanigans(before, beforeStmtType, after, afterStmtType);
}

void FollowsTable::addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                               Vector<Pair<Integer, StatementType>> afterStmttypePairs)
{
    for (auto stmtTypePair : afterStmttypePairs) {
        Integer after = stmtTypePair.first;
        StatementType afterStmtType = stmtTypePair.second;
        // set and list
        stmtBeforestarsetMap[after].insert(before);
        stmtBeforestarlistMap[after].byType[beforeStmtType].push_back(before);

        stmtAfterstarsetMap[before].insert(after);
        stmtAfterstarlistMap[before].byType[afterStmtType].push_back(after);

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
