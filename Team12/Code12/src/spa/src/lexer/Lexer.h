/**
 * Functions related to the lexing of strings.
 */

#ifndef SPA_LEXER_H
#define SPA_LEXER_H

#include "Types.h"

Boolean isWhitespace(const char* c) noexcept;
StringList* splitByWhitespace(const String& programFragment) noexcept;
StringList* splitByDelimiter(const String& str, const String& delimiter);
StringList* splitProgram(const String& program) noexcept;

#endif // SPA_LEXER_H
