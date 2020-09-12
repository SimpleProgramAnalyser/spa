#include "PKB.h"

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

// Follows stub functions
void addFollowsRelationships(Integer parent, Integer child) {}
void addFollowsRelationshipsStar(Integer parent, Vector<Integer> children) {}
