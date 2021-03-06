#ifndef PKB_H
#define PKB_H

#include <pkb/relationships/NextBip.h>

#include "PkbTypes.h"
#include "cfg/CfgTypes.h"
#include "relationships/Calls.h"
#include "relationships/Follows.h"
#include "relationships/Modifies.h"
#include "relationships/Next.h"
#include "relationships/Parent.h"
#include "relationships/Uses.h"
#include "tables/Tables.h"
#include "tree/TreeStore.h"

/**
 * Holds API methods for the Program Knowledge Base (PKB).
 */
// Uses
void addUsesRelationships(Integer stmtNum, StatementType stmtType, const Vector<String>& varNames);
void addUsesRelationships(const String& procName, const Vector<String>& varNames);
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

// Modifies
void addModifiesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames);
void addModifiesRelationships(const String& procName, Vector<String> varNames);
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
Vector<Pair<Integer, String>> getAllModifiesStatementTuple(StatementType stmtType);
Vector<Pair<String, String>> getAllModifiesProcedureTuple();

// Parent
void addParentRelationships(Integer parent, StatementType parentType, Integer child, StatementType childType);
void addParentRelationshipsStar(Integer parent, StatementType parentType,
                                const Vector<Pair<Integer, StatementType>>& childrenStmttypePairs);
Boolean checkIfParentHolds(Integer parent, Integer child);
Boolean checkIfParentHoldsStar(Integer parent, Integer child);
Vector<Integer> getAllChildStatements(Integer parent, StatementType childType);
Vector<StatementNumWithType> getParentStatement(Integer child);
Vector<Integer> getAllChildStatementsStar(Integer parent, StatementType stmtType);
Vector<Integer> getAllParentStatementsStar(Integer child, StatementType stmtType);
Vector<Integer> getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllParentStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllChildStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Pair<Integer, Integer>> getAllParentTuple(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Pair<Integer, Integer>> getAllParentTupleStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);

// Follows
void addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after, StatementType afterStmtType);
void addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                 const Vector<Pair<Integer, StatementType>>& afterStmttypePairs);
Boolean checkIfFollowsHolds(Integer before, Integer after);
Boolean checkIfFollowsHoldsStar(Integer before, Integer after);
Vector<StatementNumWithType> getAfterStatement(Integer before);
Vector<StatementNumWithType> getBeforeStatement(Integer after);
Vector<Integer> getAllAfterStatementsStar(Integer before, StatementType stmtType);
Vector<Integer> getAllBeforeStatementsStar(Integer after, StatementType stmtType);
Vector<Integer> getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Pair<Integer, Integer>> getAllFollowsTuple(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Pair<Integer, Integer>> getAllFollowsTupleStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);

// Next
void addNextRelationships(StatementNumber prev, StatementType prevType, StatementNumber next, StatementType nextType);
Boolean checkIfNextHolds(StatementNumber prev, StatementNumber next);
Vector<StatementNumber> getAllNextStatements(StatementNumber prev, StatementType nextType);
Vector<StatementNumber> getAllPreviousStatements(StatementNumber next, StatementType prevType);
Vector<StatementNumber> getAllNextStatementsTyped(StatementType prevType, StatementType nextType);
Vector<StatementNumber> getAllPreviousStatementsTyped(StatementType prevType, StatementType nextType);
Vector<Pair<StatementNumber, StatementNumber>> getAllNextTuples(StatementType prevType, StatementType nextType);

// Calls
void addCallerRelationships(const ProcedureName& caller, const ProcedureName& callee);
void addCallerRelationshipsStar(const ProcedureName& caller, const ProcedureName& callee);
Boolean checkIfCallsHolds(const ProcedureName& caller, const ProcedureName& callee);
Boolean checkIfCallsHoldsStar(const ProcedureName& caller, const ProcedureName& callee);
Vector<ProcedureName> getAllCallers();
Vector<ProcedureName> getAllCallersStar();
Vector<ProcedureName> getAllCallees();
Vector<ProcedureName> getAllCalleesStar();
Vector<ProcedureName> getAllCallers(const ProcedureName& callee);
Vector<ProcedureName> getAllCallersStar(const ProcedureName& callee);
Vector<ProcedureName> getAllCallees(const ProcedureName& caller);
Vector<ProcedureName> getAllCalleesStar(const ProcedureName& caller);
Vector<Pair<ProcedureName, ProcedureName>> getAllCallsTuple();
Vector<Pair<ProcedureName, ProcedureName>> getAllCallsTupleStar();

// NextBip
void addNextBipRelationships(StatementNumber prev, StatementType prevType, StatementNumber next,
                             StatementType nextType);
Boolean checkIfNextBipHolds(StatementNumber prev, StatementNumber next);
Vector<StatementNumber> getAllNextBipStatements(StatementNumber prev, StatementType nextType);
Vector<StatementNumber> getAllPreviousBipStatements(StatementNumber next, StatementType prevType);
Vector<StatementNumber> getAllNextBipStatementsTyped(StatementType prevType, StatementType nextType);
Vector<StatementNumber> getAllPreviousBipStatementsTyped(StatementType prevType, StatementType nextType);
Vector<Pair<StatementNumber, StatementNumber>> getAllNextBipTuples(StatementType prevType, StatementType nextType);

// Procedure
void insertIntoProcedureTable(const String& procName, StatementNumber firstStmtNum, StatementNumber lastStmtNum);
Boolean isProcedureInProgram(const String& procName);
Vector<String> getAllProcedures();
StatementNumberRange getStatementRangeByProcedure(const ProcedureName& procedureName);
Vector<ProcedureName> getContainingProcedure(StatementNumber statementNumber);

// Variable
void insertIntoVariableTable(const String& varName);
Boolean isVariableInProgram(const String& varName);
Vector<String> getAllVariables();

// Statement
void insertIntoStatementTable(Integer stmtNum, StatementType stmtType);
Boolean isStatementInProgram(Integer stmtNum);
Vector<Integer> getAllStatements(StatementType stmtType);
void insertIntoStatementTable(Integer stmtNum, const ProcedureName& procName);
Vector<String> getProcedureCalled(Integer callStmtNum);
Vector<Integer> getAllCallStatementsByProcedure(const String& procName);
Vector<String> getAllProceduresCalled();
StatementType getStatementType(StatementNumber stmtNum);

// Constant
void insertIntoConstantTable(Integer constant);
Boolean isConstantInProgram(Integer constant);
Vector<Integer> getAllConstants();

// AST Root node
void assignRootNode(ProgramNode* rootNodeToAssign);
ProgramNode* getRootNode();

// CFG
void storeCFG(CfgNode* cfg, const ProcedureName& procedureName);
CfgNode* getCFG(const ProcedureName& procedureName);
Vector<String> getProceduresWithCFG();

// CFG Bip
void storeCFGBip(CfgNode* cfgBip, const ProcedureName& procedureName);
CfgNode* getCFGBip(const ProcedureName& procedureName);
Vector<String> getProceduresWithCFGBip();

// Others
void resetPKB();

class PKB {
public:
    // Tables
    StatementTable statementTable;
    VariableTable variableTable;
    ProcedureTable procedureTable;
    // Relationships
    FollowsTable followsTable;
    ParentTable parentTable;
    UsesTable usesTable;
    ModifiesTable modifiesTable;
    ConstantTable constantTable;
    NextTable nextTable;
    CallsTable callsTable;
    NextBipTable nextBipTable;
    // Trees
    TreeStore treeStore;
};

#endif // PKB_H
