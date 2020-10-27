/**
 * Implementation of Control Flow Graph CIP Builder.
 */

#include "CfgBipBuilder.h"

#include "CfgBuilder.h"
#include "pkb/PKB.h"

/**
 * Builds the CfgBip with a CfgNode.
 *
 * @param cfgNode The cfgNode to build the CfgBip
 * @param proceduresCfg Hash map of the procedures' names and the root node oftheir respective CFG
 * @param currentNumberOfNodes The current number of nodes
 * @param currentCfgBipNode The current CfgBip node
 * @param visitedMap Hash map of the visited nodes in each procedure, it is not a nullptr if it has been visited
 * @param currentProcName The current procedure name
 * @return The pointer to the current CfgNode of the CfgBip
 */

CfgNode* buildCfgBipWithNode(CfgNode* cfgNode, std::unordered_map<Name, CfgNode*>* proceduresCfg,
                             size_t& currentNumberOfNodes, CfgNode* currentCfgBipNode,
                             std::unordered_map<Name, Vector<CfgNode*>>* visitedMap, Name currentProcName)
{
    size_t currentCfgNodeNumber = cfgNode->nodeNumber;
    CfgNode* cfgBipNodeForCfgNodeInProcedure = visitedMap->at(currentProcName).at(currentCfgNodeNumber);
    // Visited
    if (cfgBipNodeForCfgNodeInProcedure != nullptr) {

    } else {
        visitedMap->at(currentProcName).at(currentCfgNodeNumber) = currentCfgBipNode;

        Vector<StatementNode*>* stmtList = cfgNode->statementNodes;
        Vector<CfgNode*>* childrenList = cfgNode->childrenNodes;

        Boolean prevStmtIsCallType = false;
        // For each statement node in current CFG node's stmt list
        for (size_t i = 0; i < stmtList->size(); i++) {
            StatementNode* stmtNode = stmtList->at(i);
            StatementType stmtType = stmtNode->getStatementType();

            switch (stmtType) {
            case AssignmentStatement:
            case ReadStatement:
            case PrintStatement:
            case WhileStatement:
            case IfStatement: {
                // Create a new node if the previous statment is a Call Statement
                // For the other statements in the same CfgNode, so that the procedure can
                // continue when it returns from the call
                if (prevStmtIsCallType) {
                    CfgNode* newCfgBipNode = createCfgNode(stmtList->size() - i, currentNumberOfNodes);
                    currentCfgBipNode->childrenNodes->push_back(newCfgBipNode);
                    currentCfgBipNode = newCfgBipNode;
                }
                currentCfgBipNode->statementNodes->push_back(stmtNode);
                prevStmtIsCallType = false;
                break;
            }
            case CallStatement: {
                CfgNode* newCallCfgBipNode;
                if (i == 0) {
                    newCallCfgBipNode = currentCfgBipNode;
                } else {
                    newCallCfgBipNode = createCfgNode(1, currentNumberOfNodes);
                    currentCfgBipNode->childrenNodes->push_back(newCallCfgBipNode);
                }
                newCallCfgBipNode->statementNodes->push_back(stmtNode);
                currentCfgBipNode = newCallCfgBipNode;

                CallStatementNode* callStmt = dynamic_cast<CallStatementNode*>(stmtNode);
                Name procName = callStmt->procedureName;
                CfgNode* calledProcCfgRootNode = proceduresCfg->at(procName);

                CfgNode* calledNodeCfgBipPointer = visitedMap->at(procName).at(calledProcCfgRootNode->nodeNumber);

                // The called procedure has been called
                if (calledNodeCfgBipPointer != nullptr) {
                    size_t indexOfLastCfgNodeOfCalledProc = visitedMap->at(procName).size() - 1;
                    // Last node of the CfgBipNode of the called procedure, to add current cfgBipNode as the last node's
                    // child
                    CfgNode* lastCfgNodeCfgBipNode = visitedMap->at(procName).at(indexOfLastCfgNodeOfCalledProc);
                    currentCfgBipNode->childrenNodes->push_back(calledNodeCfgBipPointer);
                    currentCfgBipNode = lastCfgNodeCfgBipNode;
                } else {
                    // Create new node to traverse the CFG of the called procedure
                    CfgNode* newCfgBipNode
                        = createCfgNode(calledProcCfgRootNode->statementNodes->size(), currentNumberOfNodes);
                    currentCfgBipNode->childrenNodes->push_back(newCfgBipNode);
                    currentCfgBipNode = buildCfgBipWithNode(calledProcCfgRootNode, proceduresCfg, currentNumberOfNodes,
                                                            newCfgBipNode, visitedMap, procName);
                }
                prevStmtIsCallType = true;
                break;
            }
            default:
                // Will not reach here
                break;
            }
        }

        for (size_t j = 0; j < childrenList->size(); j++) {
            CfgNode* currentChild = childrenList->at(j);
            CfgNode* childNodeCfgBipPointer = visitedMap->at(currentProcName).at(currentChild->nodeNumber);
            CfgNode* newCfgBipNode;
            // The CfgNode of this child has already been visited (i.e. if/ else)
            if (childNodeCfgBipPointer != nullptr) {
                Vector<CfgNode*>* currentCfgChildrenNodes = currentCfgBipNode->childrenNodes;
                Boolean currentNodeContainsChild = false;
                // Go through the children of the current CfgBipNode to see if it contains
                // the CfgBipNode of the child CfgNode node
                for (size_t k = 0; k < currentCfgChildrenNodes->size(); k++) {
                    if (currentCfgChildrenNodes->at(k) == childNodeCfgBipPointer) {
                        currentNodeContainsChild = true;
                        break;
                    }
                }
                // To prevent a node from adding a duplicate child
                if (!currentNodeContainsChild) {
                    currentCfgBipNode->childrenNodes->push_back(childNodeCfgBipPointer);
                }
                currentCfgBipNode = childNodeCfgBipPointer;
            } else {
                // Create new CfgNode for child in CfgBip
                newCfgBipNode = createCfgNode(currentChild->statementNodes->size(), currentNumberOfNodes);
                currentCfgBipNode->childrenNodes->push_back(newCfgBipNode);
                // Set the currentCfgBipNode when it is the last child
                if (j == childrenList->size() - 1) {
                    currentCfgBipNode = buildCfgBipWithNode(currentChild, proceduresCfg, currentNumberOfNodes,
                                                            newCfgBipNode, visitedMap, currentProcName);
                } else {
                    buildCfgBipWithNode(currentChild, proceduresCfg, currentNumberOfNodes, newCfgBipNode, visitedMap,
                                        currentProcName);
                }
            }
        }
    }
    return currentCfgBipNode;
}
/**
 * Builds the CFGBip of a given program.
 *
 * @param proceduresCfg Hash map of the procedures' names and the root node oftheir respective CFG
 * @firstProcName The name of the first procedure
 * @numberOfCfgNodes The total number of CfgNodes
 */
CfgNode* buildCfgBip(std::unordered_map<Name, CfgNode*>* proceduresCfg, Name firstProcName,
                     std::unordered_map<Name, size_t>* numberOfCfgNodes)
{
    size_t currentNumberOfNodes = -1;
    CfgNode* firstCfg = proceduresCfg->at(firstProcName);

    CfgNode* rootCfgBipNode = createCfgNode(firstCfg->statementNodes->size(), currentNumberOfNodes);
    CfgNode* currentCfgBipNode = rootCfgBipNode;

    // Initialise visitedArray for each procedure
    std::unordered_map<Name, Vector<CfgNode*>> visitedMap;
    for (auto it = numberOfCfgNodes->begin(); it != numberOfCfgNodes->end(); ++it) {
        Vector<CfgNode*> visitedArray;
        // Initialise the visitedArray to false
        for (size_t i = 0; i < it->second + 1; i++) {
            visitedArray.push_back(nullptr);
        }
        visitedMap.insert({it->first, visitedArray});
    }

    currentCfgBipNode = buildCfgBipWithNode(firstCfg, proceduresCfg, currentNumberOfNodes, currentCfgBipNode,
                                            &visitedMap, firstProcName);
    return rootCfgBipNode;
}