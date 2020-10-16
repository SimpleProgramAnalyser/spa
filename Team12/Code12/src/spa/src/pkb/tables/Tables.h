/**
 * Procedure, Statement and Variable tables.
 */

#ifndef SPA_TABLES_H
#define SPA_TABLES_H

#include <map>

#include "ast/AstTypes.h"
#include "pkb/PkbTypes.h"

class ProcedureTable {
public:
    void insertIntoProcedureTable(const String& procName, StatementNumber firstStmtNum, StatementNumber lastStmtNum);
    Vector<String> getAllProcedures();
    Boolean isProcedureInProgram(const String& procName);
    StatementNumberRange getStatementRangeByProcedure(const ProcedureName& procedureName);
    Vector<ProcedureName> getContainingProcedure(StatementNumber statementNumber);

private:
    HashSet<String> setOfProceduresNames;                       // getAllProc
    Vector<String> listOfProcedureNames;                        // IsProcInProgram
    HashMap<String, StatementNumberRange> procNameStmtRangeMap; // store stmts in proc
    std::map<StatementNumber, String> firstStmtToProc;          // for binary search
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
    // Iteration 2
    void insertIntoStatementTable(Integer stmtNum, const ProcedureName& procName);
    Vector<String> getProcedureCalled(Integer callStmtNum);
    Vector<Integer> getAllCallStatementsByProcedure(const String& procName);
    Vector<String> getAllProceduresCalled();
    StatementType getStatementType(StatementNumber stmtNum);

private:
    StatementNumVectorsByType listOfAllStatement;       // getAllStmt
    StatementNumSetsByType setOfAllStatement;           // getAllStmt
    HashSet<Integer> setOfStatements;                   // isStatementInProgram
    HashMap<StatementNumber, StatementType> statementTypes; // getStatementType
    HashMap<StatementNumber, ProcedureName> procCalled; // getProcedureCalled
    Vector<ProcedureName> allProcCalled;                // getAllProc
    HashSet<ProcedureName> allProcCalledSet;            // de-duplication

    HashMap<ProcedureName, Vector<StatementNumber>> stmtsCalling;     // getAllCallStatementsByProcedure
    HashMap<ProcedureName, HashSet<StatementNumber>> stmtsCallingSet; // de-duplication
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
