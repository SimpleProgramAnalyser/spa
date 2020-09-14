#include "PKB.h"

#include <utility>

PKB pkb = PKB();

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
Vector<String> getUsesProcedures(const String& procName)
{
    return pkb.usesTable.getUsesProcedures(procName);
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
Vector<String> getAllUsesVariables(StatementType stmtType)
{
    return pkb.usesTable.getAllUsesVariables(stmtType);
}
Vector<String> getAllUsesVariables(const String& procName)
{
    return pkb.usesTable.getAllUsesVariables(procName);
}
Vector<String> getAllUsesProcedures()
{
    return pkb.usesTable.getAllUsesProcedures();
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
Vector<String> getModifiesProcedures(const String& procName)
{
    return pkb.modifiesTable.getModifiesProcedures(procName);
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
Vector<String> getAllModifiesVariables(StatementType stmtType)
{
    return pkb.modifiesTable.getAllModifiesVariables(stmtType);
}
Vector<String> getAllModifiesVariables(const String& procName)
{
    return pkb.modifiesTable.getAllModifiesVariables(procName);
}
Vector<String> getAllModifiesProcedures()
{
    return pkb.modifiesTable.getAllModifiesProcedures();
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
Vector<StatementNumWithType> getChildStatement(Integer parent)
{
    return pkb.parentTable.getChildStatement(parent);
}
Vector<StatementNumWithType> getParentStatement(Integer child)
{
    return pkb.parentTable.getParentStatement(child);
}
Vector<Integer> getAllChildStatementsStar(Integer parent, StatementType stmtType)
{
    return pkb.parentTable.getAllChildStatementsStar(parent, stmtType);
}
Vector<Integer> getAllParentStatementsStar(Integer parent, StatementType stmtType)
{
    return pkb.parentTable.getAllParentStatementsStar(parent, stmtType);
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

// Procedure
void insertIntoProcedureTable(const String& procName)
{
    pkb.procedureTable.insertIntoProcedureTable(procName);
}
Boolean getProcedureFromName(const String& procName)
{
    return pkb.procedureTable.getProcedureFromName(procName);
}
Vector<String> getAllProcedures()
{
    return pkb.procedureTable.getAllProcedures();
}

// Variable
void insertIntoVariableTable(const String& varName)
{
    pkb.variableTable.insertIntoVariableTable(varName);
}
Boolean getVariableFromName(const String& varName)
{
    return pkb.variableTable.getVariableFromName(varName);
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
Boolean getStatementFromIndex(Integer stmtNum)
{
    return pkb.statementTable.getStatementFromIndex(stmtNum);
}
Vector<Integer> getAllStatements(StatementType stmtType)
{
    return pkb.statementTable.getAllStatements(stmtType);
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
