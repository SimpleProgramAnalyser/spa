/**
 * Tests for the CFG BIP Builder component
 * of the Design Extractor in SPA Frontend
 */

#include "../ast_utils/AstUtils.h"
#include "../cfg_utils/CfgUtils.h"
#include "ast/AstLibrary.h"
#include "catch.hpp"
#include "cfg/CfgBipBuilder.h"
#include "cfg/CfgBuilder.h"

CfgNode* getActualCfgBipRootNode(ProgramNode* progNode, Name firstProcName)
{
    std::unordered_map<Name, CfgNode*> proceduresCfg;
    std::unordered_map<Name, size_t> numberOfCfgNodes;
    std::unordered_map<Name, Boolean> visitedProcedureCfg;

    // Build Cfg for each procedure
    const List<ProcedureNode>* procedureList = &(progNode->procedureList);
    for (size_t i = 0; i < procedureList->size(); i++) {
        Name procName = procedureList->at(i)->procedureName;
        const StmtlstNode* const stmtListNode = procedureList->at(i)->statementListNode;
        std::pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtListNode);

        // Add CFG root node into procedureCfg and numberOfCfgNodes
        proceduresCfg.insert({procName, cfgInfo.first});
        numberOfCfgNodes.insert({procName, cfgInfo.second});
        visitedProcedureCfg.insert({procName, false});
    }

    visitedProcedureCfg.at(firstProcName) = true;
    CfgNode* cfgBipRootNode = buildCfgBip(&proceduresCfg, firstProcName, &numberOfCfgNodes, &visitedProcedureCfg);

    return cfgBipRootNode;
}

TEST_CASE(
    "Cfg BIP Builder works for program with multiple procedures, with if, while, read, print, call, assign - program7")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram7Tree_computeCentroid(), "main");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram7CfgBip_computeCentroid();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for mutipleProcedures, program with multiple procedures, with if, while, read, print, "
          "call, assign - program19")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram19Tree_multipleProcedures(), "a");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram19CfgBip_multipleProcedures();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for mutipleProcedures, program with multiple procedures, with if, while, read, print, "
          "call, assign - program20")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram20Tree_multipleProceduresSpheresdf(), "main");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram20CfgBip_multipleProceduresSpheresdf();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for program with if, nested whiles, read, print, call, assign - program22")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram22Tree_whileNestedInWhile(), "readPoint");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram22CfgBip_whileNestedInWhile();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for program with nested ifs - program24")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram24Tree_nestedIfs(), "doSmth");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram24CfgBip_nestedIfs();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for program with nested ifs in a while - program25")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram25Tree_nestedIfsInWhile(), "doSmth");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram25CfgBip_nestedIfsInWhile();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for program with interleaving nested ifs in a while - program26")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram26Tree_nestedInterleavingIfsInWhile(), "doSmth");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram26CfgBip_nestedInterleavingIfsInWhile();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for program with if in if statement - program27")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram27Tree_ifInIf(), "doSmth");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram27CfgBip_ifInIf();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}

TEST_CASE("Cfg BIP Builder works for program combining procedures in programs 27, 26 and 25 - program28")
{
    // Actual
    CfgNode* cfgBipRootNode = getActualCfgBipRootNode(getProgram28Tree_combineIfInIfAndNested3AndNested2(), "ifInIf");

    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram28CfgBip_combineIfInIfAndNested3AndNested2();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}
