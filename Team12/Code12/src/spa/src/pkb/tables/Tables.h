/**
 * Procedure, Statement and Variable tables.
 */

#ifndef SPA_TABLES_H
#define SPA_TABLES_H

#include "ast/AstTypes.h"
#include "pkb/PkbTypes.h"

class ProcedureTable {
public:
    void insertIntoProcedureTable(const String& procName);
    Vector<String> getAllProcedures();
    Boolean getProcedureFromName(const String& procName);

private:
    HashSet<String> setOfProceduresNames; // getAllProc
    Vector<String> listOfProcedureNames;  // getProcName
};

class VariableTable {
public:
    void insertIntoVariableTable(const String& varName);
    Boolean getVariableFromName(const String& varName);
    Vector<String> getAllVariables();

private:
    Vector<String> listOfVariables; // getAllVar
    HashSet<String> setOfVariables; // getVarFromName
};

class StatementTable {
public:
    void insertIntoStatementTable(Integer stmtNum, StatementType stmtType);
    Boolean getStatementFromIndex(Integer stmtNum);
    Vector<Integer> getAllStatements(StatementType stmtType);

private:
    StatementNumVectorsByType listOfAllStatement; // getAllStmt
    HashSet<Integer> setOfStatements;             // getFromIndex
};

#endif
