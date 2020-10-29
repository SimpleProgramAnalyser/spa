/**
 * A place to store the various trees created by
 * Frontend and used by Query Evaluator.
 */

#ifndef SPA_PKB_TREE_STORE_H
#define SPA_PKB_TREE_STORE_H

#include "cfg/CfgTypes.h"
#include "pkb/PkbTypes.h"

class TreeStore {
private:
    // AST
    ProgramNode* rootNode = nullptr;
    // CFG
    HashMap<ProcedureName, CfgNode*> cfgByProcedure;
    Vector<ProcedureName> proceduresWithCfg;
    // CFG BIP
    HashMap<ProcedureName, CfgNode*> cfgBipByProcedure;
    Vector<ProcedureName> proceduresWithCfgBip;

public:
    TreeStore() = default;
    ~TreeStore();
    TreeStore(const TreeStore&) = default;
    TreeStore(TreeStore&&) = delete;
    TreeStore& operator=(const TreeStore&) = default;
    TreeStore& operator=(TreeStore&&) = delete;

    // Stores the AST root node in the PKB.
    void assignRootNode(ProgramNode* rootNodeToAssign);
    // Gets the AST root node from the PKB.
    ProgramNode* getRootNode();

    // Stores the CFG root node in the PKB, for a procedure.
    void storeCFG(CfgNode* cfg, const ProcedureName& procedureName);
    // Gets the CFG root node in the PKB, for a procedure.
    CfgNode* getCFG(const ProcedureName& procedureName);
    // Gets all procedures with a CFG node.
    Vector<String> getProceduresWithCFG();

    // Stores the CFG with branching into procedures in the PKB, for a procedure.
    void storeCFGBip(CfgNode* cfgBip, const ProcedureName& procedureName);
    // Gets the CFG with branching into procedures in the PKB, for a procedure.
    CfgNode* getCFGBip(const ProcedureName& procedureName);
    // Gets all procedures with a CFG node branching into procedures.
    Vector<String> getProceduresWithCFGBip();
};

#endif // SPA_PKB_TREE_STORE_H
