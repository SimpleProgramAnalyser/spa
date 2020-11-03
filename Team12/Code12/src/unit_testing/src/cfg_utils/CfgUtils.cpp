/**
 * Implementation of methods to generate Control Flow Graph, to aid in unit testing.
 *
 * Methods labelled with the same number describe the
 * same program in different forms.
 */

#include "CfgUtils.h"

#include <iostream>

#include "ast/AstLibrary.h"
#include "cfg/CfgBipBuilder.h"
#include "cfg/CfgBuilder.h"
#include "cfg/CfgTypes.h"
using namespace std;

/** CFG Builder **/
std::pair<CfgNode*, size_t> getProgram1Cfg_compute()
{
    // Create Cfg root node
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(createReadNode(1, Variable("num1")));
    expectedCfg->statementNodes->push_back(createReadNode(2, Variable("num2")));
    expectedCfg->statementNodes->push_back(createReadNode(3, Variable("num3")));
    expectedCfg->statementNodes->push_back(createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3"))));
    expectedCfg->statementNodes->push_back(
        createAssignNode(5, Variable("ave"), createDivExpr(createRefExpr("sum"), createRefExpr(3))));
    expectedCfg->statementNodes->push_back(createPrintNode(6, Variable("ave")));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram2Cfg_factorials()
{
    // Create Cfg root node
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(5, currentNumberOfNodes);
    expectedCfg->statementNodes->push_back(createAssignNode(1, Variable("limit"), createRefExpr(100)));
    expectedCfg->statementNodes->push_back(createAssignNode(2, Variable("current"), createRefExpr(0)));
    expectedCfg->statementNodes->push_back(createAssignNode(3, Variable("factorial"), createRefExpr(0)));

    // Create new nodes for While Statement
    CfgNode* firstWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgNode);

    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factorial"), createRefExpr(1))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("factorial"), createTimesExpr(createRefExpr("current"), createRefExpr("factorial")))));

    // Add children nodes
    firstWhileCfgChildrenNode->statementNodes->push_back(
        createIfNode(5, createEqExpr(createRefExpr("current"), createRefExpr(0)), createStmtlstNode(ifStatements),
                     createStmtlstNode(elseStatements)));

    // To populate if and else statementLists
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

    // Add statement nodes & children nodes
    firstWhileCfgNode->statementNodes->push_back(createWhileNode(
        4, createLtExpr(createRefExpr("current"), createRefExpr("limit")), createStmtlstNode(whileStatements)));
    firstWhileCfgNode->childrenNodes->push_back(firstWhileCfgChildrenNode);

    // Create nodes for If Statement
    CfgNode* ifCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);
    // Add statement and children nodes
    firstWhileCfgChildrenNode->childrenNodes->push_back(ifCfgNode);
    firstWhileCfgChildrenNode->childrenNodes->push_back(elseCfgNode);
    ifCfgNode->statementNodes->push_back(createAssignNode(6, Variable("factorial"), createRefExpr(1)));
    elseCfgNode->statementNodes->push_back(createAssignNode(
        7, Variable("factorial"), createTimesExpr(createRefExpr("current"), createRefExpr("factorial"))));
    ifCfgNode->childrenNodes->push_back(ifDummyNode);
    elseCfgNode->childrenNodes->push_back(ifDummyNode);
    // Add statement node and children node intodummy node
    ifDummyNode->statementNodes->push_back(
        createAssignNode(8, Variable("current"), createPlusExpr(createRefExpr("current"), createRefExpr(1))));
    ifDummyNode->childrenNodes->push_back(firstWhileCfgNode);

    // Create node for statements after If Statement
    CfgNode* nextNode = createCfgNode(1, currentNumberOfNodes);
    nextNode->statementNodes->push_back(createAssignNode(9, Variable("answer"), createRefExpr("factorial")));
    firstWhileCfgNode->childrenNodes->push_back(nextNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram3Cfg_computeAverage()
{
    // Create Cfg root node
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(createReadNode(1, Variable("num1")));
    expectedCfg->statementNodes->push_back(createReadNode(2, Variable("num2")));
    expectedCfg->statementNodes->push_back(createReadNode(3, Variable("num3")));
    expectedCfg->statementNodes->push_back(createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3"))));
    expectedCfg->statementNodes->push_back(
        createAssignNode(5, Variable("ave"), createDivExpr(createRefExpr("sum"), createRefExpr(3))));
    expectedCfg->statementNodes->push_back(createPrintNode(6, Variable("ave")));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram4Cfg_printAscending()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(7, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(createReadNode(1, Variable("num1")));
    expectedCfg->statementNodes->push_back(createReadNode(2, Variable("num2")));
    expectedCfg->statementNodes->push_back(createAssignNode(3, Variable("noSwap"), createRefExpr(0)));

    CfgNode* ifNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode = createCfgNode(3, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);

    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // To populate if and else statement lists
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

    ifNode->statementNodes->push_back(
        createIfNode(4, createGtExpr(createRefExpr("num1"), createRefExpr("num2")), ifStmtLstNode, elseStmtLstNode));
    expectedCfg->childrenNodes->push_back(ifNode);

    ifNode->childrenNodes->push_back(ifCfgNode);
    ifNode->childrenNodes->push_back(elseCfgNode);

    ifCfgNode->statementNodes->push_back(createAssignNode(5, Variable("temp"), createRefExpr("num1")));
    ifCfgNode->statementNodes->push_back(createAssignNode(6, Variable("num1"), createRefExpr("num2")));
    ifCfgNode->statementNodes->push_back(createAssignNode(7, Variable("num2"), createRefExpr("temp")));
    ifCfgNode->childrenNodes->push_back(ifDummyNode);

    elseCfgNode->statementNodes->push_back(createAssignNode(8, Variable("noSwap"), createRefExpr(1)));
    elseCfgNode->childrenNodes->push_back(ifDummyNode);

    ifDummyNode->statementNodes->push_back(createPrintNode(9, Variable("num1")));
    ifDummyNode->statementNodes->push_back(createPrintNode(10, Variable("num2")));
    ifDummyNode->statementNodes->push_back(createPrintNode(11, Variable("noSwap")));
    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram5Cfg_sumDigit()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(4, currentNumberOfNodes);

    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    expectedCfg->statementNodes->push_back(createReadNode(1, Variable("number")));
    expectedCfg->statementNodes->push_back(createAssignNode(2, Variable("sum"), createRefExpr(0)));

    // Create new nodes for While Statement
    CfgNode* firstWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(3, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgNode);

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    firstWhileCfgNode->statementNodes->push_back(
        createWhileNode(3, createGtExpr(createRefExpr("number"), createRefExpr(0)), whileStmtLstNode));
    firstWhileCfgNode->childrenNodes->push_back(firstWhileCfgChildrenNode);

    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10))));
    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit"))));
    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10))));
    firstWhileCfgChildrenNode->childrenNodes->push_back(firstWhileCfgNode);

    CfgNode* whileDummyCfgNode = createCfgNode(1, currentNumberOfNodes);
    whileDummyCfgNode->statementNodes->push_back(createPrintNode(7, Variable("sum")));

    firstWhileCfgNode->childrenNodes->push_back(whileDummyCfgNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

/*
procedure computeCentroid
{
    10. count = 0;
    11. cenX = 0;
    12. cenY = 0;
    13. call readPoint;
    14. while ((x != 0) && (y != 0)) {
    15.   count = count + 1;
    16.   cenX = cenX + x;
    17.   cenY = cenY + y;
    18.   call readPoint;
        }
    19. if (count == 0) then {
    20.   flag = 1;
    } else {
    21.   cenX = cenX / count;
    22.   cenY = cenY / count; }
    23. normSq = cenX * cenX + cenY * cenY;
}
*/
// We extract the procedure computeCentroid
std::pair<CfgNode*, size_t> getProgram7Cfg_computeCentroid()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(6, currentNumberOfNodes);

    // Compute Centroid
    expectedCfg->statementNodes->push_back(createAssignNode(10, Variable("count"), createRefExpr(0)));
    expectedCfg->statementNodes->push_back(createAssignNode(11, Variable("cenX"), createRefExpr(0)));
    expectedCfg->statementNodes->push_back(createAssignNode(12, Variable("cenY"), createRefExpr(0)));
    expectedCfg->statementNodes->push_back(createCallNode(13, "readPoint"));

    // Compute Centroid - While statement
    // Create new nodes for While Statement
    CfgNode* firstWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(4, currentNumberOfNodes);
    CfgNode* whileDummyCfgNode = createCfgNode(1, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgNode);

    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(15, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(16, Variable("cenX"), createPlusExpr(createRefExpr("cenX"), createRefExpr("x")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(17, Variable("cenY"), createPlusExpr(createRefExpr("cenY"), createRefExpr("y")))));
    whileStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(18, "readPoint")));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    firstWhileCfgNode->statementNodes->push_back(
        createWhileNode(14,
                        createAndExpr(createNeqExpr(createRefExpr("x"), (createRefExpr(0))),
                                      createNeqExpr(createRefExpr("y"), (createRefExpr(0)))),
                        whileStmtLstNode));
    firstWhileCfgNode->childrenNodes->push_back(firstWhileCfgChildrenNode);

    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(15, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1))));
    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(16, Variable("cenX"), createPlusExpr(createRefExpr("cenX"), createRefExpr("x"))));
    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(17, Variable("cenY"), createPlusExpr(createRefExpr("cenY"), createRefExpr("y"))));
    firstWhileCfgChildrenNode->statementNodes->push_back(createCallNode(18, "readPoint"));

    firstWhileCfgChildrenNode->childrenNodes->push_back(firstWhileCfgNode);

    firstWhileCfgNode->childrenNodes->push_back(whileDummyCfgNode);

    // Compute Centroid - If statement
    // Create nodes for If Statement
    CfgNode* ifCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(2, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);

    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("flag"), createRefExpr(1))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Compute Centroid - Else statement
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(21, Variable("cenX"), createDivExpr(createRefExpr("cenX"), createRefExpr("count")))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(22, Variable("cenY"), createDivExpr(createRefExpr("cenY"), createRefExpr("count")))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    whileDummyCfgNode->statementNodes->push_back(
        createIfNode(19, createEqExpr(createRefExpr("count"), createRefExpr(0)), ifStmtLstNode, elseStmtLstNode));
    whileDummyCfgNode->childrenNodes->push_back(ifCfgNode);
    whileDummyCfgNode->childrenNodes->push_back(elseCfgNode);

    ifCfgNode->statementNodes->push_back(createAssignNode(20, Variable("flag"), createRefExpr(1)));
    // Compute Centroid - Else statement
    elseCfgNode->statementNodes->push_back(
        createAssignNode(21, Variable("cenX"), createDivExpr(createRefExpr("cenX"), createRefExpr("count"))));
    elseCfgNode->statementNodes->push_back(
        createAssignNode(22, Variable("cenY"), createDivExpr(createRefExpr("cenY"), createRefExpr("count"))));

    ifCfgNode->childrenNodes->push_back(ifDummyNode);
    elseCfgNode->childrenNodes->push_back(ifDummyNode);

    ifDummyNode->statementNodes->push_back(
        createAssignNode(23, Variable("normSq"),
                         createPlusExpr(createTimesExpr(createRefExpr("cenX"), createRefExpr("cenX")),
                                        createTimesExpr(createRefExpr("cenY"), createRefExpr("cenY")))));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram13Cfg_ifExample()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(4, currentNumberOfNodes);

    CfgNode* ifCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(0, currentNumberOfNodes);

    List<StatementNode> statements;
    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // If statements
    ifStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("num2"), createPlusExpr(createRefExpr("num2"), createRefExpr(1)))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Else statements
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(3, Variable("num1"), createPlusExpr(createRefExpr("num1"), createRefExpr(1)))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    expectedCfg->statementNodes->push_back(
        createIfNode(1, createGteExpr(createRefExpr("num1"), createRefExpr("num2")), ifStmtLstNode, elseStmtLstNode));
    expectedCfg->childrenNodes->push_back(ifCfgNode);
    expectedCfg->childrenNodes->push_back(elseCfgNode);

    ifCfgNode->statementNodes->push_back(
        createAssignNode(2, Variable("num2"), createPlusExpr(createRefExpr("num2"), createRefExpr(1))));
    elseCfgNode->statementNodes->push_back(
        createAssignNode(3, Variable("num1"), createPlusExpr(createRefExpr("num1"), createRefExpr(1))));
    ifCfgNode->childrenNodes->push_back(ifDummyNode);
    elseCfgNode->childrenNodes->push_back(ifDummyNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram14Cfg_whileExample()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(3, currentNumberOfNodes);

    CfgNode* firstWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* whileDummyCfgNode = createCfgNode(0, currentNumberOfNodes);

    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("x"), createTimesExpr(createRefExpr("x"), createRefExpr(123)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    expectedCfg->statementNodes->push_back(
        createWhileNode(1, createNeqExpr(createRefExpr("x"), createRefExpr(0)), whileStmtLstNode));
    expectedCfg->childrenNodes->push_back(firstWhileCfgChildrenNode);

    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(2, Variable("x"), createTimesExpr(createRefExpr("x"), createRefExpr(123))));
    firstWhileCfgChildrenNode->childrenNodes->push_back(expectedCfg);

    expectedCfg->childrenNodes->push_back(whileDummyCfgNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram15Cfg_complicatedConditional()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(3, currentNumberOfNodes);

    CfgNode* firstWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* whileDummyCfgNode = createCfgNode(0, currentNumberOfNodes);

    ConditionalExpression* notNeqExpression
        = createNotExpr(createNeqExpr(createRefExpr("b"), createPlusExpr(createRefExpr("c"), createRefExpr("x"))));
    ConditionalExpression* eqExpression = createEqExpr(createRefExpr("y"), createRefExpr("z"));
    ConditionalExpression* orExpression = createOrExpr(eqExpression, notNeqExpression);
    Expression* minusExpression
        = createMinusExpr(createModExpr(createRefExpr("x"), createRefExpr(6)), createRefExpr(3));
    ConditionalExpression* gtExpression = createLtExpr(minusExpression, createRefExpr("a"));
    ConditionalExpression* andExpression = createAndExpr(gtExpression, orExpression);
    ConditionalExpression* predicate = createNotExpr(andExpression);

    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("x"), createTimesExpr(createRefExpr("x"), createRefExpr(123)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    expectedCfg->statementNodes->push_back(createWhileNode(1, predicate, whileStmtLstNode));
    expectedCfg->childrenNodes->push_back(firstWhileCfgChildrenNode);

    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(2, Variable("x"), createTimesExpr(createRefExpr("x"), createRefExpr(123))));
    firstWhileCfgChildrenNode->childrenNodes->push_back(expectedCfg);

    expectedCfg->childrenNodes->push_back(whileDummyCfgNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram18Cfg_endWithWhile()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(4, currentNumberOfNodes);

    Expression* xxyyzz = createPlusExpr(createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                                                       createTimesExpr(createRefExpr("y"), createRefExpr("y"))),
                                        createTimesExpr(createRefExpr("z"), createRefExpr("z")));
    expectedCfg->statementNodes->push_back(createAssignNode(1, Variable("dist"), xxyyzz));
    expectedCfg->statementNodes->push_back(createAssignNode(2, Variable("x"), createRefExpr("dist")));
    expectedCfg->statementNodes->push_back(createAssignNode(3, Variable("depth"), createRefExpr("depth")));
    expectedCfg->statementNodes->push_back(createReadNode(4, Variable("p")));

    // Create new nodes for While Statement
    CfgNode* firstWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(4, currentNumberOfNodes);
    CfgNode* whileDummyCfgNode = createCfgNode(0, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgNode);

    List<StatementNode> whileStatements;
    whileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("p"), createRefExpr("x"))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(8, Variable("dist"), createMinusExpr(createRefExpr("x"), createRefExpr(1)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        9, Variable("x"),
        createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                       createDivExpr(createTimesExpr(createRefExpr("y"), createRefExpr("y")), createRefExpr(2))))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    firstWhileCfgNode->statementNodes->push_back(
        createWhileNode(5, createNeqExpr(createRefExpr("x"), createRefExpr("p")), whileStmtLstNode));
    firstWhileCfgNode->childrenNodes->push_back(firstWhileCfgChildrenNode);

    firstWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(6, Variable("p"), createRefExpr("x")));
    firstWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(
        7, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2))));
    firstWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(8, Variable("dist"), createMinusExpr(createRefExpr("x"), createRefExpr(1))));
    firstWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(
        9, Variable("x"),
        createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                       createDivExpr(createTimesExpr(createRefExpr("y"), createRefExpr("y")), createRefExpr(2)))));

    firstWhileCfgChildrenNode->childrenNodes->push_back(firstWhileCfgNode);

    firstWhileCfgNode->childrenNodes->push_back(whileDummyCfgNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

/** CFG BIP Builder **/

std::pair<CfgNode*, size_t> getProgram7CfgBip_computeCentroid()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    // procedure main
    expectedCfg->statementNodes->push_back(createAssignNode(1, Variable("flag"), createRefExpr(0)));

    CfgNode* mainCallNewNode = createCfgNode(1, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(mainCallNewNode);
    mainCallNewNode->statementNodes->push_back(createCallNode(2, "computeCentroid"));

    // main calls computeCentroid
    CfgNode* mainFirstCallNode = createCfgNode(3, currentNumberOfNodes);
    mainCallNewNode->childrenNodes->push_back(mainFirstCallNode);

    mainFirstCallNode->statementNodes->push_back(createAssignNode(10, Variable("count"), createRefExpr(0)));
    mainFirstCallNode->statementNodes->push_back(createAssignNode(11, Variable("cenX"), createRefExpr(0)));
    mainFirstCallNode->statementNodes->push_back(createAssignNode(12, Variable("cenY"), createRefExpr(0)));

    CfgNode* computeCentroidCallNewNode = createCfgNode(1, currentNumberOfNodes);
    mainFirstCallNode->childrenNodes->push_back(computeCentroidCallNewNode);

    computeCentroidCallNewNode->statementNodes->push_back(createCallNode(13, "readPoint"));

    // computeCentroid called readPoint
    CfgNode* computeCentroidFirstCallNode = createCfgNode(1, currentNumberOfNodes);
    computeCentroidCallNewNode->childrenNodes->push_back(computeCentroidFirstCallNode);

    computeCentroidFirstCallNode->statementNodes->push_back(createReadNode(4, Variable("x")));
    computeCentroidFirstCallNode->statementNodes->push_back(createReadNode(5, Variable("y")));

    // End of readPoint, back to computeCentroid
    CfgNode* computeCentroidWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* computeCentroidWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    computeCentroidFirstCallNode->childrenNodes->push_back(computeCentroidWhileCfgNode);
    computeCentroidWhileCfgNode->childrenNodes->push_back(computeCentroidWhileCfgChildrenNode);

    List<StatementNode> computeCentroidWhileStatements;
    computeCentroidWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(15, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1)))));
    computeCentroidWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(16, Variable("cenX"), createPlusExpr(createRefExpr("cenX"), createRefExpr("x")))));
    computeCentroidWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(17, Variable("cenY"), createPlusExpr(createRefExpr("cenY"), createRefExpr("y")))));
    computeCentroidWhileStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(18, "readPoint")));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(computeCentroidWhileStatements);

    computeCentroidWhileCfgNode->statementNodes->push_back(
        createWhileNode(14,
                        createAndExpr(createNeqExpr(createRefExpr("x"), (createRefExpr(0))),
                                      createNeqExpr(createRefExpr("y"), (createRefExpr(0)))),
                        whileStmtLstNode));

    computeCentroidWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(15, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1))));
    computeCentroidWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(16, Variable("cenX"), createPlusExpr(createRefExpr("cenX"), createRefExpr("x"))));
    computeCentroidWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(17, Variable("cenY"), createPlusExpr(createRefExpr("cenY"), createRefExpr("y"))));

    CfgNode* computeCentroidSecondCallNewNode = createCfgNode(1, currentNumberOfNodes);
    computeCentroidWhileCfgChildrenNode->childrenNodes->push_back(computeCentroidSecondCallNewNode);
    computeCentroidSecondCallNewNode->statementNodes->push_back(createCallNode(18, "readPoint"));

    // computeCentroid calls readPoint again, since readPoint has been called, we want to add the current node's
    // child as a child of last node of readPoint
    // Since the child of the current node is already included as readPoint's children, we do not add again
    computeCentroidSecondCallNewNode->childrenNodes->push_back(computeCentroidFirstCallNode);

    // Return to computeCentroid
    CfgNode* computeCentroidWhileDummyNode = createCfgNode(1, currentNumberOfNodes);
    computeCentroidWhileCfgNode->childrenNodes->push_back(computeCentroidWhileDummyNode);

    List<StatementNode> computeCentroidIfStatements;
    List<StatementNode> computeCentroidElseStatements;
    computeCentroidIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("flag"), createRefExpr(1))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(computeCentroidIfStatements);
    computeCentroidElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(21, Variable("cenX"), createDivExpr(createRefExpr("cenX"), createRefExpr("count")))));
    computeCentroidElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(22, Variable("cenY"), createDivExpr(createRefExpr("cenY"), createRefExpr("count")))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(computeCentroidElseStatements);

    computeCentroidWhileDummyNode->statementNodes->push_back(
        createIfNode(19, createEqExpr(createRefExpr("count"), createRefExpr(0)), ifStmtLstNode, elseStmtLstNode));

    // nodes for computeCentroid's if stmt
    CfgNode* ifComputeCentroidCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifComputeCentroidDummyNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseComputeCentroidCfgNode = createCfgNode(2, currentNumberOfNodes);

    computeCentroidWhileDummyNode->childrenNodes->push_back(ifComputeCentroidCfgNode);
    computeCentroidWhileDummyNode->childrenNodes->push_back(elseComputeCentroidCfgNode);

    ifComputeCentroidCfgNode->statementNodes->push_back(createAssignNode(20, Variable("flag"), createRefExpr(1)));
    elseComputeCentroidCfgNode->statementNodes->push_back(
        createAssignNode(21, Variable("cenX"), createDivExpr(createRefExpr("cenX"), createRefExpr("count"))));
    elseComputeCentroidCfgNode->statementNodes->push_back(
        createAssignNode(22, Variable("cenY"), createDivExpr(createRefExpr("cenY"), createRefExpr("count"))));
    ifComputeCentroidCfgNode->childrenNodes->push_back(ifComputeCentroidDummyNode);
    elseComputeCentroidCfgNode->childrenNodes->push_back(ifComputeCentroidDummyNode);

    ifComputeCentroidDummyNode->statementNodes->push_back(
        createAssignNode(23, Variable("normSq"),
                         createPlusExpr(createTimesExpr(createRefExpr("cenX"), createRefExpr("cenX")),
                                        createTimesExpr(createRefExpr("cenY"), createRefExpr("cenY")))));

    // End of computeCentroid, back to main
    CfgNode* mainSecondNewCallNode = createCfgNode(1, currentNumberOfNodes);
    ifComputeCentroidDummyNode->childrenNodes->push_back(mainSecondNewCallNode);
    mainSecondNewCallNode->statementNodes->push_back(createCallNode(3, "printResults"));

    // main called printResults
    CfgNode* mainSecondCallNode = createCfgNode(4, currentNumberOfNodes);
    mainSecondNewCallNode->childrenNodes->push_back(mainSecondCallNode);

    mainSecondCallNode->statementNodes->push_back(createPrintNode(6, Variable("flag")));
    mainSecondCallNode->statementNodes->push_back(createPrintNode(7, Variable("cenX")));
    mainSecondCallNode->statementNodes->push_back(createPrintNode(8, Variable("cenY")));
    mainSecondCallNode->statementNodes->push_back(createPrintNode(9, Variable("normSq")));

    // Last statement Call dummy node
    CfgNode* lastStatementCallDummyNode = createCfgNode(0, currentNumberOfNodes);
    mainSecondCallNode->childrenNodes->push_back(lastStatementCallDummyNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram19CfgBip_multipleProcedures()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    // procedure a
    expectedCfg->statementNodes->push_back(createCallNode(1, "b"));

    // a called b
    CfgNode* aFirstCallNode = createCfgNode(2, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(aFirstCallNode);

    aFirstCallNode->statementNodes->push_back(createReadNode(5, Variable("num")));
    aFirstCallNode->statementNodes->push_back(createAssignNode(6, Variable("factor"), createRefExpr("factor")));

    CfgNode* bCallNewNode = createCfgNode(1, currentNumberOfNodes);
    aFirstCallNode->childrenNodes->push_back(bCallNewNode);

    bCallNewNode->statementNodes->push_back(createCallNode(7, "d"));

    // b called d
    CfgNode* bFirstCallNode = createCfgNode(1, currentNumberOfNodes);
    bCallNewNode->childrenNodes->push_back(bFirstCallNode);

    List<StatementNode> dIfStatements;
    List<StatementNode> dElseStatements;
    dIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("result"), createRefExpr("depth"))));
    dElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("result"), createRefExpr("result"))));

    bFirstCallNode->statementNodes->push_back(createIfNode(16, createGtExpr(createRefExpr("depth"), createRefExpr(0)),
                                                           createStmtlstNode(dIfStatements),
                                                           createStmtlstNode(dElseStatements)));

    // nodes for d's if stmt
    CfgNode* ifDCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDDummyNode = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseDCfgNode = createCfgNode(1, currentNumberOfNodes);

    bFirstCallNode->childrenNodes->push_back(ifDCfgNode);
    bFirstCallNode->childrenNodes->push_back(elseDCfgNode);

    // Duplicate cause unique pointers
    dIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("result"), createRefExpr("depth"))));
    dElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("result"), createRefExpr("result"))));

    ifDCfgNode->statementNodes->push_back(createAssignNode(17, Variable("result"), createRefExpr("depth")));
    elseDCfgNode->statementNodes->push_back(createAssignNode(18, Variable("result"), createRefExpr("result")));
    ifDCfgNode->childrenNodes->push_back(ifDDummyNode);
    elseDCfgNode->childrenNodes->push_back(ifDDummyNode);

    // End of d, back to b, since b has not next stmt, back to a
    CfgNode* aNewCallNode = createCfgNode(1, currentNumberOfNodes);
    ifDDummyNode->childrenNodes->push_back(aNewCallNode);
    aNewCallNode->statementNodes->push_back(createCallNode(2, "c"));

    // a called c
    CfgNode* aSecondCallNode = createCfgNode(3, currentNumberOfNodes);
    aNewCallNode->childrenNodes->push_back(aSecondCallNode);

    aSecondCallNode->statementNodes->push_back(createAssignNode(8, Variable("depth"), createRefExpr(7)));
    aSecondCallNode->statementNodes->push_back(createAssignNode(9, Variable("base"), createRefExpr(10)));
    aSecondCallNode->statementNodes->push_back(createAssignNode(10, Variable("result"), createRefExpr(0)));

    CfgNode* cWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* cWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    aSecondCallNode->childrenNodes->push_back(cWhileCfgNode);
    cWhileCfgNode->childrenNodes->push_back(cWhileCfgChildrenNode);

    // c's if and while statments
    List<StatementNode> cIfStatements;
    List<StatementNode> cElseStatements;
    List<StatementNode> cWhileStatements;
    cIfStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("num"), createDivExpr(createRefExpr("num"), createRefExpr(2)))));
    cElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(14, Variable("result"), createPlusExpr(createRefExpr("num"), createRefExpr("result")))));
    cElseStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(15, "d")));
    cWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(12, createEqExpr(createModExpr(createRefExpr("base"), createRefExpr(2)), createRefExpr(0)),
                     createStmtlstNode(cIfStatements), createStmtlstNode(cElseStatements))));

    cWhileCfgNode->statementNodes->push_back(createWhileNode(11, createGtExpr(createRefExpr("base"), createRefExpr(0)),
                                                             createStmtlstNode(cWhileStatements)));

    // Duplicate cause unique pointers
    cIfStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("num"), createDivExpr(createRefExpr("num"), createRefExpr(2)))));
    cElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(14, Variable("result"), createPlusExpr(createRefExpr("num"), createRefExpr("result")))));
    cElseStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(15, "d")));

    cWhileCfgChildrenNode->statementNodes->push_back(
        createIfNode(12, createEqExpr(createModExpr(createRefExpr("base"), createRefExpr(2)), createRefExpr(0)),
                     createStmtlstNode(cIfStatements), createStmtlstNode(cElseStatements)));

    // nodes for c's if stmt
    CfgNode* ifCCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCDummyNode = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseNewCallCfgNode = createCfgNode(1, currentNumberOfNodes);

    cWhileCfgChildrenNode->childrenNodes->push_back(ifCCfgNode);
    cWhileCfgChildrenNode->childrenNodes->push_back(elseCCfgNode);

    ifCCfgNode->statementNodes->push_back(
        createAssignNode(13, Variable("num"), createDivExpr(createRefExpr("num"), createRefExpr(2))));
    elseCCfgNode->statementNodes->push_back(
        createAssignNode(14, Variable("result"), createPlusExpr(createRefExpr("num"), createRefExpr("result"))));

    ifCCfgNode->childrenNodes->push_back(ifCDummyNode);
    elseCCfgNode->childrenNodes->push_back(elseNewCallCfgNode);

    // c calls d, since d has been called, add the current node as a child of last node of d
    elseNewCallCfgNode->childrenNodes->push_back(bFirstCallNode);
    ifDDummyNode->childrenNodes->push_back(ifCDummyNode);

    // elseNewCallCfgNode->childrenNodes->push_back(ifCDummyNode);
    elseNewCallCfgNode->statementNodes->push_back(createCallNode(15, "d"));

    ifCDummyNode->childrenNodes->push_back(cWhileCfgNode);

    CfgNode* whileCDummyNode = createCfgNode(0, currentNumberOfNodes);
    cWhileCfgNode->childrenNodes->push_back(whileCDummyNode);

    // End of C, back to a
    // New node for stmt after call stmt in the same CFG node
    CfgNode* newNodeForStmtAfterCall = createCfgNode(2, currentNumberOfNodes);
    whileCDummyNode->childrenNodes->push_back(newNodeForStmtAfterCall);
    newNodeForStmtAfterCall->statementNodes->push_back(createPrintNode(3, Variable("num")));
    newNodeForStmtAfterCall->statementNodes->push_back(createPrintNode(4, Variable("result")));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram20Cfg_main()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(3, currentNumberOfNodes);

    // procedure main
    expectedCfg->statementNodes->push_back(createReadNode(1, Variable("steps")));
    expectedCfg->statementNodes->push_back(createCallNode(2, "raymarch"));
    expectedCfg->statementNodes->push_back(createPrintNode(3, Variable("depth")));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram20Cfg_raymarch()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(3, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(createAssignNode(4, Variable("ro"), createRefExpr(13)));
    expectedCfg->statementNodes->push_back(createAssignNode(5, Variable("rd"), createRefExpr(19)));
    expectedCfg->statementNodes->push_back(createReadNode(6, Variable("depth")));

    // Create new nodes for While Statement
    CfgNode* firstWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(3, currentNumberOfNodes);
    CfgNode* whileDummyCfgNode = createCfgNode(0, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgNode);

    List<StatementNode> raymarchWhileStatements;
    raymarchWhileStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("depth"))));
    raymarchWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        9, Variable("po"),
        createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth"))))));
    raymarchWhileStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(10, "spheresdf")));

    firstWhileCfgNode->statementNodes->push_back(createWhileNode(
        7, createLtExpr(createRefExpr("count"), createRefExpr("steps")), createStmtlstNode(raymarchWhileStatements)));
    firstWhileCfgNode->childrenNodes->push_back(firstWhileCfgChildrenNode);

    firstWhileCfgChildrenNode->statementNodes->push_back(createPrintNode(8, Variable("depth")));
    firstWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(
        9, Variable("po"),
        createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth")))));
    firstWhileCfgChildrenNode->statementNodes->push_back(createCallNode(10, "spheresdf"));
    firstWhileCfgNode->childrenNodes->push_back(whileDummyCfgNode);

    CfgNode* ifNewCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);

    List<StatementNode> statements;
    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // If statements
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("done"), createRefExpr("depth"))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("depth"), createPlusExpr(createRefExpr("depth"), createRefExpr("dist")))));

    ifNewCfgNode->statementNodes->push_back(
        createIfNode(11, createLtExpr(createRefExpr("dist"), createRefExpr("epsilon")), createStmtlstNode(ifStatements),
                     createStmtlstNode(elseStatements)));
    firstWhileCfgChildrenNode->childrenNodes->push_back(ifNewCfgNode);

    ifCfgNode->statementNodes->push_back(createAssignNode(12, Variable("done"), createRefExpr("depth")));
    elseCfgNode->statementNodes->push_back(
        createAssignNode(13, Variable("depth"), createPlusExpr(createRefExpr("depth"), createRefExpr("dist"))));

    ifNewCfgNode->childrenNodes->push_back(ifCfgNode);
    ifNewCfgNode->childrenNodes->push_back(elseCfgNode);

    ifCfgNode->childrenNodes->push_back(ifDummyNode);
    elseCfgNode->childrenNodes->push_back(ifDummyNode);

    ifDummyNode->statementNodes->push_back(
        createAssignNode(14, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1))));

    ifDummyNode->childrenNodes->push_back(firstWhileCfgNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram20Cfg_spheresdf()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(4, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(
        createAssignNode(15, Variable("dist"),
                         createPlusExpr(createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                                                       createTimesExpr(createRefExpr("y"), createRefExpr("y"))),
                                        createTimesExpr(createRefExpr("z"), createRefExpr("z")))));
    expectedCfg->statementNodes->push_back(createAssignNode(16, Variable("x"), createRefExpr("dist")));
    expectedCfg->statementNodes->push_back(createAssignNode(17, Variable("depth"), createRefExpr("depth")));
    expectedCfg->statementNodes->push_back(createReadNode(18, Variable("p")));

    // Create new nodes for While Statement
    CfgNode* firstWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(2, currentNumberOfNodes);
    CfgNode* whileDummyCfgNode = createCfgNode(2, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgNode);

    // procedure spheresdf while
    List<StatementNode> spheresdfWhileStatements;
    spheresdfWhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("p"), createRefExpr("x"))));
    spheresdfWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        21, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2)))));

    firstWhileCfgNode->statementNodes->push_back(createWhileNode(
        19, createNeqExpr(createRefExpr("x"), createRefExpr("p")), createStmtlstNode(spheresdfWhileStatements)));
    firstWhileCfgNode->childrenNodes->push_back(firstWhileCfgChildrenNode);

    firstWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(20, Variable("p"), createRefExpr("x")));
    firstWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(
        21, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2))));

    firstWhileCfgChildrenNode->childrenNodes->push_back(firstWhileCfgNode);
    firstWhileCfgNode->childrenNodes->push_back(whileDummyCfgNode);

    whileDummyCfgNode->statementNodes->push_back(
        createAssignNode(22, Variable("dist"), createMinusExpr(createRefExpr("x"), createRefExpr(1))));
    whileDummyCfgNode->statementNodes->push_back(createAssignNode(
        23, Variable("x"),
        createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                       createDivExpr(createTimesExpr(createRefExpr("y"), createRefExpr("y")), createRefExpr(2)))));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram20CfgBip_multipleProceduresSpheresdf()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    // procedure main
    expectedCfg->statementNodes->push_back(createReadNode(1, Variable("steps")));

    CfgNode* mainCallNode = createCfgNode(2, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(mainCallNode);

    mainCallNode->statementNodes->push_back(createCallNode(2, "raymarch"));

    // procedure raymarch
    CfgNode* raymarchCfg = createCfgNode(3, currentNumberOfNodes);
    mainCallNode->childrenNodes->push_back(raymarchCfg);

    raymarchCfg->statementNodes->push_back(createAssignNode(4, Variable("ro"), createRefExpr(13)));
    raymarchCfg->statementNodes->push_back(createAssignNode(5, Variable("rd"), createRefExpr(19)));
    raymarchCfg->statementNodes->push_back(createReadNode(6, Variable("depth")));

    // Create new nodes for While Statement
    CfgNode* raymarchWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* raymarchWhileCfgChildrenNode = createCfgNode(3, currentNumberOfNodes);
    raymarchCfg->childrenNodes->push_back(raymarchWhileCfgNode);

    List<StatementNode> raymarchWhileStatements;
    List<StatementNode> raymarchIfStatements;
    List<StatementNode> raymarchElseStatements;
    // If and else statements
    raymarchIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("done"), createRefExpr("depth"))));
    raymarchElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("depth"), createPlusExpr(createRefExpr("depth"), createRefExpr("dist")))));

    raymarchWhileStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("depth"))));
    raymarchWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        9, Variable("po"),
        createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth"))))));
    raymarchWhileStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(10, "spheresdf")));
    raymarchWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createLtExpr(createRefExpr("dist"), createRefExpr("epsilon")),
                     createStmtlstNode(raymarchIfStatements), createStmtlstNode(raymarchElseStatements))));
    raymarchWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(14, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1)))));

    raymarchWhileCfgNode->statementNodes->push_back(createWhileNode(
        7, createLtExpr(createRefExpr("count"), createRefExpr("steps")), createStmtlstNode(raymarchWhileStatements)));
    raymarchWhileCfgNode->childrenNodes->push_back(raymarchWhileCfgChildrenNode);

    raymarchWhileCfgChildrenNode->statementNodes->push_back(createPrintNode(8, Variable("depth")));
    raymarchWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(
        9, Variable("po"),
        createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth")))));

    CfgNode* raymarchCallNode = createCfgNode(1, currentNumberOfNodes);
    raymarchWhileCfgChildrenNode->childrenNodes->push_back(raymarchCallNode);
    raymarchCallNode->statementNodes->push_back(createCallNode(10, "spheresdf"));

    CfgNode* spheresdfCfg = createCfgNode(4, currentNumberOfNodes);
    raymarchCallNode->childrenNodes->push_back(spheresdfCfg);

    spheresdfCfg->statementNodes->push_back(
        createAssignNode(15, Variable("dist"),
                         createPlusExpr(createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                                                       createTimesExpr(createRefExpr("y"), createRefExpr("y"))),
                                        createTimesExpr(createRefExpr("z"), createRefExpr("z")))));
    spheresdfCfg->statementNodes->push_back(createAssignNode(16, Variable("x"), createRefExpr("dist")));
    spheresdfCfg->statementNodes->push_back(createAssignNode(17, Variable("depth"), createRefExpr("depth")));
    spheresdfCfg->statementNodes->push_back(createReadNode(18, Variable("p")));

    // Create new nodes for While Statement
    CfgNode* spheresdfWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* spheresdfWhileCfgChildrenNode = createCfgNode(2, currentNumberOfNodes);
    CfgNode* spheresdfWhileDummyCfgNode = createCfgNode(2, currentNumberOfNodes);
    spheresdfCfg->childrenNodes->push_back(spheresdfWhileCfgNode);

    // procedure spheresdf while
    List<StatementNode> spheresdfWhileStatements;
    spheresdfWhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("p"), createRefExpr("x"))));
    spheresdfWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        21, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2)))));

    spheresdfWhileCfgNode->statementNodes->push_back(createWhileNode(
        19, createNeqExpr(createRefExpr("x"), createRefExpr("p")), createStmtlstNode(spheresdfWhileStatements)));
    spheresdfWhileCfgNode->childrenNodes->push_back(spheresdfWhileCfgChildrenNode);

    spheresdfWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(20, Variable("p"), createRefExpr("x")));
    spheresdfWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(
        21, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2))));

    spheresdfWhileCfgChildrenNode->childrenNodes->push_back(spheresdfWhileCfgNode);
    spheresdfWhileCfgNode->childrenNodes->push_back(spheresdfWhileDummyCfgNode);

    spheresdfWhileDummyCfgNode->statementNodes->push_back(
        createAssignNode(22, Variable("dist"), createMinusExpr(createRefExpr("x"), createRefExpr(1))));
    spheresdfWhileDummyCfgNode->statementNodes->push_back(createAssignNode(
        23, Variable("x"),
        createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                       createDivExpr(createTimesExpr(createRefExpr("y"), createRefExpr("y")), createRefExpr(2)))));

    // Back to raymarch
    CfgNode* ifNewCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);

    // If and else statements (duplicate cause unique pointers)
    raymarchIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("done"), createRefExpr("depth"))));
    raymarchElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("depth"), createPlusExpr(createRefExpr("depth"), createRefExpr("dist")))));

    ifNewCfgNode->statementNodes->push_back(
        createIfNode(11, createLtExpr(createRefExpr("dist"), createRefExpr("epsilon")),
                     createStmtlstNode(raymarchIfStatements), createStmtlstNode(raymarchElseStatements)));
    spheresdfWhileDummyCfgNode->childrenNodes->push_back(ifNewCfgNode);

    ifCfgNode->statementNodes->push_back(createAssignNode(12, Variable("done"), createRefExpr("depth")));
    elseCfgNode->statementNodes->push_back(
        createAssignNode(13, Variable("depth"), createPlusExpr(createRefExpr("depth"), createRefExpr("dist"))));

    ifNewCfgNode->childrenNodes->push_back(ifCfgNode);
    ifNewCfgNode->childrenNodes->push_back(elseCfgNode);

    ifCfgNode->childrenNodes->push_back(ifDummyNode);
    elseCfgNode->childrenNodes->push_back(ifDummyNode);

    ifDummyNode->statementNodes->push_back(
        createAssignNode(14, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1))));

    ifDummyNode->childrenNodes->push_back(raymarchWhileCfgNode);

    CfgNode* raymarchWhileDummyCfgNode = createCfgNode(0, currentNumberOfNodes);
    raymarchWhileCfgNode->childrenNodes->push_back(raymarchWhileDummyCfgNode);

    // New node for stmt after call stmt in the same CFG node
    CfgNode* newNodeForStmtAfterCall = createCfgNode(1, currentNumberOfNodes);
    raymarchWhileDummyCfgNode->childrenNodes->push_back(newNodeForStmtAfterCall);
    newNodeForStmtAfterCall->statementNodes->push_back(createPrintNode(3, Variable("depth")));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram22CfgBip_whileNestedInWhile()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    // Create new nodes for If Statement
    CfgNode* ifCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode = createCfgNode(1, currentNumberOfNodes);

    // procedure readPoint if & whiles
    List<StatementNode> readPointIfStatements;
    List<StatementNode> readPointElseStatements;
    List<StatementNode> readPointOuterWhileStatements;
    List<StatementNode> readPointInnerWhileStatements;

    readPointIfStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("y"), createPlusExpr(createRefExpr("z"), createRefExpr("x")))));
    readPointElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(3, Variable("y"), createPlusExpr(createRefExpr("x"), createRefExpr("y")))));

    readPointInnerWhileStatements.push_back(
        std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("normSq"))));

    readPointOuterWhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("x"), createRefExpr("y"))));
    readPointOuterWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("y"), createPlusExpr(createRefExpr("normSq"), createRefExpr(3)))));
    readPointOuterWhileStatements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(7, createGtExpr(createRefExpr("normSq"), createRefExpr("3")),
                        createStmtlstNode(readPointInnerWhileStatements))));

    // if
    expectedCfg->statementNodes->push_back(
        createIfNode(1,
                     createAndExpr(createLtExpr(createRefExpr("x"), createRefExpr("y")),
                                   createLtExpr(createRefExpr("y"), createRefExpr("z"))),
                     createStmtlstNode(readPointIfStatements), createStmtlstNode(readPointElseStatements)));
    expectedCfg->childrenNodes->push_back(ifCfgNode);

    ifCfgNode->statementNodes->push_back(
        createAssignNode(2, Variable("y"), createPlusExpr(createRefExpr("z"), createRefExpr("x"))));
    ifCfgNode->childrenNodes->push_back(ifDummyNode);

    ifDummyNode->statementNodes->push_back(createWhileNode(4, createGtExpr(createRefExpr("x"), createRefExpr("y")),
                                                           createStmtlstNode(readPointOuterWhileStatements)));

    // Create new node for outer While Statement
    CfgNode* outerWhileCfgChildrenNode = createCfgNode(2, currentNumberOfNodes);
    ifDummyNode->childrenNodes->push_back(outerWhileCfgChildrenNode);

    outerWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(5, Variable("x"), createRefExpr("y")));
    outerWhileCfgChildrenNode->statementNodes->push_back(
        createAssignNode(6, Variable("y"), createPlusExpr(createRefExpr("normSq"), createRefExpr(3))));

    // Create new nodes for inner While Statement
    CfgNode* innerWhileCfgNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* innerWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    outerWhileCfgChildrenNode->childrenNodes->push_back(innerWhileCfgNode);

    // Duplicate cause unique pointer
    readPointInnerWhileStatements.push_back(
        std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("normSq"))));

    innerWhileCfgNode->statementNodes->push_back(
        createWhileNode(7, createGtExpr(createRefExpr("normSq"), createRefExpr("3")),
                        createStmtlstNode(readPointInnerWhileStatements)));
    innerWhileCfgNode->childrenNodes->push_back(innerWhileCfgChildrenNode);

    innerWhileCfgChildrenNode->statementNodes->push_back(createPrintNode(8, Variable("normSq")));
    innerWhileCfgChildrenNode->childrenNodes->push_back(innerWhileCfgNode);

    CfgNode* innerWhileDummyCfgNode = createCfgNode(0, currentNumberOfNodes);
    innerWhileCfgNode->childrenNodes->push_back(innerWhileDummyCfgNode);
    innerWhileDummyCfgNode->childrenNodes->push_back(ifDummyNode);

    CfgNode* outerWhileDummyCfgNode = createCfgNode(1, currentNumberOfNodes);
    ifDummyNode->childrenNodes->push_back(outerWhileDummyCfgNode);
    outerWhileDummyCfgNode->statementNodes->push_back(createAssignNode(9, Variable("normSq"), createRefExpr("y")));

    CfgNode* elseCfgNode = createCfgNode(1, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(elseCfgNode);
    elseCfgNode->statementNodes->push_back(
        createAssignNode(3, Variable("y"), createPlusExpr(createRefExpr("x"), createRefExpr("y"))));
    elseCfgNode->childrenNodes->push_back(ifDummyNode);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram24CfgBip_nestedIfs()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;
    List<StatementNode> doSmth3IfStatements;
    List<StatementNode> doSmth3ElseStatements;

    List<StatementNode> doSmth4IfStatements;
    List<StatementNode> doSmth4ElseStatements;

    List<StatementNode> doSmth5IfStatements;
    List<StatementNode> doSmth5ElseStatements;

    List<StatementNode> doSmth6IfStatements;
    List<StatementNode> doSmth6ElseStatements;

    List<StatementNode> doSmth7IfStatements;
    List<StatementNode> doSmth7ElseStatements;

    List<StatementNode> doSmth8IfStatements;
    List<StatementNode> doSmth8ElseStatements;

    List<StatementNode> doSmth9IfStatements;
    List<StatementNode> doSmth9ElseStatements;

    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));

    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(
        std::move(std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6)))));

    doSmth3ElseStatements.push_back(std::move(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements)))));

    doSmth2IfStatements.push_back(
        std::move(std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4)))));

    doSmth2ElseStatements.push_back(std::move(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements)))));

    doSmth1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));

    doSmth1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    // Create new nodes for first If Statement
    CfgNode* ifCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode1 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode1 = createCfgNode(1, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(createIfNode(1, createEqExpr(createRefExpr("procedure"), createRefExpr(0)),
                                                        createStmtlstNode(doSmth1IfStatements),
                                                        createStmtlstNode(doSmth1ElseStatements)));
    expectedCfg->childrenNodes->push_back(ifCfgNode1);
    expectedCfg->childrenNodes->push_back(elseCfgNode1);

    ifCfgNode1->statementNodes->push_back(createAssignNode(2, Variable("stmt"), createRefExpr(2)));
    ifCfgNode1->childrenNodes->push_back(ifDummyNode1);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));

    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(
        std::move(std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6)))));

    doSmth3ElseStatements.push_back(std::move(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements)))));

    doSmth2IfStatements.push_back(
        std::move(std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4)))));
    doSmth2ElseStatements.push_back(std::move(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements)))));

    elseCfgNode1->statementNodes->push_back(createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                                                         createStmtlstNode(doSmth2IfStatements),
                                                         createStmtlstNode(doSmth2ElseStatements)));
    // Last statement
    ifDummyNode1->statementNodes->push_back(createAssignNode(20, Variable("stmt"), createRefExpr(20)));

    // Create new nodes for 2nd If Statement
    CfgNode* ifCfgNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode2 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode1->childrenNodes->push_back(ifCfgNode2);
    elseCfgNode1->childrenNodes->push_back(elseCfgNode2);

    ifCfgNode2->statementNodes->push_back(createAssignNode(4, Variable("stmt"), createRefExpr(4)));
    ifCfgNode2->childrenNodes->push_back(ifDummyNode2);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));

    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(
        std::move(std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6)))));

    doSmth3ElseStatements.push_back(std::move(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements)))));

    elseCfgNode2->statementNodes->push_back(createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)),
                                                         createStmtlstNode(doSmth3IfStatements),
                                                         createStmtlstNode(doSmth3ElseStatements)));

    ifDummyNode2->childrenNodes->push_back(ifDummyNode1);

    // Create new nodes for 3rd If Statement
    CfgNode* ifCfgNode3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode3 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode2->childrenNodes->push_back(ifCfgNode3);
    elseCfgNode2->childrenNodes->push_back(elseCfgNode3);

    ifCfgNode3->statementNodes->push_back(createAssignNode(6, Variable("stmt"), createRefExpr(6)));
    ifCfgNode3->childrenNodes->push_back(ifDummyNode3);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));

    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    elseCfgNode3->statementNodes->push_back(createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                                                         createStmtlstNode(doSmth4IfStatements),
                                                         createStmtlstNode(doSmth4ElseStatements)));

    ifDummyNode3->childrenNodes->push_back(ifDummyNode2);

    // Create new nodes for 4th If Statement
    CfgNode* ifCfgNode4 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode4 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode4 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode3->childrenNodes->push_back(ifCfgNode4);
    elseCfgNode3->childrenNodes->push_back(elseCfgNode4);

    ifCfgNode4->statementNodes->push_back(createAssignNode(8, Variable("stmt"), createRefExpr(8)));
    ifCfgNode4->childrenNodes->push_back(ifDummyNode4);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    elseCfgNode4->statementNodes->push_back(createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                                                         createStmtlstNode(doSmth5IfStatements),
                                                         createStmtlstNode(doSmth5ElseStatements)));

    ifDummyNode4->childrenNodes->push_back(ifDummyNode3);

    // Create new nodes for 5th If Statement
    CfgNode* ifCfgNode5 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode5 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode5 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode4->childrenNodes->push_back(ifCfgNode5);
    elseCfgNode4->childrenNodes->push_back(elseCfgNode5);

    ifCfgNode5->statementNodes->push_back(createAssignNode(10, Variable("stmt"), createRefExpr(10)));
    ifCfgNode5->childrenNodes->push_back(ifDummyNode5);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    elseCfgNode5->statementNodes->push_back(createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                                                         createStmtlstNode(doSmth6IfStatements),
                                                         createStmtlstNode(doSmth6ElseStatements)));

    ifDummyNode5->childrenNodes->push_back(ifDummyNode4);

    // Create new nodes for 6th If Statement
    CfgNode* ifCfgNode6 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode6 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode6 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode5->childrenNodes->push_back(ifCfgNode6);
    elseCfgNode5->childrenNodes->push_back(elseCfgNode6);

    ifCfgNode6->statementNodes->push_back(createAssignNode(12, Variable("stmt"), createRefExpr(12)));
    ifCfgNode6->childrenNodes->push_back(ifDummyNode6);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    elseCfgNode6->statementNodes->push_back(createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                                                         createStmtlstNode(doSmth7IfStatements),
                                                         createStmtlstNode(doSmth7ElseStatements)));

    ifDummyNode6->childrenNodes->push_back(ifDummyNode5);

    // Create new nodes for 7th If Statement
    CfgNode* ifCfgNode7 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode7 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode7 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode6->childrenNodes->push_back(ifCfgNode7);
    elseCfgNode6->childrenNodes->push_back(elseCfgNode7);

    ifCfgNode7->statementNodes->push_back(createAssignNode(14, Variable("stmt"), createRefExpr(14)));
    ifCfgNode7->childrenNodes->push_back(ifDummyNode7);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    elseCfgNode7->statementNodes->push_back(createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                                                         createStmtlstNode(doSmth8IfStatements),
                                                         createStmtlstNode(doSmth8ElseStatements)));

    ifDummyNode7->childrenNodes->push_back(ifDummyNode6);

    // Create new nodes for 8th If Statement
    CfgNode* ifCfgNode8 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode8 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode8 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode7->childrenNodes->push_back(ifCfgNode8);
    elseCfgNode7->childrenNodes->push_back(elseCfgNode8);

    ifCfgNode8->statementNodes->push_back(createAssignNode(16, Variable("stmt"), createRefExpr(16)));
    ifCfgNode8->childrenNodes->push_back(ifDummyNode8);

    // Duplicate cause unique pointers
    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    elseCfgNode8->statementNodes->push_back(createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                                                         createStmtlstNode(doSmth9IfStatements),
                                                         createStmtlstNode(doSmth9ElseStatements)));

    ifDummyNode8->childrenNodes->push_back(ifDummyNode7);

    // Create new nodes for 9th If Statement
    CfgNode* ifCfgNode9 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode9 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode9 = createCfgNode(1, currentNumberOfNodes);

    elseCfgNode8->childrenNodes->push_back(ifCfgNode9);
    elseCfgNode8->childrenNodes->push_back(elseCfgNode9);

    ifCfgNode9->statementNodes->push_back(createAssignNode(18, Variable("stmt"), createRefExpr(18)));
    ifCfgNode9->childrenNodes->push_back(ifDummyNode9);

    elseCfgNode9->statementNodes->push_back(createAssignNode(19, Variable("stmt"), createRefExpr(19)));
    elseCfgNode9->childrenNodes->push_back(ifDummyNode9);

    ifDummyNode9->childrenNodes->push_back(ifDummyNode8);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram25CfgBip_nestedIfsInWhile()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    // procedure doSmth while & ifs
    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;
    List<StatementNode> doSmth3IfStatements;
    List<StatementNode> doSmth3ElseStatements;

    List<StatementNode> doSmth4IfStatements;
    List<StatementNode> doSmth4ElseStatements;

    List<StatementNode> doSmth5IfStatements;
    List<StatementNode> doSmth5ElseStatements;

    List<StatementNode> doSmth6IfStatements;
    List<StatementNode> doSmth6ElseStatements;

    List<StatementNode> doSmth7IfStatements;
    List<StatementNode> doSmth7ElseStatements;

    List<StatementNode> doSmth8IfStatements;
    List<StatementNode> doSmth8ElseStatements;

    List<StatementNode> doSmthWhileStatements;

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));

    doSmth3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));

    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6))));

    doSmth2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));

    doSmth1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4))));

    doSmth1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    doSmthWhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));

    doSmthWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth1IfStatements),
                     createStmtlstNode(doSmth1ElseStatements))));

    // Create new nodes for While Statement
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(3, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgChildrenNode);

    expectedCfg->statementNodes->push_back(createWhileNode(1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)),
                                                           createStmtlstNode(doSmthWhileStatements)));

    firstWhileCfgChildrenNode->statementNodes->push_back(createAssignNode(2, Variable("stmt"), createRefExpr(2)));

    // Create new nodes for first If Statement
    CfgNode* ifNewCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode1 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    firstWhileCfgChildrenNode->childrenNodes->push_back(ifNewCfgNode1);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));
    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));
    doSmth3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));
    doSmth3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));
    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6))));
    doSmth2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));
    doSmth1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4))));
    doSmth1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    ifNewCfgNode1->statementNodes->push_back(createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                                                          createStmtlstNode(doSmth1IfStatements),
                                                          createStmtlstNode(doSmth1ElseStatements)));
    ifNewCfgNode1->childrenNodes->push_back(ifCfgNode1);
    ifNewCfgNode1->childrenNodes->push_back(elseCfgNode1);

    ifCfgNode1->statementNodes->push_back(createAssignNode(4, Variable("stmt"), createRefExpr(4)));
    ifCfgNode1->childrenNodes->push_back(ifDummyNode1);

    ifDummyNode1->childrenNodes->push_back(expectedCfg);

    // Create new nodes for second If Statement
    CfgNode* ifCfgNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode2 = createCfgNode(1, currentNumberOfNodes);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));
    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));
    doSmth3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));
    doSmth3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));
    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6))));
    doSmth2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));

    elseCfgNode1->statementNodes->push_back(createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)),
                                                         createStmtlstNode(doSmth2IfStatements),
                                                         createStmtlstNode(doSmth2ElseStatements)));
    elseCfgNode1->childrenNodes->push_back(ifCfgNode2);
    elseCfgNode1->childrenNodes->push_back(elseCfgNode2);

    ifCfgNode2->statementNodes->push_back(createAssignNode(6, Variable("stmt"), createRefExpr(6)));
    ifCfgNode2->childrenNodes->push_back(ifDummyNode2);

    ifDummyNode2->childrenNodes->push_back(ifDummyNode1);

    // Create new nodes for third If Statement
    CfgNode* ifCfgNode3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode3 = createCfgNode(1, currentNumberOfNodes);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));
    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));
    doSmth3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));
    doSmth3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));

    elseCfgNode2->statementNodes->push_back(createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                                                         createStmtlstNode(doSmth3IfStatements),
                                                         createStmtlstNode(doSmth3ElseStatements)));
    elseCfgNode2->childrenNodes->push_back(ifCfgNode3);
    elseCfgNode2->childrenNodes->push_back(elseCfgNode3);

    ifCfgNode3->statementNodes->push_back(createAssignNode(8, Variable("stmt"), createRefExpr(8)));
    ifCfgNode3->childrenNodes->push_back(ifDummyNode3);

    ifDummyNode3->childrenNodes->push_back(ifDummyNode2);

    // Create new nodes for forth If Statement
    CfgNode* ifCfgNode4 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode4 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode4 = createCfgNode(1, currentNumberOfNodes);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));
    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    elseCfgNode3->statementNodes->push_back(createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                                                         createStmtlstNode(doSmth4IfStatements),
                                                         createStmtlstNode(doSmth4ElseStatements)));
    elseCfgNode3->childrenNodes->push_back(ifCfgNode4);
    elseCfgNode3->childrenNodes->push_back(elseCfgNode4);

    ifCfgNode4->statementNodes->push_back(createAssignNode(10, Variable("stmt"), createRefExpr(10)));
    ifCfgNode4->childrenNodes->push_back(ifDummyNode4);

    ifDummyNode4->childrenNodes->push_back(ifDummyNode3);

    // Create new nodes for fifth If Statement
    CfgNode* ifCfgNode5 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode5 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode5 = createCfgNode(1, currentNumberOfNodes);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    elseCfgNode4->statementNodes->push_back(createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                                                         createStmtlstNode(doSmth5IfStatements),
                                                         createStmtlstNode(doSmth5ElseStatements)));
    elseCfgNode4->childrenNodes->push_back(ifCfgNode5);
    elseCfgNode4->childrenNodes->push_back(elseCfgNode5);

    ifCfgNode5->statementNodes->push_back(createAssignNode(12, Variable("stmt"), createRefExpr(12)));
    ifCfgNode5->childrenNodes->push_back(ifDummyNode5);

    ifDummyNode5->childrenNodes->push_back(ifDummyNode4);

    // Create new nodes for sixth If Statement
    CfgNode* ifCfgNode6 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode6 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode6 = createCfgNode(1, currentNumberOfNodes);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    elseCfgNode5->statementNodes->push_back(createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                                                         createStmtlstNode(doSmth6IfStatements),
                                                         createStmtlstNode(doSmth6ElseStatements)));
    elseCfgNode5->childrenNodes->push_back(ifCfgNode6);
    elseCfgNode5->childrenNodes->push_back(elseCfgNode6);

    ifCfgNode6->statementNodes->push_back(createAssignNode(14, Variable("stmt"), createRefExpr(14)));
    ifCfgNode6->childrenNodes->push_back(ifDummyNode6);

    ifDummyNode6->childrenNodes->push_back(ifDummyNode5);

    // Create new nodes for seventh If Statement
    CfgNode* ifCfgNode7 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode7 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode7 = createCfgNode(1, currentNumberOfNodes);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    elseCfgNode6->statementNodes->push_back(createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                                                         createStmtlstNode(doSmth7IfStatements),
                                                         createStmtlstNode(doSmth7ElseStatements)));
    elseCfgNode6->childrenNodes->push_back(ifCfgNode7);
    elseCfgNode6->childrenNodes->push_back(elseCfgNode7);

    ifCfgNode7->statementNodes->push_back(createAssignNode(16, Variable("stmt"), createRefExpr(16)));
    ifCfgNode7->childrenNodes->push_back(ifDummyNode7);

    ifDummyNode7->childrenNodes->push_back(ifDummyNode6);

    // Create new nodes for eighth If Statement
    CfgNode* ifCfgNode8 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode8 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode8 = createCfgNode(1, currentNumberOfNodes);

    // Duplicates cause unique pointers
    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    elseCfgNode7->statementNodes->push_back(createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                                                         createStmtlstNode(doSmth8IfStatements),
                                                         createStmtlstNode(doSmth8ElseStatements)));
    elseCfgNode7->childrenNodes->push_back(ifCfgNode8);
    elseCfgNode7->childrenNodes->push_back(elseCfgNode8);

    ifCfgNode8->statementNodes->push_back(createAssignNode(18, Variable("stmt"), createRefExpr(18)));
    ifCfgNode8->childrenNodes->push_back(ifDummyNode8);

    ifDummyNode8->childrenNodes->push_back(ifDummyNode7);

    elseCfgNode8->statementNodes->push_back(createAssignNode(19, Variable("stmt"), createRefExpr(19)));
    elseCfgNode8->childrenNodes->push_back(ifDummyNode8);

    // Dummy node for first while
    CfgNode* whileDummyCfgNode = createCfgNode(0, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(whileDummyCfgNode);
    whileDummyCfgNode->statementNodes->push_back(createAssignNode(20, Variable("stmt"), createRefExpr(20)));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram26CfgBip_nestedInterleavingIfsInWhile()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    List<StatementNode> doSmthWhileStatements;

    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;
    List<StatementNode> doSmth3IfStatements;
    List<StatementNode> doSmth3ElseStatements;

    List<StatementNode> doSmth4IfStatements;
    List<StatementNode> doSmth4ElseStatements;

    List<StatementNode> doSmth5IfStatements;
    List<StatementNode> doSmth5ElseStatements;

    List<StatementNode> doSmth6IfStatements;
    List<StatementNode> doSmth6ElseStatements;

    List<StatementNode> doSmth7IfStatements;
    List<StatementNode> doSmth7ElseStatements;

    List<StatementNode> doSmth8IfStatements;
    List<StatementNode> doSmth8ElseStatements;

    List<StatementNode> doSmth9IfStatements;
    List<StatementNode> doSmth9ElseStatements;

    List<StatementNode> doSmth10IfStatements;
    List<StatementNode> doSmth10ElseStatements;

    doSmth1ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(22))));

    doSmth2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(21))));

    doSmth3ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(20))));

    doSmth4ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth5ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));

    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));

    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));

    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(6, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));

    doSmth2IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));

    doSmth1IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    doSmthWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(2, createEqExpr(createRefExpr("stmt"), createRefExpr(2)), createStmtlstNode(doSmth1IfStatements),
                     createStmtlstNode(doSmth1ElseStatements))));

    // Create new nodes for While Statement
    CfgNode* firstWhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    expectedCfg->childrenNodes->push_back(firstWhileCfgChildrenNode);
    expectedCfg->statementNodes->push_back(createWhileNode(1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)),
                                                           createStmtlstNode(doSmthWhileStatements)));

    // Create new nodes for If Statements
    CfgNode* ifCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode4 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode5 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode6 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode7 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode8 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode9 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode10 = createCfgNode(1, currentNumberOfNodes);

    CfgNode* ifDummyNode10 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode9 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode8 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode7 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode6 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode5 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode4 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode1 = createCfgNode(0, currentNumberOfNodes);

    CfgNode* elseCfgNode10 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode9 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode8 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode7 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode6 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode5 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode4 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode1 = createCfgNode(1, currentNumberOfNodes);

    CfgNode* firstWhileCfgDummyNode = createCfgNode(1, currentNumberOfNodes);

    // First If
    // Duplicates cause unique pointers
    doSmth1ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(22))));
    doSmth2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(21))));
    doSmth3ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(20))));
    doSmth4ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth5ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));
    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));
    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(6, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));
    doSmth3IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));
    doSmth2IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));
    doSmth1IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    firstWhileCfgChildrenNode->statementNodes->push_back(
        createIfNode(2, createEqExpr(createRefExpr("stmt"), createRefExpr(2)), createStmtlstNode(doSmth1IfStatements),
                     createStmtlstNode(doSmth1ElseStatements)));
    firstWhileCfgChildrenNode->childrenNodes->push_back(ifCfgNode1);
    firstWhileCfgChildrenNode->childrenNodes->push_back(elseCfgNode1);
    elseCfgNode1->statementNodes->push_back(createAssignNode(22, Variable("stmt"), createRefExpr(22)));
    elseCfgNode1->childrenNodes->push_back(ifDummyNode1);
    ifDummyNode1->childrenNodes->push_back(expectedCfg);

    // Second If
    // Duplicates cause unique pointers
    doSmth2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(21))));
    doSmth3ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(20))));
    doSmth4ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth5ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));
    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));
    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(6, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));
    doSmth3IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));
    doSmth2IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));

    ifCfgNode1->statementNodes->push_back(createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                                                       createStmtlstNode(doSmth2IfStatements),
                                                       createStmtlstNode(doSmth2ElseStatements)));
    ifCfgNode1->childrenNodes->push_back(ifCfgNode2);
    ifCfgNode1->childrenNodes->push_back(elseCfgNode2);
    elseCfgNode2->statementNodes->push_back(createAssignNode(21, Variable("stmt"), createRefExpr(21)));
    elseCfgNode2->childrenNodes->push_back(ifDummyNode2);
    ifDummyNode2->childrenNodes->push_back(ifDummyNode1);

    // Third If
    // Duplicates cause unique pointers
    doSmth3ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(20))));
    doSmth4ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth5ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));
    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));
    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(6, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));
    doSmth3IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));

    ifCfgNode2->statementNodes->push_back(createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(4)),
                                                       createStmtlstNode(doSmth3IfStatements),
                                                       createStmtlstNode(doSmth3ElseStatements)));
    ifCfgNode2->childrenNodes->push_back(ifCfgNode3);
    ifCfgNode2->childrenNodes->push_back(elseCfgNode3);
    elseCfgNode3->statementNodes->push_back(createAssignNode(20, Variable("stmt"), createRefExpr(20)));
    elseCfgNode3->childrenNodes->push_back(ifDummyNode3);
    ifDummyNode3->childrenNodes->push_back(ifDummyNode2);

    // Forth If
    // Duplicates cause unique pointers
    doSmth4ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));
    doSmth5ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));
    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));
    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));
    doSmth4IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(6, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    ifCfgNode3->statementNodes->push_back(createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)),
                                                       createStmtlstNode(doSmth4IfStatements),
                                                       createStmtlstNode(doSmth4ElseStatements)));
    ifCfgNode3->childrenNodes->push_back(ifCfgNode4);
    ifCfgNode3->childrenNodes->push_back(elseCfgNode4);
    elseCfgNode4->statementNodes->push_back(createAssignNode(19, Variable("stmt"), createRefExpr(19)));
    elseCfgNode4->childrenNodes->push_back(ifDummyNode4);
    ifDummyNode4->childrenNodes->push_back(ifDummyNode3);

    // Fifth If
    // Duplicates cause unique pointers
    doSmth5ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));
    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));
    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));
    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));
    doSmth5IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    ifCfgNode4->statementNodes->push_back(createIfNode(6, createEqExpr(createRefExpr("stmt"), createRefExpr(6)),
                                                       createStmtlstNode(doSmth5IfStatements),
                                                       createStmtlstNode(doSmth5ElseStatements)));
    ifCfgNode4->childrenNodes->push_back(ifCfgNode5);
    ifCfgNode4->childrenNodes->push_back(elseCfgNode5);
    elseCfgNode5->statementNodes->push_back(createAssignNode(18, Variable("stmt"), createRefExpr(18)));
    elseCfgNode5->childrenNodes->push_back(ifDummyNode5);
    ifDummyNode5->childrenNodes->push_back(ifDummyNode4);

    // Sixth If
    // Duplicates cause unique pointers
    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));
    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));
    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));
    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    ifCfgNode5->statementNodes->push_back(createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)),
                                                       createStmtlstNode(doSmth6IfStatements),
                                                       createStmtlstNode(doSmth6ElseStatements)));
    ifCfgNode5->childrenNodes->push_back(ifCfgNode6);
    ifCfgNode5->childrenNodes->push_back(elseCfgNode6);
    elseCfgNode6->statementNodes->push_back(createAssignNode(17, Variable("stmt"), createRefExpr(17)));
    elseCfgNode6->childrenNodes->push_back(ifDummyNode6);
    ifDummyNode6->childrenNodes->push_back(ifDummyNode5);

    // Seventh If
    // Duplicates cause unique pointers
    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));
    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    ifCfgNode6->statementNodes->push_back(createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)),
                                                       createStmtlstNode(doSmth7IfStatements),
                                                       createStmtlstNode(doSmth7ElseStatements)));
    ifCfgNode6->childrenNodes->push_back(ifCfgNode7);
    ifCfgNode6->childrenNodes->push_back(elseCfgNode7);
    elseCfgNode7->statementNodes->push_back(createAssignNode(16, Variable("stmt"), createRefExpr(16)));
    elseCfgNode7->childrenNodes->push_back(ifDummyNode7);
    ifDummyNode7->childrenNodes->push_back(ifDummyNode6);

    // Eighth If
    // Duplicates cause unique pointers
    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));
    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    ifCfgNode7->statementNodes->push_back(createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)),
                                                       createStmtlstNode(doSmth8IfStatements),
                                                       createStmtlstNode(doSmth8ElseStatements)));
    ifCfgNode7->childrenNodes->push_back(ifCfgNode8);
    ifCfgNode7->childrenNodes->push_back(elseCfgNode8);
    elseCfgNode8->statementNodes->push_back(createAssignNode(15, Variable("stmt"), createRefExpr(15)));
    elseCfgNode8->childrenNodes->push_back(ifDummyNode8);
    ifDummyNode8->childrenNodes->push_back(ifDummyNode7);

    // Ninth If
    // Duplicates cause unique pointers
    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));
    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));

    ifCfgNode8->statementNodes->push_back(createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                                                       createStmtlstNode(doSmth9IfStatements),
                                                       createStmtlstNode(doSmth9ElseStatements)));
    ifCfgNode8->childrenNodes->push_back(ifCfgNode9);
    ifCfgNode8->childrenNodes->push_back(elseCfgNode9);
    elseCfgNode9->statementNodes->push_back(createAssignNode(14, Variable("stmt"), createRefExpr(14)));
    elseCfgNode9->childrenNodes->push_back(ifDummyNode9);
    ifDummyNode9->childrenNodes->push_back(ifDummyNode8);

    // Tenth If
    // Duplicates cause unique pointers
    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));
    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    ifCfgNode9->statementNodes->push_back(createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                                                       createStmtlstNode(doSmth10IfStatements),
                                                       createStmtlstNode(doSmth10ElseStatements)));
    ifCfgNode9->childrenNodes->push_back(ifCfgNode10);
    ifCfgNode9->childrenNodes->push_back(elseCfgNode10);
    elseCfgNode10->statementNodes->push_back(createAssignNode(13, Variable("stmt"), createRefExpr(13)));
    elseCfgNode10->childrenNodes->push_back(ifDummyNode10);
    ifDummyNode10->childrenNodes->push_back(ifDummyNode9);

    ifCfgNode10->statementNodes->push_back(createAssignNode(12, Variable("stmt"), createRefExpr(12)));
    ifCfgNode10->childrenNodes->push_back(ifDummyNode10);

    // While dummy
    expectedCfg->childrenNodes->push_back(firstWhileCfgDummyNode);
    firstWhileCfgDummyNode->statementNodes->push_back(createAssignNode(23, Variable("stmt"), createRefExpr(23)));

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram27CfgBip_ifInIf()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;

    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4))));

    doSmth2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("stmt"), createRefExpr(5))));

    doSmth1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));

    doSmth1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    // Create new nodes for first If statement
    CfgNode* ifCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode1 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode1 = createCfgNode(0, currentNumberOfNodes);

    // Create new nodes for second If statement
    CfgNode* ifCfgNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode2 = createCfgNode(1, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(createIfNode(1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)),
                                                        createStmtlstNode(doSmth1IfStatements),
                                                        createStmtlstNode(doSmth1ElseStatements)));
    expectedCfg->childrenNodes->push_back(ifCfgNode1);
    expectedCfg->childrenNodes->push_back(elseCfgNode1);

    ifCfgNode1->statementNodes->push_back(createAssignNode(2, Variable("stmt"), createRefExpr(2)));
    ifCfgNode1->childrenNodes->push_back(ifDummyNode1);

    // Duplicates cause unique pointers
    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4))));

    doSmth2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("stmt"), createRefExpr(5))));

    elseCfgNode1->statementNodes->push_back(createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)),
                                                         createStmtlstNode(doSmth2IfStatements),
                                                         createStmtlstNode(doSmth2ElseStatements)));
    elseCfgNode1->childrenNodes->push_back(ifCfgNode2);
    elseCfgNode1->childrenNodes->push_back(elseCfgNode2);

    ifCfgNode2->statementNodes->push_back(createAssignNode(4, Variable("stmt"), createRefExpr(4)));
    ifCfgNode2->childrenNodes->push_back(ifDummyNode2);
    elseCfgNode2->statementNodes->push_back(createAssignNode(5, Variable("stmt"), createRefExpr(5)));
    elseCfgNode2->childrenNodes->push_back(ifDummyNode2);

    ifDummyNode2->childrenNodes->push_back(ifDummyNode1);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}

std::pair<CfgNode*, size_t> getProgram28CfgBip_combineIfInIfAndNested3AndNested2()
{
    size_t currentNumberOfNodes = -1;
    CfgNode* expectedCfg = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* ifCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* newCallNode1 = createCfgNode(1, currentNumberOfNodes);

    expectedCfg->statementNodes->push_back(createIfNode(1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)),
                                                        createStmtlstNode(ifInIf1IfStatements),
                                                        createStmtlstNode(ifInIf1ElseStatements)));
    expectedCfg->childrenNodes->push_back(ifCfgNode1);
    ifCfgNode1->statementNodes->push_back(createAssignNode(2, Variable("stmt"), createRefExpr(2)));
    ifCfgNode1->childrenNodes->push_back(newCallNode1);
    newCallNode1->statementNodes->push_back(createCallNode(3, "nested3"));

    CfgNode* newCallNodeForNested3 = createCfgNode(1, currentNumberOfNodes);
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
    CfgNode* nested3WhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    newCallNodeForNested3->childrenNodes->push_back(nested3WhileCfgChildrenNode);
    newCallNodeForNested3->statementNodes->push_back(createWhileNode(
        8, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(nested3WhileStatements)));

    // Create new nodes for If Statements
    CfgNode* ifCfgNode1Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode2Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode3Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode4Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode5Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode6Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode7Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode8Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode9Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode10Nested3 = createCfgNode(1, currentNumberOfNodes);

    CfgNode* ifDummyNode10Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode9Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode8Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode7Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode6Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode5Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode4Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode3Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode2Nested3 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* ifDummyNode1Nested3 = createCfgNode(0, currentNumberOfNodes);

    CfgNode* elseCfgNode10Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode9Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode8Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode7Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode6Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode5Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode4Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode3Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode2Nested3 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* elseCfgNode1Nested3 = createCfgNode(1, currentNumberOfNodes);

    CfgNode* nested3WhileCfgDummyNode = createCfgNode(1, currentNumberOfNodes);
    CfgNode* nested3NewCallNode = createCfgNode(1, currentNumberOfNodes);

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

    CfgNode* newCallNodeForNested2 = createCfgNode(1, currentNumberOfNodes);
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
    CfgNode* nested2WhileCfgChildrenNode = createCfgNode(1, currentNumberOfNodes);
    newCallNodeForNested2->childrenNodes->push_back(nested2WhileCfgChildrenNode);

    newCallNodeForNested2->statementNodes->push_back(createWhileNode(
        32, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(nested2WhileStatements)));

    nested2WhileCfgChildrenNode->statementNodes->push_back(createAssignNode(33, Variable("stmt"), createRefExpr(2)));

    // Create new nodes for first If Statement
    CfgNode* ifNewCfgNode1Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifCfgNode1Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode1Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode1Nested2 = createCfgNode(1, currentNumberOfNodes);
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
    CfgNode* ifCfgNode2Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode2Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode2Nested2 = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* ifCfgNode3Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode3Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode3Nested2 = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* ifCfgNode4Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode4Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode4Nested2 = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* ifCfgNode5Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode5Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode5Nested2 = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* ifCfgNode6Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode6Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode6Nested2 = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* ifCfgNode7Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode7Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode7Nested2 = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* ifCfgNode8Nested2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode8Nested2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode8Nested2 = createCfgNode(1, currentNumberOfNodes);

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
    CfgNode* whileDummyCfgNodeNested2 = createCfgNode(0, currentNumberOfNodes);
    newCallNodeForNested2->childrenNodes->push_back(whileDummyCfgNodeNested2);
    whileDummyCfgNodeNested2->statementNodes->push_back(createAssignNode(51, Variable("stmt"), createRefExpr(20)));

    /** Back to procedure nested 3, since nested 3 is finshed, back to procedure ifInIf **/
    CfgNode* ifDummyNode1 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode1 = createCfgNode(1, currentNumberOfNodes);
    whileDummyCfgNodeNested2->childrenNodes->push_back(ifDummyNode1);

    // Create new nodes for second If statement
    CfgNode* ifCfgNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* newCallNode2 = createCfgNode(1, currentNumberOfNodes);
    CfgNode* ifDummyNode2 = createCfgNode(0, currentNumberOfNodes);
    CfgNode* elseCfgNode2 = createCfgNode(1, currentNumberOfNodes);

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
    newCallNode2->childrenNodes->push_back(newCallNodeForNested2);
    // Last node of nested 2
    whileDummyCfgNodeNested2->childrenNodes->push_back(ifDummyNode2);

    elseCfgNode2->statementNodes->push_back(createAssignNode(7, Variable("stmt"), createRefExpr(5)));
    elseCfgNode2->childrenNodes->push_back(ifDummyNode2);

    ifDummyNode2->childrenNodes->push_back(ifDummyNode1);

    return std::make_pair(expectedCfg, currentNumberOfNodes);
}