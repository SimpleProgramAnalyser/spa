/**
 * Tests for the CFG BIP Builder component
 * of the Design Extractor in SPA Frontend
 */

#include "../ast_utils/AstUtils.h"
#include "../cfg_utils/CfgUtils.h"
#include "ast/AstLibrary.h"
#include "catch.hpp"
#include "cfg/CfgBuilder.h"
#include "cfg/CfgBipBuilder.h"

TEST_CASE("Cfg BIP Builder works for mutipleProcedures, program with multiple procedures, with if, while, read, print, call, assign - program20")
{
    // Actual

    std::unordered_map<Name, CfgNode*> proceduresCfg;
    std::unordered_map<Name, size_t> numberOfCfgNodes;

    CfgNode* mainRootNode = getProgram20Cfg_main().first;
    CfgNode* raymarchRootNode = getProgram20Cfg_raymarch().first;
    CfgNode* spheresdfRootNode = getProgram20Cfg_spheresdf().first;

    proceduresCfg.insert({"main", mainRootNode});
    proceduresCfg.insert({"raymarch", raymarchRootNode});
    proceduresCfg.insert({"spheresdf", spheresdfRootNode});
    numberOfCfgNodes.insert({"main", 1});
    numberOfCfgNodes.insert({"raymarch", 7});
    numberOfCfgNodes.insert({"spheresdf", 4});

    CfgNode* cfgBipRootNode = buildCfgBip(&proceduresCfg, "main", &numberOfCfgNodes);


    // Expected
    std::pair<CfgNode*, size_t> cfgBipInfo = getProgram20CfgBip_multipleProceduresSpheresdf();
    CfgNode* expectedCfg = cfgBipInfo.first;

    Boolean isEqual = cfgBipRootNode->equals(expectedCfg, cfgBipInfo.second);
    REQUIRE(isEqual == true);
}