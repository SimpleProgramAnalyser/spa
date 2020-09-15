/**
 * Tests for the Modifies extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "ast/AstLibrary.h"
#include "catch.hpp"
#include "frontend/designExtractor/ModifiesExtractor.cpp"

/*
Sample SIMPLE program modified:

procedure a {
    call b;
    call c;
    print num;
    print result;
}

procedure b {
    read num;
    factor = factor;
    call d;
}

procedure c {
    depth = 7;
    base = 10;
    result = 0;
    while (base > 0) {
        if (base % 2 == 0) then {
            num = num / 2;
        } else {
            result = num + result;
            call d;
        }
    }
}

procedure d {
    if (depth > 0) {
        result = depth;
    } else {
        result = result;
    }
}

Annotated with statement numbers:
procedure a {
1.  call b;
2.  call c;
3.  print num;
4.  print result;
}

procedure b {
5.  read num;
6.  factor = factor;
7.  call d;
}

procedure c {
8.  depth = 7;
9.  base = 10;
10. result = 0;
11. while (base > 0) {
12.     if (base % 2 == 0) then {
13.         num = num / 2;
        } else {
14.         result = num + result;
15.         call d;
        }
    }
}

procedure d {
16. if (depth > 0) {
17.     result = depth;
    } else {
18.     result = result;
    }
}
*/
TEST_CASE("Modifies extractor works for multiple procedures, multiple statements")
{
    List<StatementNode> aStmts;
    List<StatementNode> bStmts;
    List<StatementNode> cStmts;
    List<StatementNode> dStmts;

    List<ProcedureNode> procedureList;

    // procedure a
    aStmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(1, "b")));
    aStmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(2, "c")));
    aStmts.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(3, Variable("num"))));
    aStmts.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(4, Variable("result"))));
    StmtlstNode* aStmtLstNode = createStmtlstNode(aStmts);
    ProcedureNode* aProc = createProcedureNode("a", aStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(aProc));

    // procedure b
    bStmts.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(5, Variable("num"))));
    bStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factor"), createRefExpr("factor"))));
    bStmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(7, "d")));
    StmtlstNode* bStmtLstNode = createStmtlstNode(bStmts);
    ProcedureNode* bProc = createProcedureNode("b", bStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(bProc));

    // procedure c while
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

    // procedure c
    cStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("depth"), createRefExpr(7))));
    cStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("base"), createRefExpr(10))));
    cStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("result"), createRefExpr(0))));
    cStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        11, createGtExpr(createRefExpr("base"), createRefExpr(0)), createStmtlstNode(cWhileStatements))));
    StmtlstNode* cStmtLstNode = createStmtlstNode(cStmts);
    ProcedureNode* cProc = createProcedureNode("c", cStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(cProc));

    // procedure d
    List<StatementNode> dIfStatements;
    List<StatementNode> dElseStatements;
    dIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("result"), createRefExpr("depth"))));
    dElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("result"), createRefExpr("result"))));
    dStmts.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createGtExpr(createRefExpr("depth"), createRefExpr(0)), createStmtlstNode(dIfStatements),
                     createStmtlstNode(dElseStatements))));
    StmtlstNode* dStmtLstNode = createStmtlstNode(dStmts);
    ProcedureNode* dProc = createProcedureNode("d", dStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(dProc));

    ProgramNode* programNode = createProgramNode("a", procedureList, 18);
    ProcedureModifiesMap actualMap = extractModifiesReturnMap(*programNode, std::vector<int>{3, 2, 1, 0});

    ProcedureModifiesMap expectedMap;
    VariablesSet aModifies{"base", "depth", "factor", "num", "result"};
    VariablesSet bModifies{"factor", "result", "num"};
    VariablesSet cModifies{"base", "depth", "num", "result"};
    VariablesSet dModifies{"result"};
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("a", aModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("b", bModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("c", cModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("d", dModifies));

    REQUIRE(actualMap == expectedMap);
}
