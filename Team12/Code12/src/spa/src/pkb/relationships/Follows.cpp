/**
 * Implementation of the FollowsTable Class.
 */

#include "Follows.h"

#include <cassert>
#include <pkb/PKBUtils.h>

/**
 * Adds relationship as given by parameters into the relevant basic tables (table and its inverse).
 *
 * @param beforeType
 * @param before
 * @param afterType
 * @param after
 */
void FollowsTable::addIntoBasicTables(StatementType beforeType, StatementNumber before, StatementType afterType,
                                      StatementNumber after)
{
    // store statements before it
    stmtBeforeMap[after] = std::make_pair(before, beforeType);
    // store statements after it
    stmtAfterMap[before] = std::make_pair(after, afterType);
}

/**
 * Adds relationship as given by parameters into the relevant collection tables (table and its inverse).
 *
 * @param beforeType
 * @param before
 * @param afterType
 * @param after
 */
void FollowsTable::addIntoCollectionTables(StatementType beforeType, StatementNumber before, StatementType afterType,
                                           StatementNumber after)
{
    // stmtBeforeType has shape StatementType (after) -> StatementType (before) -> StatementNumber
    tryAdd(afterType, beforeType, before, stmtBeforeType, stmtBeforeTypeSet);

    // stmtAfterType has shape StatementType (before) -> StatementType (after) -> StatementNumber
    tryAdd(beforeType, afterType, after, stmtAfterType, stmtAfterTypeSet);
}

/**
 * Adds relationship as given by parameters into the relevant tuple tables (table and its inverse).
 *
 * @param beforeType
 * @param before
 * @param afterType
 * @param after
 */
void FollowsTable::addIntoTupleTables(StatementType beforeType, StatementNumber before, StatementType afterType,
                                      StatementNumber after)
{
    auto pair = std::make_pair(before, after);
    followsTuples[AnyStatement][AnyStatement].push_back(pair);
    followsTuples[AnyStatement][afterType].push_back(pair);
    followsTuples[beforeType][AnyStatement].push_back(pair);
    followsTuples[beforeType][afterType].push_back(pair);
}

/**
 * Adds relationship (star) as given by parameters into the relevant basic tables (table and its inverse).
 *
 * @param beforeType
 * @param before
 * @param afterType
 * @param after
 */
void FollowsTable::addIntoBasicTablesStar(StatementType beforeType, StatementNumber before, StatementType afterType,
                                          StatementNumber after)
{
    // stmtBeforeStarMap has shape StatementNumber -> StatementType -> Statement number
    tryAdd(after, beforeType, before, stmtBeforeStarMap, stmtBeforeStarSet);

    // stmtAfterStarMap has shape StatementNumber -> StatementType -> Statement number
    tryAdd(before, afterType, after, stmtAfterStarMap, stmtAfterStarSet);
}

/**
 * Adds relationship (star) as given by parameters into the relevant collection tables (table and its inverse).
 *
 * @param beforeType
 * @param before
 * @param afterType
 * @param after
 */
void FollowsTable::addIntoCollectionTablesStar(StatementType beforeType, StatementNumber before,
                                               StatementType afterType, StatementNumber after)
{
    // stmtBeforeType has shape StatementType (after) -> StatementType (before) -> StatementNumber
    tryAdd(afterType, beforeType, before, stmtBeforeStarType, stmtBeforeStarTypeSet);

    // stmtAfterType has shape StatementType (before) -> StatementType (after) -> StatementNumber
    tryAdd(beforeType, afterType, after, stmtAfterStarType, stmtAfterStarTypeSet);
}

/**
 * Adds relationship (star) as given by parameters into the relevant tuple tables (table and its inverse).
 *
 * @param beforeType
 * @param before
 * @param afterType
 * @param after
 */
void FollowsTable::addIntoTupleTablesStar(StatementType beforeType, StatementNumber before, StatementType afterType,
                                          StatementNumber after)
{
    auto pair = std::make_pair(before, after);
    followsStarTuples[AnyStatement][AnyStatement].push_back(pair);
    followsStarTuples[AnyStatement][afterType].push_back(pair);
    followsStarTuples[beforeType][AnyStatement].push_back(pair);
    followsStarTuples[beforeType][afterType].push_back(pair);
}

/**
 * Given a relationship Follows(a, b), insert them into the basic tables and tuple tables. Idempotent.
 *
 * @param before
 * @param beforeStmtType
 * @param after
 * @param afterStmtType
 */
void FollowsTable::addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after,
                                           StatementType afterStmtType)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        beforeStmtType > AnyStatement && beforeStmtType < StatementTypeCount
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        afterStmtType > AnyStatement && afterStmtType < StatementTypeCount
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

    addIntoBasicTables(beforeStmtType, before, afterStmtType, after);
    addIntoCollectionTables(beforeStmtType, before, afterStmtType, after);
    addIntoTupleTables(beforeStmtType, before, afterStmtType, after);
}

/**
 * Given a relationship Follows*(a, b), insert them into the basic tables and tuple tables. Idempotent.
 *
 * @param before
 * @param beforeStmtType
 * @param after
 * @param afterStmtType
 */
void FollowsTable::addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                               const Vector<StatementNumWithType>& afterStmttypePairs)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        beforeStmtType > AnyStatement && beforeStmtType < StatementTypeCount
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    for (auto stmtTypePair : afterStmttypePairs) {
        Integer after = stmtTypePair.first;
        StatementType afterStmtType = stmtTypePair.second;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        assert(
            afterStmtType > AnyStatement && afterStmtType < StatementTypeCount
            && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

        addIntoBasicTablesStar(beforeStmtType, before, afterStmtType, after);
        addIntoCollectionTablesStar(beforeStmtType, before, afterStmtType, after);
        addIntoTupleTablesStar(beforeStmtType, before, afterStmtType, after);
    }
}

/**
 * Returns TRUE if there is a Follows relationship between before and after, else return FALSE.
 *
 * @param before
 * @param after
 * @return
 */
Boolean FollowsTable::checkIfFollowsHolds(Integer before, Integer after)
{
    return stmtAfterMap.find(before) != stmtAfterMap.end() && stmtAfterMap[before].first == after;
}

/**
 * Returns TRUE if there is a Follows* relationship between before and after, else return FALSE.
 *
 * @param before
 * @param after
 * @return
 */
Boolean FollowsTable::checkIfFollowsHoldsStar(Integer before, Integer after)
{
    if (stmtAfterStarSet.find(before) == stmtAfterStarSet.end()) {
        return false;
    }
    auto stmtSet = stmtAfterStarSet[before].byType[AnyStatement];
    return stmtSet.find(after) != stmtSet.end();
}

/**
 * Returns a *STATEMENT_LIST* containing the after *STATEMENT* of `before`. *STATEMENT_LIST* returned is empty if there
 * is no such relationship for `before`.
 *
 * @param before
 * @return
 */
Vector<StatementNumWithType> FollowsTable::getAfterStatement(Integer before)
{
    Vector<StatementNumWithType> toReturn;
    if (stmtAfterMap[before].first != 0) {
        toReturn.push_back(stmtAfterMap[before]);
    }
    return toReturn;
}

/**
 * Returns a *STATEMENT_LIST* containing the before *STATEMENT* of `after`. *STATEMENT_LIST* returned is empty if there
 * is no such relationship for `after`.
 *
 * @param after
 * @return
 */
Vector<StatementNumWithType> FollowsTable::getBeforeStatement(Integer after)
{
    Vector<StatementNumWithType> toReturn;
    if (stmtBeforeMap[after].first != 0) {
        toReturn.push_back(stmtBeforeMap[after]);
    }
    return toReturn;
}

/**
 * Returns a *STATEMENT_LIST* of statements such that `Follows(before,returned)` holds and are of STATEMENT_TYPE
 * `stmtType`.
 *
 * @param before
 * @param stmtType
 * @return
 */
Vector<Integer> FollowsTable::getAllAfterStatementsStar(Integer before, StatementType stmtType)
{
    return stmtAfterStarMap[before].byType[stmtType];
}

/**
 * Returns a *STATEMENT_LIST* of statements that are before* `after` and are of STATEMENT_TYPE `stmtType`.
 *
 * @param after
 * @param stmtType
 * @return
 */
Vector<Integer> FollowsTable::getAllBeforeStatementsStar(Integer after, StatementType stmtType)
{
    return stmtBeforeStarMap[after].byType[stmtType];
}

/**
 * Returns a *STATEMENT_LIST* of all before statements that are of *STATEMENT_TYPE* `stmtTypeOfBefore`, with a after of
 * *STATEMENT_TYPE* `stmtTypeOfAfter`.
 *
 * @param stmtTypeOfBefore
 * @param stmtTypeOfAfter
 * @return
 */
Vector<Integer> FollowsTable::getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return stmtBeforeType[stmtTypeOfAfter].byType[stmtTypeOfBefore];
}

/**
 * Returns a *STATEMENT_LIST* of statements such that `Follows*(before,returned)` holds and are of STATEMENT_TYPE
 * `stmtType`.
 *
 * @param stmtTypeOfBefore
 * @param stmtTypeOfAfter
 * @return
 */
Vector<Integer> FollowsTable::getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore,
                                                              StatementType stmtTypeOfAfter)
{
    return stmtBeforeStarType[stmtTypeOfAfter].byType[stmtTypeOfBefore];
}

/**
 * Returns a *STATEMENT_LIST* of all before statements that are of *STATEMENT_TYPE* `stmtTypeOfBefore`, with a
 * _transitive_ after of *STATEMENT_TYPE* `stmtTypeOfAfter`.
 *
 * @param stmtTypeOfBefore
 * @param stmtTypeOfAfter
 * @return
 */
Vector<Integer> FollowsTable::getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return stmtAfterType[stmtTypeOfBefore].byType[stmtTypeOfAfter];
}

/**
 * Returns a *STATEMENT_LIST* of all after statements that are of *STATEMENT_TYPE* `stmtTypeOfAfter`, with a
 * _transitive_ Follows of *STATEMENT_TYPE* `stmtTypeOfBefore`.
 *
 * @param stmtTypeOfBefore
 * @param stmtTypeOfAfter
 * @return
 */
Vector<Integer> FollowsTable::getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore,
                                                             StatementType stmtTypeOfAfter)
{
    return stmtAfterStarType[stmtTypeOfBefore].byType[stmtTypeOfAfter];
}

/**
 * Returns a list of pairs, where for each pair, the *Follows* relationship holds between them, and the before is of
 * `stmtTypeOfBefore`, and the after is of `stmtTypeOfAfter`.
 *
 * @param stmtTypeOfBefore
 * @param stmtTypeOfAfter
 * @return
 */
Vector<Pair<Integer, Integer>> FollowsTable::getAllFollowsTuple(StatementType stmtTypeOfBefore,
                                                                StatementType stmtTypeOfAfter)
{
    return followsTuples[stmtTypeOfBefore][stmtTypeOfAfter];
}

/**
 * Returns a list of pairs, where for each pair, the *Follows* relationship holds between them, and the before is of
 * `stmtTypeOfBefore`, and the after is of `stmtTypeOfAfter`.
 *
 * @param stmtTypeOfBefore
 * @param stmtTypeOfAfter
 * @return
 */
Vector<Pair<Integer, Integer>> FollowsTable::getAllFollowsTupleStar(StatementType stmtTypeOfBefore,
                                                                    StatementType stmtTypeOfAfter)
{
    return followsStarTuples[stmtTypeOfBefore][stmtTypeOfAfter];
}
