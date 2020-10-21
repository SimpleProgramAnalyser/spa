/**
 * Tests for the Next extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
#include "catch.hpp"
#include "cfg/CfgBuilder.h"
#include "frontend/designExtractor/NextExtractor.h"

bool checkIfNextRelationshipsAreEqual(std::vector<std::pair<Integer, Integer>> actualNextRelationships,
                                      std::vector<std::pair<Integer, Integer>> expectedNextRelationships)
{
    // Check if the vector are of the same size. If not, terminate early
    if (!(actualNextRelationships.size() == expectedNextRelationships.size())) {
        return false;
    }

    // Sort the vectors for comparison
    std::sort(actualNextRelationships.begin(), actualNextRelationships.end());
    std::sort(expectedNextRelationships.begin(), expectedNextRelationships.end());

    bool isEqual = true;
    for (size_t i = 0; i < actualNextRelationships.size(); i++) {
        isEqual = actualNextRelationships.at(i).first == expectedNextRelationships.at(i).first;
        // Terminate early
        if (!isEqual) {
            return false;
        }
        isEqual = actualNextRelationships.at(i).second == expectedNextRelationships.at(i).second;
        // Terminate early
        if (!isEqual) {
            return false;
        }
    }
    return isEqual;
}

std::vector<std::pair<Integer, Integer>> getActualNextRelationships(const List<ProcedureNode>* procedureList)
{
    Name procName = procedureList->at(0)->procedureName;
    const StmtlstNode* const stmtListNode = procedureList->at(0)->statementListNode;
    std::pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtListNode);

    return extractNext(cfgInfo);
}

TEST_CASE("Next extractor works for basic program with read, assign, print - compute")
{
    const List<ProcedureNode>* procedureList = &(getProgram1Tree_compute()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for if and else statements nested in while - factorials")
{
    const List<ProcedureNode>* procedureList = &(getProgram2Tree_factorials()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(5, 7));
    expectedNextRelationships.push_back(std::make_pair(6, 8));
    expectedNextRelationships.push_back(std::make_pair(7, 8));
    expectedNextRelationships.push_back(std::make_pair(8, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 9));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program with read, assign, if/else, print - printAscending")
{
    const List<ProcedureNode>* procedureList = &(getProgram4Tree_printAscending()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(6, 7));
    expectedNextRelationships.push_back(std::make_pair(4, 8));
    expectedNextRelationships.push_back(std::make_pair(7, 9));
    expectedNextRelationships.push_back(std::make_pair(8, 9));
    expectedNextRelationships.push_back(std::make_pair(9, 10));
    expectedNextRelationships.push_back(std::make_pair(10, 11));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program with read, assign, while, print - sumDigit")
{
    const List<ProcedureNode>* procedureList = &(getProgram5Tree_sumDigit()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(6, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 7));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program If statement and assign - ifExample")
{
    const List<ProcedureNode>* procedureList = &(getProgram13Tree_ifExample()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(1, 3));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program While statement and assign - whileExample")
{
    const List<ProcedureNode>* procedureList = &(getProgram14Tree_whileExample()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 1));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for procedure ending with while statement, with multiple assign - endWithWhile")
{
    const List<ProcedureNode>* procedureList = &(getProgram18Tree_endWithWhile()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(6, 7));
    expectedNextRelationships.push_back(std::make_pair(7, 8));
    expectedNextRelationships.push_back(std::make_pair(8, 9));
    expectedNextRelationships.push_back(std::make_pair(9, 5));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works forprogram with if, while, read, print, call, assign - endWithIf")
{
    const List<ProcedureNode>* procedureList = &(getProgram21Tree_endWithIf()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(6, 7));
    expectedNextRelationships.push_back(std::make_pair(7, 8));
    expectedNextRelationships.push_back(std::make_pair(7, 9));
    expectedNextRelationships.push_back(std::make_pair(8, 4));
    expectedNextRelationships.push_back(std::make_pair(9, 4));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program with if, nested whiles, read, print, call, assign - readPoints")
{
    const List<ProcedureNode>* procedureList = &(getProgram22Tree_whileNestedInWhile()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(1, 3));
    expectedNextRelationships.push_back(std::make_pair(2, 4));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(6, 7));
    expectedNextRelationships.push_back(std::make_pair(7, 8));
    expectedNextRelationships.push_back(std::make_pair(8, 7));

    expectedNextRelationships.push_back(std::make_pair(7, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 9));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program with nested whiles and ifs - doSomething")
{
    const List<ProcedureNode>* procedureList = &(getProgram23Tree_nestedWhileIfs()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(1, 7));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(4, 6));
    expectedNextRelationships.push_back(std::make_pair(5, 8));
    expectedNextRelationships.push_back(std::make_pair(6, 8));
    expectedNextRelationships.push_back(std::make_pair(8, 9));
    expectedNextRelationships.push_back(std::make_pair(9, 8));
    expectedNextRelationships.push_back(std::make_pair(7, 8));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program with nested ifs - nested1")
{
    const List<ProcedureNode>* procedureList = &(getProgram24Tree_nestedIfs()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(1, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(3, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(5, 7));
    expectedNextRelationships.push_back(std::make_pair(7, 8));
    expectedNextRelationships.push_back(std::make_pair(7, 9));
    expectedNextRelationships.push_back(std::make_pair(9, 10));
    expectedNextRelationships.push_back(std::make_pair(9, 11));
    expectedNextRelationships.push_back(std::make_pair(11, 12));
    expectedNextRelationships.push_back(std::make_pair(11, 13));
    expectedNextRelationships.push_back(std::make_pair(13, 14));
    expectedNextRelationships.push_back(std::make_pair(13, 15));
    expectedNextRelationships.push_back(std::make_pair(15, 16));
    expectedNextRelationships.push_back(std::make_pair(15, 17));
    expectedNextRelationships.push_back(std::make_pair(17, 18));
    expectedNextRelationships.push_back(std::make_pair(17, 19));
    expectedNextRelationships.push_back(std::make_pair(19, 20));
    expectedNextRelationships.push_back(std::make_pair(2, 20));
    expectedNextRelationships.push_back(std::make_pair(4, 20));
    expectedNextRelationships.push_back(std::make_pair(6, 20));
    expectedNextRelationships.push_back(std::make_pair(8, 20));
    expectedNextRelationships.push_back(std::make_pair(10, 20));
    expectedNextRelationships.push_back(std::make_pair(12, 20));
    expectedNextRelationships.push_back(std::make_pair(14, 20));
    expectedNextRelationships.push_back(std::make_pair(16, 20));
    expectedNextRelationships.push_back(std::make_pair(18, 20));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program with nested ifs in while - nested2")
{
    const List<ProcedureNode>* procedureList = &(getProgram25Tree_nestedIfsInWhile()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(3, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(5, 7));
    expectedNextRelationships.push_back(std::make_pair(7, 8));
    expectedNextRelationships.push_back(std::make_pair(7, 9));
    expectedNextRelationships.push_back(std::make_pair(9, 10));
    expectedNextRelationships.push_back(std::make_pair(9, 11));
    expectedNextRelationships.push_back(std::make_pair(11, 12));
    expectedNextRelationships.push_back(std::make_pair(11, 13));
    expectedNextRelationships.push_back(std::make_pair(13, 14));
    expectedNextRelationships.push_back(std::make_pair(13, 15));
    expectedNextRelationships.push_back(std::make_pair(15, 16));
    expectedNextRelationships.push_back(std::make_pair(15, 17));
    expectedNextRelationships.push_back(std::make_pair(17, 18));
    expectedNextRelationships.push_back(std::make_pair(17, 19));
    expectedNextRelationships.push_back(std::make_pair(4, 1));
    expectedNextRelationships.push_back(std::make_pair(6, 1));
    expectedNextRelationships.push_back(std::make_pair(8, 1));
    expectedNextRelationships.push_back(std::make_pair(10, 1));
    expectedNextRelationships.push_back(std::make_pair(12, 1));
    expectedNextRelationships.push_back(std::make_pair(14, 1));
    expectedNextRelationships.push_back(std::make_pair(16, 1));
    expectedNextRelationships.push_back(std::make_pair(18, 1));
    expectedNextRelationships.push_back(std::make_pair(19, 1));

    expectedNextRelationships.push_back(std::make_pair(1, 20));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}

TEST_CASE("Next extractor works for program with interleaving nested ifs in while - nested3")
{
    const List<ProcedureNode>* procedureList = &(getProgram26Tree_nestedInterleavingIfsInWhile()->procedureList);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = getActualNextRelationships(procedureList);

    // Expected
    std::vector<std::pair<Integer, Integer>> expectedNextRelationships;
    expectedNextRelationships.push_back(std::make_pair(1, 2));
    expectedNextRelationships.push_back(std::make_pair(2, 3));
    expectedNextRelationships.push_back(std::make_pair(3, 4));
    expectedNextRelationships.push_back(std::make_pair(4, 5));
    expectedNextRelationships.push_back(std::make_pair(5, 6));
    expectedNextRelationships.push_back(std::make_pair(6, 7));
    expectedNextRelationships.push_back(std::make_pair(7, 8));
    expectedNextRelationships.push_back(std::make_pair(8, 9));
    expectedNextRelationships.push_back(std::make_pair(9, 10));
    expectedNextRelationships.push_back(std::make_pair(10, 11));
    expectedNextRelationships.push_back(std::make_pair(11, 12));
    expectedNextRelationships.push_back(std::make_pair(11, 13));
    expectedNextRelationships.push_back(std::make_pair(10, 14));
    expectedNextRelationships.push_back(std::make_pair(9, 15));
    expectedNextRelationships.push_back(std::make_pair(8, 16));
    expectedNextRelationships.push_back(std::make_pair(7, 17));
    expectedNextRelationships.push_back(std::make_pair(6, 18));
    expectedNextRelationships.push_back(std::make_pair(5, 19));
    expectedNextRelationships.push_back(std::make_pair(4, 20));
    expectedNextRelationships.push_back(std::make_pair(3, 21));
    expectedNextRelationships.push_back(std::make_pair(2, 22));
    expectedNextRelationships.push_back(std::make_pair(1, 23));

    expectedNextRelationships.push_back(std::make_pair(12, 1));
    expectedNextRelationships.push_back(std::make_pair(13, 1));
    expectedNextRelationships.push_back(std::make_pair(14, 1));
    expectedNextRelationships.push_back(std::make_pair(15, 1));
    expectedNextRelationships.push_back(std::make_pair(16, 1));
    expectedNextRelationships.push_back(std::make_pair(17, 1));
    expectedNextRelationships.push_back(std::make_pair(18, 1));
    expectedNextRelationships.push_back(std::make_pair(19, 1));
    expectedNextRelationships.push_back(std::make_pair(20, 1));
    expectedNextRelationships.push_back(std::make_pair(21, 1));
    expectedNextRelationships.push_back(std::make_pair(22, 1));

    bool isEqual = checkIfNextRelationshipsAreEqual(actualNextRelationships, expectedNextRelationships);
    REQUIRE(isEqual == true);
}