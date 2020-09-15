/**
 * Implementation of tables library.
 */

#include "Tables.h"

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

Boolean ProcedureTable::getProcedureFromName(const String& procName)
{
    return setOfProceduresNames.find(procName) == setOfProceduresNames.end();
}

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

Boolean VariableTable::getVariableFromName(const String& varName)
{
    return setOfVariables.find(varName) == setOfVariables.end();
}

void StatementTable::insertIntoStatementTable(Integer stmtNum, StatementType stmtType)
{
    if (setOfStatements.find(stmtNum) == setOfStatements.end()) {
        setOfStatements.insert(stmtNum);
        listOfAllStatement.byType[stmtType].push_back(stmtNum);
        listOfAllStatement.byType[AnyStatement].push_back(stmtNum);
    }
}

Boolean StatementTable::getStatementFromIndex(Integer indexOfStmt)
{
    return setOfStatements.find(indexOfStmt) == setOfStatements.end();
}

std::vector<Integer> StatementTable::getAllStatements(StatementType stmtType)
{
    return listOfAllStatement.byType[stmtType];
}
