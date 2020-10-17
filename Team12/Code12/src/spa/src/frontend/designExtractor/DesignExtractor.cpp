/**
 * Implementation of SPA frontend design extractor.
 */
#include <iostream>

#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "SemanticErrorsValidator.h"
#include "UsesExtractor.h"

Boolean extractDesign(ProgramNode& rootNode)
{
    SemanticErrorsValidator seValidator(rootNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    if (!isSemanticallyValid) {
        // Terminate program
        return false;
    } else {
        extractFollows(rootNode);
        extractParent(rootNode);
        extractUses(rootNode, seValidator);
        extractModifies(rootNode, seValidator);
        return true;
    }
}
