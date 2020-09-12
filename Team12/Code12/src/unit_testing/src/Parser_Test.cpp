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

TEST_CASE("parseExpression works for plus expression")
{
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

TEST_CASE("parseExpression obeys operator precedence and brackets")
{
    //         >=====< bracket1
    //         |     |   >====================< bracket2
    //     >=========< timesExpression1       |
    //     |   :     |   |    >=======< timesExpression2
    //     |   :     |   |    |       |       |
    // x + y * (z / a) - (b + c / d * e - f + g)
    String str0 = "x";
    String str1 = "+";
    String str2 = "y";
    String str3 = "*";
    String str4 = "(";
    String str5 = "z";
    String str6 = "/";
    String str7 = "a";
    String str8 = ")";
    String str9 = "-";
    String str10 = "(";
    String str11 = "b";
    String str12 = "+";
    String str13 = "c";
    String str14 = "/";
    String str15 = "d";
    String str16 = "*";
    String str17 = "e";
    String str18 = "-";
    String str19 = "f";
    String str20 = "+";
    String str21 = "g";
    String str22 = ")";
    auto* arithmeticExpression = new StringList(23);
    arithmeticExpression->at(0) = std::unique_ptr<std::string>(&str0);
    arithmeticExpression->at(1) = std::unique_ptr<std::string>(&str1);
    arithmeticExpression->at(2) = std::unique_ptr<std::string>(&str2);
    arithmeticExpression->at(3) = std::unique_ptr<std::string>(&str3);
    arithmeticExpression->at(4) = std::unique_ptr<std::string>(&str4);
    arithmeticExpression->at(5) = std::unique_ptr<std::string>(&str5);
    arithmeticExpression->at(6) = std::unique_ptr<std::string>(&str6);
    arithmeticExpression->at(7) = std::unique_ptr<std::string>(&str7);
    arithmeticExpression->at(8) = std::unique_ptr<std::string>(&str8);
    arithmeticExpression->at(9) = std::unique_ptr<std::string>(&str9);
    arithmeticExpression->at(10) = std::unique_ptr<std::string>(&str10);
    arithmeticExpression->at(11) = std::unique_ptr<std::string>(&str11);
    arithmeticExpression->at(12) = std::unique_ptr<std::string>(&str12);
    arithmeticExpression->at(13) = std::unique_ptr<std::string>(&str13);
    arithmeticExpression->at(14) = std::unique_ptr<std::string>(&str14);
    arithmeticExpression->at(15) = std::unique_ptr<std::string>(&str15);
    arithmeticExpression->at(16) = std::unique_ptr<std::string>(&str16);
    arithmeticExpression->at(17) = std::unique_ptr<std::string>(&str17);
    arithmeticExpression->at(18) = std::unique_ptr<std::string>(&str18);
    arithmeticExpression->at(19) = std::unique_ptr<std::string>(&str19);
    arithmeticExpression->at(20) = std::unique_ptr<std::string>(&str20);
    arithmeticExpression->at(21) = std::unique_ptr<std::string>(&str21);
    arithmeticExpression->at(22) = std::unique_ptr<std::string>(&str22);
    Expression* parsed = parseExpression(arithmeticExpression);
    Expression* bracket1 = createDivExpr(createRefExpr("z"), createRefExpr("a"));
    Expression* timesExpression1 = createTimesExpr(createRefExpr("y"), bracket1);

    Expression* timesExpression2
        = createTimesExpr(createDivExpr(createRefExpr("c"), createRefExpr("d")), createRefExpr("e"));
    Expression* bracket2 = createPlusExpr(
        createMinusExpr(createPlusExpr(createRefExpr("b"), timesExpression2), createRefExpr("f")), createRefExpr("g"));
    Expression* expected = createMinusExpr(createPlusExpr(createRefExpr("x"), timesExpression1), bracket2);
    REQUIRE(*(parsed) == *(expected));
}

TEST_CASE("parseExpression does not work for while statement")
{
    String str0 = "while";
    String str1 = "(";
    String str2 = "x";
    String str3 = "==";
    String str4 = "y";
    String str5 = ")";
    String str6 = "{";
    String str7 = "x";
    String str8 = "=";
    String str9 = "x";
    String str10 = "-";
    String str11 = "1";
    String str12 = ";";
    String str13 = "}";
    auto* arithmeticExpression = new StringList(14);
    arithmeticExpression->at(0) = std::unique_ptr<std::string>(&str0);
    arithmeticExpression->at(1) = std::unique_ptr<std::string>(&str1);
    arithmeticExpression->at(2) = std::unique_ptr<std::string>(&str2);
    arithmeticExpression->at(3) = std::unique_ptr<std::string>(&str3);
    arithmeticExpression->at(4) = std::unique_ptr<std::string>(&str4);
    arithmeticExpression->at(5) = std::unique_ptr<std::string>(&str5);
    arithmeticExpression->at(6) = std::unique_ptr<std::string>(&str6);
    arithmeticExpression->at(7) = std::unique_ptr<std::string>(&str7);
    arithmeticExpression->at(8) = std::unique_ptr<std::string>(&str8);
    arithmeticExpression->at(9) = std::unique_ptr<std::string>(&str9);
    arithmeticExpression->at(10) = std::unique_ptr<std::string>(&str10);
    arithmeticExpression->at(11) = std::unique_ptr<std::string>(&str11);
    arithmeticExpression->at(12) = std::unique_ptr<std::string>(&str12);
    arithmeticExpression->at(13) = std::unique_ptr<std::string>(&str13);
    Expression* parsed = parseExpression(arithmeticExpression);
    REQUIRE(parsed == nullptr);
}

TEST_CASE("Syntax error, mismatched braces") {}

TEST_CASE("Syntax error, mismatched brackets") {}
