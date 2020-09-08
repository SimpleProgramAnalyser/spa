#ifndef SPA_MODIFIES_H
#define SPA_MODIFIES_H

#include <pkb/PkbTypes.h>

class ModifiesTable {
public:
    // writing
    void addModifiesRelationships(Integer stmtNum, Vector<String> varNames);
    void addModifiesRelationships(String procName, Vector<String> varNames);

    // reading
    String checkIfProcedureModifies(String procName, String varName);
    Integer checkIfStatementModifies(Integer stmt, String varName);
    Vector<Integer> getModifiesStatements(String varName, StatementType stmtType);
    Vector<String> getModifiesProcedures(String procName);
    Vector<String> getModifiesVariablesFromStatement(Integer stmt);
    Vector<String> getusesVariablesFromProcedure(String procName);
    Vector<Integer> getAllModifiesStatements(StatementType stmtType);
    Vector<String> getAllModifiesVariables(StatementType stmtType);
    Vector<String> getAllModifiesVariables(String procName);
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
    Vector<String> stmttypeVarlistMap[STATEMENT_TYPE_COUNT];
    HashSet<String> allVarUsedByProc;

    // for getAllStmt
    Vector<Integer> stmttypeStmtlistMap[STATEMENT_TYPE_COUNT];

    // for getAllProcedure
    HashSet<String> allModifiesProc;
};

#endif // SPA_MODIFIES_H
