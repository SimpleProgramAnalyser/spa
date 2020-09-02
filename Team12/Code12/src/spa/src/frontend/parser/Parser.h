/**
 * Concrete API for SPA frontend parser. The parser
 * transforms a SIMPLE program into an Abstract
 * Syntax Tree, and checks for syntax errors.
 */

#ifndef SPA_FRONTEND_PARSER_H
#define SPA_FRONTEND_PARSER_H

#include "Error.h"
#include "ast/AstTypes.h"

Void parseSimple(String rawProgram);
ArithmeticExpression* parseArithmeticExpression(StringList lexedArithmeticExpression);

#endif // SPA_FRONTEND_PARSER_H
