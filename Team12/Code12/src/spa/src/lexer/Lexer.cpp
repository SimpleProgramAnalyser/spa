/**
 * Implementation of Lexer shared library
 */

#include "Lexer.h"

#include <cctype>
#include <functional>

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
    return *c == ' ' || *c == '\n' || *c == '\t' || *c == '\r' || *c == '\v' || *c == '\f';
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
static StringList* splitWithPredicate(const String& str, const std::function<bool(const char*)>& predicate)
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

StringList* splitByWhitespace(const String& str) noexcept
{
    return splitWithPredicate(str, isWhitespace);
}

StringList* splitByDelimiter(const String& str, const String& delimiter)
{
    if (delimiter.size() == 1) {
        return splitWithPredicate(str, [delimiter](const char* c) -> bool { return *c == delimiter.at(0); });
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
            // add in the current character
            currentString->push_back(*strChar);

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

enum LexerState : char { DefaultLexerState = '\0', AlphanumericLexerState = 'a', SymbolicLexerState = '=' };

StringList* splitProgram(const String& program) noexcept
{
    auto* splitStrings = new StringList();
    const char* currentChar = program.c_str();
    std::unique_ptr<String> currentString(new String());
    // keep track of what the lexer is expecting next
    LexerState state = DefaultLexerState;
    char expectedNextChar = '\0';

    while (*currentChar != '\0') {
        if (isWhitespace(currentChar) && state != DefaultLexerState) {
            // whitespace always separates two entities
            splitStrings->push_back(std::move(currentString));
            currentString.reset(new String());
            state = DefaultLexerState;
        } else if (isWhitespace(currentChar)) {
            // ignore the whitespace
        } else if (state == SymbolicLexerState && isalnum(*currentChar)) {
            // change state from symbolic to alphanumeric
            // symbol was only one char long
            splitStrings->push_back(std::move(currentString));
            currentString.reset(new String());
            currentString->push_back(*currentChar);
            state = AlphanumericLexerState;
        } else if (isalnum(*currentChar)) {
            currentString->push_back(*currentChar);
            state = AlphanumericLexerState;
        } else if (state == SymbolicLexerState && expectedNextChar == *currentChar) {
            // we managed to find a two character symbol
            currentString->push_back(*currentChar);
            splitStrings->push_back(std::move(currentString));
            currentString.reset(new String());
            state = DefaultLexerState;
        } else {
            if (state == AlphanumericLexerState) {
                // change state from alphanumeric to symbolic
                // we push the lexed token into the list
                splitStrings->push_back(std::move(currentString));
                currentString.reset(new String());
            }
            // check for symbols with more than one characters
            switch (*currentChar) {
            case '>':
            case '<':
            case '=':
            case '!':
                expectedNextChar = '=';
                currentString->push_back(*currentChar);
                state = SymbolicLexerState;
                break;
            case '&':
                expectedNextChar = '&';
                currentString->push_back(*currentChar);
                state = SymbolicLexerState;
                break;
            case '|':
                expectedNextChar = '|';
                currentString->push_back(*currentChar);
                state = SymbolicLexerState;
                break;
            default:
                // symbol is only one character long
                currentString->push_back(*currentChar);
                splitStrings->push_back(std::move(currentString));
                currentString.reset(new String());
                state = DefaultLexerState;
            }
        }
        currentChar++;
    }

    // insert last word if any
    if (!currentString->empty()) {
        splitStrings->push_back(std::move(currentString));
    }

    return splitStrings;
}

String trimWhitespace(const String& str)
{
    // boundary check
    if (str.length() == 0) {
        return str;
    }

    size_t firstIndex = 0;
    size_t lastIndex = str.length() - 1;

    // find first char non-whitespace
    const char* currentChar = &(str.at(firstIndex));
    while (isWhitespace(currentChar) && firstIndex < lastIndex) {
        firstIndex++;
        currentChar = &(str.at(firstIndex));
    }
    // find last char non-whitespace
    currentChar = &(str.at(lastIndex));
    while (isWhitespace(currentChar) && firstIndex < lastIndex) {
        lastIndex--;
        currentChar = &(str.at(lastIndex));
    }
    // if not found
    if (firstIndex > lastIndex) {
        return str;
    } else if (isWhitespace(currentChar)) {
        // the string consists only of whitespaces
        return "";
    }
    // copy chars
    return str.substr(firstIndex, lastIndex - firstIndex + 1);
}
