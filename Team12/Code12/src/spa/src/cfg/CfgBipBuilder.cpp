/**
 * Implementation of Control Flow Graph CIP Builder.
 */

#include "CfgBipBuilder.h"

#include "CfgBuilder.h"
#include "pkb/PKB.h"

Void copyAndAddStmtNode(StatementNode* stmtNode, CfgNode* newCfgNode)
{
    (*newCfgNode->statementNodes).push_back(std::unique_ptr<StatementNode>(stmtNode));
}

CfgNode* buildCfgBipWithNode(CfgNode* cfgNode, std::unordered_map<Name, CfgNode*>* proceduresCfg,
                             size_t& currentNumberOfNodes, CfgNode* currentCfgBipNode,
                             std::unordered_map<Name, Vector<CfgNode*>>* visitedMap, Name currentProcName)
{
    size_t currentCfgNodeNumber = cfgNode->nodeNumber;
    CfgNode* cfgBipNodeForCfgNodeInProcedure = visitedMap->at(currentProcName).at(currentCfgNodeNumber);
    //Visited
    if (cfgBipNodeForCfgNodeInProcedure != nullptr) {

    } else { 
        visitedMap->at(currentProcName).at(currentCfgNodeNumber) = currentCfgBipNode;

        List<StatementNode>* stmtList = cfgNode->statementNodes;
        List<CfgNode>* childrenList = cfgNode->childrenNodes;

        // For each statement node in current CFG node's stmt list
        for (size_t i = 0; i < stmtList->size(); i++) {
            StatementNode* stmtNode = stmtList->at(i).get();
            StatementType stmtType = stmtNode->getStatementType();

            switch (stmtType) {
            case AssignmentStatement:
            case ReadStatement:
            case PrintStatement:
            case WhileStatement:
            case IfStatement: {
                copyAndAddStmtNode(stmtNode, currentCfgBipNode);
                break;
            } 
            case CallStatement: {
                copyAndAddStmtNode(stmtNode, currentCfgBipNode);

                CallStatementNode* callStmt = dynamic_cast<CallStatementNode*>(stmtNode);
                Name procName = callStmt->procedureName;
                CfgNode* calledProcCfgRootNode = proceduresCfg->at(procName);

                CfgNode* calledNodeCfgBipPointer = visitedMap->at(procName).at(calledProcCfgRootNode->nodeNumber);

                if (calledNodeCfgBipPointer != nullptr) {
                    size_t indexOfLastCfgNodeOfCalledProc = visitedMap->at(procName).size() - 1;
                    // Last node of the CfgBipNode of the called procedure
                    CfgNode* lastCfgNodeCfgBipNode = visitedMap->at(procName).at(indexOfLastCfgNodeOfCalledProc);
                    currentCfgBipNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(calledNodeCfgBipPointer));

                    lastCfgNodeCfgBipNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(currentCfgBipNode));
                } else {
                    CfgNode* newCfgBipNode
                        = createCfgNode(calledProcCfgRootNode->statementNodes->size(), currentNumberOfNodes);
                    currentCfgBipNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(newCfgBipNode));

                    currentCfgBipNode = buildCfgBipWithNode(calledProcCfgRootNode, proceduresCfg, currentNumberOfNodes,
                                                            newCfgBipNode, visitedMap, procName);
                }

                break;
            }
            default:
                // Will not reach here
                break;
            }
        }

        for (size_t j = 0; j < childrenList->size(); j++) {
            CfgNode* currentChild = childrenList->at(j).get();
            CfgNode* childNodeCfgBipPointer = visitedMap->at(currentProcName).at(currentChild->nodeNumber);
            CfgNode* newCfgBipNode;
            if (childNodeCfgBipPointer != nullptr) {
                newCfgBipNode = childNodeCfgBipPointer;
                currentCfgBipNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(newCfgBipNode));

            } else {
                newCfgBipNode = createCfgNode(currentChild->statementNodes->size(), currentNumberOfNodes);
                currentCfgBipNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(newCfgBipNode));

                if (j == childrenList->size() - 1) {
                    currentCfgBipNode = buildCfgBipWithNode(currentChild, proceduresCfg, currentNumberOfNodes,
                                                            newCfgBipNode, visitedMap, currentProcName);
                } else {
                    buildCfgBipWithNode(currentChild, proceduresCfg, currentNumberOfNodes,
                                                            newCfgBipNode, visitedMap,
                                        currentProcName);
                }
            }

        }
    }
    return currentCfgBipNode;
}

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

    currentCfgBipNode = buildCfgBipWithNode(firstCfg, proceduresCfg, currentNumberOfNodes, currentCfgBipNode, &visitedMap,
                                         firstProcName);
    return rootCfgBipNode;
}
