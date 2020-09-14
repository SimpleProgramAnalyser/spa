/**
 * Tests for the Follows extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "ast_utils/AstUtils.h"
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

TEST_CASE("Follows extractor works when last statement is container statement")
{
    FollowsList* followsList = extractFollowsReturnAdjacencyList(*getProgram18Tree_endWithWhile());
    //        Follows relationships: 1  2  3  4     6  7  8
    //                               |  |  |  |     |  |  |
    //                               V  V  V  V     V  V  V
    FollowsList expectedResult = {0, 2, 3, 4, 5, 0, 7, 8, 9, 0};

    REQUIRE(expectedResult == *followsList);
}
