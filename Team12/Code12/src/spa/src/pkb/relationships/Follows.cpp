/**
 * Implementation of the FollowsTable Class.
 */

#include "Follows.h"

void FollowsTable::tryAddBefore(Integer before, StatementType beforeType, StatementType afterType, ArrayArrayList& aal,
                                ArrayArraySet& aas)
{
    // store the before statement into (lists of statements before type afterStmtType)
    auto setBeforeType = &aas[afterType].byType[beforeType];
    // returns true if new object is created (emplace is successful)
    if (setBeforeType->find(before) == setBeforeType->end()) {
        setBeforeType->insert(before);
        aal[afterType].byType[beforeType].push_back(before);
    }
}
void FollowsTable::tryAddAfter(Integer after, StatementType beforeType, StatementType afterType, ArrayArrayList& aal,
                               ArrayArraySet& aas)
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
    tryAddBefore(before, AnyStatement, afterStmtType, stmtBeforeType, stmtBeforeTypeSet);
    tryAddBefore(before, AnyStatement, AnyStatement, stmtBeforeType, stmtBeforeTypeSet);

    tryAddAfter(after, beforeStmtType, afterStmtType, stmtAfterType, stmtAfterTypeSet);
    tryAddAfter(after, beforeStmtType, AnyStatement, stmtAfterType, stmtAfterTypeSet);
    tryAddAfter(after, AnyStatement, afterStmtType, stmtAfterType, stmtAfterTypeSet);
    tryAddAfter(after, AnyStatement, AnyStatement, stmtAfterType, stmtAfterTypeSet);

    // for tuples
    auto pair = std::make_pair(before, after);
    followsTuples[AnyStatement][AnyStatement].push_back(pair);
    followsTuples[AnyStatement][afterStmtType].push_back(pair);
    followsTuples[beforeStmtType][AnyStatement].push_back(pair);
    followsTuples[beforeStmtType][afterStmtType].push_back(pair);
}
// important private helper function
void FollowsTable::typedShenanigansStar(Integer before, StatementType beforeStmtType, Integer after,
                                        StatementType afterStmtType)
{
    tryAddBefore(before, beforeStmtType, afterStmtType, stmtBeforeStarType, stmtBeforeStarTypeSet);
    tryAddBefore(before, beforeStmtType, AnyStatement, stmtBeforeStarType, stmtBeforeStarTypeSet);
    tryAddBefore(before, AnyStatement, afterStmtType, stmtBeforeStarType, stmtBeforeStarTypeSet);
    tryAddBefore(before, AnyStatement, AnyStatement, stmtBeforeStarType, stmtBeforeStarTypeSet);

    tryAddAfter(after, beforeStmtType, afterStmtType, stmtAfterStarType, stmtAfterStarTypeSet);
    tryAddAfter(after, beforeStmtType, AnyStatement, stmtAfterStarType, stmtAfterStarTypeSet);
    tryAddAfter(after, AnyStatement, afterStmtType, stmtAfterStarType, stmtAfterStarTypeSet);
    tryAddAfter(after, AnyStatement, AnyStatement, stmtAfterStarType, stmtAfterStarTypeSet);

    // for tuples
    auto pair = std::make_pair(before, after);
    followsStarTuples[AnyStatement][AnyStatement].push_back(pair);
    followsStarTuples[AnyStatement][afterStmtType].push_back(pair);
    followsStarTuples[beforeStmtType][AnyStatement].push_back(pair);
    followsStarTuples[beforeStmtType][afterStmtType].push_back(pair);
}

void FollowsTable::addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after,
                                           StatementType afterStmtType)
{
    // store statements before it
    stmtBeforeMap[after] = std::make_pair(before, beforeStmtType);
    // store statements after it
    stmtAfterMap[before] = std::make_pair(after, afterStmtType);

    typedShenanigans(before, beforeStmtType, after, afterStmtType);
}

void FollowsTable::addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                               const Vector<StatementNumWithType>& afterStmttypePairs)
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
    return stmtAfterMap[before].first == after;
}

Boolean FollowsTable::checkIfFollowsHoldsStar(Integer before, Integer after)
{
    auto stmtSet = stmtAfterstarsetMap[before];
    return stmtSet.find(after) != stmtSet.end();
}

Vector<StatementNumWithType> FollowsTable::getAfterStatement(Integer before)
{
    Vector<StatementNumWithType> toReturn;
    if (stmtAfterMap[before].first != 0) {
        toReturn.push_back(stmtAfterMap[before]);
    }
    return toReturn;
}
Vector<StatementNumWithType> FollowsTable::getBeforeStatement(Integer after)
{
    Vector<StatementNumWithType> toReturn;
    if (stmtBeforeMap[after].first != 0) {
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
Vector<Pair<Integer, Integer>> FollowsTable::getAllFollowsTuple(StatementType stmtTypeOfBefore,
                                                                StatementType stmtTypeOfAfter)
{
    return followsTuples[stmtTypeOfBefore][stmtTypeOfAfter];
}
Vector<Pair<Integer, Integer>> FollowsTable::getAllFollowsTupleStar(StatementType stmtTypeOfBefore,
                                                                    StatementType stmtTypeOfAfter)
{
    return followsStarTuples[stmtTypeOfBefore][stmtTypeOfAfter];
}
