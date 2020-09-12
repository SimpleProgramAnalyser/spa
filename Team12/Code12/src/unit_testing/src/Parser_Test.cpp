#include "catch.hpp"

#include "ast_utils/AstUtils.h"
#include "frontend/parser/Parser.cpp"

TEST_CASE("Parser can parse an example program correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram1String_compute());
    REQUIRE(*(ast) == *(getProgram1Tree_compute()));
}

TEST_CASE("Parser parses if statements correctly") {}

TEST_CASE("Parser parses while statements correctly") {}

TEST_CASE("Parser parses if nested in while correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram2String_factorials());
    REQUIRE(*(ast) == *(getProgram2Tree_factorials()));
}

TEST_CASE("Parser parses complicated arithmetic expressions statements correctly") {}

TEST_CASE("Parser parses complicated conditional expressions statements correctly") {}

TEST_CASE("Parser parses multiple procedures correctly") {
    ProgramNode* ast = parseSimpleReturnNode(getProgram7String_computeCentroid());
    REQUIRE(*(ast) == *getProgram7Tree_computeCentroid());
}

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
