/**
 * Implementation of Control Flow Graph Builder.
 */

#include "CfgBuilder.h"

/**
* Creates a CFG node.
* 
* @param stmtListSize The size of the current statement list 
* @param currentNumberOfNodes The current number of nodes in the CFG
* @return The pointer to the CFGNode created
*/
CfgNode* createCfgNode(size_t stmtListSize, size_t &currentNumberOfNodes)
{
    List<StatementNode>* statements = new List<StatementNode>();
    List<CfgNode>* children = new List<CfgNode>();

    // Reserve enough space for the statements in the node
    // At least the size of the statement list, in case the whole
    // statement list belongs to this node
    statements->reserve(stmtListSize);
    // Reserve 2 spaces for a node's children as a node has 2 children or less 
    children->reserve(2);
    currentNumberOfNodes++;

    return new CfgNode(statements, children, currentNumberOfNodes);
}

/**
* Checks is a CfgIsEmpty (i.e. does not have statements or children)
* 
* @param node A CfgNode
* @return A boolean to indicate if the CfgNode is empty
*/
Boolean cfgNodeIsEmpty(CfgNode* node) {
    return node->childrenNodes->size() == 0 && node->statementNodes->size() == 0;
}

/**
* Builds on the current CFG, given a StatementNode.
*
* @param statementNodePtr Pointer to the StatementNode
* @param currentCfgNode Pointer to the CfgNode that we will continue building on
* @param currentNumberOfNodes The current number of nodes in the CFG 
* @param wholeStmtListSize The size of the statement list of the current procedure
* @return Pointer to the CfgNode that is the next to be built on
*/
CfgNode* buildCfgWithStatementNode(StatementNode* statementNodePtr, CfgNode* currentCfgNode, size_t &currentNumberOfNodes, size_t wholeStmtListSize)
{   
    StatementType stmtType = statementNodePtr->getStatementType();
    switch (stmtType) {
    case AssignmentStatement:
    case ReadStatement:
    case PrintStatement:
    case CallStatement:
    {
        (*currentCfgNode->statementNodes).push_back(std::unique_ptr<StatementNode> (statementNodePtr));
        return currentCfgNode;
        break;
    }
    case WhileStatement: 
    {
        WhileStatementNode* whileStmt = dynamic_cast<WhileStatementNode*>(statementNodePtr);
        const StmtlstNode& whileStmtListNode = *(whileStmt->statementList);
        const List<StatementNode>& stmtList = whileStmtListNode.statementList;
        size_t stmtListSize = stmtList.size();

        CfgNode* whileNewNode = createCfgNode(1, currentNumberOfNodes);
        CfgNode* whileNode = createCfgNode(stmtListSize, currentNumberOfNodes);

        (*currentCfgNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(whileNewNode));
        (*whileNewNode->statementNodes).push_back(std::unique_ptr<StatementNode> (statementNodePtr));
        (*whileNewNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(whileNode));

        // Loop through the statement list of the While Statement
        currentCfgNode = whileNode;
        for (size_t l = 0; l < stmtListSize; l++) {
            StatementNode* stmtNode = stmtList.at(l).get();
            // Recurse
            currentCfgNode = buildCfgWithStatementNode(stmtNode, currentCfgNode, currentNumberOfNodes, stmtListSize);
        }
        (*currentCfgNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(whileNewNode));

        // To complete the while loop shape of the CFG
        CfgNode* whileDummyNode = createCfgNode(wholeStmtListSize, currentNumberOfNodes);
        (*whileNewNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(whileDummyNode));
        currentCfgNode = whileDummyNode;
        return currentCfgNode;
        break;
    }
    case IfStatement: {
        IfStatementNode* ifStmt = dynamic_cast<IfStatementNode*>(statementNodePtr);
        const StmtlstNode& ifStmtListNode = *(ifStmt->ifStatementList);
        const StmtlstNode& elseStmtListNode = *(ifStmt->elseStatementList);
        const List<StatementNode>& ifStmtList = ifStmtListNode.statementList;
        const List<StatementNode>& elseStmtList = elseStmtListNode.statementList;

        size_t ifStmtListSize = ifStmtList.size();
        size_t elseStmtListSize = elseStmtList.size();

        Boolean currentNodeIsEmpty = cfgNodeIsEmpty(currentCfgNode);
        CfgNode* ifNewNode;
        if (!currentNodeIsEmpty) {
            ifNewNode = createCfgNode(1, currentNumberOfNodes);
            (*currentCfgNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(ifNewNode));
        } else {
            ifNewNode = currentCfgNode;
        }
        CfgNode* ifNode = createCfgNode(ifStmtListSize, currentNumberOfNodes);
        CfgNode* elseNode = createCfgNode(elseStmtListSize, currentNumberOfNodes);

        (*ifNewNode->statementNodes).push_back(std::unique_ptr<StatementNode>(statementNodePtr));
        (*ifNewNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(ifNode));
        (*ifNewNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(elseNode));

        // Loop through the statement lists (if & else ) of the If Statement
        currentCfgNode = ifNode;
        for (size_t m = 0; m < ifStmtListSize; m++) {
            StatementNode* stmtNode = ifStmtList.at(m).get();
            // Recurse
            currentCfgNode = buildCfgWithStatementNode(stmtNode, currentCfgNode, currentNumberOfNodes, ifStmtListSize);
        }

        currentCfgNode = elseNode;
        for (size_t n = 0; n < elseStmtListSize; n++) {
            StatementNode* stmtNode = elseStmtList.at(n).get();
            // Recurse
            currentCfgNode = buildCfgWithStatementNode(stmtNode, currentCfgNode, currentNumberOfNodes , elseStmtListSize);
        }

        // To complete the if-else diamond shape of the CFG
        CfgNode* ifDummyNode = createCfgNode(wholeStmtListSize, currentNumberOfNodes);
        (*ifNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(ifDummyNode));
        (*elseNode->childrenNodes).push_back(std::unique_ptr<CfgNode>(ifDummyNode));
        currentCfgNode = ifDummyNode;
        return currentCfgNode;
        break;
    }
    default:
        // Will not enter here
        return currentCfgNode;
        break;
    }
}

/**
* Builds the CFG of a given program.
* 
* @param stmtListNode Statement List Node of a procedure
* @return Pointer to the root CfgNode so that we can access the whole CFG
*/
CfgNode* buildCfg(const StmtlstNode* const stmtListNode)
{
    // We want the Cfg node number to start from 0 
    size_t currentNumber = -1;
    size_t stmtListSize = stmtListNode->statementList.size();
    CfgNode* cfgRootNode = createCfgNode(stmtListSize, currentNumber);
    CfgNode* currentCfgNode = cfgRootNode;

    // Go through statement nodes in the statement list
    for (size_t j = 0; j < stmtListSize; j++) 
    {
        StatementNode* stmtNode = (stmtListNode->statementList).at(j).get();
        currentCfgNode = buildCfgWithStatementNode(stmtNode, currentCfgNode, currentNumber, stmtListSize);
    }
    return cfgRootNode;
}

