/**
 * Identifies Next relationships.
 */

#ifndef SPA_FRONTEND_NEXT_EXTRACTOR_H
#define SPA_FRONTEND_NEXT_EXTRACTOR_H

#include <unordered_set>

#include "ast/AstTypes.h"
#include "cfg/CfgTypes.h"

std::vector<std::pair<Integer, Integer>> extractNext(std::pair<CfgNode*, size_t> cfgInfo);

#endif // SPA_FRONTEND_NEXT_EXTRACTOR_H
