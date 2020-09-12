/**
 * Implementation of parseSimple for the Frontend Manager.
 */
#include "FrontendManager.h"

#include "designExtractor/DesignExtractor.h"
#include "parser/Parser.h"

Void parseSimple(const String& rawProgram)
{
    ProgramNode* abstractSyntaxTree = parseSimpleReturnNode(rawProgram);
    extractDesign(*abstractSyntaxTree);
    // pass AST to PKB, if no error
    // TODO
}
