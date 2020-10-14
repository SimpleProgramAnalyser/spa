#include "Next.h"

#include <cassert>
#include <pkb/PKBUtils.h>

/**
 * Adds relationship as given by parameters into the relevant basic tables (table and its inverse).
 *
 * @param previous
 * @param previousType
 * @param next
 * @param nextType
 */
void NextTable::addIntoBasicTables(Integer previous, StatementType previousType, Integer next, StatementType nextType)
{
    tryAdd(previous, nextType, next, stmtNextMap, stmtNextSet);
    tryAdd(next, previousType, previous, stmtPreviousMap, stmtPreviousSet);
}

/**
 * Adds relationship as given by parameters into the relevant collection tables (table and its inverse).
 *
 * @param previous
 * @param previousType
 * @param next
 * @param nextType
 */
void NextTable::addIntoCollectionTables(Integer previous, StatementType previousType, Integer next,
                                        StatementType nextType)
{
    tryAdd(previousType, nextType, next, stmtPreviousType, stmtPreviousTypeSet);
    tryAdd(nextType, previousType, previous, stmtPreviousType, stmtPreviousTypeSet);
}

/**
 * Adds relationship as given by parameters into the relevant tuple tables.
 *
 * @param previous
 * @param previousType
 * @param next
 * @param nextType
 */
void NextTable::addIntoTupleTables(Integer previous, StatementType previousType, Integer next, StatementType nextType)
{
    auto pair = std::make_pair(previous, next);
    followsTuples[AnyStatement][AnyStatement].push_back(pair);
    followsTuples[AnyStatement][nextType].push_back(pair);
    followsTuples[previousType][AnyStatement].push_back(pair);
    followsTuples[previousType][nextType].push_back(pair);
}

/**
 * Given a relationship Next(prev, next) and their respective types, insert into all related structures in the Next
 * Table. Usually 3 types of tables to add to: the table and inverse table, collection table and inverse, and tuple
 * table. Given Next(a,b):
 *
 * @param previous a
 * @param previousStmtType type of a
 * @param next b
 * @param nextStmtType type of b
 */
void NextTable::addNextRelationships(Integer previous, StatementType previousStmtType, Integer next,
                                     StatementType nextStmtType)
{
    // disable warnings due to assertions
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        previousStmtType > AnyStatement && previousStmtType < STATEMENT_TYPE_COUNT
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        nextStmtType > AnyStatement && nextStmtType < STATEMENT_TYPE_COUNT
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

    addIntoBasicTables(previous, previousStmtType, next, nextStmtType);
    addIntoCollectionTables(previous, previousStmtType, next, nextStmtType);
    addIntoTupleTables(previous, previousStmtType, next, nextStmtType);
}

/**
 * Returns true if Next(previous, next), and false otherwise.
 *
 * @param previous
 * @param next
 * @return
 */
Boolean NextTable::checkIfNextHolds(Integer previous, Integer next)
{
    auto targetSet = stmtNextSet[previous].byType[AnyStatement];
    return targetSet.find(next) != targetSet.end();
}

/**
 * Get all statements such that Next(previous, x) is true and x has type nextType in a vector. Vector is empty if no
 * relevant x is found.
 *
 * @param previous
 * @param nextType
 * @return
 */
Vector<StatementNumber> NextTable::getAllNextStatements(StatementNumber previous, StatementType nextType)
{
    return stmtNextMap[previous].byType[nextType];
}

/**
 * Get all statements such that Next(x, next) is true and x has type previousType in a vector. Vector is empty if no
 * relevant x is found.
 *
 * @param previous
 * @param nextType
 * @return
 */
Vector<StatementNumber> NextTable::getAllPreviousStatements(StatementNumber next, StatementType previousType)
{
    return stmtPreviousMap[next].byType[previousType];
}

/**
 * Get all x such that Next(x,y) are true and x, y are of types previousType, nextType respectively in a vector. Vector
 * is empty if no relevant x is found.
 *
 * @param previousType
 * @param nextType
 * @return
 */
Vector<Integer> NextTable::getAllPreviousStatementsTyped(StatementType previousType, StatementType nextType)
{
    return stmtPreviousType[nextType].byType[previousType];
}

/**
 * Get all y such that Next(x,y) are true and x, y are of types previousType, nextType respectively in a vector. Vector
 * is empty if no relevant y is found.
 *
 * @param previousType
 * @param nextType
 * @return
 */
Vector<Integer> NextTable::getAllNextStatementsTyped(StatementType previousType, StatementType nextType)
{
    return stmtNextType[previousType].byType[nextType];
}

/**
 * Get all (x,y) pairs such that Next(x,y) are true and x, y are of types previousType, nextType respectively in a
 * vector. Vector is empty if no relevant (x,y) is found.
 *
 * @param previousType
 * @param nextType
 * @return
 */
Vector<Pair<Integer, Integer>> NextTable::getAllNextTuples(StatementType previousType, StatementType nextType)
{
    return followsTuples[previousType][nextType];
}
