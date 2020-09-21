#ifndef SPA_USES_H
#define SPA_USES_H

#include <pkb/PkbTypes.h>

/**
 * Stores Uses relationships.
 */
class UsesTable {
public:
    // writing
    void addUsesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames);
    void addUsesRelationships(const String& procName, Vector<String> varNames);

    // reading
    Boolean checkIfProcedureUses(const String& procName, const String& varName);
    Boolean checkIfStatementUses(Integer stmt, const String& varName);
    Vector<Integer> getUsesStatements(const String& varName, StatementType stmtType);
    Vector<String> getUsesProcedures(const String& varName);
    Vector<String> getUsesVariablesFromStatement(Integer stmt);
    Vector<String> getUsesVariablesFromProcedure(const String& procName);
    Vector<Integer> getAllUsesStatements(StatementType stmtType);
    Vector<String> getAllUsesVariablesFromStatementType(StatementType stmtType);
    Vector<String> getAllUsesVariablesFromProgram();
    Vector<String> getAllUsesProcedures();
    Vector<Pair<Integer, String>> getAllUsesStatementTuple(StatementType stmtType);
    Vector<Pair<String, String>> getAllUsesProcedureTuple();

private:
    // for checkIf*Uses
    HashMap<Integer, HashSet<String>> stmtVarsetMap;
    HashMap<String, HashSet<String>> procVarsetMap;

    // for getAllUsesVar given stmt/proc
    HashMap<Integer, Vector<String>> stmtVarlistMap;
    HashMap<String, Vector<String>> procVarlistMap;

    // for getUsesStmts/Procs
    HashMap<String, StatementNumVectorsByType> varStmtlistMap;
    HashMap<String, Vector<String>> varProclistMap;

    // for getAllVar
    Array<Vector<String>, STATEMENT_TYPE_COUNT> stmttypeVarlistMap;
    HashSet<String> allVarUsedByProcSet;
    Vector<String> allVarUsedByProcList;

    // for tuples
    Array<Vector<Pair<Integer, String>>, STATEMENT_TYPE_COUNT> statementTuples;
    Vector<Pair<String, String>> procTuples;

    // for getAllStmt
    Array<Vector<Integer>, STATEMENT_TYPE_COUNT> stmttypeStmtlistMap;

    // for getAllProcedure
    Vector<String> allUsesProc;
};

#endif // SPA_USES_H
