/**
 * Tests for the Next extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
#include "catch.hpp"
#include "cfg/CfgBuilder.h"
#include "frontend/designExtractor/NextExtractor.h"

TEST_CASE("Next extractor works for if and else statements nested in while - factorials")
{
    const List<ProcedureNode>* procedureList = &(getProgram2Tree_factorials()->procedureList);

    Name procName = procedureList->at(0)->procedureName;
    const StmtlstNode* const stmtListNode = procedureList->at(0)->statementListNode;
    std::pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtListNode);
    std::vector<std::pair<Integer, Integer>> actualNextRelationships = extractNext(cfgInfo);

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

    REQUIRE(actualNextRelationships.size() == expectedNextRelationships.size());

    std::sort(actualNextRelationships.begin(), actualNextRelationships.end());
    std::sort(expectedNextRelationships.begin(), expectedNextRelationships.end());

    bool isEqual = true;
    for (size_t i = 0; i < actualNextRelationships.size(); i++) {
        isEqual = actualNextRelationships.at(i).first == expectedNextRelationships.at(i).first;
        isEqual = actualNextRelationships.at(i).second == expectedNextRelationships.at(i).second;
    }

    REQUIRE(isEqual == true);
}