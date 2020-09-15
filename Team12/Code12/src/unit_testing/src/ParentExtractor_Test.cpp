/**
 * Tests for the Parent extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/designExtractor/ParentExtractor.cpp"

TEST_CASE("Parent extractor works for if and else statements nested in while")
{
    ParentList* parentList = extractParentReturnAdjacencyList(*getProgram2Tree_factorials());
    //        Parent relationships: 1  2  3  4  5  6  7  8
    //                              |  |  |  |  |  |  |  |
    //                              V  V  V  V  V  V  V  V
    ParentList expectedResult = {0, 0, 0, 0, 0, 4, 5, 5, 4, 0};

    REQUIRE(expectedResult == *parentList);
}
