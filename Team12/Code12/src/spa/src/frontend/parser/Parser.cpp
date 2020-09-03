/**
 * Implementation of SPA frontend parser.
 */

#include "Parser.h"

#include "lexer/Lexer.h"
#include "Token.h"

Void parseSimple(String rawProgram)
{
    StringList* splitProgram = splitByWhitespace(rawProgram);
    frontend::TokenList* tokenisedProgram = frontend::tokeniseSimple(splitProgram);
    delete splitProgram;



    delete tokenisedProgram;

}

ArithmeticExpression* parseArithmeticExpression(StringList lexedArithmeticExpression) {}
