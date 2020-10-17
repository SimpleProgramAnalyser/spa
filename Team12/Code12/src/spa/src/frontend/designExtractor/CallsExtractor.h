/**
 * Identifies Calls relationships.
 */

#ifndef SPA_FRONTEND_CALLS_EXTRACTOR_H
#define SPA_FRONTEND_CALLS_EXTRACTOR_H

#include <ast/AstTypes.h>
#include <unordered_set>

#include "SemanticErrorsValidator.h"

std::vector<std::pair<String, String>> extractCalls(const ProgramNode& rootNode, Matrix adjacencyMatrix);

#endif // SPA_FRONTEND_CALLS_EXTRACTOR_H
