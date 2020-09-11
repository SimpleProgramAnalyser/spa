/**
 * Implementation of SPA frontend design extractor.
 */
#include <iostream>
#include <string>
#include <vector>

#include "SemanticErrorsValidator.h"
#include "FollowsExtractor.h"

Void extractDesign(ProgramNode& rootNode)
{
    SemanticErrorsValidator seValidator(rootNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    if (!isSemanticallyValid) {
        // Terminate Program
        throw std::runtime_error("Semantically invalid program!!"); // TODO: more elegant way to handle
    }

    extractFollows(rootNode);
}
