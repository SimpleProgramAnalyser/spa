/**
 * Implementation of SPA frontend design extractor.
 */
#include <iostream>

#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "SemanticErrorsValidator.h"
#include "UsesExtractor.h"

Void extractDesign(ProgramNode& rootNode)
{
    SemanticErrorsValidator seValidator(rootNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    if (!isSemanticallyValid) {
        // Terminate Program
        throw std::runtime_error("Semantically invalid program!!"); // TODO: more elegant way to handle
    }

    extractFollows(rootNode);
    extractUses(rootNode, seValidator);
    extractModifies(rootNode, seValidator);
}
