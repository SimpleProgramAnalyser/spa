#include "Uses.h"

void UsesTable::addUsesRelationships(const String& procName, Vector<String> varNames)
{
    // add to procVarsetMap
    procVarsetMap[procName].insert(varNames.begin(), varNames.end());

    // add to procVarlistMap
    auto varList = &procVarlistMap[procName];
    varList->insert(varList->end(), varNames.begin(), varNames.end());

    // add to varProclistMap
    for (const auto& varName : varNames) {
        // if doesn't exist, create an empty vector
        varProclistMap[varName].push_back(procName);
    }

    // add to allVarUsedByProc
    allVarUsedByProc.insert(varNames.begin(), varNames.end());

    // add to allUsesProc
    allUsesProc.push_back(procName);
}

void UsesTable::addUsesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames)
{
    // add to stmtVarsetMap
    stmtVarsetMap[stmtNum].insert(varNames.begin(), varNames.end());

    // add to stmtVarlistMap
    auto varList = &stmtVarlistMap[stmtNum];
    varList->insert(varList->end(), varNames.begin(), varNames.end());

    // add to varProclistMap
    for (const auto& varName : varNames) {
        varStmtlistMap[varName].byType[stmtType].push_back(stmtNum);
        varStmtlistMap[varName].byType[StatementType::AnyStatement].push_back(stmtNum);
    }

    // add to stmttypeVarlistMap
    auto typeSpecificVarList = &stmttypeVarlistMap[stmtType];
    typeSpecificVarList->insert(typeSpecificVarList->end(), varNames.begin(), varNames.end());
    auto allStmtTypeVarList = &stmttypeVarlistMap[StatementType::AnyStatement];
    allStmtTypeVarList->insert(allStmtTypeVarList->end(), varNames.begin(), varNames.end());

    // add to stmttypeStmtlistMap
    stmttypeStmtlistMap[stmtType].push_back(stmtNum);
    stmttypeStmtlistMap[StatementType::AnyStatement].push_back(stmtNum);
}

Boolean UsesTable::checkIfProcedureUses(const String& procName, const String& varName)
{
    auto varSet = procVarsetMap[procName];
    return varSet.find(varName) != varSet.end();
}

Boolean UsesTable::checkIfStatementUses(Integer stmt, const String& varName)
{
    auto varSet = stmtVarsetMap[stmt];
    return varSet.find(varName) != varSet.end();
}
Vector<Integer> UsesTable::getUsesStatements(const String& varName, StatementType stmtType)
{
    return varStmtlistMap[varName].byType[stmtType];
}
Vector<String> UsesTable::getUsesProcedures(const String& varName)
{
    return varProclistMap[varName];
}
Vector<String> UsesTable::getUsesVariablesFromStatement(Integer stmt)
{
    return stmtVarlistMap[stmt];
}
Vector<String> UsesTable::getusesVariablesFromProcedure(const String& procName)
{
    return procVarlistMap[procName];
}
Vector<Integer> UsesTable::getAllUsesStatements(StatementType stmtType)
{
    return stmttypeStmtlistMap[stmtType];
}
Vector<String> UsesTable::getAllUsesVariables(StatementType stmtType)
{
    return stmttypeVarlistMap[stmtType];
}
Vector<String> UsesTable::getAllUsesVariables(const String& procName)
{
    return procVarlistMap[procName];
}
Vector<String> UsesTable::getAllUsesProcedures()
{
    return allUsesProc;
}
