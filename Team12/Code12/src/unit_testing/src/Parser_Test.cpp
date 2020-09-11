#include "catch.hpp"

#include "ast_utils/AstUtils.h"
#include "frontend/parser/Parser.cpp"

TEST_CASE("Parser can parse an example program correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram1String());
    REQUIRE(*(ast) == *(getProgram1Tree()));
}

TEST_CASE("Parser parses if statements correctly") {}

TEST_CASE("Parser parses while statements correctly") {}

TEST_CASE("Parser parses if nested in while correctly")
{
    ProgramNode* ast = parseSimpleReturnNode(getProgram2String());
    REQUIRE(*(ast) == *(getProgram2Tree()));
}

TEST_CASE("Parser parses complicated arithmetic expressions statements correctly") {}

TEST_CASE("Parser parses complicated conditional expressions statements correctly") {}

TEST_CASE("Parser parses multiple procedures correctly") {}

TEST_CASE("Parser parses keywords as identifiers correctly") {}

TEST_CASE("Parser parses variables with same name as procedure") {}

TEST_CASE("Syntax error, mismatched braces") {}

TEST_CASE("Syntax error, mismatched brackets") {}
