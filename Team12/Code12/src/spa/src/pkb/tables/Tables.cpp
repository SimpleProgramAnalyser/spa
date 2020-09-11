/**
 * Implementation of tables library.
 */

#include "Tables.h"


void ProcedureTable::insertIntoProcedureTable(String procName)
{
    listOfProcedureNames.push_back(procName);
    setOfProceduresNames.insert(procName);
}

std::vector<String> ProcedureTable::getAllProcedures()
{
    return listOfProcedureNames;
}

Boolean ProcedureTable::getProcedureFromName(String procName)
{
    std::unordered_set<String>::const_iterator iter = setOfProceduresNames.find(procName);
    if (iter == setOfProceduresNames.end()) {
        return false;
    } else {
        return true;
    }
}


void VariableTable::insertIntoVariableTable(String varName)
{
    listOfVariables.push_back(varName);
    setOfVariables.insert(varName);
}

std::vector<String> VariableTable::getAllVariables()
{
    return listOfVariables;
}

Boolean VariableTable::getVariableFromName(String varName)
{
    std::unordered_set<String>::const_iterator iter = setOfVariables.find(varName);
    if (iter == setOfVariables.end()) {
        return false;
    } else {
        return true;
    }
}

void StatementTable::insertIntoStatementTable(StatementNode* stmtNode, Integer lineNumber)
{
    StatementType stmtType = stmtNode->getStatementType();
    setOfStatements.insert(lineNumber);
    listOfAllStatement.byType[stmtType].push_back(lineNumber);
}

Boolean StatementTable::getStatementFromIndex(Integer indexOfStmt)
{
    std::unordered_set<Integer>::const_iterator iter = setOfStatements.find(indexOfStmt);
    if (iter == setOfStatements.end()) {
        return false;
    } else {
        return true;
    }
}

std::vector<Integer> StatementTable::getAllStatements(StatementType stmtType)
{
    return listOfAllStatement.byType[stmtType];
}
