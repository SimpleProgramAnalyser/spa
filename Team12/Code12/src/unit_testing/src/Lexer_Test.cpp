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
    StringVector program = splitProgram(getProgram15String_complicatedConditional());
    REQUIRE(program == getProgram15StringList_complicatedConditional());
}

TEST_CASE("splitProgram works for double length symbols")
{
    StringVector program = splitProgram("abc>=0!=3==bcd<=cde");
    String str0{"abc"};
    String str1{">="};
    String str2{"0"};
    String str3{"!="};
    String str4{"3"};
    String str5{"=="};
    String str6{"bcd"};
    String str7{"<="};
    String str8{"cde"};
    StringVector expected;
    expected.reserve(9);
    expected.push_back(std::move(str0));
    expected.push_back(std::move(str1));
    expected.push_back(std::move(str2));
    expected.push_back(std::move(str3));
    expected.push_back(std::move(str4));
    expected.push_back(std::move(str5));
    expected.push_back(std::move(str6));
    expected.push_back(std::move(str7));
    expected.push_back(std::move(str8));
    REQUIRE(program == expected);
}

TEST_CASE("splitProgram works for operators followed by character without whitespace")
{
    StringVector program = splitProgram("abc>0!3=bcd<cde");
    String str0{"abc"};
    String str1{">"};
    String str2{"0"};
    String str3{"!"};
    String str4{"3"};
    String str5{"="};
    String str6{"bcd"};
    String str7{"<"};
    String str8{"cde"};
    StringVector expected;
    expected.reserve(9);
    expected.push_back(std::move(str0));
    expected.push_back(std::move(str1));
    expected.push_back(std::move(str2));
    expected.push_back(std::move(str3));
    expected.push_back(std::move(str4));
    expected.push_back(std::move(str5));
    expected.push_back(std::move(str6));
    expected.push_back(std::move(str7));
    expected.push_back(std::move(str8));
    REQUIRE(program == expected);
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
    StringVector program = splitProgram("test test   \ronetwo\n\fthree four  five\tsix");
    String str0{"test"};
    String str1{"test"};
    String str2{"onetwo"};
    String str3{"three"};
    String str4{"four"};
    String str5{"five"};
    String str6{"six"};
    StringVector expected;
    expected.reserve(7);
    expected.push_back(std::move(str0));
    expected.push_back(std::move(str1));
    expected.push_back(std::move(str2));
    expected.push_back(std::move(str3));
    expected.push_back(std::move(str4));
    expected.push_back(std::move(str5));
    expected.push_back(std::move(str6));
    REQUIRE(program == expected);
}
