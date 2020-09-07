/**
 * Functions related to the lexing of strings.
 */

#ifndef SPA_LEXER_H
#define SPA_LEXER_H

#include "Types.h"

StringList* splitByWhitespace(const String& programFragment) noexcept;
StringList* splitByDelimiter(const String& str, const String& delimiter);
StringList* splitProgram(const String& program) noexcept;
String trimWhitespace(const String& str);

#endif // SPA_LEXER_H
