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
    Vector<String> getusesVariablesFromProcedure(const String& procName);
    Vector<Integer> getAllUsesStatements(StatementType stmtType);
    Vector<String> getAllUsesVariables(StatementType stmtType);
    Vector<String> getAllUsesVariables(const String& procName);
    Vector<String> getAllUsesProcedures();

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
    HashSet<String> allVarUsedByProc;

    // for getAllStmt
    Array<Vector<Integer>, STATEMENT_TYPE_COUNT> stmttypeStmtlistMap;

    // for getAllProcedure
    Vector<String> allUsesProc;
};

#endif // SPA_USES_H
