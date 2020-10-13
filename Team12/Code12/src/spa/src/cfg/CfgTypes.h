/**
 * Class structure of Control Flow Graph,
 * types, fields and methods.
 */

#ifndef SPA_CFG_TYPES_H
#define SPA_CFG_TYPES_HSS

#include <Types.h>
#include <ast/AstTypes.h>
#include <utility>

typedef String Name;

class CfgNode {
public:
    List<StatementNode>* statementNodes;
    List<CfgNode>* childrenNodes;
    size_t nodeNumber;

    CfgNode(List<StatementNode>* statements, List<CfgNode>* children, size_t cfgNodeNumber);
    ~CfgNode();
    CfgNode(const CfgNode&) = delete;
    CfgNode& operator=(const CfgNode&) = delete;
    CfgNode(CfgNode&&) = delete;
    CfgNode& operator=(CfgNode&&) = delete;

    Boolean equals(CfgNode* cfgn, size_t numberOfNodes);
};

#endif // SPA_CFG_TYPES_H
