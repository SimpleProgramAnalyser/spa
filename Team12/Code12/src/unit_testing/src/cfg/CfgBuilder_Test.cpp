/**
 * Tests for the CFG Builder component
 * of the Design Extractor in SPA Frontend
 */

//#include "../ast_utils/AstUtils.cpp"
#include "catch.hpp"
#include "cfg/CfgBuilder.cpp"
#include "ast/AstLibrary.h"


TEST_CASE("Cfg Builder works for if and else statements nested in while")
{
    List<StatementNode> statements;
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(1, Variable("limit"), createRefExpr(100))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("current"), createRefExpr(0))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("factorial"), createRefExpr(0))));
    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factorial"), createRefExpr(1))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("factorial"), createTimesExpr(createRefExpr("current"), createRefExpr("factorial")))));
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("current"), createRefExpr(0)), createStmtlstNode(ifStatements),
                     createStmtlstNode(elseStatements))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(8, Variable("current"), createPlusExpr(createRefExpr("current"), createRefExpr(1)))));
    statements.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        4, createLtExpr(createRefExpr("current"), createRefExpr("limit")), createStmtlstNode(whileStatements))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("answer"), createRefExpr("factorial"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);

    CfgNode* cfgRootNode = buildCfg(stmtLstNode);
    

    //Expected
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(5, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(1, Variable("limit"), createRefExpr(100))));
    expectedCfg->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("current"), createRefExpr(0))));
    expectedCfg->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("factorial"), createRefExpr(0))));

    CfgNode* firstWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(std::unique_ptr<CfgNode> (firstWhileCfgNode));

    // To populate if and else statementLists
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factorial"), createRefExpr(1))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("factorial"), createTimesExpr(createRefExpr("current"), createRefExpr("factorial")))));

    firstWhileCfgChildrenNode->statementNodes->push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("current"), createRefExpr(0)), createStmtlstNode(ifStatements),
                     createStmtlstNode(elseStatements))));


     // To populate if and else statementLists
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factorial"), createRefExpr(1))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("factorial"), createTimesExpr(createRefExpr("current"), createRefExpr("factorial")))));
    
    whileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("current"), createRefExpr(0)), createStmtlstNode(ifStatements),
                     createStmtlstNode(elseStatements))));
                     

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(8, Variable("current"), createPlusExpr(createRefExpr("current"), createRefExpr(1)))));

    firstWhileCfgNode->statementNodes->push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        4, createLtExpr(createRefExpr("current"), createRefExpr("limit")), createStmtlstNode(whileStatements))));
    firstWhileCfgNode->childrenNodes->push_back(std::unique_ptr<CfgNode> (firstWhileCfgChildrenNode));

    CfgNode* ifCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);

    firstWhileCfgChildrenNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(ifCfgNode));
    firstWhileCfgChildrenNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(elseCfgNode));

    ifCfgNode->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factorial"), createRefExpr(1))));
    elseCfgNode->statementNodes->push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("factorial"), createTimesExpr(createRefExpr("current"), createRefExpr("factorial")))));

    ifCfgNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(ifDummyNode));
    elseCfgNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(ifDummyNode));

    ifDummyNode->statementNodes->push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(8, Variable("current"), createPlusExpr(createRefExpr("current"), createRefExpr(1)))));

    ifDummyNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(firstWhileCfgNode));


    CfgNode* nextNode = createCfgNode(1, currentNumberOfNodes);

    nextNode->statementNodes->push_back(
         std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("answer"), createRefExpr("factorial"))));

    firstWhileCfgNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(nextNode));


    Boolean isEqual = cfgRootNode->equals(expectedCfg, currentNumberOfNodes);
    REQUIRE(isEqual == true);
}



TEST_CASE("CfgBuilder works for if and else statements")
{
    List<StatementNode> statements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("noSwap"), createRefExpr(0))));

    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // If statements
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("temp"), createRefExpr("num1"))));
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("num1"), createRefExpr("num2"))));
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(7, Variable("num2"), createRefExpr("temp"))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Else statements
    elseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("noSwap"), createRefExpr(1))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    statements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createGtExpr(createRefExpr("num1"), createRefExpr("num2")), ifStmtLstNode, elseStmtLstNode)));

    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(9, Variable("num1"))));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(10, Variable("num2"))));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(11, Variable("noSwap"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);

    CfgNode* cfgRootNode = buildCfg(stmtLstNode);

    // Expected
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(7, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(
                std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    expectedCfg->statementNodes->push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));
    expectedCfg->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("noSwap"), createRefExpr(0))));

    CfgNode* ifNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode = createCfgNode(3, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);


    // To populate if and else statement lists
    // If statements
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("temp"), createRefExpr("num1"))));
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("num1"), createRefExpr("num2"))));
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(7, Variable("num2"), createRefExpr("temp"))));
    ifStmtLstNode = createStmtlstNode(ifStatements);
    // Else statements
    elseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("noSwap"), createRefExpr(1))));
    elseStmtLstNode = createStmtlstNode(elseStatements);

    ifNode->statementNodes->push_back(std::unique_ptr<IfStatementNode>(createIfNode(4, createGtExpr(createRefExpr("num1"), createRefExpr("num2")), ifStmtLstNode, elseStmtLstNode)));
    expectedCfg->childrenNodes->push_back(std::unique_ptr<CfgNode>(ifNode));

    ifNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(ifCfgNode));
    ifNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(elseCfgNode));

   
    ifCfgNode->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("temp"), createRefExpr("num1"))));
    ifCfgNode->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("num1"), createRefExpr("num2"))));
    ifCfgNode->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(7, Variable("num2"), createRefExpr("temp"))));
    ifCfgNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(ifDummyNode));

    elseCfgNode->statementNodes->push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("noSwap"), createRefExpr(1))));
    elseCfgNode->childrenNodes->push_back(std::unique_ptr<CfgNode>(ifDummyNode));

    ifDummyNode->statementNodes->push_back(
        std::unique_ptr<PrintStatementNode>(createPrintNode(9, Variable("num1"))));
    ifDummyNode->statementNodes->push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(10, Variable("num2"))));
    ifDummyNode->statementNodes->push_back(
        std::unique_ptr<PrintStatementNode>(createPrintNode(11, Variable("noSwap"))));

    Boolean isEqual = cfgRootNode->equals(expectedCfg, currentNumberOfNodes);
    REQUIRE(isEqual == true);
}
