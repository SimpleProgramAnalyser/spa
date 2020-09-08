/**
 * Implementation of tables library.
 */

#include "Tables.h"

RootNode::RootNode(): rootNode(NULL){};

void RootNode::assignRootNode(ProgramNode* rootNodeToAssign)
{
    rootNode = rootNodeToAssign;
}

ProgramNode* RootNode::getRootNode()
{
    return rootNode;
}

ProcedureTable::ProcedureTable(){};

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

VariableTable::VariableTable(){};

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

StatementTable::StatementTable():
    listOfAllStatement({{AnyStatement, std::vector<Integer>()},
                        {AssignmentStatement, std::vector<Integer>()},
                        {CallStatement, std::vector<Integer>()},
                        {IfStatement, std::vector<Integer>()},
                        {PrintStatement, std::vector<Integer>()},
                        {ReadStatement, std::vector<Integer>()},
                        {WhileStatement, std::vector<Integer>()}})
{}

void StatementTable::insertIntoStatementTable(StatementNode* stmtNode, Integer lineNumber)
{
    StatementType stmtType = stmtNode->getStatementType();
    setOfStatements.insert(lineNumber);
    listOfAllStatement.at(stmtType).push_back(lineNumber);
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
    return listOfAllStatement.at(stmtType);
}
