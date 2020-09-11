/**
 * Procedure, Statement and Variable tables.
 */

#ifndef SPA_TABLES_H
#define SPA_TABLES_H

#include "ast/AstTypes.h"
#include "pkb/PkbTypes.h"

class ProcedureTable {
public:
    void insertIntoProcedureTable(String procName);
    Vector<String> getAllProcedures();
    Boolean getProcedureFromName(String procName);

private:
    HashSet<String> setOfProceduresNames; // getAllProc
    Vector<String> listOfProcedureNames;  // getProcName
};

class VariableTable {
public:
    void insertIntoVariableTable(String varName);
    Boolean getVariableFromName(String varName);
    Vector<String> getAllVariables();

private:
    Vector<String> listOfVariables; // getAllVar
    HashSet<String> setOfVariables; // getVarFromName
};

class StatementTable {
public:
    void insertIntoStatementTable(StatementNode* stmtNode, Integer lineNumber);
    Boolean getStatementFromIndex(Integer indexOfStmt);
    Vector<Integer> getAllStatements(StatementType stmtType);

private:
    HashMap<Integer, Vector<Integer>> listOfAllStatement; // getAllStmt
    HashSet<Integer> setOfStatements;                     // getFromIndex
};

#endif
