/**
 * Unit tests for Lexer component, methods that operate
 * on strings that are used before parsing.
 */

#include "ast_utils/AstUtils.h"
#include "catch.hpp"
#include "lexer/Lexer.h"
#include "Util.h"

TEST_CASE("splitProgram works for complicated conditional") {
    StringList* program = splitProgram(getProgram15String_complicatedConditional());
    auto* expected = new StringList(46);
    String str0 = "procedure";
    String str1 = "complicatedConditional";
    String str2 = "{";
    String str3 = "while";
    String str4 = "(";
    String str5 = "!";
    String str6 = "(";
    String str7 = "(";
    String str8 = "x";
    String str9 = "%";
    String str10 = "6";
    String str11 = "-";
    String str12 = "3";
    String str13 = "<";
    String str14 = "a";
    String str15 = ")";
    String str16 = "&&";
    String str17 = "(";
    String str18 = "(";
    String str19 = "y";
    String str20 = "==";
    String str21 = "z";
    String str22 = ")";
    String str23 = "||";
    String str24 = "(";
    String str25 = "!";
    String str26 = "(";
    String str27 = "b";
    String str28 = "!=";
    String str29 = "c";
    String str30 = "+";
    String str31 = "x";
    String str32 = ")";
    String str33 = ")";
    String str34 = ")";
    String str35 = ")";
    String str36 = ")";
    String str37 = "{";
    String str38 = "x";
    String str39 = "=";
    String str40 = "x";
    String str41 = "*";
    String str42 = "123";
    String str43 = ";";
    String str44 = "}";
    String str45 = "}";

    expected->at(0) = std::unique_ptr<std::string>(&str0);
    expected->at(1) = std::unique_ptr<std::string>(&str1);
    expected->at(2) = std::unique_ptr<std::string>(&str2);
    expected->at(3) = std::unique_ptr<std::string>(&str3);
    expected->at(4) = std::unique_ptr<std::string>(&str4);
    expected->at(5) = std::unique_ptr<std::string>(&str5);
    expected->at(6) = std::unique_ptr<std::string>(&str6);
    expected->at(7) = std::unique_ptr<std::string>(&str7);
    expected->at(8) = std::unique_ptr<std::string>(&str8);
    expected->at(9) = std::unique_ptr<std::string>(&str9);
    expected->at(10) = std::unique_ptr<std::string>(&str10);
    expected->at(11) = std::unique_ptr<std::string>(&str11);
    expected->at(12) = std::unique_ptr<std::string>(&str12);
    expected->at(13) = std::unique_ptr<std::string>(&str13);
    expected->at(14) = std::unique_ptr<std::string>(&str14);
    expected->at(15) = std::unique_ptr<std::string>(&str15);
    expected->at(16) = std::unique_ptr<std::string>(&str16);
    expected->at(17) = std::unique_ptr<std::string>(&str17);
    expected->at(18) = std::unique_ptr<std::string>(&str18);
    expected->at(19) = std::unique_ptr<std::string>(&str19);
    expected->at(20) = std::unique_ptr<std::string>(&str20);
    expected->at(21) = std::unique_ptr<std::string>(&str21);
    expected->at(22) = std::unique_ptr<std::string>(&str22);
    expected->at(23) = std::unique_ptr<std::string>(&str23);
    expected->at(24) = std::unique_ptr<std::string>(&str24);
    expected->at(25) = std::unique_ptr<std::string>(&str25);
    expected->at(26) = std::unique_ptr<std::string>(&str26);
    expected->at(27) = std::unique_ptr<std::string>(&str27);
    expected->at(28) = std::unique_ptr<std::string>(&str28);
    expected->at(29) = std::unique_ptr<std::string>(&str29);
    expected->at(30) = std::unique_ptr<std::string>(&str30);
    expected->at(31) = std::unique_ptr<std::string>(&str31);
    expected->at(32) = std::unique_ptr<std::string>(&str32);
    expected->at(33) = std::unique_ptr<std::string>(&str33);
    expected->at(34) = std::unique_ptr<std::string>(&str34);
    expected->at(35) = std::unique_ptr<std::string>(&str35);
    expected->at(36) = std::unique_ptr<std::string>(&str36);
    expected->at(37) = std::unique_ptr<std::string>(&str37);
    expected->at(38) = std::unique_ptr<std::string>(&str38);
    expected->at(39) = std::unique_ptr<std::string>(&str39);
    expected->at(40) = std::unique_ptr<std::string>(&str40);
    expected->at(41) = std::unique_ptr<std::string>(&str41);
    expected->at(42) = std::unique_ptr<std::string>(&str42);
    expected->at(43) = std::unique_ptr<std::string>(&str43);
    expected->at(44) = std::unique_ptr<std::string>(&str44);
    expected->at(45) = std::unique_ptr<std::string>(&str45);

    REQUIRE(util::checkListValuesEqual(*program, *expected));
}

TEST_CASE("trimWhitespace works on empty string")
{
    REQUIRE(trimWhitespace("") == "");
}

TEST_CASE("trimWhitespace works on whitespace-only string")
{
    REQUIRE(trimWhitespace("     \n     \t    \v\v\v\r\r\f\f\f\f\f") == "");
}

TEST_CASE("trimWhitespace works on string with one character and whitespace")
{
    REQUIRE(trimWhitespace("     \n     \t   d \v\v\v\r\r\f\f\f\f\f") == "d");
}

TEST_CASE("trimWhitespace returns same string with only one character")
{
    REQUIRE(trimWhitespace("g") == "g");
}

TEST_CASE("trimWhitespace does not truncate whitespace between words")
{
    REQUIRE(trimWhitespace("     \n  hola   \t   que\v\v\v\r\r\f\ftal           \f\f\f")
            == "hola   \t   que\v\v\v\r\r\f\ftal");
}
