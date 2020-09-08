/**
 * Unit tests for Lexer component, methods that operate
 * on strings that are used before parsing.
 */

#include "catch.hpp"
#include "lexer/Lexer.h"

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
