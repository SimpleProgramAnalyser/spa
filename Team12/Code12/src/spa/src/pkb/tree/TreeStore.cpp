/**
 * Implementation of TreeStore class for the PKB.
 * This class handles storage of the AST and CFG.
 */

#include "TreeStore.h"

#include <iterator>

// Instantiate a new TreeStore
TreeStore::TreeStore():
    rootNode(nullptr), cfgByProcedure(), proceduresWithCfg(), cfgBipByProcedure(), proceduresWithCfgBip()
{}

// Clear all the trees when deleted
TreeStore::~TreeStore()
{
    // ensure deleted nodes are unique
    std::unordered_set<CfgNode*> nodesToDelete;
    // traverse CFG
    for (std::pair<ProcedureName, CfgNode*> mapEntry : cfgByProcedure) {
        std::vector<CfgNode*> childNodes = mapEntry.second->findAllChildren();
        std::copy(childNodes.begin(), childNodes.end(), std::inserter(nodesToDelete, nodesToDelete.begin()));
    }
    // traverse CFG BIP
    for (std::pair<ProcedureName, CfgNode*> mapEntry : cfgBipByProcedure) {
        std::vector<CfgNode*> childNodes = mapEntry.second->findAllChildren();
        std::copy(childNodes.begin(), childNodes.end(), std::inserter(nodesToDelete, nodesToDelete.begin()));
    }
    // delete the CFG nodes
    for (CfgNode* node : nodesToDelete) {
        delete node;
    }
    // delete AST
    delete rootNode;
}

// AST RootNode
void TreeStore::assignRootNode(ProgramNode* rootNodeToAssign)
{
    rootNode = rootNodeToAssign;
}
ProgramNode* TreeStore::getRootNode()
{
    return rootNode;
}

// CFG
void TreeStore::storeCFG(CfgNode* cfg, const ProcedureName& procedureName)
{
    cfgByProcedure[procedureName] = cfg;
    proceduresWithCfg.push_back(procedureName);
}
CfgNode* TreeStore::getCFG(const ProcedureName& procedureName)
{
    if (cfgByProcedure.find(procedureName) == cfgByProcedure.end()) {
        return nullptr;
    } else {
        return cfgByProcedure[procedureName];
    }
}
Vector<String> TreeStore::getProceduresWithCFG()
{
    return proceduresWithCfg;
}

// CFG Bip
void TreeStore::storeCFGBip(CfgNode* cfgBip, const ProcedureName& procedureName)
{
    cfgBipByProcedure[procedureName] = cfgBip;
    proceduresWithCfgBip.push_back(procedureName);
}
CfgNode* TreeStore::getCFGBip(const ProcedureName& procedureName)
{
    if (cfgBipByProcedure.find(procedureName) == cfgBipByProcedure.end()) {
        return nullptr;
    } else {
        return cfgBipByProcedure[procedureName];
    }
}
Vector<String> TreeStore::getProceduresWithCFGBip()
{
    return proceduresWithCfgBip;
}
