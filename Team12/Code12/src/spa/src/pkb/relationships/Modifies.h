#ifndef SPA_MODIFIES_H
#define SPA_MODIFIES_H

#include <pkb/PkbTypes.h>

/**
 * Stores Modifies relationships.
 */
class ModifiesTable {
public:
    // writing
    void addModifiesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames);
    void addModifiesRelationships(const String& procName, Vector<String> varNames);

    // reading
    Boolean checkIfProcedureModifies(const String& procName, const String& varName);
    Boolean checkIfStatementModifies(Integer stmt, const String& varName);
    Vector<Integer> getModifiesStatements(const String& varName, StatementType stmtType);
    Vector<String> getModifiesProcedures(const String& varName);
    Vector<String> getModifiesVariablesFromStatement(Integer stmt);
    Vector<String> getModifiesVariablesFromProcedure(const String& procName);
    Vector<Integer> getAllModifiesStatements(StatementType stmtType);
    Vector<String> getAllModifiesVariablesFromStatementType(StatementType stmtType);
    Vector<String> getAllModifiesVariablesFromProgram();
    Vector<String> getAllModifiesProcedures();

private:
    // for checkIf*Modifies
    HashMap<Integer, HashSet<String>> stmtVarsetMap;
    HashMap<String, HashSet<String>> procVarsetMap;

    // for getAllModifiesVar given stmt/proc
    HashMap<Integer, Vector<String>> stmtVarlistMap;
    HashMap<String, Vector<String>> procVarlistMap;

    // for getModifiesStmts/Procs
    HashMap<String, StatementNumVectorsByType> varStmtlistMap;
    HashMap<String, Vector<String>> varProclistMap;

    // for getAllVar
    Array<Vector<String>, STATEMENT_TYPE_COUNT> stmttypeVarlistMap;
    HashSet<String> allVarUsedByProcSet;
    Vector<String> allVarUsedByProcList;

    // for getAllStmt
    Array<Vector<Integer>, STATEMENT_TYPE_COUNT> stmttypeStmtlistMap;

    // for getAllProcedure
    Vector<String> allModifiesProc;
};

#endif // SPA_MODIFIES_H
