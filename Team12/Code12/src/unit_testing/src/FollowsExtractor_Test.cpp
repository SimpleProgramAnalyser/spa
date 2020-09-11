/**
 * Tests for the Follows extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "catch.hpp"
#include "frontend/designExtractor/FollowsExtractor.cpp"

/*
Sample SIMPLE program used:

procedure factorials {
    limit = 100;
    current = 0;
    factorial = 0;
    while (current < limit) {
        if (current == 0) then {
            factorial = 1;
        } else {
            factorial = current * factorial;
        }
        current = current + 1;
    }
    answer = factorial;
}

Annotated with statement numbers:
procedure factorials {
    limit = 100;                             // 1
    current = 0;                             // 2
    factorial = 0;                           // 3
    while (current < limit) {                // 4
        if (current == 0) then {             // 5
            factorial = 1;                   // 6
        } else {                             //
            factorial = current * factorial; // 7
        }                                    //
        current = current + 1;               // 8
    }                                        //
    answer = factorial;                      // 9
}

*/
TEST_CASE("Follows extractor works for if and else statements nested in while")
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
    ProcedureNode* computeProcedure = createProcedureNode("factorials", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* constructedAst = createProgramNode("SIMPLE program", procedureList, 9);

    FollowsList* followsList = extractFollowsReturnAdjacencyList(*constructedAst);
    //        Follows relationships: 1  2  3  4  5
    //                               |  |  |  |  |
    //                               V  V  V  V  V
    FollowsList expectedResult = {0, 2, 3, 4, 9, 8, 0, 0, 0, 0};

    REQUIRE(expectedResult == *followsList);
}
