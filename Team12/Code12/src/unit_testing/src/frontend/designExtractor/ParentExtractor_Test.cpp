/**
 * Tests for the Parent extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
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

TEST_CASE("Parent extractor works for if and else statements")
{
    ParentList* parentList = extractParentReturnAdjacencyList(*getProgram4Tree_printAscending());
    //        Parent relationships: 1  2  3  4  5  6  7  8
    //                              |  |  |  |  |  |  |  |
    //                              V  V  V  V  V  V  V  V
    ParentList expectedResult = {0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0};

    REQUIRE(expectedResult == *parentList);
}

TEST_CASE("Parent extractor works for multiple procedures, multiple statements - keywordsAsIdentifier")
{
    ParentList* parentList = extractParentReturnAdjacencyList(*getProgram16Tree_keywordsAsIdentifier());
    //        Parent relationships: 1  2  3  4  5  6  7  8  9 10 11 12
    //                              |  |  |  |  |  |  |  |  |  |  |  |
    //                              V  V  V  V  V  V  V  V  V  V  V  V
    ParentList expectedResult = {0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 9, 9, 0};

    REQUIRE(expectedResult == *parentList);
}

TEST_CASE("Parent extractor works for mutiple procedures, variable with same name as procedure")
{
    ParentList* parentList = extractParentReturnAdjacencyList(*getProgram17Tree_sameVariableAndProcedureName());
    //        Parent relationships: 1  2  3  4  5  6  7
    //                              |  |  |  |  |  |  |
    //                              V  V  V  V  V  V  V
    ParentList expectedResult = {0, 0, 0, 0, 0, 0, 0, 0};

    REQUIRE(expectedResult == *parentList);
}

TEST_CASE("Parent extractor works when last statement is container statement")
{
    ParentList* parentList = extractParentReturnAdjacencyList(*getProgram18Tree_endWithWhile());
    //        Parent relationships: 1  2  3  4     6  7  8  9
    //                              |  |  |  |     |  |  |  |
    //                              V  V  V  V     V  V  V  V
    ParentList expectedResult = {0, 0, 0, 0, 0, 0, 5, 5, 5, 5};

    REQUIRE(expectedResult == *parentList);
}

TEST_CASE("Parent extractor works for multiple procedures, multiple statements - multipleProcedures")
{
    ParentList* parentList = extractParentReturnAdjacencyList(*getProgram19Tree_multipleProcedures());
    //        Parent relationships: 1  2  3  4  5  6  7  8  9 10 11 12  13  14  15  16  17  18
    //                              |  |  |  |  |  |  |  |  |  |  |  |   |   |   |   |   |   |
    //                              V  V  V  V  V  V  V  V  V  V  V  V   V   V   V   V   V   V
    ParentList expectedResult = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 12, 12, 12, 0, 16, 16};

    REQUIRE(expectedResult == *parentList);
}
