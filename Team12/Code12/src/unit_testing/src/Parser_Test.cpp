#include "catch.hpp"
#include "frontend/parser/Parser.cpp"

std::string getDefaultProgramName()
{
    return "SIMPLE program";
}

TEST_CASE("Parser can parse an example program correctly")
{
    String simpleProgram = "\
procedure compute {\n\
\n\
    read num1;\n\
    read num2;\n\
    read num3;\n\
\n\
    sum = (num1 + num2) * num3;\n\
    ave = sum / 3;\n\
\n\
    print ave;\n\
}";
    ProgramNode* ast = parseSimpleReturnNode(simpleProgram);

    List<StatementNode> statements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(3, Variable("num3"))));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3")))));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("ave"), createDivExpr(createRefExpr("sum"), createRefExpr(3)))));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(6, Variable("ave"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* computeProcedure = createProcedureNode("compute", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* constructedAst = createProgramNode(getDefaultProgramName(), procedureList, 6);
    REQUIRE(*(ast) == *(constructedAst));
}

TEST_CASE("Parser parses if statements correctly") {}

TEST_CASE("Parser parses while statements correctly") {}

TEST_CASE("Parser parses if nested in while correctly")
{
    String simpleProgram = "\
procedure factorials {\n\
    limit = 100;\n\
    current = 0;\n\
    factorial = 0;\n\
    while (current < limit) {\n\
        if (current == 0) then {\n\
            factorial = 1;\n\
        } else {\n\
            factorial = current * factorial;\n\
        }\n\
        current = current + 1;\n\
    }\n\
    answer = factorial;\n\
}";
    ProgramNode* ast = parseSimpleReturnNode(simpleProgram);

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
    ProgramNode* constructedAst = createProgramNode(getDefaultProgramName(), procedureList, 9);

    REQUIRE(*(ast) == *(constructedAst));
}

TEST_CASE("Parser parses complicated arithmetic expressions statements correctly") {}

TEST_CASE("Parser parses complicated conditional expressions statements correctly") {}

TEST_CASE("Parser parses multiple procedures correctly") {}

TEST_CASE("Parser parses keywords as identifiers correctly") {}

TEST_CASE("Parser parses variables with same name as procedure") {}

TEST_CASE("Syntax error, mismatched braces") {}

TEST_CASE("Syntax error, mismatched brackets") {}
