/**
 * Implementation of tables library.
 */

#include "Tables.h"

// Procedure Table
void ProcedureTable::insertIntoProcedureTable(const String& procName)
{
    if (setOfProceduresNames.find(procName) == setOfProceduresNames.end()) {
        listOfProcedureNames.push_back(procName);
        setOfProceduresNames.insert(procName);
    }
}
std::vector<String> ProcedureTable::getAllProcedures()
{
    return listOfProcedureNames;
}
Boolean ProcedureTable::isProcedureInProgram(const String& procName)
{
    return setOfProceduresNames.find(procName) != setOfProceduresNames.end();
}

// Variable Table
void VariableTable::insertIntoVariableTable(const String& varName)
{
    if (setOfVariables.find(varName) == setOfVariables.end()) {
        listOfVariables.push_back(varName);
        setOfVariables.insert(varName);
    }
}
Vector<String> VariableTable::getAllVariables()
{
    return listOfVariables;
}
Boolean VariableTable::isVariableInProgram(const String& varName)
{
    return setOfVariables.find(varName) != setOfVariables.end();
}

// Statement Table
void StatementTable::insertIntoStatementTable(Integer stmtNum, StatementType stmtType)
{
    if (setOfStatements.find(stmtNum) == setOfStatements.end()) {
        setOfStatements.insert(stmtNum);
        listOfAllStatement.byType[stmtType].push_back(stmtNum);
        listOfAllStatement.byType[AnyStatement].push_back(stmtNum);
    }
}
Boolean StatementTable::isStatementInProgram(Integer stmtNum)
{
    return setOfStatements.find(stmtNum) != setOfStatements.end();
}
Vector<Integer> StatementTable::getAllStatements(StatementType stmtType)
{
    return listOfAllStatement.byType[stmtType];
}

// Constant Table
void ConstantTable::insertIntoConstantTable(Integer constant)
{
    if (setOfConstants.find(constant) == setOfConstants.end()) {
        setOfConstants.insert(constant);
        listOfConstants.push_back(constant);
    }
}
Boolean ConstantTable::isConstantInProgram(Integer constant)
{
    return setOfConstants.find(constant) != setOfConstants.end();
}
Vector<Integer> ConstantTable::getAllConstants()
{
    return listOfConstants;
}
