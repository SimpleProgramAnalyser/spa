/**
 * Functions related to the lexing of strings.
 */

#ifndef SPA_LEXER_H
#define SPA_LEXER_H

#include "Types.h"

Boolean isWhitespace(const char* c) noexcept;
StringVector splitByWhitespace(const String& str) noexcept;
StringVector splitByDelimiter(const String& str, const String& delimiter);
StringVector splitProgram(const String& program) noexcept;
String trimWhitespace(const String& str);

#endif // SPA_LEXER_H
