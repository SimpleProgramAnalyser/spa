/**
 * Implementation of Control Flow Graph
 * classes and methods.
 */

#include "CfgTypes.h"

#include "Util.h"

CfgNode::CfgNode(List<StatementNode>* statements, List<CfgNode>* children, size_t cfgNodeNumber):
    statementNodes(statements), childrenNodes(children), nodeNumber(cfgNodeNumber)
{}


CfgNode::~CfgNode()
{
    delete statementNodes;
    delete childrenNodes;
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
Boolean nodesAreEqual(CfgNode* node1, CfgNode* node2, Vector<Boolean>* visitedArray) {
    Boolean isEqual = true;
    Boolean nodeIsVisited = visitedArray->at(node1->nodeNumber);
    if (nodeIsVisited) {
        return true;
    } else {
        visitedArray->at(node1->nodeNumber) = true;
        // Check that the values in the StatementNode lists are equal
        isEqual = util::checkListValuesEqual<StatementNode>(*(node1->statementNodes), *(node2->statementNodes));
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

        //Check childrens' node numbers are the same
        for (size_t j = 0; j < node1->childrenNodes->size(); j++) {
            size_t currentNodeNumber = node1->childrenNodes->at(j).get()->nodeNumber;
            isEqual = currentNodeNumber == node2->childrenNodes->at(j).get()->nodeNumber;
            // Terminate early if it is not equal
            if (!isEqual) {
                return false;
            }
        }

        // Check if the children nodes are equal
        for (size_t k = 0; k < node1->childrenNodes->size(); k++) {
            isEqual = nodesAreEqual(node1->childrenNodes->at(k).get(), node2->childrenNodes->at(k).get(), visitedArray);
            // Terminate early if it is not equal
            if (!isEqual) {
                return false;
            }
        }
    }
    return isEqual;
}

/**
* Checks the equality of 2 Cfg nodes
* 
* @param cfgn CFG node for comprison
* @param numberOfNodes The total number of nodes in the CFG
* @return A boolean indicating if the CFG is equal
*/
Boolean CfgNode::equals(CfgNode* cfgn, size_t numberOfNodes)
{
    Vector<Boolean> visitedArray;
    // Initialise the visitedArray to false
    for (size_t i = 0; i < numberOfNodes + 1; i++) {
        visitedArray.push_back(false);
    }
    Boolean isEqual = nodesAreEqual(this, cfgn, &visitedArray);
    return isEqual;
}