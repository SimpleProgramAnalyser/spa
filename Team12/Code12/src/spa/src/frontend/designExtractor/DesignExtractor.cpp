/**
 * Implementation of SPA frontend design extractor.
 */
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "SemanticErrorsValidator.h"

Void extractDesign(ProgramNode& rootNode)
{
    SemanticErrorsValidator seValidator(rootNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    if (!isSemanticallyValid) {
        // Terminate Program
    }
}
