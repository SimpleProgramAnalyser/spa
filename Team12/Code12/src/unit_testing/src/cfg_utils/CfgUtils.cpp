/**
 * Implementation of methods to generate Control Flow Graph, to aid in unit testing.
 *
 * Methods labelled with the same number describe the
 * same program in different forms.
 */

#include "CfgUtils.h"

#include "ast/AstLibrary.h"
#include "cfg/CfgBuilder.h"
#include "cfg/CfgTypes.h"

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
