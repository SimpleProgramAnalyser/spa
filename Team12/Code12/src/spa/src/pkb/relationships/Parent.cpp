/**
 * Implementation of the ParentTable Class.
 */

#include "Parent.h"

#include <cassert>

void ParentTable::tryAddParent(Integer parent, StatementType parentType, StatementType childType, ArrayArrayList& aal,
                               ArrayArraySet& aas)
{
    // store the parent statement into (lists of statements parent type childStmtType)
    auto setParentType = &aas[childType].byType[parentType];
    // returns true if new object is created (emplace is successful)
    if (setParentType->find(parent) == setParentType->end()) {
        setParentType->insert(parent);
        aal[childType].byType[parentType].push_back(parent);
    }
}
void ParentTable::tryAddChild(Integer child, StatementType parentType, StatementType childType, ArrayArrayList& aal,
                              ArrayArraySet& aas)
{
    // store the child statement into (lists of statements child type childStmtType)
    auto setChildType = &aas[parentType].byType[childType];
    // returns true if new object is created (emplace is successful)
    if (setChildType->find(child) == setChildType->end()) {
        setChildType->insert(child);
        aal[parentType].byType[childType].push_back(child);
    }
}
// important private helper function
void ParentTable::typedShenanigans(Integer parent, StatementType parentStmtType, Integer child,
                                   StatementType childStmtType)
{
    tryAddParent(parent, parentStmtType, childStmtType, stmtParentType, stmtParentTypeSet);
    tryAddParent(parent, parentStmtType, AnyStatement, stmtParentType, stmtParentTypeSet);
    tryAddParent(parent, AnyStatement, childStmtType, stmtParentType, stmtParentTypeSet);
    tryAddParent(parent, AnyStatement, AnyStatement, stmtParentType, stmtParentTypeSet);

    tryAddChild(child, parentStmtType, childStmtType, stmtChildType, stmtChildTypeSet);
    tryAddChild(child, parentStmtType, AnyStatement, stmtChildType, stmtChildTypeSet);
    tryAddChild(child, AnyStatement, childStmtType, stmtChildType, stmtChildTypeSet);
    tryAddChild(child, AnyStatement, AnyStatement, stmtChildType, stmtChildTypeSet);

    // for tuples
    auto pair = std::make_pair(parent, child);
    parentTuples[AnyStatement][AnyStatement].push_back(pair);
    parentTuples[AnyStatement][childStmtType].push_back(pair);
    parentTuples[parentStmtType][AnyStatement].push_back(pair);
    parentTuples[parentStmtType][childStmtType].push_back(pair);
}
// important private helper function
void ParentTable::typedShenanigansStar(Integer parent, StatementType parentStmtType, Integer child,
                                       StatementType childStmtType)
{
    tryAddParent(parent, parentStmtType, childStmtType, stmtParentStarType, stmtParentStarTypeSet);
    tryAddParent(parent, parentStmtType, AnyStatement, stmtParentStarType, stmtParentStarTypeSet);
    tryAddParent(parent, AnyStatement, childStmtType, stmtParentStarType, stmtParentStarTypeSet);
    tryAddParent(parent, AnyStatement, AnyStatement, stmtParentStarType, stmtParentStarTypeSet);

    tryAddChild(child, parentStmtType, childStmtType, stmtChildStarType, stmtChildStarTypeSet);
    tryAddChild(child, parentStmtType, AnyStatement, stmtChildStarType, stmtChildStarTypeSet);
    tryAddChild(child, AnyStatement, childStmtType, stmtChildStarType, stmtChildStarTypeSet);
    tryAddChild(child, AnyStatement, AnyStatement, stmtChildStarType, stmtChildStarTypeSet);

    // for tuples
    auto pair = std::make_pair(parent, child);
    parentStarTuples[AnyStatement][AnyStatement].push_back(pair);
    parentStarTuples[AnyStatement][childStmtType].push_back(pair);
    parentStarTuples[parentStmtType][AnyStatement].push_back(pair);
    parentStarTuples[parentStmtType][childStmtType].push_back(pair);
}

void ParentTable::addParentRelationships(Integer parent, StatementType parentStmtType, Integer child,
                                         StatementType childStmtType)
{
    assert(
        parentStmtType > AnyStatement && parentStmtType < STATEMENT_TYPE_COUNT
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        childStmtType > AnyStatement && childStmtType < STATEMENT_TYPE_COUNT
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    // store statements parent it
    stmtParentMap[child] = std::make_pair(parent, parentStmtType);
    // store statements child it
    if (stmtChildsetMap[parent].find(child) == stmtChildsetMap[parent].end()) {
        stmtChildsetMap[parent].insert(child);
        stmtChildlistMap[parent].byType[childStmtType].push_back(child);
        stmtChildlistMap[parent].byType[AnyStatement].push_back(child);
    }

    typedShenanigans(parent, parentStmtType, child, childStmtType);
}

void ParentTable::addParentRelationshipsStar(Integer parent, StatementType parentStmtType,
                                             const Vector<StatementNumWithType>& childStmttypePairs)
{
    assert(
        parentStmtType > AnyStatement && parentStmtType < STATEMENT_TYPE_COUNT
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    for (auto stmtTypePair : childStmttypePairs) {
        Integer child = stmtTypePair.first;
        StatementType childStmtType = stmtTypePair.second;
        assert(
            childStmtType > AnyStatement && childStmtType < STATEMENT_TYPE_COUNT
            && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        // set and list
        stmtParentstarsetMap[child].insert(parent);
        stmtParentstarlistMap[child].byType[parentStmtType].push_back(parent);
        stmtParentstarlistMap[child].byType[AnyStatement].push_back(parent);

        stmtChildstarsetMap[parent].insert(child);
        stmtChildstarlistMap[parent].byType[childStmtType].push_back(child);
        stmtChildstarlistMap[parent].byType[AnyStatement].push_back(child);

        typedShenanigansStar(parent, parentStmtType, child, childStmtType);
    }
}

// reading
Boolean ParentTable::checkIfParentHolds(Integer parent, Integer child)
{
    return stmtChildsetMap[parent].find(child) != stmtChildsetMap[parent].end();
}

Boolean ParentTable::checkIfParentHoldsStar(Integer parent, Integer child)
{
    auto stmtSet = stmtChildstarsetMap[parent];
    return stmtSet.find(child) != stmtSet.end();
}

Vector<Integer> ParentTable::getAllChildStatements(Integer parent, StatementType childType)
{
    return stmtChildlistMap[parent].byType[childType];
}
Vector<StatementNumWithType> ParentTable::getParentStatement(Integer child)
{
    Vector<StatementNumWithType> toReturn;
    if (stmtParentMap[child].first != 0) {
        toReturn.push_back(stmtParentMap[child]);
    }
    return toReturn;
}
Vector<Integer> ParentTable::getAllChildStatementsStar(Integer parent, StatementType stmtType)
{
    return stmtChildstarlistMap[parent].byType[stmtType];
}
Vector<Integer> ParentTable::getAllParentStatementsStar(Integer child, StatementType stmtType)
{
    return stmtParentstarlistMap[child].byType[stmtType];
}

Vector<Integer> ParentTable::getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return stmtParentType[stmtTypeOfChild].byType[stmtTypeOfParent];
}
Vector<Integer> ParentTable::getAllParentStatementsTypedStar(StatementType stmtTypeOfParent,
                                                             StatementType stmtTypeOfChild)
{
    return stmtParentStarType[stmtTypeOfChild].byType[stmtTypeOfParent];
}
Vector<Integer> ParentTable::getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return stmtChildType[stmtTypeOfParent].byType[stmtTypeOfChild];
}
Vector<Integer> ParentTable::getAllChildStatementsTypedStar(StatementType stmtTypeOfParent,
                                                            StatementType stmtTypeOfChild)
{
    return stmtChildStarType[stmtTypeOfParent].byType[stmtTypeOfChild];
}
Vector<Pair<Integer, Integer>> ParentTable::getAllParentTuple(StatementType stmtTypeOfParent,
                                                              StatementType stmtTypeOfChild)
{
    return parentTuples[stmtTypeOfParent][stmtTypeOfChild];
}
Vector<Pair<Integer, Integer>> ParentTable::getAllParentTupleStar(StatementType stmtTypeOfParent,
                                                                  StatementType stmtTypeOfChild)
{
    return parentStarTuples[stmtTypeOfParent][stmtTypeOfChild];
}
