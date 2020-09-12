/**
 * Implementation of Uses extractor.
 */
#include "UsesExtractor.h"

Void extractUses(ProgramNode& rootNode, SemanticErrorsValidator& sev) {
    // determine order to extract uses
    std::vector<int> callOrder = sev.reverseTopologicalSort();
}
