/**
 * Class structure of Control Flow Graph,
 * types, fields and methods.
 */

#ifndef SPA_CFG_TYPES_H
#define SPA_CFG_TYPES_H

#include <Types.h>
#include <ast/AstTypes.h>
#include <utility>

typedef String Name;

class CfgNode {
public:
    Vector<StatementNode*>* statementNodes;
    Vector<CfgNode*>* childrenNodes;
    size_t nodeNumber;

    CfgNode(Vector<StatementNode*>* statements, Vector<CfgNode*>* children, size_t cfgNodeNumber);
    ~CfgNode();
    CfgNode(const CfgNode&) = delete;
    CfgNode& operator=(const CfgNode&) = delete;
    CfgNode(CfgNode&&) = delete;
    CfgNode& operator=(CfgNode&&) = delete;

    // A method to find all unique children of this node.
    // This method is non-recursive.
    Vector<CfgNode*> findAllChildren() const;

    // Checks if this CFG is equal to another CFG,
    // including all accessible children.
    Boolean equals(CfgNode* cfgn, size_t numberOfNodes);

    // Counts the number of unique children of this node.
    size_t size() const;
};

#endif // SPA_CFG_TYPES_H
