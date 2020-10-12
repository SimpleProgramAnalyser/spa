#include "PKB.h"

#include <utility>

PKB pkb = PKB();

void resetPKB()
{
    delete pkb.rootNode;
    pkb = PKB();
}

// Uses
void addUsesRelationships(Integer stmtNum, StatementType stmtType, const Vector<String>& varNames)
{
    pkb.usesTable.addUsesRelationships(stmtNum, stmtType, varNames);
}
void addUsesRelationships(const String& procName, const Vector<String>& varNames)
{
    pkb.usesTable.addUsesRelationships(procName, varNames);
}
Boolean checkIfProcedureUses(const String& procName, const String& varName)
{
    return pkb.usesTable.checkIfProcedureUses(procName, varName);
}
Boolean checkIfStatementUses(Integer stmt, const String& varName)
{
    return pkb.usesTable.checkIfStatementUses(stmt, varName);
}
Vector<Integer> getUsesStatements(const String& varName, StatementType stmtType)
{
    return pkb.usesTable.getUsesStatements(varName, stmtType);
}
Vector<String> getUsesProcedures(const String& varName)
{
    return pkb.usesTable.getUsesProcedures(varName);
}
Vector<String> getUsesVariablesFromStatement(Integer stmt)
{
    return pkb.usesTable.getUsesVariablesFromStatement(stmt);
}
Vector<String> getUsesVariablesFromProcedure(const String& procName)
{
    return pkb.usesTable.getUsesVariablesFromProcedure(procName);
}
Vector<Integer> getAllUsesStatements(StatementType stmtType)
{
    return pkb.usesTable.getAllUsesStatements(stmtType);
}
Vector<String> getAllUsesVariablesFromStatementType(StatementType stmtType)
{
    return pkb.usesTable.getAllUsesVariablesFromStatementType(stmtType);
}
Vector<String> getAllUsesVariablesFromProgram()
{
    return pkb.usesTable.getAllUsesVariablesFromProgram();
}
Vector<String> getAllUsesProcedures()
{
    return pkb.usesTable.getAllUsesProcedures();
}
Vector<Pair<Integer, String>> getAllUsesStatementTuple(StatementType stmtType)
{
    return pkb.usesTable.getAllUsesStatementTuple(stmtType);
}
Vector<Pair<String, String>> getAllUsesProcedureTuple()
{
    return pkb.usesTable.getAllUsesProcedureTuple();
}

// Follows
Void addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after, StatementType afterStmtType)
{
    pkb.followsTable.addFollowsRelationships(before, beforeStmtType, after, afterStmtType);
}
Void addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                 const Vector<Pair<Integer, StatementType>>& afterStmttypePairs)
{
    pkb.followsTable.addFollowsRelationshipsStar(before, beforeStmtType, afterStmttypePairs);
}
Boolean checkIfFollowsHolds(Integer before, Integer after)
{
    return pkb.followsTable.checkIfFollowsHolds(before, after);
}
Boolean checkIfFollowsHoldsStar(Integer before, Integer after)
{
    return pkb.followsTable.checkIfFollowsHoldsStar(before, after);
}
Vector<StatementNumWithType> getAfterStatement(Integer before)
{
    return pkb.followsTable.getAfterStatement(before);
}
Vector<StatementNumWithType> getBeforeStatement(Integer after)
{
    return pkb.followsTable.getBeforeStatement(after);
}
Vector<Integer> getAllAfterStatementsStar(Integer before, StatementType stmtType)
{
    return pkb.followsTable.getAllAfterStatementsStar(before, stmtType);
}
Vector<Integer> getAllBeforeStatementsStar(Integer after, StatementType stmtType)
{
    return pkb.followsTable.getAllBeforeStatementsStar(after, stmtType);
}
Vector<Integer> getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return pkb.followsTable.getAllBeforeStatementsTyped(stmtTypeOfBefore, stmtTypeOfAfter);
}
Vector<Integer> getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return pkb.followsTable.getAllBeforeStatementsTypedStar(stmtTypeOfBefore, stmtTypeOfAfter);
}
Vector<Integer> getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return pkb.followsTable.getAllAfterStatementsTyped(stmtTypeOfBefore, stmtTypeOfAfter);
}
Vector<Integer> getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return pkb.followsTable.getAllAfterStatementsTypedStar(stmtTypeOfBefore, stmtTypeOfAfter);
}
Vector<Pair<Integer, Integer>> getAllFollowsTuple(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return pkb.followsTable.getAllFollowsTuple(stmtTypeOfBefore, stmtTypeOfAfter);
}
Vector<Pair<Integer, Integer>> getAllFollowsTupleStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter)
{
    return pkb.followsTable.getAllFollowsTupleStar(stmtTypeOfBefore, stmtTypeOfAfter);
}

// Modifies
void addModifiesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames)
{
    return pkb.modifiesTable.addModifiesRelationships(stmtNum, stmtType, std::move(varNames));
}
void addModifiesRelationships(const String& procName, Vector<String> varNames)
{
    return pkb.modifiesTable.addModifiesRelationships(procName, std::move(varNames));
}
Boolean checkIfProcedureModifies(const String& procName, const String& varName)
{
    return pkb.modifiesTable.checkIfProcedureModifies(procName, varName);
}
Boolean checkIfStatementModifies(Integer stmt, const String& varName)
{
    return pkb.modifiesTable.checkIfStatementModifies(stmt, varName);
}
Vector<Integer> getModifiesStatements(const String& varName, StatementType stmtType)
{
    return pkb.modifiesTable.getModifiesStatements(varName, stmtType);
}
Vector<String> getModifiesProcedures(const String& varName)
{
    return pkb.modifiesTable.getModifiesProcedures(varName);
}
Vector<String> getModifiesVariablesFromStatement(Integer stmt)
{
    return pkb.modifiesTable.getModifiesVariablesFromStatement(stmt);
}
Vector<String> getModifiesVariablesFromProcedure(const String& procName)
{
    return pkb.modifiesTable.getModifiesVariablesFromProcedure(procName);
}
Vector<Integer> getAllModifiesStatements(StatementType stmtType)
{
    return pkb.modifiesTable.getAllModifiesStatements(stmtType);
}
Vector<String> getAllModifiesVariablesFromStatementType(StatementType stmtType)
{
    return pkb.modifiesTable.getAllModifiesVariablesFromStatementType(stmtType);
}
Vector<String> getAllModifiesVariablesFromProgram()
{
    return pkb.modifiesTable.getAllModifiesVariablesFromProgram();
}
Vector<String> getAllModifiesProcedures()
{
    return pkb.modifiesTable.getAllModifiesProcedures();
}
Vector<Pair<Integer, String>> getAllModifiesStatementTuple(StatementType stmtType)
{
    return pkb.modifiesTable.getAllModifiesStatementTuple(stmtType);
}
Vector<Pair<String, String>> getAllModifiesProcedureTuple()
{
    return pkb.modifiesTable.getAllModifiesProcedureTuple();
}

// Parent
void addParentRelationships(Integer parent, StatementType parentType, Integer child, StatementType childType)
{
    return pkb.parentTable.addParentRelationships(parent, parentType, child, childType);
}
void addParentRelationshipsStar(Integer parent, StatementType parentType,
                                const Vector<StatementNumWithType>& childrenStmttypePairs)
{
    return pkb.parentTable.addParentRelationshipsStar(parent, parentType, childrenStmttypePairs);
}
Boolean checkIfParentHolds(Integer parent, Integer child)
{
    return pkb.parentTable.checkIfParentHolds(parent, child);
}
Boolean checkIfParentHoldsStar(Integer parent, Integer child)
{
    return pkb.parentTable.checkIfParentHoldsStar(parent, child);
}
Vector<Integer> getAllChildStatements(Integer parent, StatementType childType)
{
    return pkb.parentTable.getAllChildStatements(parent, childType);
}
Vector<StatementNumWithType> getParentStatement(Integer child)
{
    return pkb.parentTable.getParentStatement(child);
}
Vector<Integer> getAllChildStatementsStar(Integer parent, StatementType stmtType)
{
    return pkb.parentTable.getAllChildStatementsStar(parent, stmtType);
}
Vector<Integer> getAllParentStatementsStar(Integer child, StatementType stmtType)
{
    return pkb.parentTable.getAllParentStatementsStar(child, stmtType);
}
Vector<Integer> getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return pkb.parentTable.getAllParentStatementsTyped(stmtTypeOfParent, stmtTypeOfChild);
}
Vector<Integer> getAllParentStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return pkb.parentTable.getAllParentStatementsTypedStar(stmtTypeOfParent, stmtTypeOfChild);
}
Vector<Integer> getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return pkb.parentTable.getAllChildStatementsTyped(stmtTypeOfParent, stmtTypeOfChild);
}
Vector<Integer> getAllChildStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return pkb.parentTable.getAllChildStatementsTypedStar(stmtTypeOfParent, stmtTypeOfChild);
}
Vector<Pair<Integer, Integer>> getAllParentTuple(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return pkb.parentTable.getAllParentTuple(stmtTypeOfParent, stmtTypeOfChild);
}
Vector<Pair<Integer, Integer>> getAllParentTupleStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild)
{
    return pkb.parentTable.getAllParentTupleStar(stmtTypeOfParent, stmtTypeOfChild);
}

// Procedure
void insertIntoProcedureTable(const String& procName, StatementNumber firstStmtNum, StatementNumber lastStmtNum)
{
    pkb.procedureTable.insertIntoProcedureTable(procName, firstStmtNum, lastStmtNum);
}
Boolean isProcedureInProgram(const String& procName)
{
    return pkb.procedureTable.isProcedureInProgram(procName);
}
Vector<String> getAllProcedures()
{
    return pkb.procedureTable.getAllProcedures();
}
StatementNumberRange getStatementRangeByProcedure(const ProcedureName& procedureName)
{
    return pkb.procedureTable.getStatementRangeByProcedure(procedureName);
}
Vector<ProcedureName> getContainingProcedure(StatementNumber statementNumber)
{
    return pkb.procedureTable.getContainingProcedure(statementNumber);
}

// Variable
void insertIntoVariableTable(const String& varName)
{
    pkb.variableTable.insertIntoVariableTable(varName);
}
Boolean isVariableInProgram(const String& varName)
{
    return pkb.variableTable.isVariableInProgram(varName);
}
Vector<String> getAllVariables()
{
    return pkb.variableTable.getAllVariables();
}

// Statement
void insertIntoStatementTable(Integer stmtNum, StatementType stmtType)
{
    pkb.statementTable.insertIntoStatementTable(stmtNum, stmtType);
}
Boolean isStatementInProgram(Integer stmtNum)
{
    return pkb.statementTable.isStatementInProgram(stmtNum);
}
Vector<Integer> getAllStatements(StatementType stmtType)
{
    return pkb.statementTable.getAllStatements(stmtType);
}
void insertIntoStatementTable(Integer stmtNum, const ProcedureName& procName)
{
    return pkb.statementTable.insertIntoStatementTable(stmtNum, procName);
}
Vector<String> getProcedureCalled(Integer callStmtNum)
{
    return pkb.statementTable.getProcedureCalled(callStmtNum);
}
Vector<Integer> getAllCallStatementsByProcedure(const String& procName)
{
    return pkb.statementTable.getAllCallStatementsByProcedure(procName);
}
Vector<String> getAllProceduresCalled()
{
    return pkb.statementTable.getAllProceduresCalled();
}

// RootNode
void assignRootNode(ProgramNode* rootNodeToAssign)
{
    pkb.rootNode = rootNodeToAssign;
}
ProgramNode* getRootNode()
{
    return pkb.rootNode;
}

// Constant
void insertIntoConstantTable(Integer constant)
{
    pkb.constantTable.insertIntoConstantTable(constant);
}
Boolean isConstantInProgram(Integer constant)
{
    return pkb.constantTable.isConstantInProgram(constant);
}
Vector<Integer> getAllConstants()
{
    return pkb.constantTable.getAllConstants();
}

// Next
void addNextRelationships(StatementNumber prev, StatementType prevType, StatementNumber next, StatementType nextType)
{
    return pkb.nextTable.addNextRelationships(prev, prevType, next, nextType);
}
Boolean checkIfNextHolds(StatementNumber prev, StatementNumber next)
{
    return pkb.nextTable.checkIfNextHolds(prev, next);
}
Vector<StatementNumber> getAllNextStatements(StatementNumber prev, StatementType nextType)
{
    return pkb.nextTable.getAllNextStatements(prev, nextType);
}
Vector<StatementNumber> getAllPreviousStatements(StatementNumber next, StatementType prevType)
{
    return pkb.nextTable.getAllPreviousStatements(next, prevType);
}
Vector<StatementNumber> getAllNextStatementsTyped(StatementType prevType, StatementType nextType)
{
    return pkb.nextTable.getAllNextStatementsTyped(prevType, nextType);
}
Vector<StatementNumber> getAllPreviousStatementsTyped(StatementType prevType, StatementType nextType)
{
    return pkb.nextTable.getAllPreviousStatementsTyped(prevType, nextType);
}
Vector<Pair<StatementNumber, StatementNumber>> getAllNextTuples(StatementType prevType, StatementType nextType)
{
    return pkb.nextTable.getAllNextTuples(prevType, nextType);
}

// Calls
void addCallerRelationships(const ProcedureName& caller, const ProcedureName& callee)
{
    return pkb.callsTable.addCallerRelationships(caller, callee);
}
void addCallerRelationshipsStar(const ProcedureName& caller, const ProcedureName& callee)
{
    return pkb.callsTable.addCallerRelationshipsStar(caller, callee);
}
Boolean checkIfCallsHolds(const ProcedureName& caller, const ProcedureName& callee)
{
    return pkb.callsTable.checkIfCallsHolds(caller, callee);
}
Boolean checkIfCallsHoldsStar(const ProcedureName& caller, const ProcedureName& callee)
{
    return pkb.callsTable.checkIfCallsHoldsStar(caller, callee);
}
Vector<ProcedureName> getAllCallers(const ProcedureName& callee)
{
    return pkb.callsTable.getAllCallers(callee);
}
Vector<ProcedureName> getAllCallersStar(const ProcedureName& callee)
{
    return pkb.callsTable.getAllCallersStar(callee);
}
Vector<ProcedureName> getAllCallees(const ProcedureName& caller)
{
    return pkb.callsTable.getAllCallees(caller);
}
Vector<ProcedureName> getAllCalleesStar(const ProcedureName& caller)
{
    return pkb.callsTable.getAllCalleesStar(caller);
}
Vector<Pair<ProcedureName, ProcedureName>> getAllCallsTuple()
{
    return pkb.callsTable.getAllCallsTuple();
}
Vector<Pair<ProcedureName, ProcedureName>> getAllCallsTupleStar()
{
    return pkb.callsTable.getAllCallsTupleStar();
}
