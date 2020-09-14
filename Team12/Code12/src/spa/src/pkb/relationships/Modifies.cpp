#include "Modifies.h"

void ModifiesTable::addModifiesRelationships(const String& procName, Vector<String> varNames)
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

    // add to allModifiesProc
    allModifiesProc.push_back(procName);
}

void ModifiesTable::addModifiesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames)
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

Boolean ModifiesTable::checkIfProcedureModifies(const String& procName, const String& varName)
{
    auto varSet = procVarsetMap[procName];
    return varSet.find(varName) != varSet.end();
}

Boolean ModifiesTable::checkIfStatementModifies(Integer stmt, const String& varName)
{
    auto varSet = stmtVarsetMap[stmt];
    return varSet.find(varName) != varSet.end();
}
Vector<Integer> ModifiesTable::getModifiesStatements(const String& varName, StatementType stmtType)
{
    return varStmtlistMap[varName].byType[stmtType];
}
Vector<String> ModifiesTable::getModifiesProcedures(const String& varName)
{
    return varProclistMap[varName];
}
Vector<String> ModifiesTable::getModifiesVariablesFromStatement(Integer stmt)
{
    return stmtVarlistMap[stmt];
}
Vector<String> ModifiesTable::getModifiesVariablesFromProcedure(const String& procName)
{
    return procVarlistMap[procName];
}
Vector<Integer> ModifiesTable::getAllModifiesStatements(StatementType stmtType)
{
    return stmttypeStmtlistMap[stmtType];
}
Vector<String> ModifiesTable::getAllModifiesVariables(StatementType stmtType)
{
    return stmttypeVarlistMap[stmtType];
}
Vector<String> ModifiesTable::getAllModifiesVariables(const String& procName)
{
    return procVarlistMap[procName];
}
Vector<String> ModifiesTable::getAllModifiesProcedures()
{
    return allModifiesProc;
}
