/**
 * Functions for the Control Flow Graph Builder,
 * namely for creation of Control Flow Graph nodes.
 */

#ifndef SPA_CFG_BUILDER_H
#define SPA_CFG_BUILDER_H

#include "CfgTypes.h"

CfgNode* createCfgNode(size_t stmtListSize, size_t &currentNumberOfNodes);
CfgNode* buildCfg(const StmtlstNode* const rootNode);

#endif // SPA_CFG_BUILDER_H
