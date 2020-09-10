#include "catch.hpp"
#include "frontend/parser/Parser.cpp"

std::string getDefaultProgramName() {
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
    ProgramNode* constructedAst = createProgramNode(getDefaultProgramName(), procedureList);
    REQUIRE(*(ast) == *(constructedAst));
}

TEST_CASE("Parser parses if statements correctly") {

}

TEST_CASE("Parser parses while statements correctly") {

}

TEST_CASE("Parser parses complicated arithmetic expressions statements correctly") {

}

TEST_CASE("Parser parses complicated conditional expressions statements correctly") {

}

TEST_CASE("Parser parses multiple procedures correctly") {

}

TEST_CASE("Parser parses keywords as identifiers correctly") {

}

TEST_CASE("Parser parses variables with same name as procedure") {

}

TEST_CASE("parseArithmeticExpression works for plus expression") {
    auto* arithmeticExpression = new StringList(3);
    String x = "x";
    String plus = "+";
    String y = "y";
    arithmeticExpression->at(0) = std::unique_ptr<std::string>(&x);
    arithmeticExpression->at(1) = std::unique_ptr<std::string>(&plus);
    arithmeticExpression->at(2) = std::unique_ptr<std::string>(&y);
    Expression* parsed = parseExpression(arithmeticExpression);
    Expression* expected = createPlusExpr(createRefExpr("x"), createRefExpr("y"));
    REQUIRE(*(parsed) == *(expected));
}

TEST_CASE("Syntax error, mismatched braces") {

}

TEST_CASE("Syntax error, mismatched brackets") {

}
