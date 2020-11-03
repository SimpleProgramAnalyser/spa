/**
 * Implementation of utility functions for SPA.
 */

#include "Util.h"

#include <regex>

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

Boolean util::isLiteral(const String& str)
{
    return str.size() > 2 && str[0] == '"' && str[str.size() - 1] == '"';
}

Boolean util::isLiteralIdent(const String& str)
{
    if (isLiteral(str)) {
        String possibleIdent = str.substr(1, str.size() - 2);
        return util::isPossibleIdentifier(possibleIdent);
    } else {
        return false;
    }
}

String util::removeCharFromBothEnds(String str)
{
    return str.substr(1, str.size() - 2);
}

/**
 * Returns a pair of Strings, split by the
 * first appearance of the delimiter. If the
 * delimiter does not exist, a pair of empty
 * strings will be returned. The particular
 * delimiter will be removed.
 *
 * @param str String to be split.
 * @param c Delimiter.
 * @return Pair of Strings split by delimiter.
 */
StringPair util::splitByFirstDelimiter(const String& str, char c)
{
    StringPair stringVector;
    const char* currentChar = str.c_str();
    String currentToken;

    // Find first delimiter
    while (*currentChar != c && *currentChar != '\0') {
        currentToken.push_back(*currentChar);
        currentChar++;
    }

    String leftString = currentToken;
    currentToken.clear();

    if (*currentChar != '\0') {
        currentChar++; // Skips '('
    }

    while (*currentChar != '\0') {
        currentToken.push_back(*currentChar);
        currentChar++;
    }

    return std::make_pair(leftString, currentToken);
}

/**
 * Returns true if a non-empty substring is enclosed
 * by the given left end and right character.
 *
 * @param str String to be checked
 * @param leftEnd Character at left end
 * @param rightEnd Character at right end
 */
Boolean util::isEnclosedWith(String str, char leftEnd, char rightEnd)
{
    return str[0] == leftEnd && str.size() > 2 && str[str.size() - 1] == rightEnd;
}