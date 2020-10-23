#ifndef SPA_PARENT_H
#define SPA_PARENT_H

#include <pkb/PkbTypes.h>

typedef ArrayArrayTupleList<StatementNumber, StatementNumber> TupleTable;

/**
 * Stores Parent, Parent* relationships.
 */
class ParentTable {
public:
    // writing
    void addParentRelationships(StatementNumber parent, StatementType parentStmtType, StatementNumber child,
                                StatementType childStmtType);
    void addParentRelationshipsStar(StatementNumber parent, StatementType parentStmtType,
                                    const Vector<StatementNumWithType>& childStmttypePairs);

    // reading
    Boolean checkIfParentHolds(StatementNumber parent, StatementNumber child);
    Boolean checkIfParentHoldsStar(StatementNumber parent, StatementNumber child);

    Vector<StatementNumber> getAllChildStatements(StatementNumber parent, StatementType childType);
    Vector<StatementNumWithType> getParentStatement(StatementNumber child);
    Vector<StatementNumber> getAllChildStatementsStar(StatementNumber parent, StatementType stmtType);
    Vector<StatementNumber> getAllParentStatementsStar(StatementNumber child, StatementType stmtType);

    Vector<StatementNumber> getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<StatementNumber> getAllParentStatementsTypedStar(StatementType stmtTypeOfParent,
                                                            StatementType stmtTypeOfChild);
    Vector<StatementNumber> getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<StatementNumber> getAllChildStatementsTypedStar(StatementType stmtTypeOfParent,
                                                           StatementType stmtTypeOfChild);
    Vector<Pair<StatementNumber, StatementNumber>> getAllParentTuple(StatementType stmtTypeOfParent,
                                                                     StatementType stmtTypeOfChild);
    Vector<Pair<StatementNumber, StatementNumber>> getAllParentTupleStar(StatementType stmtTypeOfParent,
                                                                         StatementType stmtTypeOfChild);

private:
    // Table and inverse tables
    /**
     * Primary key: statement number
     * Result: next/previous statements, stored by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    // there can only be one parent statement for a given statement.
    // idempotent, just overwrite if multiple values received
    HashMap<StatementNumber, StatementNumWithType> stmtParentMap;

    // this is not the case for child, parent* and child*. Multiple values means we need to deduplicate
    HashMap<StatementNumber, StatementNumVectorsByType> stmtChildMap;
    HashMap<StatementNumber, StatementNumVectorsByType> stmtParentStarMap;
    HashMap<StatementNumber, StatementNumVectorsByType> stmtChildStarMap;

    // sets used to deduplicate the above structures
    HashMap<StatementNumber, StatementNumSetsByType> stmtChildSet;
    HashMap<StatementNumber, StatementNumSetsByType> stmtParentStarSet;
    HashMap<StatementNumber, StatementNumSetsByType> stmtChildStarSet;

    // Collection Tables
    /**
     * Primary key: statement type
     * Result: a table by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtParentType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtParentStarType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtChildType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtChildStarType;
    // hashsets to prevent duplication in lists above
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtParentTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtParentStarTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtChildTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtChildStarTypeSet;

    // Tuples
    /**
     * Primary key: statement type
     * Result: a table by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    TupleTable parentTuples;
    TupleTable parentStarTuples;

    void addIntoBasicTables(StatementType parentType, StatementNumber parent, StatementType childType,
                            StatementNumber child);
    void addIntoCollectionTables(StatementType parentType, StatementNumber parent, StatementType childType,
                                 StatementNumber child);
    void addIntoTupleTables(StatementType parentType, StatementNumber parent, StatementType childType,
                            StatementNumber child);
    void addIntoBasicTablesStar(StatementType parentType, StatementNumber parent, StatementType childType,
                                StatementNumber child);
    void addIntoCollectionTablesStar(StatementType parentType, StatementNumber parent, StatementType childType,
                                     StatementNumber child);
    void addIntoTupleTablesStar(StatementType parentType, StatementNumber parent, StatementType childType,
                                StatementNumber child);
};

#endif // SPA_PARENT_H
