#include "PKB.h"

#include <utility>

PKB pkb = PKB();

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

Vector<String> getusesVariablesFromProcedure(const String& procName)
{
    return pkb.usesTable.getusesVariablesFromProcedure(procName);
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

Void addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after, StatementType afterStmtType)
{
    pkb.followsTable.addFollowsRelationships(before, beforeStmtType, after, afterStmtType);
}

Void addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                 Vector<Pair<Integer, StatementType>> afterStmttypePairs)
{
    pkb.followsTable.addFollowsRelationshipsStar(before, beforeStmtType, std::move(afterStmttypePairs));
}

Boolean checkIfFollowsHolds(Integer before, Integer after)
{
    return pkb.followsTable.checkIfFollowsHolds(before, after);
}

Boolean checkIfFollowsHoldsStar(Integer before, Integer after)
{
    return pkb.followsTable.checkIfFollowsHoldsStar(before, after);
}

Vector<Integer> getAllAfterStatements(Integer before, StatementType stmtType)
{
    return pkb.followsTable.getAfterStatement(before); // TODO: what happens to stmt type?
}

Vector<Integer> getAllBeforeStatements(Integer after, StatementType stmtType)
{
    return pkb.followsTable.getBeforeStatement(after); // TODO: what happens to stmt type?
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
