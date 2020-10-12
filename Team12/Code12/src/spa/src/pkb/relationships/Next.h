#ifndef SPA_NEXT_H
#define SPA_NEXT_H

#include <pkb/PkbTypes.h>

typedef ArrayArrayTupleList<Integer, Integer> TupleTable;

class NextTable {
public:
    // Section 1: Adding relationships
    void addNextRelationships(StatementNumber prev, StatementType prevType, StatementNumber next,
                              StatementType nextType);

    // Section 2: Table and inverse table methods
    Boolean checkIfNextHolds(StatementNumber prev, StatementNumber next);
    Vector<StatementNumber> getAllNextStatements(StatementNumber prev, StatementType nextType);
    Vector<StatementNumber> getAllPreviousStatements(StatementNumber prev, StatementType nextType);

    // Section 3: Collection table methods
    Vector<StatementNumber> getAllNextStatementsTyped(StatementType prevType, StatementType nextType);
    Vector<StatementNumber> getAllPreviousStatementsTyped(StatementType prevType, StatementType nextType);

    // Section 4: Tuple methods
    Vector<Pair<StatementNumber, StatementNumber>> getAllNextTuples(StatementType prevType, StatementType nextType);

private:
    // Table and inverse tables
    /**
     * Primary key: statement number
     * Result: next/previous statements, stored by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    HashMap<Integer, StatementNumVectorsByType> stmtPreviousMap;
    HashMap<Integer, StatementNumVectorsByType> stmtNextMap;
    // sets used to deduplicate the above structure
    HashMap<Integer, StatementNumSetsByType> stmtPreviousSet;
    HashMap<Integer, StatementNumSetsByType> stmtNextSet;

    // Collection Tables
    /**
     * Primary key: statement type
     * Result: a table by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtPreviousType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtNextType;
    // hashsets to prevent duplication in lists above
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtPreviousTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtNextTypeSet;

    // Tuples
    /**
     * Primary key: statement type
     * Result: a table by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    TupleTable followsTuples;

    // The three main adding methods.
    void addIntoBasicTables(Integer previous, StatementType previousType, Integer next, StatementType nextType);
    void addIntoCollectionTables(Integer previous, StatementType previousType, Integer next, StatementType nextType);
    void addIntoTupleTables(Integer previous, StatementType previousType, Integer next, StatementType nextType);
    // deprecated.
    void typedShenanigans(Integer previous, StatementType previousType, Integer next, StatementType nextType);
};

#endif // SPA_NEXT_H
