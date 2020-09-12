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
