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
    Boolean isProcedureInProgram(const String& procName);

private:
    HashSet<String> setOfProceduresNames; // getAllProc
    Vector<String> listOfProcedureNames;  // IsProcInProgram
};

class VariableTable {
public:
    void insertIntoVariableTable(const String& varName);
    Boolean isVariableInProgram(const String& varName);
    Vector<String> getAllVariables();

private:
    Vector<String> listOfVariables; // getAllVar
    HashSet<String> setOfVariables; // isVarInProgram
};

class StatementTable {
public:
    void insertIntoStatementTable(Integer stmtNum, StatementType stmtType);
    Boolean isStatementInProgram(Integer stmtNum);
    Vector<Integer> getAllStatements(StatementType stmtType);

private:
    StatementNumVectorsByType listOfAllStatement; // getAllStmt
    HashSet<Integer> setOfStatements;             // isStatementInProgram
};

class ConstantTable {
public:
    void insertIntoConstantTable(Integer constant);
    Boolean isConstantInProgram(Integer constant);
    Vector<Integer> getAllConstants();

private:
    Vector<Integer> listOfConstants; // getAllConstants
    HashSet<Integer> setOfConstants; // isConstantInProgram
};

#endif
