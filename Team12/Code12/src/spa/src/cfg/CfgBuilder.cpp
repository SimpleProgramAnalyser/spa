/**
 * Implementation of Control Flow Graph Builder.
 */

#include "CfgBuilder.h"

#include "pkb/PKB.h"

/**
 * Creates a CFG node.
 *
 * @param stmtListSize The size of the current statement list
 * @param currentNumberOfNodes The current number of nodes in the CFG
 * @return The pointer to the CFGNode created
 */
CfgNode* createCfgNode(size_t stmtListSize, size_t& currentNumberOfNodes)
{
    auto* statements = new Vector<StatementNode*>();
    auto* children = new Vector<CfgNode*>();
    CfgNode* ifJoinNode = nullptr;

    // Reserve enough space for the statements in the node
    // At least the size of the statement list, in case the whole
    // statement list belongs to this node
    statements->reserve(stmtListSize);
    // Reserve 2 spaces for a node's children as a node has 2 children or less
    children->reserve(2);
    currentNumberOfNodes++;

    return new CfgNode(statements, children, currentNumberOfNodes, ifJoinNode);
}

/**
 * Checks is a Cfg node is empty (i.e. does not have statements or children)
 *
 * @param node A CfgNode
 * @return A boolean to indicate if the CfgNode is empty
 */
Boolean cfgNodeIsEmpty(CfgNode* node)
{
    return node->childrenNodes->empty() && node->statementNodes->empty();
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
CfgNode* buildCfgWithStatementNode(StatementNode* statementNodePtr, CfgNode* currentCfgNode,
                                   size_t& currentNumberOfNodes, size_t wholeStmtListSize)
{
    StatementType stmtType = statementNodePtr->getStatementType();
    switch (stmtType) {
    case AssignmentStatement:
    case ReadStatement:
    case PrintStatement:
    case CallStatement: {
        (*currentCfgNode->statementNodes).push_back(statementNodePtr);
        return currentCfgNode;
    }
    case WhileStatement: {
        auto* whileStmt = dynamic_cast<WhileStatementNode*>(statementNodePtr);
        const StmtlstNode& whileStmtListNode = *(whileStmt->statementList);
        const List<StatementNode>& stmtList = whileStmtListNode.statementList;
        size_t stmtListSize = stmtList.size();

        CfgNode* whileNewNode;
        Boolean currentNodeIsEmpty = cfgNodeIsEmpty(currentCfgNode);
        if (!currentNodeIsEmpty) {
            whileNewNode = createCfgNode(1, currentNumberOfNodes);
            (*currentCfgNode->childrenNodes).push_back(whileNewNode);

        } else {
            whileNewNode = currentCfgNode;
        }

        CfgNode* whileNode = createCfgNode(stmtListSize, currentNumberOfNodes);
        (*whileNewNode->statementNodes).push_back(statementNodePtr);
        (*whileNewNode->childrenNodes).push_back(whileNode);

        // Loop through the statement list of the While Statement
        currentCfgNode = whileNode;
        for (size_t l = 0; l < stmtListSize; l++) {
            StatementNode* stmtNode = stmtList.at(l).get();
            // Recurse
            currentCfgNode = buildCfgWithStatementNode(stmtNode, currentCfgNode, currentNumberOfNodes, stmtListSize);
        }
        (*currentCfgNode->childrenNodes).push_back(whileNewNode);

        // To complete the while loop shape of the CFG
        CfgNode* whileDummyNode = createCfgNode(wholeStmtListSize, currentNumberOfNodes);
        (*whileNewNode->childrenNodes).push_back(whileDummyNode);
        currentCfgNode = whileDummyNode;
        return currentCfgNode;
    }
    case IfStatement: {
        auto* ifStmt = dynamic_cast<IfStatementNode*>(statementNodePtr);
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
            (*currentCfgNode->childrenNodes).push_back(ifNewNode);
        } else {
            ifNewNode = currentCfgNode;
        }
        CfgNode* ifNode = createCfgNode(ifStmtListSize, currentNumberOfNodes);
        CfgNode* elseNode = createCfgNode(elseStmtListSize, currentNumberOfNodes);

        (*ifNewNode->statementNodes).push_back(statementNodePtr);
        (*ifNewNode->childrenNodes).push_back(ifNode);
        (*ifNewNode->childrenNodes).push_back(elseNode);

        // Loop through the statement lists (if & else ) of the If Statement
        for (size_t m = 0; m < ifStmtListSize; m++) {
            StatementNode* stmtNode = ifStmtList.at(m).get();
            // Recurse
            ifNode = buildCfgWithStatementNode(stmtNode, ifNode, currentNumberOfNodes, ifStmtListSize);
        }

        for (size_t n = 0; n < elseStmtListSize; n++) {
            StatementNode* stmtNode = elseStmtList.at(n).get();
            // Recurse
            elseNode = buildCfgWithStatementNode(stmtNode, elseNode, currentNumberOfNodes, elseStmtListSize);
        }

        // To complete the if-else diamond shape of the CFG
        CfgNode* ifDummyNode = createCfgNode(wholeStmtListSize, currentNumberOfNodes);
        (*ifNode->childrenNodes).push_back(ifDummyNode);
        (*elseNode->childrenNodes).push_back(ifDummyNode);
        // Connect the fork node to the join node for Affects Evaluator
        ifNewNode->ifJoinNode = ifDummyNode;
        return ifDummyNode;
    }
    default:
        // Will not enter here
        return currentCfgNode;
    }
}

/**
 * Builds the CFG of a given program.
 *
 * @param stmtListNode Statement List Node of a procedure
 * @return Pointer to the root CfgNode so that we can access the whole CFG
 */
Pair<CfgNode*, size_t> buildCfg(const StmtlstNode* const stmtListNode)
{
    // We want the Cfg node number to start from 0
    size_t currentNumber = -1;
    size_t stmtListSize = stmtListNode->statementList.size();
    CfgNode* cfgRootNode = createCfgNode(stmtListSize, currentNumber);
    CfgNode* currentCfgNode = cfgRootNode;

    // Go through statement nodes in the statement list
    for (size_t j = 0; j < stmtListSize; j++) {
        StatementNode* stmtNode = (stmtListNode->statementList).at(j).get();
        currentCfgNode = buildCfgWithStatementNode(stmtNode, currentCfgNode, currentNumber, stmtListSize);
    }
    return {cfgRootNode, currentNumber};
}
