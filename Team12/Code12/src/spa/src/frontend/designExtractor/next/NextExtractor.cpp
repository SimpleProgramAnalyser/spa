/**
 * Implementation of Next extractor.
 */
#include "NextExtractor.h"

#include <stdexcept>
#include <unordered_set>

#include "pkb/PKB.h"

/**
 * Calls the PKB API to add a Next relationship between two statement nodes. Also updates
 * the nextRelationships vector to represent a next relationship for testing.
 *
 * @param currentNode The previous node in the Next relationship
 * @param previousNode The next node in the Next relationship
 * @param nextRelationships Vector of pairs of integers to represent Next
 *        Relationships. Solely for testing purposes
 */
Void addNextRelationshipBetweenNodes(StatementNode* currentNode, StatementNode* nextNode,
                                     std::vector<Pair<Integer, Integer>>& nextRelationships)
{
    addNextRelationships(currentNode->getStatementNumber(), currentNode->getStatementType(),
                         nextNode->getStatementNumber(), nextNode->getStatementType());

    // For testing
    Pair<Integer, Integer> nextRelationship(currentNode->getStatementNumber(), nextNode->getStatementNumber());
    nextRelationships.push_back(nextRelationship);
}

NextExtractor::NextExtractor(const CfgNode* procedureNode, size_t numberOfNodes):
    node(procedureNode), nextRelationships(), visitedArray()
{
    // Initialise the visitedArray to false
    for (size_t i = 0; i < numberOfNodes + 1; i++) {
        visitedArray.push_back(false);
    }
}

Void NextExtractor::extractNextFromNode(const CfgNode* cfgNode, StatementNode* prevStmtNode)
{
    Boolean nodeIsVisited = visitedArray.at(cfgNode->nodeNumber);
    Vector<StatementNode*>* stmtList = cfgNode->statementNodes;
    Vector<CfgNode*>* childrenList = cfgNode->childrenNodes;

    // Dummy node with no statement nodes
    if (stmtList->empty() && childrenList->size() == 1) {
        if (!childrenList->at(0)->statementNodes->empty()) {
            addNextRelationshipBetweenNodes(prevStmtNode, childrenList->at(0)->statementNodes->at(0),
                                            nextRelationships);
        } else {
            while (childrenList->at(0)->statementNodes->empty()) {
                if (!childrenList->at(0)->childrenNodes->empty()) {
                    childrenList = childrenList->at(0)->childrenNodes;
                } else {
                    break;
                }
            }
            Vector<StatementNode*>* currentChildStatementList = childrenList->at(0)->statementNodes;
            if (!currentChildStatementList->empty()) {
                addNextRelationshipBetweenNodes(prevStmtNode, currentChildStatementList->at(0), nextRelationships);
            }
        }
    }

    // If the statement list is empty, we mark the current CFG as visited and terminate early
    if (stmtList->empty()) {
        visitedArray.at(cfgNode->nodeNumber) = true;
        return;
    }

    // If the previous statement node is a nullptr, the current statement is the first statement
    // We add a Next relationship between the previous statement node, if any, and the first
    // statement of the current CFG node (visited or not)
    if (prevStmtNode != nullptr && !stmtList->empty()) {
        StatementNode* firstNode = stmtList->at(0);

        addNextRelationshipBetweenNodes(prevStmtNode, firstNode, nextRelationships);
    }

    // We extract the Next relationships from consecutive statement nodes in the current CFG node'
    // Early termination if this CFG node has been visited
    if (nodeIsVisited) {
        return;
    } else {
        visitedArray.at(cfgNode->nodeNumber) = true;

        // We run for stmtList.size() - 1 because we do not consider the last node for
        // Next relationships within a node
        for (size_t i = 0; i < stmtList->size() - 1; i++) {
            StatementNode* prevStmtNodeI = stmtList->at(i);
            StatementNode* currentStmtNode = stmtList->at(i + 1);

            addNextRelationshipBetweenNodes(prevStmtNodeI, currentStmtNode, nextRelationships);
        }

        // We recurse with the last statement in the current CFG node and the current CFG node's children
        StatementNode* lastNode = stmtList->back();

        for (auto& child : *childrenList) {
            extractNextFromNode(child, lastNode);
        }
    }
}

Vector<Pair<Integer, Integer>> NextExtractor::extractNext()
{
    // Start by extracting the Next relationship with th root node
    extractNextFromNode(node, nullptr);

    // Solely for testing purposes
    return nextRelationships;
}

std::vector<Pair<Integer, Integer>> extractNext(std::pair<CfgNode*, size_t> cfgInfo)
{
    NextExtractor extractor(cfgInfo.first, cfgInfo.second);
    return extractor.extractNext();
}
