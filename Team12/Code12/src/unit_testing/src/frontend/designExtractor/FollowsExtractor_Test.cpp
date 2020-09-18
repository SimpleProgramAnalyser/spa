/**
 * Tests for the Follows extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/designExtractor/FollowsExtractor.cpp"

TEST_CASE("Follows extractor works for if and else statements nested in while")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram2Tree_factorials());
    //        Follows relationships: 1  2  3  4  5
    //                               |  |  |  |  |
    //                               V  V  V  V  V
    FollowsList expectedResult = {0, 2, 3, 4, 9, 8, 0, 0, 0, 0};

    REQUIRE(expectedResult == *followsList);
}

TEST_CASE("Follows extractor works for if and else statements")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram4Tree_printAscending());
    //        Follows relationships: 1  2  3  4  5  6  7  8
    //                               |  |  |  |  |  |  |  |
    //                               V  V  V  V  V  V  V  V
    FollowsList expectedResult = {0, 2, 3, 4, 9, 6, 7, 0, 0, 10, 11, 0};

    REQUIRE(expectedResult == *followsList);
}

TEST_CASE("Follows extractor works for while statement - complicatedConditional")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram15Tree_complicatedConditional());
    //        Follows relationships: 1  2
    //                               |  |
    //                               V  V
    FollowsList expectedResult = {0, 0, 0};

    REQUIRE(expectedResult == *followsList);
}

TEST_CASE("Follows extractor works for multiple procedures, multiple statements - keywordsAsIdentifier")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram16Tree_keywordsAsIdentifier());
    //        Follows relationships: 1  2  3  4  5  6  7  8  9  10  11 12
    //                               |  |  |  |  |  |  |  |  |   |  |  |
    //                               V  V  V  V  V  V  V  V  V   V  V  V
    FollowsList expectedResult = {0, 2, 3, 6, 5, 0, 0, 8, 9, 12, 0, 0, 0};

    REQUIRE(expectedResult == *followsList);
}

TEST_CASE("Follows extractor works for mutiple procedures, variable with same name as procedure")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram17Tree_sameVariableAndProcedureName());
    //        Follows relationships: 1  2  3  4  5  6  7
    //                               |  |  |  |  |  |  |
    //                               V  V  V  V  V  V  V
    FollowsList expectedResult = {0, 2, 3, 0, 5, 6, 0, 0};

    REQUIRE(expectedResult == *followsList);
}

TEST_CASE("Follows extractor works when last statement is container statement")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram18Tree_endWithWhile());
    //        Follows relationships: 1  2  3  4     6  7  8
    //                               |  |  |  |     |  |  |
    //                               V  V  V  V     V  V  V
    FollowsList expectedResult = {0, 2, 3, 4, 5, 0, 7, 8, 9, 0};

    REQUIRE(expectedResult == *followsList);
}

TEST_CASE("Follow extractor works for multiple procedures, multiple statements - multipleProcedures")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram19Tree_multipleProcedures());
    //        Follows relationships: 1  2  3  4  5  6  7  8  9  10  11 12 13  14 15 16 17 18
    //                               |  |  |  |  |  |  |  |  |   |   |  |  |   |  |  |  |  |
    //                               V  V  V  V  V  V  V  V  V   V   V  V  V   V  V  V  V  V
    FollowsList expectedResult = {0, 2, 3, 4, 0, 6, 7, 0, 9, 10, 11, 0, 0, 0, 15, 0, 0, 0, 0};

    REQUIRE(expectedResult == *followsList);
}
