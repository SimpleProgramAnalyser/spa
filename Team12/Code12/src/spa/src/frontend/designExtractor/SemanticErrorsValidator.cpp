/**
 * Implementation of Semantic Errors Validator
 * classes and methods.
 */

#include "SemanticErrorsValidator.h"

#include <unordered_map>
#include <vector>

#include "ast/AstTypes.h"

SemanticErrorsValidator::SemanticErrorsValidator(ProgramNode& progNode): programNode(progNode) {}

Boolean SemanticErrorsValidator::isProgramValid()
{
    List<ProcedureNode>* procedureList = &(programNode.procedureList);

    // Check for duplicates
    std::unordered_map<std::string, int> procedureNameSet;
    int index = 0;
    for (size_t i = 0; i < procedureList->size(); i++) {
        ProcedureNode& procNode = *(procedureList->at(i));
        Name procName = procNode.procedureName;

        if (procedureNameSet.find(procName) != procedureNameSet.end()) {
            // String exists in set,it is a duplicate
            // Throw error

            return false;
        }
        index += 1;
        procedureNameSet.insert({procName, index});
    }

    // Fill up adj list
    size_t numberOfProcs = procedureList->size();
    std::vector<std::vector<int>> adjList(numberOfProcs);

    for (size_t i = 0; i < procedureList->size(); i++) {
        ProcedureNode& procNode = *(procedureList->at(i));
        Name procName = procNode.procedureName;
        int procIndex = getProcNameIndex(procedureNameSet, procName);
        const StmtlstNode& stmtListNode = *procNode.statementListNode;

        for (size_t j = 0; j < stmtListNode.statementList.size(); j++) {
            const List<StatementNode>& stmtList = stmtListNode.statementList;
            StatementNode* stmtNode = stmtList.at(j).get();
            populateCallGraphWithStatementNode(stmtNode, procedureNameSet, adjList, procIndex);
        }
    }

    return !isCyclic(adjList, numberOfProcs);
}

Void SemanticErrorsValidator::populateCallGraphWithStatementNode(StatementNode* stmtNode,
                                                                 std::unordered_map<std::string, int>& procedureNameSet,
                                                                 std::vector<std::vector<int>>& adjList,
                                                                 int currProcNameIndex)
{
    StatementType stmtType = stmtNode->getStatementType();
    switch (stmtType) {
    case WhileStatement:
        populateCallGraphWithWhileStatement(dynamic_cast<WhileStatementNode*>(stmtNode), procedureNameSet, adjList,
                                            currProcNameIndex);
        break;
    case CallStatement:
        addCallEdge(dynamic_cast<CallStatementNode*>(stmtNode), procedureNameSet, adjList, currProcNameIndex);
        break;
    case IfStatement:
        populateCallGraphWithIfStatement(dynamic_cast<IfStatementNode*>(stmtNode), procedureNameSet, adjList,
                                         currProcNameIndex);
        break;
    default:
        break;
    }
}

Void SemanticErrorsValidator::populateCallGraphWithIfStatement(IfStatementNode* stmtNode,
                                                               std::unordered_map<std::string, int>& procedureNameSet,
                                                               std::vector<std::vector<int>>& adjList,
                                                               int currProcNameIndex)
{
    const StmtlstNode& ifStmtListNode = *(stmtNode->ifStatementList);
    const StmtlstNode& elseStmtListNode = *(stmtNode->elseStatementList);
    const List<StatementNode>& ifStmtList = ifStmtListNode.statementList;
    const List<StatementNode>& elseStmtList = elseStmtListNode.statementList;

    for (size_t i = 0; i < ifStmtList.size(); i++) {
        StatementNode* statementNode = ifStmtList.at(i).get();
        populateCallGraphWithStatementNode(statementNode, procedureNameSet, adjList, currProcNameIndex);
    }

    for (size_t i = 0; i < elseStmtList.size(); i++) {
        StatementNode* statementNode = elseStmtList.at(i).get();
        populateCallGraphWithStatementNode(statementNode, procedureNameSet, adjList, currProcNameIndex);
    }
}

Void SemanticErrorsValidator::populateCallGraphWithWhileStatement(
    WhileStatementNode* stmtNode, std::unordered_map<std::string, int>& procedureNameSet,
    std::vector<std::vector<int>>& adjList, int currProcNameIndex)
{
    const ConditionalExpression* predicate = stmtNode->predicate;
    const StmtlstNode& stmtListNode = *(stmtNode->statementList);

    if (predicate == nullptr) {
        // TODO: throw error
    } else if (stmtListNode.statementList.size() == 0) {
        // TODO: throw error
    } else {
        const List<StatementNode>& stmtList = stmtListNode.statementList;
        for (size_t i = 0; i < stmtList.size(); i++) {
            StatementNode* statementNode = stmtList.at(i).get();
            populateCallGraphWithStatementNode(statementNode, procedureNameSet, adjList, currProcNameIndex);
        }
    }
}

Void SemanticErrorsValidator::addCallEdge(CallStatementNode* stmtNode,
                                          std::unordered_map<std::string, int>& procedureNameSet,
                                          std::vector<std::vector<int>>& adjList, int currProcNameIndex)
{
    Name procedureName = stmtNode->procedureName;
    if (procedureName == "") {
        // Throw error
    }

    int procIndex = getProcNameIndex(procedureNameSet, procedureName);

    // Add call procedureName in graph
    adjList[currProcNameIndex].push_back(procIndex);
}

int SemanticErrorsValidator::getProcNameIndex(std::unordered_map<std::string, int>& procedureNameSet,
                                              std::string procName)
{
    std::unordered_map<std::string, int>::const_iterator procedureIndexPair = procedureNameSet.find(procName);
    int procIndex = procedureIndexPair->second;
    return procIndex;
}

bool SemanticErrorsValidator::isCyclicUtil(int v, bool visited[], bool* recStack,
                                           std::vector<std::vector<int>>& adjList)
{
    if (visited[v] == false) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;

        // Recur for all the vertices adjacent to this vertex
        std::vector<int>::iterator i;
        for (i = adjList[v].begin(); i < adjList[v].end(); i++) {
            if (!visited[*i] && isCyclicUtil(*i, visited, recStack, adjList)) {
                return true;
            } else if (recStack[*i]) {
                return true;
            }
        }
    }
    recStack[v] = false; // remove the vertex from recursion stack
    return false;
}

bool SemanticErrorsValidator::isCyclic(std::vector<std::vector<int>>& adjList, size_t procListSize)
{
    // Mark all the vertices as not visited and not part of recursion stack
    bool* visited = new bool[procListSize];
    bool* recStack = new bool[procListSize];
    for (size_t i = 0; i < procListSize; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    // Call the recursive helper function to detect cycles
    for (size_t i = 0; i < procListSize; i++)
        if (isCyclicUtil(i, visited, recStack, adjList)) {
            // throw error
            return true;
        }
    return false;
}
