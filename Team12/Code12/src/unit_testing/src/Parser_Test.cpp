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

TEST_CASE("Parser parses multiple procedures correctly") {}

TEST_CASE("Parser parses keywords as identifiers correctly") {}

TEST_CASE("Parser parses variables with same name as procedure") {}

TEST_CASE("Syntax error, mismatched braces") {}

TEST_CASE("Syntax error, mismatched brackets") {}
