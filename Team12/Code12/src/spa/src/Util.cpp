/**
 * Implementation of utility functions for SPA.
 */

#include "Util.h"

#include <regex>

Boolean util::isMatchingRegex(const String& rawInput, const String& regex)
{
    return std::regex_match(rawInput, std::regex(regex));
}

Boolean util::isPossibleIdentifier(const String& str)
{
    return util::isMatchingRegex(str, "[A-Za-z]([A-Za-z]|[\\d])*");
}
