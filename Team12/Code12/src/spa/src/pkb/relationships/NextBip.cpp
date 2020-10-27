#include "NextBip.h"

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
void NextBipTable::addIntoBasicTables(Integer previous, StatementType previousType, Integer next,
                                      StatementType nextType)
{
    tryAdd(previous, nextType, next, stmtNextBipMap, stmtNextBipSet);
    tryAdd(next, previousType, previous, stmtPreviousBipMap, stmtPreviousBipSet);
}

/**
 * Adds relationship as given by parameters into the relevant collection tables (table and its inverse).
 *
 * @param previous
 * @param previousType
 * @param next
 * @param nextType
 */
void NextBipTable::addIntoCollectionTables(Integer previous, StatementType previousType, Integer next,
                                           StatementType nextType)
{
    tryAdd(previousType, nextType, next, stmtNextBipType, stmtNextBipTypeSet);
    tryAdd(nextType, previousType, previous, stmtPreviousBipType, stmtPreviousBipTypeSet);
}

/**
 * Adds relationship as given by parameters into the relevant tuple tables.
 *
 * @param previous
 * @param previousType
 * @param next
 * @param nextType
 */
void NextBipTable::addIntoTupleTables(Integer previous, StatementType previousType, Integer next,
                                      StatementType nextType)
{
    auto pair = std::make_pair(previous, next);
    followsTuples[AnyStatement][AnyStatement].push_back(pair);
    followsTuples[AnyStatement][nextType].push_back(pair);
    followsTuples[previousType][AnyStatement].push_back(pair);
    followsTuples[previousType][nextType].push_back(pair);
}

/**
 * Given a relationship NextBip(prev, next) and their respective types, insert into all related structures in the
 * NextBip Table. Usually 3 types of tables to add to: the table and inverse table, collection table and inverse, and
 * tuple table. Given NextBip(a,b):
 *
 * @param previous a
 * @param previousStmtType type of a
 * @param next b
 * @param nextStmtType type of b
 */
void NextBipTable::addNextBipRelationships(Integer previous, StatementType previousStmtType, Integer next,
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
 * Returns true if NextBip(previous, next), and false otherwise.
 *
 * @param previous
 * @param next
 * @return
 */
Boolean NextBipTable::checkIfNextBipHolds(Integer previous, Integer next)
{
    auto targetSet = stmtNextBipSet[previous].byType[AnyStatement];
    return targetSet.find(next) != targetSet.end();
}

/**
 * Get all statements such that NextBip(previous, x) is true and x has type nextType in a vector. Vector is empty if no
 * relevant x is found.
 *
 * @param previous
 * @param nextType
 * @return
 */
Vector<StatementNumber> NextBipTable::getAllNextBipStatements(StatementNumber previous, StatementType nextType)
{
    return stmtNextBipMap[previous].byType[nextType];
}

/**
 * Get all statements such that NextBip(x, next) is true and x has type previousType in a vector. Vector is empty if no
 * relevant x is found.
 *
 * @param previous
 * @param nextType
 * @return
 */
Vector<StatementNumber> NextBipTable::getAllPreviousBipStatements(StatementNumber next, StatementType previousType)
{
    return stmtPreviousBipMap[next].byType[previousType];
}

/**
 * Get all x such that NextBip(x,y) are true and x, y are of types previousType, nextType respectively in a vector.
 * Vector is empty if no relevant x is found.
 *
 * @param previousType
 * @param nextType
 * @return
 */
Vector<Integer> NextBipTable::getAllPreviousBipStatementsTyped(StatementType previousType, StatementType nextType)
{
    return stmtPreviousBipType[nextType].byType[previousType];
}

/**
 * Get all y such that NextBip(x,y) are true and x, y are of types previousType, nextType respectively in a vector.
 * Vector is empty if no relevant y is found.
 *
 * @param previousType
 * @param nextType
 * @return
 */
Vector<Integer> NextBipTable::getAllNextBipStatementsTyped(StatementType previousType, StatementType nextType)
{
    return stmtNextBipType[previousType].byType[nextType];
}

/**
 * Get all (x,y) pairs such that NextBip(x,y) are true and x, y are of types previousType, nextType respectively in a
 * vector. Vector is empty if no relevant (x,y) is found.
 *
 * @param previousType
 * @param nextType
 * @return
 */
Vector<Pair<Integer, Integer>> NextBipTable::getAllNextBipTuples(StatementType previousType, StatementType nextType)
{
    return followsTuples[previousType][nextType];
}
