#include "../../ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/parser/Parser.cpp"

TEST_CASE("getBracketStart returns correct brackets for 5 nesting levels")
{
    StringVector program = getProgram15StringList_complicatedConditional();
    frontend::TokenList programTokens = frontend::tokeniseSimple(std::move(program));
    REQUIRE(getBracketStart(&programTokens, 35) == 6);
    REQUIRE(getBracketStart(&programTokens, 34) == 17);
}

TEST_CASE("getBracketEnd returns correct brackets for 5 nesting levels")
{
    StringVector program = getProgram15StringList_complicatedConditional();
    frontend::TokenList programTokens = frontend::tokeniseSimple(std::move(program));
    REQUIRE(getBracketEnd(&programTokens, 6) == 35);
    REQUIRE(getBracketEnd(&programTokens, 7) == 15);
}

TEST_CASE("Parser can parse an example program correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram1String_compute());
    REQUIRE(*(ast) == *(getProgram1Tree_compute()));
}

TEST_CASE("Parser parses if statements correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram13String_ifExample());
    REQUIRE(*(ast) == *(getProgram13Tree_ifExample()));
}

TEST_CASE("Parser parses while statements correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram14String_whileExample());
    REQUIRE(*(ast) == *(getProgram14Tree_whileExample()));
}

TEST_CASE("Parser parses if nested in while correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram2String_factorials());
    REQUIRE(*(ast) == *(getProgram2Tree_factorials()));
}

TEST_CASE("Parser parses complicated conditional expressions correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram15String_complicatedConditional());
    REQUIRE(*(ast) == *getProgram15Tree_complicatedConditional());
}

TEST_CASE("Parser parses multiple procedures correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram7String_computeCentroid());
    REQUIRE(*(ast) == *getProgram7Tree_computeCentroid());
}

TEST_CASE("Parser parses keywords as identifiers correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram16String_keywordsAsIdentifier());
    ProgramNode* expected = getProgram16Tree_keywordsAsIdentifier();
    REQUIRE(*(ast) == *(expected));
}

TEST_CASE("Parser parses variables with same name as procedure")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram17String_sameVariableAndProcedureName());
    ProgramNode* expected = getProgram17Tree_sameVariableAndProcedureName();
    REQUIRE(*(ast) == *(expected));
}

TEST_CASE("parseExpression works for plus expression")
{
    StringVector arithmeticExpression(3);
    String x = "x";
    String plus = "+";
    String y = "y";
    arithmeticExpression.at(0) = std::move(x);
    arithmeticExpression.at(1) = std::move(plus);
    arithmeticExpression.at(2) = std::move(y);
    Expression* parsed = parseExpression(arithmeticExpression);
    Expression* expected = createPlusExpr(createRefExpr("x"), createRefExpr("y"));
    REQUIRE(*(parsed) == *(expected));
    delete parsed;
    delete expected;
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
    StringVector arithmeticExpression(23);
    arithmeticExpression.at(0) = std::move(str0);
    arithmeticExpression.at(1) = std::move(str1);
    arithmeticExpression.at(2) = std::move(str2);
    arithmeticExpression.at(3) = std::move(str3);
    arithmeticExpression.at(4) = std::move(str4);
    arithmeticExpression.at(5) = std::move(str5);
    arithmeticExpression.at(6) = std::move(str6);
    arithmeticExpression.at(7) = std::move(str7);
    arithmeticExpression.at(8) = std::move(str8);
    arithmeticExpression.at(9) = std::move(str9);
    arithmeticExpression.at(10) = std::move(str10);
    arithmeticExpression.at(11) = std::move(str11);
    arithmeticExpression.at(12) = std::move(str12);
    arithmeticExpression.at(13) = std::move(str13);
    arithmeticExpression.at(14) = std::move(str14);
    arithmeticExpression.at(15) = std::move(str15);
    arithmeticExpression.at(16) = std::move(str16);
    arithmeticExpression.at(17) = std::move(str17);
    arithmeticExpression.at(18) = std::move(str18);
    arithmeticExpression.at(19) = std::move(str19);
    arithmeticExpression.at(20) = std::move(str20);
    arithmeticExpression.at(21) = std::move(str21);
    arithmeticExpression.at(22) = std::move(str22);
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
    StringVector arithmeticExpression(14);
    arithmeticExpression.at(0) = std::move(str0);
    arithmeticExpression.at(1) = std::move(str1);
    arithmeticExpression.at(2) = std::move(str2);
    arithmeticExpression.at(3) = std::move(str3);
    arithmeticExpression.at(4) = std::move(str4);
    arithmeticExpression.at(5) = std::move(str5);
    arithmeticExpression.at(6) = std::move(str6);
    arithmeticExpression.at(7) = std::move(str7);
    arithmeticExpression.at(8) = std::move(str8);
    arithmeticExpression.at(9) = std::move(str9);
    arithmeticExpression.at(10) = std::move(str10);
    arithmeticExpression.at(11) = std::move(str11);
    arithmeticExpression.at(12) = std::move(str12);
    arithmeticExpression.at(13) = std::move(str13);
    Expression* parsed = parseExpression(arithmeticExpression);
    REQUIRE(parsed == nullptr);
}

TEST_CASE("Syntax error, mismatched braces")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram7StringMismatchedBraces_computeCentroid());
    REQUIRE(ast == nullptr);
}

TEST_CASE("Syntax error, mismatched brackets")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram7StringMismatchedBrackets_computeCentroid());
    REQUIRE(ast == nullptr);
}
