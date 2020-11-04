/**
 * Implementation of Control Flow Graph Types
 * classes and methods.
 */

#include "CfgTypes.h"

#include <cassert>
#include <queue>
#include <unordered_set>

#include "Util.h"

CfgNode::CfgNode(Vector<StatementNode*>* statements, Vector<CfgNode*>* children, size_t cfgNodeNumber,
                 CfgNode* joinNode):
    statementNodes(statements),
    childrenNodes(children), nodeNumber(cfgNodeNumber), ifJoinNode(joinNode)
{}

CfgNode::~CfgNode()
{
    delete statementNodes;
    delete childrenNodes;
    delete ifJoinNode;
}

/**
 * Helper method for equals to check if the statement ndoes and children
 * nodes in 2 CFG node are equal.
 *
 * @param node1 Root node of CFG 1
 * @param node2 Root node of CFG 2
 * @param visitedArray A vector to indicate whethera node has been visited
 * @return A boolean indicating if the 2 nodes are equal
 */
Boolean nodesAreEqual(CfgNode* node1, CfgNode* node2, Vector<Boolean>* visitedArray)
{
    Boolean isEqual = true;
    Boolean nodeIsVisited = visitedArray->at(node1->nodeNumber);
    if (nodeIsVisited) {
        return true;
    } else {
        visitedArray->at(node1->nodeNumber) = true;
        // Check that the values in the StatementNode lists are equal
        isEqual = util::checkVectorOfPointersEqual<StatementNode*>(*(node1->statementNodes), *(node2->statementNodes));
        // Terminate early if it is not equal
        if (!isEqual) {
            return false;
        }

        // Check that children size is equal
        isEqual = node1->childrenNodes->size() == node2->childrenNodes->size();
        // Terminate early if it is not equal
        if (!isEqual) {
            return false;
        }

        // Check childrens' node numbers are the same
        for (size_t j = 0; j < node1->childrenNodes->size(); j++) {
            size_t currentNodeNumber = node1->childrenNodes->at(j)->nodeNumber;
            isEqual = currentNodeNumber == node2->childrenNodes->at(j)->nodeNumber;
            // Terminate early if it is not equal
            if (!isEqual) {
                return false;
            }
        }

        // Check if the children nodes are equal
        for (size_t k = 0; k < node1->childrenNodes->size(); k++) {
            isEqual = nodesAreEqual(node1->childrenNodes->at(k), node2->childrenNodes->at(k), visitedArray);
            // Terminate early if it is not equal
            if (!isEqual) {
                return false;
            }
        }
    }
    return isEqual;
}

Vector<CfgNode*> CfgNode::findAllChildren() const
{
    // breadth-first search to get all nodes
    std::unordered_set<CfgNode*> visitedNodes;
    std::queue<CfgNode*> queue;
    for (CfgNode* child : *childrenNodes) {
        assert(child != nullptr); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        queue.push(child);
    }
    while (!queue.empty()) {
        CfgNode* current = queue.front();
        if (visitedNodes.find(current) == visitedNodes.end()) {
            visitedNodes.insert(current);
            for (CfgNode* childOfCurrent : *(current->childrenNodes)) {
                assert(childOfCurrent != nullptr); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
                queue.push(childOfCurrent);
            }
        }
        queue.pop();
    }
    // return all visited nodes
    return Vector<CfgNode*>(visitedNodes.begin(), visitedNodes.end());
}

/**
 * Checks the equality of 2 Cfg nodes
 *
 * @param cfgn CFG node for comprison
 * @param numberOfNodes The total number of nodes in the CFG
 * @return Boolean of whehter the current CFG node is equals to cfgn
 */
Boolean CfgNode::equals(CfgNode* cfgn, size_t numberOfNodes)
{
    bool isEqual = true;
    Vector<Boolean> visitedArray;
    // Initialise visitedArray to false
    for (size_t i = 0; i < numberOfNodes + 1; i++) {
        visitedArray.push_back(false);
    }
    isEqual = nodesAreEqual(this, cfgn, &visitedArray);
    return isEqual;
}

size_t CfgNode::size() const
{
    return findAllChildren().size();
}
