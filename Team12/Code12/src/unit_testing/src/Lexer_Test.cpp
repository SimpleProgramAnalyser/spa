/**
 * Unit tests for Lexer component, methods that operate
 * on strings that are used before parsing.
 */

#include "Util.h"
#include "ast_utils/AstUtils.h"
#include "catch.hpp"
#include "lexer/Lexer.h"

TEST_CASE("splitProgram works for complicated conditional")
{
    StringList* program = splitProgram(getProgram15String_complicatedConditional());
    REQUIRE(util::checkListValuesEqual(*program, *(getProgram15StringList_complicatedConditional())));
}

TEST_CASE("splitProgram works for double length symbols")
{
    StringList* program = splitProgram("abc>=0!=3==bcd<=cde");
    auto* str0 = new String{"abc"};
    auto* str1 = new String{">="};
    auto* str2 = new String{"0"};
    auto* str3 = new String{"!="};
    auto* str4 = new String{"3"};
    auto* str5 = new String{"=="};
    auto* str6 = new String{"bcd"};
    auto* str7 = new String{"<="};
    auto* str8 = new String{"cde"};
    auto* expected = new StringList();
    expected->reserve(9);
    expected->push_back(std::unique_ptr<String>(str0));
    expected->push_back(std::unique_ptr<String>(str1));
    expected->push_back(std::unique_ptr<String>(str2));
    expected->push_back(std::unique_ptr<String>(str3));
    expected->push_back(std::unique_ptr<String>(str4));
    expected->push_back(std::unique_ptr<String>(str5));
    expected->push_back(std::unique_ptr<String>(str6));
    expected->push_back(std::unique_ptr<String>(str7));
    expected->push_back(std::unique_ptr<String>(str8));
    REQUIRE(util::checkListValuesEqual(*program, *expected));
    delete expected;
}

TEST_CASE("splitProgram works for operators followed by character without whitespace")
{
    StringList* program = splitProgram("abc>0!3=bcd<cde");
    auto* str0 = new String{"abc"};
    auto* str1 = new String{">"};
    auto* str2 = new String{"0"};
    auto* str3 = new String{"!"};
    auto* str4 = new String{"3"};
    auto* str5 = new String{"="};
    auto* str6 = new String{"bcd"};
    auto* str7 = new String{"<"};
    auto* str8 = new String{"cde"};
    auto* expected = new StringList();
    expected->reserve(9);
    expected->push_back(std::unique_ptr<String>(str0));
    expected->push_back(std::unique_ptr<String>(str1));
    expected->push_back(std::unique_ptr<String>(str2));
    expected->push_back(std::unique_ptr<String>(str3));
    expected->push_back(std::unique_ptr<String>(str4));
    expected->push_back(std::unique_ptr<String>(str5));
    expected->push_back(std::unique_ptr<String>(str6));
    expected->push_back(std::unique_ptr<String>(str7));
    expected->push_back(std::unique_ptr<String>(str8));
    REQUIRE(util::checkListValuesEqual(*program, *expected));
    delete expected;
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

TEST_CASE("splitProgram works on different whitespaces")
{
    StringList* program = splitProgram("test test   \ronetwo\n\fthree four  five\tsix");
    auto* str0 = new String{"test"};
    auto* str1 = new String{"test"};
    auto* str2 = new String{"onetwo"};
    auto* str3 = new String{"three"};
    auto* str4 = new String{"four"};
    auto* str5 = new String{"five"};
    auto* str6 = new String{"six"};
    auto* expected = new StringList();
    expected->reserve(7);
    expected->push_back(std::unique_ptr<String>(str0));
    expected->push_back(std::unique_ptr<String>(str1));
    expected->push_back(std::unique_ptr<String>(str2));
    expected->push_back(std::unique_ptr<String>(str3));
    expected->push_back(std::unique_ptr<String>(str4));
    expected->push_back(std::unique_ptr<String>(str5));
    expected->push_back(std::unique_ptr<String>(str6));
    REQUIRE(util::checkListValuesEqual(*program, *expected));
    delete expected;
}
