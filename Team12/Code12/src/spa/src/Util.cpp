/**
 * Implementation of utility functions for SPA.
 */

#ifndef SPA_UTIL_CPP
#define SPA_UTIL_CPP

#include "Util.h"

Boolean isMatchingRegex(const String& rawInput, const String& regex)
{
    return std::regex_match(rawInput, std::regex(regex));
}

Boolean util::isPossibleIdentifier(const String& str)
{
    return isMatchingRegex(str, "[A-Za-z]([A-Za-z]|[\\d])*");
}

Boolean util::isPossibleConstant(const String& str)
{
    return isMatchingRegex(str, "\\d+");
}

Boolean util::isRelationshipReference(const String& str)
{
    return isMatchingRegex(str, "^(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies)$");
}

Boolean util::isLiteralIdent(const String& str)
{
    if (str.size() > 1 && str[0] == '"' && str[str.size() - 1] == '"') {
        String possibleIdent = str.substr(1, str.size() - 1);
        return util::isPossibleIdentifier(possibleIdent);
    } else {
        return false;
    }
}

#endif // SPA_UTIL_CPP
