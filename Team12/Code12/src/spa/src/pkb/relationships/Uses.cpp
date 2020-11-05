#include "Uses.h"

#include <cassert>

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

    for (const auto& varName : varNames) {
        if (allVarUsedByProcSet.find(varName) == allVarUsedByProcSet.end()) {
            allVarUsedByProcSet.insert(varName);
            allVarUsedByProcList.push_back(varName);
        }
    }

    // add to allUsesProc
    allUsesProc.push_back(procName);

    // add tuple
    for (const auto& varName : varNames) {
        procTuples.push_back(std::make_pair(procName, varName));
    }
}

void UsesTable::addUsesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(
        stmtType > AnyStatement && stmtType < StatementTypeCount
        && "Statement type cannot be AnyStatement or STATEMENT_TYPE_COUNT"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
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

    // add tuple
    for (const auto& varName : varNames) {
        statementTuples[AnyStatement].push_back(std::make_pair(stmtNum, varName));
        statementTuples[stmtType].push_back(std::make_pair(stmtNum, varName));
    }
}

Boolean UsesTable::checkIfProcedureUses(const String& procName, const String& varName)
{
    if (procVarsetMap.find(procName) == procVarsetMap.end()) {
        return false;
    }
    auto varSet = procVarsetMap[procName];
    return varSet.find(varName) != varSet.end();
}

Boolean UsesTable::checkIfStatementUses(Integer stmt, const String& varName)
{
    if (stmtVarsetMap.find(stmt) == stmtVarsetMap.end()) {
        return false;
    }
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
Vector<String> UsesTable::getUsesVariablesFromProcedure(const String& procName)
{
    return procVarlistMap[procName];
}
Vector<Integer> UsesTable::getAllUsesStatements(StatementType stmtType)
{
    return stmttypeStmtlistMap[stmtType];
}
Vector<String> UsesTable::getAllUsesVariablesFromStatementType(StatementType stmtType)
{
    return stmttypeVarlistMap[stmtType];
}
Vector<String> UsesTable::getAllUsesVariablesFromProgram()
{
    return allVarUsedByProcList;
}
Vector<String> UsesTable::getAllUsesProcedures()
{
    return allUsesProc;
}
Vector<Pair<Integer, String>> UsesTable::getAllUsesStatementTuple(StatementType stmtType)
{
    return statementTuples[stmtType];
}
Vector<Pair<String, String>> UsesTable::getAllUsesProcedureTuple()
{
    return procTuples;
}
