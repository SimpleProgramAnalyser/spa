/**
 * Implementation of SPA frontend design extractor.
 */
#include <iostream>
#include <utility>

#include "../src/cfg/CfgBipBuilder.h"
#include "../src/cfg/CfgBuilder.h"
#include "./pkb/PKB.h"
#include "CallsExtractor.h"
#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "NextExtractor.h"
#include "ParentExtractor.h"
#include "SemanticErrorsValidator.h"
#include "UsesExtractor.h"

Void storeCurrentCfg(CfgNode* cfgRootNode, Name procName, std::unordered_map<Name, CfgNode*>* proceduresCfg)
{
    storeCFG(cfgRootNode, procName);

    proceduresCfg->insert({procName, cfgRootNode});
}

Boolean extractDesign(ProgramNode& rootNode)
{
    SemanticErrorsValidator seValidator(rootNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    // CFG of each procedure
    std::unordered_map<Name, CfgNode*> proceduresCfg;
    std::unordered_map<Name, size_t> numberOfCfgNodes;
    // Hash map to check if a Cfg is visited when building CfgBip
    std::unordered_map<Name, Boolean> visitedProcedureCfg;

    if (!isSemanticallyValid) {
        // Terminate program
        return false;
    } else {
        extractFollows(rootNode);
        extractParent(rootNode);
        extractUses(rootNode, seValidator);
        extractModifies(rootNode, seValidator);
        extractCalls(rootNode, seValidator.adjacencyMatrixOfCalls);

        // Build Cfg for each procedure  + extract Next relationships
        const List<ProcedureNode>* procedureList = &(rootNode.procedureList);
        for (size_t i = 0; i < procedureList->size(); i++) {
            Name procName = procedureList->at(i)->procedureName;
            const StmtlstNode* const stmtListNode = procedureList->at(i)->statementListNode;
            std::pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtListNode);
            // Add CFG root node into PKB
            storeCurrentCfg(cfgInfo.first, procName, &proceduresCfg);
            numberOfCfgNodes.insert({procName, cfgInfo.second});

            // Extract Next relationships
            extractNext(cfgInfo);

            // Initialise visitedProcedureCfg to keep track if a procedure
            // has been visited when building CfgBip
            visitedProcedureCfg.insert({procName, false});
        }

        // Ensure that all procedures is included in a CfgBip
        for (size_t j = 0; j < procedureList->size(); j++) {
            Name procName = procedureList->at(j)->procedureName;
            if (!visitedProcedureCfg.at(procName)) {
                visitedProcedureCfg.at(procName) = true;
                CfgNode* currentCfgBipRootNode
                    = buildCfgBip(&proceduresCfg, procName, &numberOfCfgNodes, &visitedProcedureCfg);
                // Store root node of the CfgBip with the current procedure as the "top"
                storeCFGBip(currentCfgBipRootNode, procName);
            }
        }
        return true;
    }
}
