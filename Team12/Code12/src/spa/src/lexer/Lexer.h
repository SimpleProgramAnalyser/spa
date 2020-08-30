/**
 * Functions related to the lexing of strings.
 */

#ifndef SPA_LEXER_H
#define SPA_LEXER_H

#include "Types.h"

StringList splitByWhitespace(String programFragment);
StringList splitByDelimiter(String str, String delimiter);

#endif // SPA_LEXER_H
