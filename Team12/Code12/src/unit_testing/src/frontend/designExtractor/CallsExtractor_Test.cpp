/**
 * Tests for the Calls extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/designExtractor/CallsExtractor.cpp"

bool callsComp(const std::pair<String, String>& a, const std::pair<String, String>& b)
{
    String term1 = a.first;
    String term2 = b.first;

    String term3 = a.second;
    String term4 = b.second;
    return term1 < term2 || (term1 == term2 && term3 < term4);
}

bool callsStarComp(const std::pair<size_t, std::unordered_set<size_t>>& a,
                   const std::pair<size_t, std::unordered_set<size_t>>& b)
{
    size_t term1 = a.first;
    size_t term2 = b.first;
    return term1 < term2;
}

bool checkIfMemosAreEqual(std::vector<std::pair<size_t, std::unordered_set<size_t>>> memo1,
                          std::vector<std::pair<size_t, std::unordered_set<size_t>>>* memo2)
{
    bool memosAreEqual = true;
    std::sort(memo1.begin(), memo1.end(), callsStarComp);
    std::sort(memo2->begin(), memo2->end(), callsStarComp);
    for (size_t i = 0; i < memo1.size(); i++) {
        memosAreEqual = memo1.at(i).first == memo2->at(i).first;
        if (!memosAreEqual) {
            return false;
        }
        memosAreEqual = memo1.at(i).second == memo2->at(i).second;
        if (!memosAreEqual) {
            return false;
        }
    }

    return memosAreEqual;
}

bool checkIfCallRelationshipsAreEqual(std::vector<std::pair<String, String>> call1,
                                      std::vector<std::pair<String, String>>* call2)
{
    bool callsAreEqual = true;
    std::sort(call1.begin(), call1.end(), callsComp);
    std::sort(call2->begin(), call2->end(), callsComp);

    for (size_t i = 0; i < call1.size(); i++) {
        callsAreEqual = call1.at(i).first == call2->at(i).first;
        if (!callsAreEqual) {
            return false;
        }
        callsAreEqual = call1.at(i).second == call2->at(i).second;
        if (!callsAreEqual) {
            return false;
        }
    }
    return callsAreEqual;
}

TEST_CASE("Calls extractor works for multiple procedure program - computeCentroid")
{
    std::vector<std::vector<bool>> adjacencyMatrix{
        {false, false, true, true},
        {false, false, false, false},
        {false, false, false, false},
        {false, true, false, false},
    };
    const List<ProcedureNode>* procedureList = &(getProgram7Tree_computeCentroid()->procedureList);
    std::vector<std::pair<size_t, std::unordered_set<size_t>>> actualMemo;
    actualMemo = extractCallsStar(procedureList, adjacencyMatrix);

    std::vector<std::pair<size_t, std::unordered_set<size_t>>> expectedMemo;
    ;
    expectedMemo.push_back(std::make_pair(0, std::unordered_set<size_t>({2, 3, 1})));
    expectedMemo.push_back(std::make_pair(1, std::unordered_set<size_t>({})));
    expectedMemo.push_back(std::make_pair(2, std::unordered_set<size_t>({})));
    expectedMemo.push_back(std::make_pair(3, std::unordered_set<size_t>({1})));

    REQUIRE(actualMemo.size() == expectedMemo.size());
    REQUIRE(checkIfMemosAreEqual(actualMemo, &expectedMemo) == true);

    std::vector<std::pair<String, String>> actualCallRelationships
        = extractCalls(*getProgram7Tree_computeCentroid(), adjacencyMatrix);
    std::vector<std::pair<String, String>> expectedCallRelationships;
    expectedCallRelationships.push_back(std::make_pair("main", "computeCentroid"));
    expectedCallRelationships.push_back(std::make_pair("main", "printResults"));
    expectedCallRelationships.push_back(std::make_pair("computeCentroid", "readPoint"));

    REQUIRE(actualCallRelationships.size() == expectedCallRelationships.size());
    REQUIRE(checkIfCallRelationshipsAreEqual(actualCallRelationships, &expectedCallRelationships) == true);
}
TEST_CASE("Calls extractor works for program with keywords used as identifier - keywordsAsIdentifier")
{
    std::vector<std::vector<bool>> adjacencyMatrix{
        {false, true},
        {false, false},
    };
    const List<ProcedureNode>* procedureList = &(getProgram16Tree_keywordsAsIdentifier()->procedureList);
    std::vector<std::pair<size_t, std::unordered_set<size_t>>> actualMemo;
    actualMemo = extractCallsStar(procedureList, adjacencyMatrix);

    std::vector<std::pair<size_t, std::unordered_set<size_t>>> expectedMemo;

    expectedMemo.push_back(std::make_pair(0, std::unordered_set<size_t>({1})));
    expectedMemo.push_back(std::make_pair(1, std::unordered_set<size_t>({})));

    REQUIRE(actualMemo.size() == expectedMemo.size());
    REQUIRE(checkIfMemosAreEqual(actualMemo, &expectedMemo) == true);

    std::vector<std::pair<String, String>> actualCallRelationships
        = extractCalls(*getProgram16Tree_keywordsAsIdentifier(), adjacencyMatrix);
    std::vector<std::pair<String, String>> expectedCallRelationships;
    expectedCallRelationships.push_back(std::make_pair("procedure", "if"));

    REQUIRE(actualCallRelationships.size() == expectedCallRelationships.size());
    REQUIRE(checkIfCallRelationshipsAreEqual(actualCallRelationships, &expectedCallRelationships) == true);
}
TEST_CASE("Calls extractor works for program with variable with same name as procedure - sameVariableAndProcedureName")
{
    std::vector<std::vector<bool>> adjacencyMatrix{
        {false, true, false},
        {false, false, true},
        {false, false, false},
    };
    const List<ProcedureNode>* procedureList = &(getProgram17Tree_sameVariableAndProcedureName()->procedureList);
    std::vector<std::pair<size_t, std::unordered_set<size_t>>> actualMemo;
    actualMemo = extractCallsStar(procedureList, adjacencyMatrix);

    std::vector<std::pair<size_t, std::unordered_set<size_t>>> expectedMemo;
    std::unordered_set<size_t> set({1, 2});
    expectedMemo.push_back(std::make_pair(0, set));
    set = {2};
    expectedMemo.push_back(std::make_pair(1, set));
    set = {};
    expectedMemo.push_back(std::make_pair(2, set));

    REQUIRE(actualMemo.size() == expectedMemo.size());
    REQUIRE(checkIfMemosAreEqual(actualMemo, &expectedMemo) == true);

    std::vector<std::pair<String, String>> actualCallRelationships
        = extractCalls(*getProgram17Tree_sameVariableAndProcedureName(), adjacencyMatrix);
    std::vector<std::pair<String, String>> expectedCallRelationships;
    expectedCallRelationships.push_back(std::make_pair("procedure", "call"));
    expectedCallRelationships.push_back(std::make_pair("call", "string"));

    REQUIRE(actualCallRelationships.size() == expectedCallRelationships.size());
    REQUIRE(checkIfCallRelationshipsAreEqual(actualCallRelationships, &expectedCallRelationships) == true);
}

TEST_CASE("Calls extractor works for program with mutipleProcedures, program with multiple procedures, with if, while, "
          "read, print, call, assign - multipleProcedures")
{
    std::vector<std::vector<bool>> adjacencyMatrix{
        {false, true, true, false},
        {false, false, false, true},
        {false, false, false, true},
        {false, false, false, false},

    };
    const List<ProcedureNode>* procedureList = &(getProgram19Tree_multipleProcedures()->procedureList);
    std::vector<std::pair<size_t, std::unordered_set<size_t>>> actualMemo;
    actualMemo = extractCallsStar(procedureList, adjacencyMatrix);

    std::vector<std::pair<size_t, std::unordered_set<size_t>>> expectedMemo;
    std::unordered_set<size_t> set({3, 2, 1});
    expectedMemo.push_back(std::make_pair(0, set));
    set = {3};
    expectedMemo.push_back(std::make_pair(1, set));
    set = {3};
    expectedMemo.push_back(std::make_pair(2, set));
    set = {};
    expectedMemo.push_back(std::make_pair(3, set));

    REQUIRE(actualMemo.size() == expectedMemo.size());
    REQUIRE(checkIfMemosAreEqual(actualMemo, &expectedMemo) == true);

    std::vector<std::pair<String, String>> actualCallRelationships
        = extractCalls(*getProgram19Tree_multipleProcedures(), adjacencyMatrix);
    std::vector<std::pair<String, String>> expectedCallRelationships;
    expectedCallRelationships.push_back(std::make_pair("a", "b"));
    expectedCallRelationships.push_back(std::make_pair("a", "c"));
    expectedCallRelationships.push_back(std::make_pair("b", "d"));
    expectedCallRelationships.push_back(std::make_pair("c", "d"));

    REQUIRE(actualCallRelationships.size() == expectedCallRelationships.size());
    REQUIRE(checkIfCallRelationshipsAreEqual(actualCallRelationships, &expectedCallRelationships) == true);
}