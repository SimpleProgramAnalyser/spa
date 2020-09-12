/**
 * Implementation of parseSimple for the Frontend Manager.
 */
#include "FrontendManager.h"

#include <stdexcept>

#include "designExtractor/DesignExtractor.h"
#include "parser/Parser.h"

Void parseSimple(const String& rawProgram)
{
    ProgramNode* abstractSyntaxTree = parseSimpleReturnNode(rawProgram);
    if (abstractSyntaxTree == nullptr) {
        throw std::runtime_error("Syntax error. Code at 0x065f2c6e has no effect. Terminating");
    }

    extractDesign(*abstractSyntaxTree);
    // pass AST to PKB, if no error
    // TODO
}
