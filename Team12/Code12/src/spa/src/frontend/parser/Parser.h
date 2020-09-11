/**
 * Concrete API for SPA frontend parser. The parser
 * transforms a SIMPLE program into an Abstract
 * Syntax Tree, and checks for syntax errors.
 */

#ifndef SPA_FRONTEND_PARSER_H
#define SPA_FRONTEND_PARSER_H

#include "error/InputError.h"
#include "ast/AstTypes.h"

Void parseSimple(const String& rawProgram);
Expression* parseExpression(StringList* lexedExpression);
Void postSyntaxError(InputError syntaxError);

#endif // SPA_FRONTEND_PARSER_H
