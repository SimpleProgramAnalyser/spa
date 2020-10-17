/**
 * Tests for the CFG Builder component
 * of the Design Extractor in SPA Frontend
 */

#include "../ast_utils/AstUtils.h"
#include "../cfg_utils/CfgUtils.h"
#include "ast/AstLibrary.h"
#include "catch.hpp"
#include "cfg/CfgBuilder.cpp"

TEST_CASE("Cfg Builder works for basic program with read, assign, print - program1, compute")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram1Tree_compute()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram1Cfg_compute();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg Builder works for if and else statements nested in while - program2, factorials")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram2Tree_factorials()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram2Cfg_factorials();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg Builder works for program with read, assign, print - program3, computeAverage")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram3Tree_computeAverage()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram3Cfg_computeAverage();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("CfgBuilder works for if and else statements - program4, printAscending")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram4Tree_printAscending()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram4Cfg_printAscending();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("CfgBuilder works for program with read, assign, while, print - program5, sumDigit")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram5Tree_sumDigit()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram5Cfg_sumDigit();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("CfgBuilder works for program with if right after while, call - program7, computeCentroid")
{
    // Actual
    // Compute Centroid
    List<StatementNode> computeCentroidStatements;
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("count"), createRefExpr(0))));
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(11, Variable("cenX"), createRefExpr(0))));
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("cenY"), createRefExpr(0))));
    computeCentroidStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(13, "readPoint")));

    // Compute Centroid - While statement
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(15, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(16, Variable("cenX"), createPlusExpr(createRefExpr("cenX"), createRefExpr("x")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(17, Variable("cenY"), createPlusExpr(createRefExpr("cenY"), createRefExpr("y")))));
    whileStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(18, "readPoint")));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    computeCentroidStatements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(14,
                        createAndExpr(createNeqExpr(createRefExpr("x"), (createRefExpr(0))),
                                      createNeqExpr(createRefExpr("y"), (createRefExpr(0)))),
                        whileStmtLstNode)));

    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // Compute Centroid - If statement
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("flag"), createRefExpr(1))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Compute Centroid - Else statement
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(21, Variable("cenX"), createDivExpr(createRefExpr("cenX"), createRefExpr("count")))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(22, Variable("cenY"), createDivExpr(createRefExpr("cenY"), createRefExpr("count")))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    computeCentroidStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(19, createEqExpr(createRefExpr("count"), createRefExpr(0)), ifStmtLstNode, elseStmtLstNode)));

    computeCentroidStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(23, Variable("normSq"),
                         createPlusExpr(createTimesExpr(createRefExpr("cenX"), createRefExpr("cenX")),
                                        createTimesExpr(createRefExpr("cenY"), createRefExpr("cenY"))))));
    StmtlstNode* stmtLstNode = createStmtlstNode(computeCentroidStatements);

    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram7Cfg_computeCentroid();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("CfgBuilder works for program with if statement and assign - program13, ifExample")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram13Tree_ifExample()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram13Cfg_ifExample();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("CfgBuilder works for program with while statement and assign - program14, whileExample")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram14Tree_whileExample()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram14Cfg_whileExample();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("CfgBuilder works for program with complicated conditional in while - program15, complicatedConditional")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram15Tree_complicatedConditional()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram15Cfg_complicatedConditional();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("CfgBuilder works for program with procedure ending with while statement, with multiple assign - program18, "
          "endWithWhile")
{
    // Actual
    const List<ProcedureNode>* procedureList = &(getProgram18Tree_endWithWhile()->procedureList);
    const StmtlstNode* const stmtLstNode = procedureList->at(0)->statementListNode;
    Pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtLstNode);
    CfgNode* cfgRootNode = cfgInfo.first;

    // Expected
    std::pair<CfgNode*, size_t> expectedCfgInfo = getProgram18Cfg_endWithWhile();
    CfgNode* expectedCfg = expectedCfgInfo.first;

    Boolean isEqual = cfgRootNode->equals(expectedCfg, expectedCfgInfo.second);
    REQUIRE(isEqual == true);
}
