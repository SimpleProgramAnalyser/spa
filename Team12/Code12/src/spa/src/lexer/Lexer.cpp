/**
 * Implementation of Lexer shared library
 */

#include <functional>

#include "Lexer.h"

/**
 * Checks if a given char should be treated as
 * whitespace by the lexer.
 *
 * @param c The character to check.
 *
 * @return True, if the character represents some whitespace.
 */
static inline bool isWhitespace(const char* c) noexcept
{
    return *c == ' ' || *c == '\n' || *c == '\t';
}

/**
 * Given a predicate function that accepts a char* as
 * the argument, split the given string at the points
 * where the character returns true with the predicate,
 * omitting that character from the final output.
 *
 * Note that the result list of strings will need
 * to be deleted to prevent memory leaks.
 *
 * @param str The string to be split.
 * @param predicate A predicate function to check
 * whether a given character should be used as
 * a splitting point for the string.
 *
 * @return List of the split strings.
 */
static StringList* splitWithPredicate(
        const String& str,
        const std::function<bool(const char*)>& predicate)
{
    auto* splitStrings = new StringList();
    const char* currentChar = str.c_str();
    std::unique_ptr<String> currentString(new String());

    while (*currentChar != '\0') {
        if (!currentString->empty() && predicate(currentChar)) {
            splitStrings->push_back(std::move(currentString));
            currentString.reset(new String());
        } else if (!predicate(currentChar)) {
            currentString->push_back(*currentChar);
        }

        currentChar++;
    }

    // insert last word if any
    if (!currentString->empty()) {
        splitStrings->push_back(std::move(currentString));
    }

    return splitStrings;
}

StringList* splitByWhitespace(const String& programFragment) noexcept
{
    return splitWithPredicate(programFragment, isWhitespace);
}


StringList* splitByDelimiter(const String& str, const String& delimiter)
{
    if (delimiter.size() == 1) {
        return splitWithPredicate(str,
                [delimiter](const char* c) -> bool
                {
                    return *c == delimiter.at(0);
                }
        );
    }

    auto* splitStrings = new StringList();
    const char* strChar = str.c_str();
    const char* delimiterChar = delimiter.c_str();
    bool isMatchingDelimiter = false;
    std::unique_ptr<String> currentString(new String());
    std::unique_ptr<String> bufferString(new String());

    while (*strChar != '\0') {
        if (*strChar == *delimiterChar) {
            // still unsure whether delimiter is matched or not
            isMatchingDelimiter = true;
            bufferString->push_back(*strChar);
            delimiterChar++;

        } else if (isMatchingDelimiter && *delimiterChar == '\0') {
            // successfully matched the entire delimiter
            splitStrings->push_back(std::move(currentString));
            currentString.reset(new String());
            isMatchingDelimiter = false;
            // clear buffer
            bufferString.reset(new String());
            // reset delimiterChar
            delimiterChar = delimiter.c_str();

        } else if (isMatchingDelimiter) {
            // failed to match the entire delimiter
            isMatchingDelimiter = false;
            currentString->append(*bufferString);
            currentString->push_back(*strChar);
            // clear buffer
            bufferString.reset(new String());
            // reset delimiterChar
            delimiterChar = delimiter.c_str();

        } else {
            // did not match delimiter at all
            currentString->push_back(*strChar);
        }

        strChar++;
    }

    // insert last word if any
    if (!currentString->empty()) {
        currentString->append(*bufferString);
        splitStrings->push_back(std::move(currentString));
    }

    return splitStrings;
}
