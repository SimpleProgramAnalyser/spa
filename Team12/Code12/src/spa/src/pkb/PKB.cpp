#include "PKB.h"

PKB pkb = PKB();

void PKB::addUsesRelationships(Integer stmtNum, StatementType stmtType, const Vector<String>& varNames)
{
    return usesTable.addUsesRelationships(stmtNum, stmtType, varNames);
}
void PKB::addUsesRelationships(const String& procName, const Vector<String>& varNames)
{
    return usesTable.addUsesRelationships(procName, varNames);
}
Boolean PKB::checkIfProcedureUses(const String& procName, const String& varName)
{
    return usesTable.checkIfProcedureUses(procName, varName);
}
Boolean PKB::checkIfStatementUses(Integer stmt, const String& varName)
{
    return usesTable.checkIfStatementUses(stmt, varName);
}

Vector<Integer> PKB::getUsesStatements(const String& varName, StatementType stmtType)
{
    return usesTable.getUsesStatements(varName, stmtType);
}
Vector<String> PKB::getUsesProcedures(const String& procName)
{
    return usesTable.getUsesProcedures(procName);
}
Vector<String> PKB::getUsesVariablesFromStatement(Integer stmt)
{
    return usesTable.getUsesVariablesFromStatement(stmt);
}
Vector<String> PKB::getusesVariablesFromProcedure(const String& procName)
{
    return usesTable.getusesVariablesFromProcedure(procName);
}
Vector<Integer> PKB::getAllUsesStatements(StatementType stmtType)
{
    return usesTable.getAllUsesStatements(stmtType);
}
Vector<String> PKB::getAllUsesVariables(StatementType stmtType)
{
    return usesTable.getAllUsesVariables(stmtType);
}
Vector<String> PKB::getAllUsesVariables(const String& procName)
{
    return usesTable.getAllUsesVariables(procName);
}
Vector<String> PKB::getAllUsesProcedures()
{
    return usesTable.getAllUsesProcedures();
}

// Follows stub functions
void PKB::addFollowsRelationships(Integer parent, Integer child) {}
void PKB::addFollowsRelationshipsStar(Integer parent, Vector<Integer> children) {}
