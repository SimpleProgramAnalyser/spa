/**
 * Implementation of a method to generate Control Flow Graph
 * with branching into procedures for program #28 (raw
 * program can be found in AstUtils).
 *
 * This program has an especially large CFG due to large
 * cyclomatic complexity from many nested if/else and
 * while statements, leading to high branching.
 */

#include "CfgUtils.h"
#include "ast/AstLibrary.h"

std::pair<CfgNode*, size_t> getProgram28CfgBip_combineIfInIfAndNested3AndNested2()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes); // NODE 0

    // procedure ifInIf
    List<StatementNode> ifInIf1IfStatements;
    List<StatementNode> ifInIf1ElseStatements;

    List<StatementNode> ifInIf2IfStatements;
    List<StatementNode> ifInIf2ElseStatements;

    ifInIf2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("stmt"), createRefExpr(4))));
    ifInIf2IfStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(6, "nested2")));

    ifInIf2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(7, Variable("stmt"), createRefExpr(5))));

    ifInIf1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));
    ifInIf1IfStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(3, "nested3")));

    ifInIf1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(ifInIf2IfStatements),
                     createStmtlstNode(ifInIf2ElseStatements))));

    // Create new nodes for first If statement
    CfgNode* ifCfgNode1 = createCfgNode(1, currentNumberOfNodes);   // NODE 1
    CfgNode* newCallNode1 = createCfgNode(1, currentNumberOfNodes); // NODE 2

    expectedCfg->statementNodes->push_back(createIfNode(1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)),
                                                        createStmtlstNode(ifInIf1IfStatements),
                                                        createStmtlstNode(ifInIf1ElseStatements)));
    expectedCfg->childrenNodes->push_back(ifCfgNode1);
    ifCfgNode1->statementNodes->push_back(createAssignNode(2, Variable("stmt"), createRefExpr(2)));
    ifCfgNode1->childrenNodes->push_back(newCallNode1);
    newCallNode1->statementNodes->push_back(createCallNode(3, "nested3"));

    CfgNode* newCallNodeForNested3 = createCfgNode(1, currentNumberOfNodes); // NODE 3
    newCallNode1->childrenNodes->push_back(newCallNodeForNested3);

    // procedure nested 3
    List<StatementNode> nested3WhileStatements;

    List<StatementNode> nested31IfStatements;
    List<StatementNode> nested31ElseStatements;

    List<StatementNode> nested32IfStatements;
    List<StatementNode> nested32ElseStatements;
    List<StatementNode> nested33IfStatements;
    List<StatementNode> nested33ElseStatements;

    List<StatementNode> nested34IfStatements;
    List<StatementNode> nested34ElseStatements;

    List<StatementNode> nested35IfStatements;
    List<StatementNode> nested35ElseStatements;

    List<StatementNode> nested36IfStatements;
    List<StatementNode> nested36ElseStatements;

    List<StatementNode> nested37IfStatements;
    List<StatementNode> nested37ElseStatements;

    List<StatementNode> nested38IfStatements;
    List<StatementNode> nested38ElseStatements;

    List<StatementNode> nested39IfStatements;
    List<StatementNode> nested39ElseStatements;

    List<StatementNode> nested310IfStatements;
    List<StatementNode> nested310ElseStatements;

    nested31ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(29, Variable("stmt"), createRefExpr(22))));
    nested32ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(28, Variable("stmt"), createRefExpr(21))));
    nested33ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(27, Variable("stmt"), createRefExpr(20))));
    nested34ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(26, Variable("stmt"), createRefExpr(19))));
    nested35ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(25, Variable("stmt"), createRefExpr(18))));
    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));
    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));
    nested35IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested36IfStatements),
                     createStmtlstNode(nested36ElseStatements))));
    nested34IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(nested35IfStatements),
                     createStmtlstNode(nested35ElseStatements))));
    nested33IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(12, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested34IfStatements),
                     createStmtlstNode(nested34ElseStatements))));
    nested32IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(nested33IfStatements),
                     createStmtlstNode(nested33ElseStatements))));
    nested31IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(nested32IfStatements),
                     createStmtlstNode(nested32ElseStatements))));
    nested3WhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(2)), createStmtlstNode(nested31IfStatements),
                     createStmtlstNode(nested31ElseStatements))));

    // Create new nodes for While Statement
    CfgNode* nested3WhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes); // NODE 4
    newCallNodeForNested3->childrenNodes->push_back(nested3WhileCfgChildrenNode);
    newCallNodeForNested3->statementNodes->push_back(createWhileNode(
        8, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(nested3WhileStatements)));

    // Create new nodes for If Statements
    CfgNode* ifCfgNode1Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 5
    CfgNode* ifCfgNode2Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 6
    CfgNode* ifCfgNode3Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 7
    CfgNode* ifCfgNode4Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 8
    CfgNode* ifCfgNode5Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 9
    CfgNode* ifCfgNode6Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 10
    CfgNode* ifCfgNode7Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 11
    CfgNode* ifCfgNode8Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 12
    CfgNode* ifCfgNode9Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 13
    CfgNode* ifCfgNode10Nested3 = createCfgNode(1, currentNumberOfNodes); // NODE 14

    CfgNode* ifDummyNode10Nested3 = createCfgNode(0, currentNumberOfNodes); // NODE 15
    CfgNode* ifDummyNode9Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 16
    CfgNode* ifDummyNode8Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 17
    CfgNode* ifDummyNode7Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 18
    CfgNode* ifDummyNode6Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 19
    CfgNode* ifDummyNode5Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 20
    CfgNode* ifDummyNode4Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 21
    CfgNode* ifDummyNode3Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 22
    CfgNode* ifDummyNode2Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 23
    CfgNode* ifDummyNode1Nested3 = createCfgNode(0, currentNumberOfNodes);  // NODE 24

    CfgNode* elseCfgNode10Nested3 = createCfgNode(1, currentNumberOfNodes); // NODE 25
    CfgNode* elseCfgNode9Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 26
    CfgNode* elseCfgNode8Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 27
    CfgNode* elseCfgNode7Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 28
    CfgNode* elseCfgNode6Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 29
    CfgNode* elseCfgNode5Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 30
    CfgNode* elseCfgNode4Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 31
    CfgNode* elseCfgNode3Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 32
    CfgNode* elseCfgNode2Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 33
    CfgNode* elseCfgNode1Nested3 = createCfgNode(1, currentNumberOfNodes);  // NODE 34

    CfgNode* nested3WhileCfgDummyNode = createCfgNode(1, currentNumberOfNodes); // NODE 35
    CfgNode* nested3NewCallNode = createCfgNode(1, currentNumberOfNodes);       // NODE 36

    // First If
    // Duplicates cause unique pointers
    nested31ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(29, Variable("stmt"), createRefExpr(22))));
    nested32ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(28, Variable("stmt"), createRefExpr(21))));
    nested33ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(27, Variable("stmt"), createRefExpr(20))));
    nested34ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(26, Variable("stmt"), createRefExpr(19))));
    nested35ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(25, Variable("stmt"), createRefExpr(18))));
    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));
    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));
    nested35IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested36IfStatements),
                     createStmtlstNode(nested36ElseStatements))));
    nested34IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(nested35IfStatements),
                     createStmtlstNode(nested35ElseStatements))));
    nested33IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(12, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested34IfStatements),
                     createStmtlstNode(nested34ElseStatements))));
    nested32IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(nested33IfStatements),
                     createStmtlstNode(nested33ElseStatements))));
    nested31IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(nested32IfStatements),
                     createStmtlstNode(nested32ElseStatements))));

    nested3WhileCfgChildrenNode->statementNodes->push_back(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(2)), createStmtlstNode(nested31IfStatements),
                     createStmtlstNode(nested31ElseStatements)));
    nested3WhileCfgChildrenNode->childrenNodes->push_back(ifCfgNode1Nested3);
    nested3WhileCfgChildrenNode->childrenNodes->push_back(elseCfgNode1Nested3);
    elseCfgNode1Nested3->statementNodes->push_back(createAssignNode(29, Variable("stmt"), createRefExpr(22)));
    elseCfgNode1Nested3->childrenNodes->push_back(ifDummyNode1Nested3);
    ifDummyNode1Nested3->childrenNodes->push_back(newCallNodeForNested3);

    // Second If
    // Duplicates cause unique pointers
    nested32ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(28, Variable("stmt"), createRefExpr(21))));
    nested33ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(27, Variable("stmt"), createRefExpr(20))));
    nested34ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(26, Variable("stmt"), createRefExpr(19))));
    nested35ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(25, Variable("stmt"), createRefExpr(18))));
    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));
    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));
    nested35IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested36IfStatements),
                     createStmtlstNode(nested36ElseStatements))));
    nested34IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(nested35IfStatements),
                     createStmtlstNode(nested35ElseStatements))));
    nested33IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(12, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested34IfStatements),
                     createStmtlstNode(nested34ElseStatements))));
    nested32IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(nested33IfStatements),
                     createStmtlstNode(nested33ElseStatements))));

    ifCfgNode1Nested3->statementNodes->push_back(createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                                                              createStmtlstNode(nested32IfStatements),
                                                              createStmtlstNode(nested32ElseStatements)));
    ifCfgNode1Nested3->childrenNodes->push_back(ifCfgNode2Nested3);
    ifCfgNode1Nested3->childrenNodes->push_back(elseCfgNode2Nested3);
    elseCfgNode2Nested3->statementNodes->push_back(createAssignNode(28, Variable("stmt"), createRefExpr(21)));
    elseCfgNode2Nested3->childrenNodes->push_back(ifDummyNode2Nested3);
    ifDummyNode2Nested3->childrenNodes->push_back(ifDummyNode1Nested3);

    // Third If
    // Duplicates cause unique pointers
    nested33ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(27, Variable("stmt"), createRefExpr(20))));
    nested34ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(26, Variable("stmt"), createRefExpr(19))));
    nested35ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(25, Variable("stmt"), createRefExpr(18))));
    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));
    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));
    nested35IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested36IfStatements),
                     createStmtlstNode(nested36ElseStatements))));
    nested34IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(nested35IfStatements),
                     createStmtlstNode(nested35ElseStatements))));
    nested33IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(12, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested34IfStatements),
                     createStmtlstNode(nested34ElseStatements))));

    ifCfgNode2Nested3->statementNodes->push_back(createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(4)),
                                                              createStmtlstNode(nested33IfStatements),
                                                              createStmtlstNode(nested33ElseStatements)));
    ifCfgNode2Nested3->childrenNodes->push_back(ifCfgNode3Nested3);
    ifCfgNode2Nested3->childrenNodes->push_back(elseCfgNode3Nested3);
    elseCfgNode3Nested3->statementNodes->push_back(createAssignNode(27, Variable("stmt"), createRefExpr(20)));
    elseCfgNode3Nested3->childrenNodes->push_back(ifDummyNode3Nested3);
    ifDummyNode3Nested3->childrenNodes->push_back(ifDummyNode2Nested3);

    // Forth If
    // Duplicates cause unique pointers
    nested34ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(26, Variable("stmt"), createRefExpr(19))));
    nested35ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(25, Variable("stmt"), createRefExpr(18))));
    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));
    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));
    nested35IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested36IfStatements),
                     createStmtlstNode(nested36ElseStatements))));
    nested34IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(nested35IfStatements),
                     createStmtlstNode(nested35ElseStatements))));

    ifCfgNode3Nested3->statementNodes->push_back(createIfNode(12, createEqExpr(createRefExpr("stmt"), createRefExpr(5)),
                                                              createStmtlstNode(nested34IfStatements),
                                                              createStmtlstNode(nested34ElseStatements)));
    ifCfgNode3Nested3->childrenNodes->push_back(ifCfgNode4Nested3);
    ifCfgNode3Nested3->childrenNodes->push_back(elseCfgNode4Nested3);
    elseCfgNode4Nested3->statementNodes->push_back(createAssignNode(26, Variable("stmt"), createRefExpr(19)));
    elseCfgNode4Nested3->childrenNodes->push_back(ifDummyNode4Nested3);
    ifDummyNode4Nested3->childrenNodes->push_back(ifDummyNode3Nested3);

    // Fifth If
    // Duplicates cause unique pointers
    nested35ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(25, Variable("stmt"), createRefExpr(18))));
    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));
    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));
    nested35IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested36IfStatements),
                     createStmtlstNode(nested36ElseStatements))));

    ifCfgNode4Nested3->statementNodes->push_back(createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(6)),
                                                              createStmtlstNode(nested35IfStatements),
                                                              createStmtlstNode(nested35ElseStatements)));
    ifCfgNode4Nested3->childrenNodes->push_back(ifCfgNode5Nested3);
    ifCfgNode4Nested3->childrenNodes->push_back(elseCfgNode5Nested3);
    elseCfgNode5Nested3->statementNodes->push_back(createAssignNode(25, Variable("stmt"), createRefExpr(18)));
    elseCfgNode5Nested3->childrenNodes->push_back(ifDummyNode5Nested3);
    ifDummyNode5Nested3->childrenNodes->push_back(ifDummyNode4Nested3);

    // Sixth If
    // Duplicates cause unique pointers
    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));
    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));

    ifCfgNode5Nested3->statementNodes->push_back(createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                                                              createStmtlstNode(nested36IfStatements),
                                                              createStmtlstNode(nested36ElseStatements)));
    ifCfgNode5Nested3->childrenNodes->push_back(ifCfgNode6Nested3);
    ifCfgNode5Nested3->childrenNodes->push_back(elseCfgNode6Nested3);
    elseCfgNode6Nested3->statementNodes->push_back(createAssignNode(24, Variable("stmt"), createRefExpr(17)));
    elseCfgNode6Nested3->childrenNodes->push_back(ifDummyNode6Nested3);
    ifDummyNode6Nested3->childrenNodes->push_back(ifDummyNode5Nested3);

    // Seventh If
    // Duplicates cause unique pointers
    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));
    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));

    ifCfgNode6Nested3->statementNodes->push_back(createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)),
                                                              createStmtlstNode(nested37IfStatements),
                                                              createStmtlstNode(nested37ElseStatements)));
    ifCfgNode6Nested3->childrenNodes->push_back(ifCfgNode7Nested3);
    ifCfgNode6Nested3->childrenNodes->push_back(elseCfgNode7Nested3);
    elseCfgNode7Nested3->statementNodes->push_back(createAssignNode(23, Variable("stmt"), createRefExpr(16)));
    elseCfgNode7Nested3->childrenNodes->push_back(ifDummyNode7Nested3);
    ifDummyNode7Nested3->childrenNodes->push_back(ifDummyNode6Nested3);

    // Eighth If
    // Duplicates cause unique pointers
    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));
    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));

    ifCfgNode7Nested3->statementNodes->push_back(createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                                                              createStmtlstNode(nested38IfStatements),
                                                              createStmtlstNode(nested38ElseStatements)));
    ifCfgNode7Nested3->childrenNodes->push_back(ifCfgNode8Nested3);
    ifCfgNode7Nested3->childrenNodes->push_back(elseCfgNode8Nested3);
    elseCfgNode8Nested3->statementNodes->push_back(createAssignNode(22, Variable("stmt"), createRefExpr(15)));
    elseCfgNode8Nested3->childrenNodes->push_back(ifDummyNode8Nested3);
    ifDummyNode8Nested3->childrenNodes->push_back(ifDummyNode7Nested3);

    // Ninth If
    // Duplicates cause unique pointers
    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));
    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));

    ifCfgNode8Nested3->statementNodes->push_back(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements)));
    ifCfgNode8Nested3->childrenNodes->push_back(ifCfgNode9Nested3);
    ifCfgNode8Nested3->childrenNodes->push_back(elseCfgNode9Nested3);
    elseCfgNode9Nested3->statementNodes->push_back(createAssignNode(21, Variable("stmt"), createRefExpr(14)));
    elseCfgNode9Nested3->childrenNodes->push_back(ifDummyNode9Nested3);
    ifDummyNode9Nested3->childrenNodes->push_back(ifDummyNode8Nested3);

    // Tenth If
    // Duplicates cause unique pointers
    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));
    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));

    ifCfgNode9Nested3->statementNodes->push_back(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements)));
    ifCfgNode9Nested3->childrenNodes->push_back(ifCfgNode10Nested3);
    ifCfgNode9Nested3->childrenNodes->push_back(elseCfgNode10Nested3);
    elseCfgNode10Nested3->statementNodes->push_back(createAssignNode(20, Variable("stmt"), createRefExpr(13)));
    elseCfgNode10Nested3->childrenNodes->push_back(ifDummyNode10Nested3);
    ifDummyNode10Nested3->childrenNodes->push_back(ifDummyNode9Nested3);

    ifCfgNode10Nested3->statementNodes->push_back(createAssignNode(19, Variable("stmt"), createRefExpr(12)));
    ifCfgNode10Nested3->childrenNodes->push_back(ifDummyNode10Nested3);

    // While dummy
    newCallNodeForNested3->childrenNodes->push_back(nested3WhileCfgDummyNode);
    nested3WhileCfgDummyNode->statementNodes->push_back(createAssignNode(30, Variable("stmt"), createRefExpr(23)));
    nested3WhileCfgDummyNode->childrenNodes->push_back(nested3NewCallNode);
    nested3NewCallNode->statementNodes->push_back(createCallNode(31, "nested2"));

    CfgNode* newCallNodeForNested2 = createCfgNode(1, currentNumberOfNodes); // NODE 37
    nested3NewCallNode->childrenNodes->push_back(newCallNodeForNested2);

    /** procedure nested2 **/
    List<StatementNode> nested2Stmts;

    List<StatementNode> nested21IfStatements;
    List<StatementNode> nested21ElseStatements;

    List<StatementNode> nested22IfStatements;
    List<StatementNode> nested22ElseStatements;
    List<StatementNode> nested23IfStatements;
    List<StatementNode> nested23ElseStatements;

    List<StatementNode> nested24IfStatements;
    List<StatementNode> nested24ElseStatements;

    List<StatementNode> nested25IfStatements;
    List<StatementNode> nested25ElseStatements;

    List<StatementNode> nested26IfStatements;
    List<StatementNode> nested26ElseStatements;

    List<StatementNode> nested27IfStatements;
    List<StatementNode> nested27ElseStatements;

    List<StatementNode> nested28IfStatements;
    List<StatementNode> nested28ElseStatements;

    List<StatementNode> nested2WhileStatements;

    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));

    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));

    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));

    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));

    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));

    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));

    nested25IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));

    nested25ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements))));

    nested24IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));

    nested24ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested25IfStatements), createStmtlstNode(nested25ElseStatements))));

    nested23IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));

    nested23ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested24IfStatements),
                     createStmtlstNode(nested24ElseStatements))));

    nested22IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(37, Variable("stmt"), createRefExpr(6))));

    nested22ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested23IfStatements),
                     createStmtlstNode(nested23ElseStatements))));

    nested21IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(35, Variable("stmt"), createRefExpr(4))));

    nested21ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(36, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested22IfStatements),
                     createStmtlstNode(nested22ElseStatements))));

    nested2WhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(33, Variable("stmt"), createRefExpr(2))));

    nested2WhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(34, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(nested21IfStatements),
                     createStmtlstNode(nested21ElseStatements))));

    // Create new nodes for While Statement
    CfgNode* nested2WhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes); // NODE 38
    newCallNodeForNested2->childrenNodes->push_back(nested2WhileCfgChildrenNode);

    newCallNodeForNested2->statementNodes->push_back(createWhileNode(
        32, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(nested2WhileStatements)));

    nested2WhileCfgChildrenNode->statementNodes->push_back(createAssignNode(33, Variable("stmt"), createRefExpr(2)));

    // Create new nodes for first If Statement
    CfgNode* ifNewCfgNode1Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 39
    CfgNode* ifCfgNode1Nested2 = createCfgNode(1, currentNumberOfNodes);    // NODE 40
    CfgNode* ifDummyNode1Nested2 = createCfgNode(0, currentNumberOfNodes);  // NODE 41
    CfgNode* elseCfgNode1Nested2 = createCfgNode(1, currentNumberOfNodes);  // NODE 42
    nested2WhileCfgChildrenNode->childrenNodes->push_back(ifNewCfgNode1Nested2);

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));
    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));
    nested25IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested25ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements))));
    nested24IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested24ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested25IfStatements), createStmtlstNode(nested25ElseStatements))));
    nested23IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));
    nested23ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested24IfStatements),
                     createStmtlstNode(nested24ElseStatements))));
    nested22IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(37, Variable("stmt"), createRefExpr(6))));
    nested22ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested23IfStatements),
                     createStmtlstNode(nested23ElseStatements))));
    nested21IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(35, Variable("stmt"), createRefExpr(4))));
    nested21ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(36, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested22IfStatements),
                     createStmtlstNode(nested22ElseStatements))));

    ifNewCfgNode1Nested2->statementNodes->push_back(
        createIfNode(34, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(nested21IfStatements),
                     createStmtlstNode(nested21ElseStatements)));
    ifNewCfgNode1Nested2->childrenNodes->push_back(ifCfgNode1Nested2);
    ifNewCfgNode1Nested2->childrenNodes->push_back(elseCfgNode1Nested2);

    ifCfgNode1Nested2->statementNodes->push_back(createAssignNode(35, Variable("stmt"), createRefExpr(4)));
    ifCfgNode1Nested2->childrenNodes->push_back(ifDummyNode1Nested2);

    ifDummyNode1Nested2->childrenNodes->push_back(newCallNodeForNested2);

    // Create new nodes for second If Statement
    CfgNode* ifCfgNode2Nested2 = createCfgNode(1, currentNumberOfNodes);   // NODE 43
    CfgNode* ifDummyNode2Nested2 = createCfgNode(0, currentNumberOfNodes); // NODE 44
    CfgNode* elseCfgNode2Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 45

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));
    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));
    nested25IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested25ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements))));
    nested24IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested24ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested25IfStatements), createStmtlstNode(nested25ElseStatements))));
    nested23IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));
    nested23ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested24IfStatements),
                     createStmtlstNode(nested24ElseStatements))));
    nested22IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(37, Variable("stmt"), createRefExpr(6))));
    nested22ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested23IfStatements),
                     createStmtlstNode(nested23ElseStatements))));

    elseCfgNode1Nested2->statementNodes->push_back(
        createIfNode(36, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested22IfStatements),
                     createStmtlstNode(nested22ElseStatements)));
    elseCfgNode1Nested2->childrenNodes->push_back(ifCfgNode2Nested2);
    elseCfgNode1Nested2->childrenNodes->push_back(elseCfgNode2Nested2);

    ifCfgNode2Nested2->statementNodes->push_back(createAssignNode(37, Variable("stmt"), createRefExpr(6)));
    ifCfgNode2Nested2->childrenNodes->push_back(ifDummyNode2Nested2);

    ifDummyNode2Nested2->childrenNodes->push_back(ifDummyNode1Nested2);

    // Create new nodes for third If Statement
    CfgNode* ifCfgNode3Nested2 = createCfgNode(1, currentNumberOfNodes);   // NODE 46
    CfgNode* ifDummyNode3Nested2 = createCfgNode(0, currentNumberOfNodes); // NODE 47
    CfgNode* elseCfgNode3Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 48

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));
    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));
    nested25IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested25ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements))));
    nested24IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested24ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested25IfStatements), createStmtlstNode(nested25ElseStatements))));
    nested23IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));
    nested23ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested24IfStatements),
                     createStmtlstNode(nested24ElseStatements))));

    elseCfgNode2Nested2->statementNodes->push_back(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested23IfStatements),
                     createStmtlstNode(nested23ElseStatements)));
    elseCfgNode2Nested2->childrenNodes->push_back(ifCfgNode3Nested2);
    elseCfgNode2Nested2->childrenNodes->push_back(elseCfgNode3Nested2);

    ifCfgNode3Nested2->statementNodes->push_back(createAssignNode(39, Variable("stmt"), createRefExpr(8)));
    ifCfgNode3Nested2->childrenNodes->push_back(ifDummyNode3Nested2);

    ifDummyNode3Nested2->childrenNodes->push_back(ifDummyNode2Nested2);

    // Create new nodes for forth If Statement
    CfgNode* ifCfgNode4Nested2 = createCfgNode(1, currentNumberOfNodes);   // NODE 49
    CfgNode* ifDummyNode4Nested2 = createCfgNode(0, currentNumberOfNodes); // NODE 50
    CfgNode* elseCfgNode4Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 51

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));
    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));
    nested25IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested25ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements))));
    nested24IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested24ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested25IfStatements), createStmtlstNode(nested25ElseStatements))));

    elseCfgNode3Nested2->statementNodes->push_back(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested24IfStatements),
                     createStmtlstNode(nested24ElseStatements)));
    elseCfgNode3Nested2->childrenNodes->push_back(ifCfgNode4Nested2);
    elseCfgNode3Nested2->childrenNodes->push_back(elseCfgNode4Nested2);

    ifCfgNode4Nested2->statementNodes->push_back(createAssignNode(41, Variable("stmt"), createRefExpr(10)));
    ifCfgNode4Nested2->childrenNodes->push_back(ifDummyNode4Nested2);

    ifDummyNode4Nested2->childrenNodes->push_back(ifDummyNode3Nested2);

    // Create new nodes for fifth If Statement
    CfgNode* ifCfgNode5Nested2 = createCfgNode(1, currentNumberOfNodes);   // NODE 52
    CfgNode* ifDummyNode5Nested2 = createCfgNode(0, currentNumberOfNodes); // NODE 53
    CfgNode* elseCfgNode5Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 54

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));
    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));
    nested25IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested25ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements))));

    elseCfgNode4Nested2->statementNodes->push_back(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested25IfStatements), createStmtlstNode(nested25ElseStatements)));
    elseCfgNode4Nested2->childrenNodes->push_back(ifCfgNode5Nested2);
    elseCfgNode4Nested2->childrenNodes->push_back(elseCfgNode5Nested2);

    ifCfgNode5Nested2->statementNodes->push_back(createAssignNode(43, Variable("stmt"), createRefExpr(12)));
    ifCfgNode5Nested2->childrenNodes->push_back(ifDummyNode5Nested2);

    ifDummyNode5Nested2->childrenNodes->push_back(ifDummyNode4Nested2);

    // Create new nodes for sixth If Statement
    CfgNode* ifCfgNode6Nested2 = createCfgNode(1, currentNumberOfNodes);   // NODE 55
    CfgNode* ifDummyNode6Nested2 = createCfgNode(0, currentNumberOfNodes); // NODE 56
    CfgNode* elseCfgNode6Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 57

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));
    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));

    elseCfgNode5Nested2->statementNodes->push_back(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements)));
    elseCfgNode5Nested2->childrenNodes->push_back(ifCfgNode6Nested2);
    elseCfgNode5Nested2->childrenNodes->push_back(elseCfgNode6Nested2);

    ifCfgNode6Nested2->statementNodes->push_back(createAssignNode(45, Variable("stmt"), createRefExpr(14)));
    ifCfgNode6Nested2->childrenNodes->push_back(ifDummyNode6Nested2);

    ifDummyNode6Nested2->childrenNodes->push_back(ifDummyNode5Nested2);

    // Create new nodes for seventh If Statement
    CfgNode* ifCfgNode7Nested2 = createCfgNode(1, currentNumberOfNodes);   // NODE 58
    CfgNode* ifDummyNode7Nested2 = createCfgNode(0, currentNumberOfNodes); // NODE 59
    CfgNode* elseCfgNode7Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 60

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));

    elseCfgNode6Nested2->statementNodes->push_back(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements)));
    elseCfgNode6Nested2->childrenNodes->push_back(ifCfgNode7Nested2);
    elseCfgNode6Nested2->childrenNodes->push_back(elseCfgNode7Nested2);

    ifCfgNode7Nested2->statementNodes->push_back(createAssignNode(47, Variable("stmt"), createRefExpr(16)));
    ifCfgNode7Nested2->childrenNodes->push_back(ifDummyNode7Nested2);

    ifDummyNode7Nested2->childrenNodes->push_back(ifDummyNode6Nested2);

    // Create new nodes for eighth If Statement
    CfgNode* ifCfgNode8Nested2 = createCfgNode(1, currentNumberOfNodes);   // NODE 61
    CfgNode* ifDummyNode8Nested2 = createCfgNode(0, currentNumberOfNodes); // NODE 62
    CfgNode* elseCfgNode8Nested2 = createCfgNode(1, currentNumberOfNodes); // NODE 63

    // Duplicates cause unique pointers
    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));

    elseCfgNode7Nested2->statementNodes->push_back(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements)));
    elseCfgNode7Nested2->childrenNodes->push_back(ifCfgNode8Nested2);
    elseCfgNode7Nested2->childrenNodes->push_back(elseCfgNode8Nested2);

    ifCfgNode8Nested2->statementNodes->push_back(createAssignNode(49, Variable("stmt"), createRefExpr(18)));
    ifCfgNode8Nested2->childrenNodes->push_back(ifDummyNode8Nested2);

    ifDummyNode8Nested2->childrenNodes->push_back(ifDummyNode7Nested2);

    elseCfgNode8Nested2->statementNodes->push_back(createAssignNode(50, Variable("stmt"), createRefExpr(19)));
    elseCfgNode8Nested2->childrenNodes->push_back(ifDummyNode8Nested2);

    // Dummy node for first while
    CfgNode* whileDummyCfgNodeNested2 = createCfgNode(0, currentNumberOfNodes); // NODE 64
    newCallNodeForNested2->childrenNodes->push_back(whileDummyCfgNodeNested2);
    whileDummyCfgNodeNested2->statementNodes->push_back(createAssignNode(51, Variable("stmt"), createRefExpr(20)));

    /** Back to procedure nested 3, since nested 3 is finshed, back to procedure ifInIf **/

    CfgNode* ifDummyNode1 = createCfgNode(0, currentNumberOfNodes); // NODE 65
    CfgNode* elseCfgNode1 = createCfgNode(1, currentNumberOfNodes); // NODE 66
    whileDummyCfgNodeNested2->childrenNodes->push_back(ifDummyNode1);

    // Create new nodes for second If statement
    CfgNode* ifCfgNode2 = createCfgNode(1, currentNumberOfNodes);              // NODE 67
    CfgNode* newCallNode2 = createCfgNode(1, currentNumberOfNodes);            // NODE 68
    CfgNode* newCallNodeForNested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 69

    // procedure nested2_2_ again
    List<StatementNode> nested2_2_Stmts;

    List<StatementNode> nested2_2_1IfStatements;
    List<StatementNode> nested2_2_1ElseStatements;

    List<StatementNode> nested2_2_2IfStatements;
    List<StatementNode> nested2_2_2ElseStatements;
    List<StatementNode> nested2_2_3IfStatements;
    List<StatementNode> nested2_2_3ElseStatements;

    List<StatementNode> nested2_2_4IfStatements;
    List<StatementNode> nested2_2_4ElseStatements;

    List<StatementNode> nested2_2_5IfStatements;
    List<StatementNode> nested2_2_5ElseStatements;

    List<StatementNode> nested2_2_6IfStatements;
    List<StatementNode> nested2_2_6ElseStatements;

    List<StatementNode> nested2_2_7IfStatements;
    List<StatementNode> nested2_2_7ElseStatements;

    List<StatementNode> nested2_2_8IfStatements;
    List<StatementNode> nested2_2_8ElseStatements;

    List<StatementNode> nested2_2_WhileStatements;

    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));

    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));

    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));

    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));

    nested2_2_6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));

    nested2_2_6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements))));

    nested2_2_5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));

    nested2_2_5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested2_2_6IfStatements), createStmtlstNode(nested2_2_6ElseStatements))));

    nested2_2_4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));

    nested2_2_4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested2_2_5IfStatements), createStmtlstNode(nested2_2_5ElseStatements))));

    nested2_2_3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));

    nested2_2_3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                     createStmtlstNode(nested2_2_4IfStatements), createStmtlstNode(nested2_2_4ElseStatements))));

    nested2_2_2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(37, Variable("stmt"), createRefExpr(6))));

    nested2_2_2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                     createStmtlstNode(nested2_2_3IfStatements), createStmtlstNode(nested2_2_3ElseStatements))));

    nested2_2_1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(35, Variable("stmt"), createRefExpr(4))));

    nested2_2_1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(36, createEqExpr(createRefExpr("stmt"), createRefExpr(5)),
                     createStmtlstNode(nested2_2_2IfStatements), createStmtlstNode(nested2_2_2ElseStatements))));

    nested2_2_WhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(33, Variable("stmt"), createRefExpr(2))));

    nested2_2_WhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(34, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                     createStmtlstNode(nested2_2_1IfStatements), createStmtlstNode(nested2_2_1ElseStatements))));

    // Create new nodes for While Statement
    CfgNode* nested2_2_WhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes); // NODE 70
    newCallNodeForNested2_2->childrenNodes->push_back(nested2_2_WhileCfgChildrenNode);

    newCallNodeForNested2_2->statementNodes->push_back(createWhileNode(
        32, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(nested2_2_WhileStatements)));

    nested2_2_WhileCfgChildrenNode->statementNodes->push_back(createAssignNode(33, Variable("stmt"), createRefExpr(2)));

    // Create new nodes for first If Statement
    CfgNode* ifNewCfgNode1Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 71
    CfgNode* ifCfgNode1Nested2_2 = createCfgNode(1, currentNumberOfNodes);    // NODE 72
    CfgNode* ifDummyNode1Nested2_2 = createCfgNode(0, currentNumberOfNodes);  // NODE 73
    CfgNode* elseCfgNode1Nested2_2 = createCfgNode(1, currentNumberOfNodes);  // NODE 74
    nested2_2_WhileCfgChildrenNode->childrenNodes->push_back(ifNewCfgNode1Nested2_2);

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));
    nested2_2_6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested2_2_6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements))));
    nested2_2_5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested2_2_5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested2_2_6IfStatements), createStmtlstNode(nested2_2_6ElseStatements))));
    nested2_2_4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested2_2_4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested2_2_5IfStatements), createStmtlstNode(nested2_2_5ElseStatements))));
    nested2_2_3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));
    nested2_2_3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                     createStmtlstNode(nested2_2_4IfStatements), createStmtlstNode(nested2_2_4ElseStatements))));
    nested2_2_2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(37, Variable("stmt"), createRefExpr(6))));
    nested2_2_2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                     createStmtlstNode(nested2_2_3IfStatements), createStmtlstNode(nested2_2_3ElseStatements))));
    nested2_2_1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(35, Variable("stmt"), createRefExpr(4))));
    nested2_2_1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(36, createEqExpr(createRefExpr("stmt"), createRefExpr(5)),
                     createStmtlstNode(nested2_2_2IfStatements), createStmtlstNode(nested2_2_2ElseStatements))));

    ifNewCfgNode1Nested2_2->statementNodes->push_back(
        createIfNode(34, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                     createStmtlstNode(nested2_2_1IfStatements), createStmtlstNode(nested2_2_1ElseStatements)));
    ifNewCfgNode1Nested2_2->childrenNodes->push_back(ifCfgNode1Nested2_2);
    ifNewCfgNode1Nested2_2->childrenNodes->push_back(elseCfgNode1Nested2_2);

    ifCfgNode1Nested2_2->statementNodes->push_back(createAssignNode(35, Variable("stmt"), createRefExpr(4)));
    ifCfgNode1Nested2_2->childrenNodes->push_back(ifDummyNode1Nested2_2);

    ifDummyNode1Nested2_2->childrenNodes->push_back(newCallNodeForNested2_2);

    // Create new nodes for second If Statement
    CfgNode* ifCfgNode2Nested2_2 = createCfgNode(1, currentNumberOfNodes);   // NODE 75
    CfgNode* ifDummyNode2Nested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 76
    CfgNode* elseCfgNode2Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 77

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));
    nested2_2_6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested2_2_6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements))));
    nested2_2_5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested2_2_5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested2_2_6IfStatements), createStmtlstNode(nested2_2_6ElseStatements))));
    nested2_2_4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested2_2_4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested2_2_5IfStatements), createStmtlstNode(nested2_2_5ElseStatements))));
    nested2_2_3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));
    nested2_2_3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                     createStmtlstNode(nested2_2_4IfStatements), createStmtlstNode(nested2_2_4ElseStatements))));
    nested2_2_2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(37, Variable("stmt"), createRefExpr(6))));
    nested2_2_2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                     createStmtlstNode(nested2_2_3IfStatements), createStmtlstNode(nested2_2_3ElseStatements))));

    elseCfgNode1Nested2_2->statementNodes->push_back(
        createIfNode(36, createEqExpr(createRefExpr("stmt"), createRefExpr(5)),
                     createStmtlstNode(nested2_2_2IfStatements), createStmtlstNode(nested2_2_2ElseStatements)));
    elseCfgNode1Nested2_2->childrenNodes->push_back(ifCfgNode2Nested2_2);
    elseCfgNode1Nested2_2->childrenNodes->push_back(elseCfgNode2Nested2_2);

    ifCfgNode2Nested2_2->statementNodes->push_back(createAssignNode(37, Variable("stmt"), createRefExpr(6)));
    ifCfgNode2Nested2_2->childrenNodes->push_back(ifDummyNode2Nested2_2);

    ifDummyNode2Nested2_2->childrenNodes->push_back(ifDummyNode1Nested2_2);

    // Create new nodes for third If Statement
    CfgNode* ifCfgNode3Nested2_2 = createCfgNode(1, currentNumberOfNodes);   // NODE 78
    CfgNode* ifDummyNode3Nested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 79
    CfgNode* elseCfgNode3Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 80

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));
    nested2_2_6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested2_2_6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements))));
    nested2_2_5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested2_2_5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested2_2_6IfStatements), createStmtlstNode(nested2_2_6ElseStatements))));
    nested2_2_4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested2_2_4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested2_2_5IfStatements), createStmtlstNode(nested2_2_5ElseStatements))));
    nested2_2_3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));
    nested2_2_3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                     createStmtlstNode(nested2_2_4IfStatements), createStmtlstNode(nested2_2_4ElseStatements))));

    elseCfgNode2Nested2_2->statementNodes->push_back(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                     createStmtlstNode(nested2_2_3IfStatements), createStmtlstNode(nested2_2_3ElseStatements)));
    elseCfgNode2Nested2_2->childrenNodes->push_back(ifCfgNode3Nested2_2);
    elseCfgNode2Nested2_2->childrenNodes->push_back(elseCfgNode3Nested2_2);

    ifCfgNode3Nested2_2->statementNodes->push_back(createAssignNode(39, Variable("stmt"), createRefExpr(8)));
    ifCfgNode3Nested2_2->childrenNodes->push_back(ifDummyNode3Nested2_2);

    ifDummyNode3Nested2_2->childrenNodes->push_back(ifDummyNode2Nested2_2);

    // Create new nodes for forth If Statement
    CfgNode* ifCfgNode4Nested2_2 = createCfgNode(1, currentNumberOfNodes);   // NODE 81
    CfgNode* ifDummyNode4Nested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 82
    CfgNode* elseCfgNode4Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 83

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));
    nested2_2_6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested2_2_6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements))));
    nested2_2_5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested2_2_5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested2_2_6IfStatements), createStmtlstNode(nested2_2_6ElseStatements))));
    nested2_2_4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));
    nested2_2_4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested2_2_5IfStatements), createStmtlstNode(nested2_2_5ElseStatements))));

    elseCfgNode3Nested2_2->statementNodes->push_back(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                     createStmtlstNode(nested2_2_4IfStatements), createStmtlstNode(nested2_2_4ElseStatements)));
    elseCfgNode3Nested2_2->childrenNodes->push_back(ifCfgNode4Nested2_2);
    elseCfgNode3Nested2_2->childrenNodes->push_back(elseCfgNode4Nested2_2);

    ifCfgNode4Nested2_2->statementNodes->push_back(createAssignNode(41, Variable("stmt"), createRefExpr(10)));
    ifCfgNode4Nested2_2->childrenNodes->push_back(ifDummyNode4Nested2_2);

    ifDummyNode4Nested2_2->childrenNodes->push_back(ifDummyNode3Nested2_2);

    // Create new nodes for fifth If Statement
    CfgNode* ifCfgNode5Nested2_2 = createCfgNode(1, currentNumberOfNodes);   // NODE 84
    CfgNode* ifDummyNode5Nested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 85
    CfgNode* elseCfgNode5Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 86

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));
    nested2_2_6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested2_2_6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements))));
    nested2_2_5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));
    nested2_2_5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested2_2_6IfStatements), createStmtlstNode(nested2_2_6ElseStatements))));

    elseCfgNode4Nested2_2->statementNodes->push_back(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested2_2_5IfStatements), createStmtlstNode(nested2_2_5ElseStatements)));
    elseCfgNode4Nested2_2->childrenNodes->push_back(ifCfgNode5Nested2_2);
    elseCfgNode4Nested2_2->childrenNodes->push_back(elseCfgNode5Nested2_2);

    ifCfgNode5Nested2_2->statementNodes->push_back(createAssignNode(43, Variable("stmt"), createRefExpr(12)));
    ifCfgNode5Nested2_2->childrenNodes->push_back(ifDummyNode5Nested2_2);

    ifDummyNode5Nested2_2->childrenNodes->push_back(ifDummyNode4Nested2_2);

    // Create new nodes for sixth If Statement
    CfgNode* ifCfgNode6Nested2_2 = createCfgNode(1, currentNumberOfNodes);   // NODE 87
    CfgNode* ifDummyNode6Nested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 88
    CfgNode* elseCfgNode6Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 89

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));
    nested2_2_6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));
    nested2_2_6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements))));

    elseCfgNode5Nested2_2->statementNodes->push_back(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested2_2_6IfStatements), createStmtlstNode(nested2_2_6ElseStatements)));
    elseCfgNode5Nested2_2->childrenNodes->push_back(ifCfgNode6Nested2_2);
    elseCfgNode5Nested2_2->childrenNodes->push_back(elseCfgNode6Nested2_2);

    ifCfgNode6Nested2_2->statementNodes->push_back(createAssignNode(45, Variable("stmt"), createRefExpr(14)));
    ifCfgNode6Nested2_2->childrenNodes->push_back(ifDummyNode6Nested2_2);

    ifDummyNode6Nested2_2->childrenNodes->push_back(ifDummyNode5Nested2_2);

    // Create new nodes for seventh If Statement
    CfgNode* ifCfgNode7Nested2_2 = createCfgNode(1, currentNumberOfNodes);   // NODE 90
    CfgNode* ifDummyNode7Nested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 91
    CfgNode* elseCfgNode7Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 92

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));
    nested2_2_7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));
    nested2_2_7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements))));

    elseCfgNode6Nested2_2->statementNodes->push_back(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested2_2_7IfStatements), createStmtlstNode(nested2_2_7ElseStatements)));
    elseCfgNode6Nested2_2->childrenNodes->push_back(ifCfgNode7Nested2_2);
    elseCfgNode6Nested2_2->childrenNodes->push_back(elseCfgNode7Nested2_2);

    ifCfgNode7Nested2_2->statementNodes->push_back(createAssignNode(47, Variable("stmt"), createRefExpr(16)));
    ifCfgNode7Nested2_2->childrenNodes->push_back(ifDummyNode7Nested2_2);

    ifDummyNode7Nested2_2->childrenNodes->push_back(ifDummyNode6Nested2_2);

    // Create new nodes for eighth If Statement
    CfgNode* ifCfgNode8Nested2_2 = createCfgNode(1, currentNumberOfNodes);   // NODE 93
    CfgNode* ifDummyNode8Nested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 94
    CfgNode* elseCfgNode8Nested2_2 = createCfgNode(1, currentNumberOfNodes); // NODE 95

    // Duplicates cause unique pointers
    nested2_2_8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));
    nested2_2_8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));

    elseCfgNode7Nested2_2->statementNodes->push_back(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested2_2_8IfStatements), createStmtlstNode(nested2_2_8ElseStatements)));
    elseCfgNode7Nested2_2->childrenNodes->push_back(ifCfgNode8Nested2_2);
    elseCfgNode7Nested2_2->childrenNodes->push_back(elseCfgNode8Nested2_2);

    ifCfgNode8Nested2_2->statementNodes->push_back(createAssignNode(49, Variable("stmt"), createRefExpr(18)));
    ifCfgNode8Nested2_2->childrenNodes->push_back(ifDummyNode8Nested2_2);

    ifDummyNode8Nested2_2->childrenNodes->push_back(ifDummyNode7Nested2_2);

    elseCfgNode8Nested2_2->statementNodes->push_back(createAssignNode(50, Variable("stmt"), createRefExpr(19)));
    elseCfgNode8Nested2_2->childrenNodes->push_back(ifDummyNode8Nested2_2);

    // Dummy node for first while
    CfgNode* whileDummyCfgNodeNested2_2 = createCfgNode(0, currentNumberOfNodes); // NODE 96
    newCallNodeForNested2_2->childrenNodes->push_back(whileDummyCfgNodeNested2_2);
    whileDummyCfgNodeNested2_2->statementNodes->push_back(createAssignNode(51, Variable("stmt"), createRefExpr(20)));

    CfgNode* ifDummyNode2 = createCfgNode(0, currentNumberOfNodes); // NODE 97
    CfgNode* elseCfgNode2 = createCfgNode(1, currentNumberOfNodes); // NODE 98

    expectedCfg->childrenNodes->push_back(elseCfgNode1);

    // Duplicates cause unique pointers
    ifInIf2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("stmt"), createRefExpr(4))));
    ifInIf2IfStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(6, "nested2")));
    ifInIf2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(7, Variable("stmt"), createRefExpr(5))));

    elseCfgNode1->statementNodes->push_back(createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                                                         createStmtlstNode(ifInIf2IfStatements),
                                                         createStmtlstNode(ifInIf2ElseStatements)));
    elseCfgNode1->childrenNodes->push_back(ifCfgNode2);
    elseCfgNode1->childrenNodes->push_back(elseCfgNode2);

    ifCfgNode2->statementNodes->push_back(createAssignNode(5, Variable("stmt"), createRefExpr(4)));
    ifCfgNode2->childrenNodes->push_back(newCallNode2);

    newCallNode2->statementNodes->push_back(createCallNode(6, "nested2"));
    // First node of nested 2
    newCallNode2->childrenNodes->push_back(newCallNodeForNested2_2);
    // Last node of nested 2
    whileDummyCfgNodeNested2_2->childrenNodes->push_back(ifDummyNode2);

    elseCfgNode2->statementNodes->push_back(createAssignNode(7, Variable("stmt"), createRefExpr(5)));
    elseCfgNode2->childrenNodes->push_back(ifDummyNode2);

    ifDummyNode2->childrenNodes->push_back(ifDummyNode1);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}
