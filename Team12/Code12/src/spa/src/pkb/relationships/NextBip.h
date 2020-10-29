#ifndef SPA_NEXT_BIP_H
#define SPA_NEXT_BIP_H

#include <pkb/PkbTypes.h>

typedef ArrayArrayTupleList<Integer, Integer> TupleTable;

class NextBipTable {
public:
    // Section 1: Adding relationships
    void addNextBipRelationships(StatementNumber prev, StatementType prevType, StatementNumber next,
                                 StatementType nextType);

    // Section 2: Table and inverse table methods
    Boolean checkIfNextBipHolds(StatementNumber prev, StatementNumber next);
    Vector<StatementNumber> getAllNextBipStatements(StatementNumber prev, StatementType nextType);
    Vector<StatementNumber> getAllPreviousBipStatements(StatementNumber next, StatementType prevType);

    // Section 3: Collection table methods
    Vector<StatementNumber> getAllNextBipStatementsTyped(StatementType prevType, StatementType nextType);
    Vector<StatementNumber> getAllPreviousBipStatementsTyped(StatementType prevType, StatementType nextType);

    // Section 4: Tuple methods
    Vector<Pair<StatementNumber, StatementNumber>> getAllNextBipTuples(StatementType prevType, StatementType nextType);

private:
    // Table and inverse tables
    /**
     * Primary key: statement number
     * Result: next/previous statements, stored by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    HashMap<Integer, StatementNumVectorsByType> stmtPreviousBipMap;
    HashMap<Integer, StatementNumVectorsByType> stmtNextBipMap;
    // sets used to deduplicate the above structures
    HashMap<Integer, StatementNumSetsByType> stmtPreviousBipSet;
    HashMap<Integer, StatementNumSetsByType> stmtNextBipSet;

    // Collection Tables
    /**
     * Primary key: statement type
     * Result: a table by statement type
     *
     * Secondary key: statement type
     * Result: Vector<Statement Number>
     */
    Array<StatementNumVectorsByType, StatementTypeCount> stmtPreviousBipType;
    Array<StatementNumVectorsByType, StatementTypeCount> stmtNextBipType;
    // hashsets to prevent duplication in lists above
    Array<StatementNumSetsByType, StatementTypeCount> stmtPreviousBipTypeSet;
    Array<StatementNumSetsByType, StatementTypeCount> stmtNextBipTypeSet;

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
};

#endif // SPA_NEXT_BIP_H
