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
 * @param visitedMap Hash map of the visited nodes in the current procedure.
 *                   Entry is not a nullptr if the node has been visited.
 * @param currentProcName The current procedure name
 * @param visitedCfgProcedure Hashmap of the visited status of the CFG of a procedure. True if the CFG of a procedure
 * has been visited.
 * @param procNameOfRootNode The procedure name of the first procedure (at the root node)
 * @return The pointer to the current CfgNode of the CfgBip
 */

CfgNode* buildCfgBipWithNode(const CfgNode* cfgNode, std::unordered_map<Name, CfgNode*>* proceduresCfg,
                             size_t& currentNumberOfNodes, CfgNode* const currentCfgBipNode,
                             std::unordered_map<Name, Vector<CfgNode*>>* visitedMap, const Name& currentProcName,
                             std::unordered_map<Name, Boolean>* visitedCfgProcedure, const Name& procNameOfRootNode)
{
    size_t currentCfgNodeNumber = cfgNode->nodeNumber;
    if (currentCfgBipNode->nodeNumber == 4) {
        currentCfgNodeNumber = currentCfgNodeNumber;
    }
    visitedMap->at(currentProcName).at(currentCfgNodeNumber) = currentCfgBipNode;

    Vector<StatementNode*>* stmtList = cfgNode->statementNodes;
    Vector<CfgNode*>* childrenList = cfgNode->childrenNodes;
    CfgNode* returnedCfgBipNode = currentCfgBipNode;
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
                returnedCfgBipNode->childrenNodes->push_back(newCfgBipNode);
                returnedCfgBipNode = newCfgBipNode;
            }
            returnedCfgBipNode->statementNodes->push_back(stmtNode);
            prevStmtIsCallType = false;
            break;
        }
        case CallStatement: {
            CfgNode* newCallCfgBipNode;
            if (i == 0) {
                newCallCfgBipNode = returnedCfgBipNode;
            } else {
                newCallCfgBipNode = createCfgNode(1, currentNumberOfNodes);
                returnedCfgBipNode->childrenNodes->push_back(newCallCfgBipNode);
            }
            newCallCfgBipNode->statementNodes->push_back(stmtNode);
            returnedCfgBipNode = newCallCfgBipNode;

            auto* callStmt = dynamic_cast<CallStatementNode*>(stmtNode);
            Name procName = callStmt->procedureName;
            CfgNode* calledProcCfgRootNode = proceduresCfg->at(procName);

            CfgNode* calledNodeCfgBipPointer = visitedMap->at(procName).at(calledProcCfgRootNode->nodeNumber);

            bool needToCreateNewProcedure = false;

            // The called procedure has been called
            if (calledNodeCfgBipPointer != nullptr) {
                size_t indexOfLastCfgNodeOfCalledProc = visitedMap->at(procName).size() - 1;
                // Last node of the CfgBipNode of the called procedure, to add current cfgBipNode as the last node's
                // child
                CfgNode* lastCfgNodeCfgBipNode = visitedMap->at(procName).at(indexOfLastCfgNodeOfCalledProc);
                if (lastCfgNodeCfgBipNode->childrenNodes->empty()) {
                    returnedCfgBipNode->childrenNodes->push_back(calledNodeCfgBipPointer);
                    returnedCfgBipNode = lastCfgNodeCfgBipNode;
                } else {
                    // create a new copy of procedure instead
                    needToCreateNewProcedure = true;
                }
            } else {
                needToCreateNewProcedure = true;
            }

            if (needToCreateNewProcedure) {
                visitedCfgProcedure->at(procName) = true;
                // Create new node to traverse the CFG of the called procedure
                CfgNode* newCfgBipNode
                    = createCfgNode(calledProcCfgRootNode->statementNodes->size(), currentNumberOfNodes);
                // New visited map for the new procedure, to prevent
                // connection back to the old nodes
                std::unordered_map<Name, Vector<CfgNode*>> newProcVisitedMap;
                for (const std::pair<const Name, Vector<CfgNode*>>& mapEntry : *visitedMap) {
                    Vector<CfgNode*> visitedArray;
                    size_t arraySize = mapEntry.second.size();
                    for (size_t is = 0; is < arraySize; is++) {
                        visitedArray.push_back(nullptr);
                    }
                    newProcVisitedMap.insert({mapEntry.first, std::move(visitedArray)});
                }
                // Now, handle the building of new procedure nodes
                returnedCfgBipNode->childrenNodes->push_back(newCfgBipNode);
                returnedCfgBipNode
                    = buildCfgBipWithNode(calledProcCfgRootNode, proceduresCfg, currentNumberOfNodes, newCfgBipNode,
                                          &newProcVisitedMap, procName, visitedCfgProcedure, procNameOfRootNode);
            }
            prevStmtIsCallType = true;
            break;
        }
        default:
            // Will not reach here
            break;
        }
    }

    // If the last statement of the first procedure is a call statement, we create a dummy node
    if (prevStmtIsCallType && childrenList->empty() && currentProcName == procNameOfRootNode) {
        CfgNode* newCfgBipNode = createCfgNode(0, currentNumberOfNodes);
        returnedCfgBipNode->childrenNodes->push_back(newCfgBipNode);
        return newCfgBipNode;
    }

    for (size_t j = 0; j < childrenList->size(); j++) {
        CfgNode* currentChild = childrenList->at(j);
        CfgNode* childNodeCfgBipPointer = visitedMap->at(currentProcName).at(currentChild->nodeNumber);
        CfgNode* newCfgBipNode;
        // The CfgNode of this child has already been visited (i.e. if/ else)
        if (childNodeCfgBipPointer != nullptr) {
            Vector<CfgNode*>* currentCfgChildrenNodes = returnedCfgBipNode->childrenNodes;
            Boolean currentNodeContainsChild = false;
            // Go through the children of the current CfgBipNode to see if it contains
            // the CfgBipNode of the child CfgNode node
            for (CfgNode* currentCfgChildrenNode : *currentCfgChildrenNodes) {
                if (currentCfgChildrenNode == childNodeCfgBipPointer) {
                    currentNodeContainsChild = true;
                    break;
                }
            }
            // To prevent a node from adding a duplicate child
            if (!currentNodeContainsChild) {
                returnedCfgBipNode->childrenNodes->push_back(childNodeCfgBipPointer);
            }

            // Assign the return node as the CFGBip equivalent of the last node
            // in the CFG of the current procedure
            size_t sizeOfCfgOfCurrentProcedure = visitedMap->at(currentProcName).size();
            returnedCfgBipNode = visitedMap->at(currentProcName).at(sizeOfCfgOfCurrentProcedure - 1);
        } else {
            // Create new CfgNode for child in CfgBip
            newCfgBipNode = createCfgNode(currentChild->statementNodes->size(), currentNumberOfNodes);
            returnedCfgBipNode->childrenNodes->push_back(newCfgBipNode);
            // Set the returnedCfgBipNode when it is the last child
            if (j == childrenList->size() - 1) {
                returnedCfgBipNode
                    = buildCfgBipWithNode(currentChild, proceduresCfg, currentNumberOfNodes, newCfgBipNode, visitedMap,
                                          currentProcName, visitedCfgProcedure, procNameOfRootNode);
            } else {
                buildCfgBipWithNode(currentChild, proceduresCfg, currentNumberOfNodes, newCfgBipNode, visitedMap,
                                    currentProcName, visitedCfgProcedure, procNameOfRootNode);
            }
        }

        // Adds the CfgBip join node to If/else nodes of the If Statement
        // The CfgBip join node will be the CFGBip equivalent of the CFG's dummy node
        if (stmtList->at(0)->getStatementType() == IfStatement) {
            newCfgBipNode->ifJoinNode = visitedMap->at(currentProcName).at(currentChild->ifJoinNode->nodeNumber);
        }
    }

    return returnedCfgBipNode;
}
/**
 * Builds the CFGBip of a given program.
 *
 * @param proceduresCfg Hash map of the procedures' names and the root node oftheir respective CFG
 * @param procName The name of the current procedure
 * @param numberOfCfgNodes The total number of CfgNodes for each procedure in its CFG
 * @param visitedCfgProcedure Hashmap of the visited status of the CFG of a procedure. True if the CFG of a procedure
 * has been visited.
 * @return The root CfgBip node
 */
CfgNode* buildCfgBip(std::unordered_map<Name, CfgNode*>* proceduresCfg, Name procName,
                     std::unordered_map<Name, size_t>* numberOfCfgNodes,
                     std::unordered_map<Name, Boolean>* visitedCfgProcedure)
{
    size_t currentNumberOfNodes = -1;
    CfgNode* firstCfg = proceduresCfg->at(procName);

    CfgNode* rootCfgBipNode = createCfgNode(firstCfg->statementNodes->size(), currentNumberOfNodes);

    // Initialise visitedArray for each CFG node in all the procedures
    std::unordered_map<Name, Vector<CfgNode*>> visitedMap;
    for (const std::pair<const Name, size_t>& numberOfCfgNode : *numberOfCfgNodes) {
        Vector<CfgNode*> visitedArray;
        // Initialise the visitedArray to false
        for (size_t i = 0; i < numberOfCfgNode.second + 1; i++) {
            visitedArray.push_back(nullptr);
        }
        visitedMap.insert({numberOfCfgNode.first, visitedArray});
    }

    buildCfgBipWithNode(firstCfg, proceduresCfg, currentNumberOfNodes, rootCfgBipNode, &visitedMap, procName,
                        visitedCfgProcedure, procName);
    return rootCfgBipNode;
}
