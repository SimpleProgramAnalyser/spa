/**
 * Functions for the Control Flow Graph BIP Builder,
 * namely to combine the CFGs of the different procedures.
 */

#ifndef SPA_CFG_BUILDER_BIP_H
#define SPA_CFG_BUILDER_BIP_H

#include "CfgTypes.h"

CfgNode* buildCfgBip(std::unordered_map<Name, CfgNode*>* proceduresCfg, Name firstProcName,
                     std::unordered_map<Name, size_t>* numberOfCfgNodes,
                     std::unordered_map<Name, Boolean>* visitedCfgProcedure);
#endif // SPA_CFG_BUILDER_BIP_H
