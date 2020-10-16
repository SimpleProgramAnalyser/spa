/**
 * Tests for the Calls extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/designExtractor/CallsExtractor.cpp"

TEST_CASE("Calls extractor works for multiple procedure program - computeCentroid")
{
    std::vector<std::vector<bool>> adjacencyMatrix {
        {false, false, true, true}, 
        {false, false, false, false},
        {false, false, false, false}, 
        {false, true, false, false}, 
    };
    const List<ProcedureNode>* procedureList = &(getProgram7Tree_computeCentroid()->procedureList);
    std::vector<std::pair<size_t, std::unordered_set<size_t>>> actualMemo;
    actualMemo = extractCallsStar(procedureList, adjacencyMatrix);

    std::vector<std::pair<size_t, std::unordered_set<size_t>>> expectedMemo;
    std::unordered_set<size_t> set({2, 3, 1});
    expectedMemo.push_back(std::make_pair(0, set));
    set = {2};
    expectedMemo.push_back(std::make_pair(1, set));
    set = {};
    expectedMemo.push_back(std::make_pair(2, set));
    set = {1};
    expectedMemo.push_back(std::make_pair(3, set));


    std::vector<std::pair<String, String>> actualCallRelationships = extractCalls(*getProgram7Tree_computeCentroid(), adjacencyMatrix);

    std::vector<std::pair<String, String>> expectedCallRelationships;
    expectedCallRelationships.push_back(std::make_pair("main", "computeCentroid"));
    expectedCallRelationships.push_back(std::make_pair("main", "printResults"));
    expectedCallRelationships.push_back(std::make_pair("computeCentroid", "readPoint"));

    bool extractCallsStarIsEqual = true;
    REQUIRE(actualMemo.size() == expectedMemo.size());
    for (size_t i = 0; i < actualMemo.size(); i++) {
        extractCallsStarIsEqual = actualMemo.at(i).first == expectedMemo.at(i).first;
        extractCallsStarIsEqual = actualMemo.at(i).second == expectedMemo.at(i).second;
    }
    REQUIRE(extractCallsStarIsEqual == true);

    bool extractCallsIsEqual = true;
    REQUIRE(actualCallRelationships.size() == expectedCallRelationships.size());
    for (size_t i = 0; i < actualCallRelationships.size(); i++) {
        extractCallsIsEqual = actualCallRelationships.at(i).first == expectedCallRelationships.at(i).first;
        extractCallsIsEqual = actualCallRelationships.at(i).second == expectedCallRelationships.at(i).second;
    }
    REQUIRE(extractCallsIsEqual == true);
}