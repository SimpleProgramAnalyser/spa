/**
 * Implementation of Semantic Errors Validator
 * classes and methods.
 */

#include "SemanticErrorsValidator.h"

#include <unordered_map>
#include <vector>

SemanticErrorsValidator::SemanticErrorsValidator(ProgramNode& progNode):
    programNode(progNode), hasExecutedValidityChecks(false), programValidity(false)
{
    // initiate adjacency matrix
    size_t length = progNode.procedureList.size();
    adjacencyMatrixOfCalls.reserve(length);
    for (size_t i = 0; i < length; i++) {
        adjacencyMatrixOfCalls.push_back(std::vector<bool>());
        adjacencyMatrixOfCalls.at(i).reserve(length);
        for (size_t j = 0; j < length; j++) {
            // initialise all edges to 0
            adjacencyMatrixOfCalls.at(i).push_back(false);
        }
    }
}

/**
 * Returns valid if the program is semantically valid.
 *
 * @return A boolean to indicate if the program is semantically valid
 */
Boolean SemanticErrorsValidator::isProgramValid()
{
    if (!hasExecutedValidityChecks) {
        programValidity = checkForAllSemanticErrors();
        hasExecutedValidityChecks = true;
    }
    return programValidity;
}

/**
 * Checks for all semantic errors in the program such as having
 * duplicated procedure names, calling non-existing procedure
 * and having recursive and cyclic calls.
 *
 * @return A boolean to indicate if the program is semantically valid
 */
Boolean SemanticErrorsValidator::checkForAllSemanticErrors()
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
        procedureNameSet.insert({procName, index});
        index += 1;
    }

    // Check if call statements call existent procedures and fill up adj list
    size_t numberOfProcs = procedureList->size();

    // Go through the statement list to look for call statements
    for (size_t i = 0; i < procedureList->size(); i++) {
        ProcedureNode& procNode = *(procedureList->at(i));
        Name procName = procNode.procedureName;
        int procIndex = getProcNameIndex(procedureNameSet, procName);
        const StmtlstNode& stmtListNode = *procNode.statementListNode;

        for (size_t j = 0; j < stmtListNode.statementList.size(); j++) {
            StatementNode* stmtNode = (stmtListNode.statementList).at(j).get();

            // Check if all call statements call existent procedures
            Boolean callStatementCallsExistentProcedure
                = checkCallStatementCallsValidProcedure(stmtNode, procedureNameSet, procIndex);
            if (!callStatementCallsExistentProcedure) {
                return false;
            }

            // Fill up adj list
            populateCallGraphWithStatementNode(stmtNode, procedureNameSet, procIndex);
        }
    }

    return !isCyclic(numberOfProcs);
}

/**
 * Checks the call statement calls a valid procedure.
 *
 * @param stmtNode The statement node
 * @param procedureNameSet The set of procedure names
 * @param currProcNameIndex The index of the current procedure in the adjacencyMatrixOfCalls
 */
Boolean SemanticErrorsValidator::checkCallStatementCallsValidProcedure(
    StatementNode* stmtNode, std::unordered_map<std::string, int>& procedureNameSet, int currProcNameIndex)
{
    StatementType stmtType = stmtNode->getStatementType();
    if (stmtType == CallStatement) {
        CallStatementNode* callStmtNode = dynamic_cast<CallStatementNode*>(stmtNode);
        Name procedureName = callStmtNode->procedureName;
        std::unordered_map<std::string, int>::const_iterator procedureIndexPair = procedureNameSet.find(procedureName);
        // Check if procedure exists or procedure is recursive
        if (procedureIndexPair == procedureNameSet.end() || procedureIndexPair->second == currProcNameIndex) {
            // Throw error
            return false;
        }
    }
    return true;
}

/**
 * Populates the adjacencyMatrixOfCalls with the statement Nodes.
 *
 * @param stmtNode The statement Node
 * @param procedureNameSet The set of procedure names
 * @param currProcNameIndex The index of the current procedure in the adjacencyMatrixOfCalls
 */
Void SemanticErrorsValidator::populateCallGraphWithStatementNode(StatementNode* stmtNode,
                                                                 std::unordered_map<std::string, int>& procedureNameSet,
                                                                 int currProcNameIndex)
{
    StatementType stmtType = stmtNode->getStatementType();
    switch (stmtType) {
    case WhileStatement:
        populateCallGraphWithWhileStatement(dynamic_cast<WhileStatementNode*>(stmtNode), procedureNameSet,
                                            currProcNameIndex);
        break;
    case CallStatement:
        addCallEdge(dynamic_cast<CallStatementNode*>(stmtNode), procedureNameSet, currProcNameIndex);
        break;
    case IfStatement:
        populateCallGraphWithIfStatement(dynamic_cast<IfStatementNode*>(stmtNode), procedureNameSet, currProcNameIndex);
        break;
    default:
        break;
    }
}

/**
 * Populates the adjacencyMatrixOfCalls with the IfStatement Node.
 *
 * @param stmtNode The IfStatement Node
 * @param procedureNameSet The set of procedure names
 * @param currProcNameIndex The index of the current procedure in the adjacencyMatrixOfCalls
 */
Void SemanticErrorsValidator::populateCallGraphWithIfStatement(IfStatementNode* stmtNode,
                                                               std::unordered_map<std::string, int>& procedureNameSet,
                                                               int currProcNameIndex)
{
    const StmtlstNode& ifStmtListNode = *(stmtNode->ifStatementList);
    const StmtlstNode& elseStmtListNode = *(stmtNode->elseStatementList);
    const List<StatementNode>& ifStmtList = ifStmtListNode.statementList;
    const List<StatementNode>& elseStmtList = elseStmtListNode.statementList;

    for (size_t i = 0; i < ifStmtList.size(); i++) {
        StatementNode* statementNode = ifStmtList.at(i).get();
        populateCallGraphWithStatementNode(statementNode, procedureNameSet, currProcNameIndex);
    }

    for (size_t i = 0; i < elseStmtList.size(); i++) {
        StatementNode* statementNode = elseStmtList.at(i).get();
        populateCallGraphWithStatementNode(statementNode, procedureNameSet, currProcNameIndex);
    }
}

/**
 * Populates the adjacencyMatrixOfCalls with the WhileStatement Node.
 *
 * @param stmtNode The WhileStatement Node
 * @param procedureNameSet The set of procedure names
 * @param currProcNameIndex The index of the current procedure in the adjacencyMatrixOfCalls
 */
Void SemanticErrorsValidator::populateCallGraphWithWhileStatement(
    WhileStatementNode* stmtNode, std::unordered_map<std::string, int>& procedureNameSet, int currProcNameIndex)
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
            populateCallGraphWithStatementNode(statementNode, procedureNameSet, currProcNameIndex);
        }
    }
}

/**
 * Adds an edge to the adjacencyMatrixOfCalls so that we can
 * use it to identify cyclic calls.
 *
 * @param stmtNode The CallStatement node
 * @param procedureNameSet The set of procedure names
 * @param currProcNameIndex The index of the current procedure in the adjacencyMatrixOfCalls
 */
Void SemanticErrorsValidator::addCallEdge(CallStatementNode* stmtNode,
                                          std::unordered_map<std::string, int>& procedureNameSet, int currProcNameIndex)
{
    Name procedureName = stmtNode->procedureName;
    if (procedureName == "") {
        // Throw error
    }

    int procIndex = getProcNameIndex(procedureNameSet, procedureName);

    // Add call procedureName in graph
    adjacencyMatrixOfCalls.at(currProcNameIndex).at(procIndex) = true;
}

/**
 * Returns the index of a procedure in the procedureNameSet.
 *
 * @param procedureNameSet The set of procedure names
 * @param procName The procedure name
 * @return The index of the named procedure in procedureNameSet
 */
int SemanticErrorsValidator::getProcNameIndex(std::unordered_map<std::string, int> procedureNameSet,
                                              const std::string& procName)
{
    std::unordered_map<std::string, int>::const_iterator procedureIndexPair = procedureNameSet.find(procName);
    // We assume that all procedures exists
    int procIndex = 0;
    if (procedureIndexPair != procedureNameSet.end()) {
        procIndex = procedureIndexPair->second;
    }
    return procIndex;
}

/**
 * Helper function to check for cyclic calls using the
 * adjacencyMatrixOfCalls.
 *
 * @param v The index of the current node
 * @param visited[] The visited array to mark all the visited nodes
 * @param recstack The current recursion stack
 * @return A boolean indicating if there is a cycle
 */
bool SemanticErrorsValidator::isCyclicUtil(int v, bool visited[], bool* recStack)
{
    if (!visited[v]) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;

        // Recur for all the vertices adjacent to this vertex
        size_t length = adjacencyMatrixOfCalls.size();
        for (size_t i = 0; i < length; i++) {
            if (adjacencyMatrixOfCalls.at(v).at(i)
                && ((!visited[i] && isCyclicUtil(i, visited, recStack)) || recStack[i])) {
                return true;
            }
        }
    }
    recStack[v] = false; // remove the vertex from recursion stack
    return false;
}

/**
 * Checks for cyclic calls in the program.
 *
 * @param procListSize The size of the procedure list to
 *        initialise the visited and recStack arrays
 * @return A boolean indicating is there is a cycle in the program.
 */
bool SemanticErrorsValidator::isCyclic(size_t procListSize)
{
    // Mark all the vertices as not visited and not part of recursion stack
    bool* visited = new bool[procListSize];
    bool* recStack = new bool[procListSize];
    for (size_t i = 0; i < procListSize; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    // Call the recursive helper function to detect cycles
    for (size_t i = 0; i < procListSize; i++) {
        if (isCyclicUtil(i, visited, recStack)) {
            // throw error
            delete[] visited;
            delete[] recStack;
            return true;
        }
    }

    delete[] visited;
    delete[] recStack;
    return false;
}

/**
 * Helper function for Kahn's algorithm. After a node is
 * removed from the graph and put in the sorted list,
 * this function will update the adjacency matrix and
 * add any new edgeless nodes to the list of nodes that
 * can be added to the sorted list next.
 *
 * @param adjacencyMatrix The adjacency matrix representing
 *                                the directed acyclic graph.
 * @param nodesToSearch The list of nodes to be looked at next.
 * @param removedNode The index of the node that was removed.
 */
void handleNeighbours(Matrix& adjacencyMatrix, std::vector<size_t>& nodesToSearch, size_t removedNode)
{
    size_t length = adjacencyMatrix.size();
    for (size_t i = 0; i < length; i++) {
        if (!adjacencyMatrix.at(i).at(removedNode)) {
            continue;
        }
        // remove edge i -> currentNode from the graph
        adjacencyMatrix.at(i).at(removedNode) = false;
        // check if i has any more outgoing edges
        bool hasOutgoingEdge = false;
        for (size_t j = 0; j < length; j++) {
            if (adjacencyMatrix.at(i).at(j)) {
                hasOutgoingEdge = true;
                break;
            }
        }
        if (!hasOutgoingEdge) {
            // i is now pointing to nothing
            nodesToSearch.push_back(i);
        }
    }
}

std::vector<int> SemanticErrorsValidator::reverseTopologicalSort()
{
    if (!hasExecutedValidityChecks || !programValidity) {
        // program has not been executed, or program is invalid
        return std::vector<int>();
    }

    // Kahn's algorithm, modified for reverse order
    // deep-clone the matrix to keep track of edges
    size_t length = adjacencyMatrixOfCalls.size();
    Matrix adjacencyMatrixForKahns;
    adjacencyMatrixForKahns.reserve(length);
    for (size_t i = 0; i < length; i++) {
        adjacencyMatrixForKahns.push_back(std::vector<bool>());
        adjacencyMatrixForKahns.at(i).reserve(length);
        for (size_t j = 0; j < length; j++) {
            // copy all edges
            adjacencyMatrixForKahns.at(i).push_back(adjacencyMatrixOfCalls.at(i).at(j));
        }
    }
    std::vector<int> reverseSortedNodes;
    std::vector<size_t> nodesToSearch;
    // Find nodes that have no edges pointing away from them
    for (size_t i = 0; i < length; i++) {
        bool hasEdgePointingAway = false;
        for (size_t j = 0; j < length; j++) {
            if (adjacencyMatrixForKahns.at(i).at(j)) {
                // i has an edge pointing to another node j
                hasEdgePointingAway = true;
                break;
            }
        }
        if (!hasEdgePointingAway) {
            // found a node i with no edges pointing away from it
            nodesToSearch.push_back(i);
        }
    }

    // start from nodes pointing to nothing
    while (!nodesToSearch.empty()) {
        size_t currentNode = nodesToSearch.at(nodesToSearch.size() - 1);
        nodesToSearch.pop_back();
        // add node to the reverse sorted list
        reverseSortedNodes.push_back(static_cast<int>(currentNode));
        // remove node currentNode from the graph
        handleNeighbours(adjacencyMatrixForKahns, nodesToSearch, currentNode);
    }

    return reverseSortedNodes;
}
