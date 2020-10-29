/**
 * Implementation of the ParentTable Class.
 */

#include "Parent.h"

#include <cassert>
#include <pkb/PKBUtils.h>

/**
 * Adds relationship as given by parameters into the relevant basic tables (table and its inverse).
 *
 * @param parentType
 * @param parent
 * @param childType
 * @param child
 */
void ParentTable::addIntoBasicTables(StatementType parentType, StatementNumber parent, StatementType childType,
                                     StatementNumber child)
{
    // store statements parent it
    stmtParentMap[child] = std::make_pair(parent, parentType);
    // store statements child it
    tryAdd(parent, childType, child, stmtChildMap, stmtChildSet);
}

/**
 * Adds relationship as given by parameters into the relevant collection tables (table and its inverse).
 *
 * @param parentType
 * @param parent
 * @param childType
 * @param child
 */
void ParentTable::addIntoCollectionTables(StatementType parentType, StatementNumber parent, StatementType childType,
                                          StatementNumber child)
{
    // stmtParentType has shape StatementType (child) -> StatementType (parent) -> StatementNumber
    tryAdd(childType, parentType, parent, stmtParentType, stmtParentTypeSet);

    // stmtChildType has shape StatementType (parent) -> StatementType (child) -> StatementNumber
    tryAdd(parentType, childType, child, stmtChildType, stmtChildTypeSet);
}

/**
 * Adds relationship as given by parameters into the relevant tuple tables (table and its inverse).
 *
 * @param parentType
 * @param parent
 * @param childType
 * @param child
 */
void ParentTable::addIntoTupleTables(StatementType parentType, StatementNumber parent, StatementType childType,
                                     StatementNumber child)
{
    auto pair = std::make_pair(parent, child);
    parentTuples[AnyStatement][AnyStatement].push_back(pair);
    parentTuples[AnyStatement][childType].push_back(pair);
    parentTuples[parentType][AnyStatement].push_back(pair);
    parentTuples[parentType][childType].push_back(pair);
}

/**
 * Adds relationship (star) as given by parameters into the relevant basic tables (table and its inverse).
 *
 * @param parentType
 * @param parent
 * @param childType
 * @param child
 */
void ParentTable::addIntoBasicTablesStar(StatementType parentType, StatementNumber parent, StatementType childType,
                                         StatementNumber child)
{
    // stmtParentStarMap has shape StatementNumber -> StatementType -> Statement number
    tryAdd(child, parentType, parent, stmtParentStarMap, stmtParentStarSet);

    // stmtChildStarMap has shape StatementNumber -> StatementType -> Statement number
    tryAdd(parent, childType, child, stmtChildStarMap, stmtChildStarSet);
}

/**
 * Adds relationship (star) as given by parameters into the relevant collection tables (table and its inverse).
 *
 * @param parentType
 * @param parent
 * @param childType
 * @param child
 */
void ParentTable::addIntoCollectionTablesStar(StatementType parentType, StatementNumber parent, StatementType childType,
                                              StatementNumber child)
{
    // stmtParentType has shape StatementType (child) -> StatementType (parent) -> StatementNumber
    tryAdd(childType, parentType, parent, stmtParentStarType, stmtParentStarTypeSet);

    // stmtChildType has shape StatementType (parent) -> StatementType (child) -> StatementNumber
    tryAdd(parentType, childType, child, stmtChildStarType, stmtChildStarTypeSet);
}

/**
 * Adds relationship (star) as given by parameters into the relevant tuple tables (table and its inverse).
 *
 * @param parentType
 * @param parent
 * @param childType
 * @param child
 */
void ParentTable::addIntoTupleTablesStar(StatementType parentType, StatementNumber parent, StatementType childType,
                                         StatementNumber child)
{
    auto pair = std::make_pair(parent, child);
    parentStarTuples[AnyStatement][AnyStatement].push_back(pair);
    parentStarTuples[AnyStatement][childType].push_back(pair);
    parentStarTuples[parentType][AnyStatement].push_back(pair);
    parentStarTuples[parentType][childType].push_back(pair);
}

/**
 * Given a relationship Parent(a, b), insert them into the basic tables and tuple tables. Idempotent.
 *
 * @param parent
 * @param parentStmtType
 * @param child
 * @param childStmtType
 */
void ParentTable::addParentRelationships(StatementNumber parent, StatementType parentStmtType, StatementNumber child,
                                         StatementType childStmtType)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        parentStmtType > AnyStatement && parentStmtType < StatementTypeCount
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        childStmtType > AnyStatement && childStmtType < StatementTypeCount
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

    addIntoBasicTables(parentStmtType, parent, childStmtType, child);
    addIntoCollectionTables(parentStmtType, parent, childStmtType, child);
    addIntoTupleTables(parentStmtType, parent, childStmtType, child);
}

/**
 * Given a relationship Parent*(a, b), insert them into the basic tables and tuple tables. Idempotent.
 *
 * @param parent
 * @param parentStmtType
 * @param child
 * @param childStmtType
 */
void ParentTable::addParentRelationshipsStar(StatementNumber parent, StatementType parentStmtType,
                                             const Vector<StatementNumWithType>& childStmttypePairs)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        parentStmtType > AnyStatement && parentStmtType < StatementTypeCount
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    for (auto stmtTypePair : childStmttypePairs) {
        StatementNumber child = stmtTypePair.first;
        StatementType childStmtType = stmtTypePair.second;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        assert(
            childStmtType > AnyStatement && childStmtType < StatementTypeCount
            && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

        addIntoBasicTablesStar(parentStmtType, parent, childStmtType, child);
        addIntoCollectionTablesStar(parentStmtType, parent, childStmtType, child);
        addIntoTupleTablesStar(parentStmtType, parent, childStmtType, child);
    }
}

/**
 * Returns `TRUE` if there is a Parent relationship between `parent` and `child`, else return `FALSE`.
 *
 * @param parent
 * @param child
 * @return
 */
Boolean ParentTable::checkIfParentHolds(StatementNumber parent, StatementNumber child)
{
    return stmtChildSet[parent].byType[AnyStatement].find(child) != stmtChildSet[parent].byType[AnyStatement].end();
}

/**
 * Returns `TRUE` if there is a Parent* relationship between `parent` and `child`, else return `FALSE`.
 *
 * @param parent
 * @param child
 * @return
 */
Boolean ParentTable::checkIfParentHoldsStar(StatementNumber parent, StatementNumber child)
{
    auto stmtSet = stmtChildStarSet[parent].byType[AnyStatement];
    return stmtSet.find(child) != stmtSet.end();
}

/**
 * Returns a *STATEMENT_LIST* containing the child *STATEMENT* of `parent`, all of which are of `childType`.
 * *STATEMENT_LIST* returned is empty if there is no such statements exist for `parent`.
 *
 * @param parent
 * @param childType
 * @return
 */
Vector<StatementNumber> ParentTable::getAllChildStatements(StatementNumber parent, StatementType childType)
{
    return stmtChildMap[parent].byType[childType];
}

/**
 * Returns a *STATEMENT_LIST* containing the parent *STATEMENT* of `child`. *STATEMENT_LIST* returned is empty if there
 * is no such relationship for `child`.
 *
 * @param child
 * @return
 */
Vector<StatementNumWithType> ParentTable::getParentStatement(StatementNumber child)
{
    Vector<StatementNumWithType> toReturn;
    if (stmtParentMap[child].first != 0) {
        toReturn.push_back(stmtParentMap[child]);
    }
    return toReturn;
}

/**
 * Returns a *STATEMENT_LIST* of all _transitive_ child *STATEMENT* of `parent`. Child *STATEMENT* are of
 * *STATEMENT_TYPE* `stmtType`.
 *
 * @param parent
 * @param stmtType
 * @return
 */
Vector<StatementNumber> ParentTable::getAllChildStatementsStar(StatementNumber parent, StatementType stmtType)
{
    return stmtChildStarMap[parent].byType[stmtType];
}

/**
 * Returns a *STATEMENT_LIST* of all _transitive_ Parent of `child`. Parents are of STATEMENT_TYPE `stmtType`.
 *
 * @param child
 * @param stmtType
 * @return
 */
Vector<StatementNumber> ParentTable::getAllParentStatementsStar(StatementNumber child, StatementType stmtType)
{
    return stmtParentStarMap[child].byType[stmtType];
}

/**
 * Returns a *STATEMENT_LIST* of all Parents that are of *STATEMENT_TYPE* `stmtTypeOfParent`, with a child of
 * *STATEMENT_TYPE* `stmtTypeOfChild`.
 *
 * @param stmtTypeOfParent
 * @param stmtTypeOfChild
 * @return
 */
Vector<StatementNumber> ParentTable::getAllParentStatementsTyped(StatementType stmtTypeOfParent,
                                                                 StatementType stmtTypeOfChild)
{
    return stmtParentType[stmtTypeOfChild].byType[stmtTypeOfParent];
}

/**
 * Returns a *STATEMENT_LIST* of all Parents that are of *STATEMENT_TYPE* `stmtTypeOfParent`, with a _transitive_ child
 * of *STATEMENT_TYPE* `stmtTypeOfChild`.
 *
 * @param stmtTypeOfParent
 * @param stmtTypeOfChild
 * @return
 */
Vector<StatementNumber> ParentTable::getAllParentStatementsTypedStar(StatementType stmtTypeOfParent,
                                                                     StatementType stmtTypeOfChild)
{
    return stmtParentStarType[stmtTypeOfChild].byType[stmtTypeOfParent];
}

/**
 * Returns a *STATEMENT_LIST* of all children that are of *STATEMENT_TYPE* `stmtTypeOfChild`, with a child
 *of *STATEMENT_TYPE* `stmtTypeOfParent`.
 *
 * @param stmtTypeOfParent
 * @param stmtTypeOfChild
 * @return
 */
Vector<StatementNumber> ParentTable::getAllChildStatementsTyped(StatementType stmtTypeOfParent,
                                                                StatementType stmtTypeOfChild)
{
    return stmtChildType[stmtTypeOfParent].byType[stmtTypeOfChild];
}

/**
 * Returns a *STATEMENT_LIST* of all children that are of *STATEMENT_TYPE* `stmtTypeOfChild`, with a _transitive_ Parent
 * of *STATEMENT_TYPE* `stmtTypeOfParent`.
 *
 * @param stmtTypeOfParent
 * @param stmtTypeOfChild
 * @return
 */
Vector<StatementNumber> ParentTable::getAllChildStatementsTypedStar(StatementType stmtTypeOfParent,
                                                                    StatementType stmtTypeOfChild)
{
    return stmtChildStarType[stmtTypeOfParent].byType[stmtTypeOfChild];
}

/**
 *Description*: Returns a list of pairs, where for each pair, the *Parent* relationship holds between them, and the
 *parent is of `stmtTypeOfParent`, and the child is of `stmtTypeOfChild`.
 *
 * @param stmtTypeOfParent
 * @param stmtTypeOfChild
 * @return
 */
Vector<Pair<StatementNumber, StatementNumber>> ParentTable::getAllParentTuple(StatementType stmtTypeOfParent,
                                                                              StatementType stmtTypeOfChild)
{
    return parentTuples[stmtTypeOfParent][stmtTypeOfChild];
}

/**
 * Returns a list of pairs, where for each pair, the *Parent** relationship holds between them, and the parent is of
 * `stmtTypeOfParent`, and the child is of `stmtTypeOfChild`.
 *
 * @param stmtTypeOfParent
 * @param stmtTypeOfChild
 * @return
 */
Vector<Pair<StatementNumber, StatementNumber>> ParentTable::getAllParentTupleStar(StatementType stmtTypeOfParent,
                                                                                  StatementType stmtTypeOfChild)
{
    return parentStarTuples[stmtTypeOfParent][stmtTypeOfChild];
}
