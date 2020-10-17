/**
 * Concrete API for SPA frontend parser. The parser
 * transforms a SIMPLE program into an Abstract
 * Syntax Tree, and checks for syntax errors.
 */

#ifndef SPA_FRONTEND_PARSER_H
#define SPA_FRONTEND_PARSER_H

#include "ast/AstTypes.h"
#include "error/InputError.h"

typedef Integer TokenListIndex;

const std::size_t maxErrorCode = 33;

// An array to map error codes to error messages
extern std::array<String, maxErrorCode> errorMessages;

/**
 * This class is used to wrap the result of parsing
 * (an Abstract Syntax Tree node) with a value which
 * refers to the index of the next unparsed Token in
 * the list of Tokens. Also, if this index is set to
 * a negative number, it signifies a syntax error.
 *
 * @tparam T Type of the AST node.
 */
template <typename T>
class ParserReturnType {
public:
    T astNode;
    TokenListIndex nextUnparsedToken;

    explicit ParserReturnType(T nullValue): astNode(std::move(nullValue)), nextUnparsedToken(0) {}
    ParserReturnType(T ast, TokenListIndex next): astNode(std::move(ast)), nextUnparsedToken(next) {}

    // Checks whether there was an error in the parsing.
    Boolean hasError() const
    {
        return nextUnparsedToken < 0;
    }

    // Returns the error code, if there is an error.
    // If there is no error, the behaviour is undefined.
    Integer getErrorCode() const
    {
        return -nextUnparsedToken;
    }

    String getErrorString() const
    {
        if (hasError()) {
            std::size_t errorCode = -nextUnparsedToken;
            if (errorCode > maxErrorCode) {
                return "Syntax Error: Unknown syntax error #" + std::to_string(errorCode);
            } else {
                return "Syntax Error #" + std::to_string(errorCode) + ": " + errorMessages.at(errorCode);
            }
        } else {
            return "";
        }
    }
};

/**
 * Parses a SIMPLE program and returns the root
 * node of the AST, if possible. If there is a
 * syntax error, the ParserReturnType will indicate
 * the type of error.
 *
 * @param rawProgram The raw SIMPLE program string.
 *
 * @return ParserReturnType containing the AST
 *         representing the program.
 */
ParserReturnType<ProgramNode*> parseSimpleReturnNode(const String& rawProgram);

/**
 * Parses a SIMPLE arithmetic expression and returns
 * the root node of the Expression. If there is a
 * syntax error, this function returns nullptr.
 *
 * @param lexedExpression The vector of strings to be parsed.
 * @return Pointer to the Expression node, or nullptr.
 */
Expression* parseExpression(const StringVector& lexedExpression);

#endif // SPA_FRONTEND_PARSER_H
