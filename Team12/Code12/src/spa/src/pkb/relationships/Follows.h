#ifndef SPA_BEFORE_H
#define SPA_BEFORE_H

#include <pkb/PkbTypes.h>

typedef ArrayArrayTupleList<Integer, Integer> TupleTable;

/**
 * Stores Follows, Follows* relationships.
 */
class FollowsTable {
public:
    // writing
    void addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after,
                                 StatementType afterStmtType);
    void addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                     const Vector<StatementNumWithType>& afterStmttypePairs);

    // reading
    Boolean checkIfFollowsHolds(Integer before, Integer after);
    Boolean checkIfFollowsHoldsStar(Integer before, Integer after);

    Vector<StatementNumWithType> getAfterStatement(Integer before);
    Vector<StatementNumWithType> getBeforeStatement(Integer after);
    Vector<Integer> getAllAfterStatementsStar(Integer before, StatementType stmtType);
    Vector<Integer> getAllBeforeStatementsStar(Integer after, StatementType stmtType);

    Vector<Integer> getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Pair<Integer, Integer>> getAllFollowsTuple(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Pair<Integer, Integer>> getAllFollowsTupleStar(StatementType stmtTypeOfBefore,
                                                          StatementType stmtTypeOfAfter);

private:
    // Table and inverse tables
    /**
     * Primary key: statement number
     * Result: next/previous statements, stored by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    // there can only be one statement before/after a given statement.
    // idempotent, just overwrite if multiple values received
    HashMap<StatementNumber, StatementNumWithType> stmtBeforeMap;
    HashMap<StatementNumber, StatementNumWithType> stmtAfterMap;

    // this is not the case for star. Multiple values means we need to deduplicate
    HashMap<StatementNumber, StatementNumVectorsByType> stmtBeforeStarMap;
    HashMap<StatementNumber, StatementNumVectorsByType> stmtAfterStarMap;
    // sets used to deduplicate the above structures
    HashMap<StatementNumber, StatementNumSetsByType> stmtBeforeStarSet;
    HashMap<StatementNumber, StatementNumSetsByType> stmtAfterStarSet;

    // Collection Tables
    /**
     * Primary key: statement type
     * Result: a table by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    Array<StatementNumVectorsByType, StatementTypeCount> stmtBeforeType;
    Array<StatementNumVectorsByType, StatementTypeCount> stmtBeforeStarType;
    Array<StatementNumVectorsByType, StatementTypeCount> stmtAfterType;
    Array<StatementNumVectorsByType, StatementTypeCount> stmtAfterStarType;
    // hashsets to prevent duplication in lists above
    Array<StatementNumSetsByType, StatementTypeCount> stmtBeforeTypeSet;
    Array<StatementNumSetsByType, StatementTypeCount> stmtBeforeStarTypeSet;
    Array<StatementNumSetsByType, StatementTypeCount> stmtAfterTypeSet;
    Array<StatementNumSetsByType, StatementTypeCount> stmtAfterStarTypeSet;

    // Tuples
    /**
     * Primary key: statement type
     * Result: a table by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    TupleTable followsTuples;
    TupleTable followsStarTuples;

    void addIntoBasicTables(StatementType beforeType, StatementNumber before, StatementType afterType,
                            StatementNumber after);
    void addIntoCollectionTables(StatementType beforeType, StatementNumber before, StatementType afterType,
                                 StatementNumber after);
    void addIntoTupleTables(StatementType beforeType, StatementNumber before, StatementType afterType,
                            StatementNumber after);
    void addIntoBasicTablesStar(StatementType beforeType, StatementNumber before, StatementType afterType,
                                StatementNumber after);
    void addIntoCollectionTablesStar(StatementType beforeType, StatementNumber before, StatementType afterType,
                                     StatementNumber after);
    void addIntoTupleTablesStar(StatementType beforeType, StatementNumber before, StatementType afterType,
                                StatementNumber after);
};
#endif // SPA_BEFORE_H
