/**
 * Implementation of parseSimple for the Frontend Manager.
 */
#include "FrontendManager.h"

#include "Ui.h"
#include "designExtractor/DesignExtractor.h"
#include "parser/Parser.h"
#include "pkb/PKB.h"

Void parseSimple(const String& rawProgram, Ui& ui)
{
    ParserReturnType<ProgramNode*> parsedProgram = parseSimpleReturnNode(rawProgram);
    if (parsedProgram.hasError()) {
        ui.postUiError(InputError(parsedProgram.getErrorString(), 1, 1, SimpleProgram, Syntax));
        resetPKB();
        return;
    }
    // if no error, we get the root node
    ProgramNode* abstractSyntaxTree = parsedProgram.astNode;
    // populate Program Knowledge Base with Design Extractor
    Boolean hasSemanticError = !extractDesign(*abstractSyntaxTree);
    if (hasSemanticError) {
        ui.postUiError(InputError("Semantic Error: Check called procedures.", 1, 1, SimpleProgram, Semantic));
        resetPKB();
        return;
    }
    // if no error, store root node in Program Knowledge Base
    assignRootNode(abstractSyntaxTree);
}
