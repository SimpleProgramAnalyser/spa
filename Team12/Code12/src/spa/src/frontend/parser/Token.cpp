/**
 * Implementation of methods used for tokenising
 * strings of SIMPLE programs.
 */

#include "Token.h"

using namespace frontend;

Token::Token(String s, frontend::Tag t): rawString(std::move(s)), tokenTag(t) {}

TokenList tokeniseSimple(StringList lexedSimpleProgram) {
    TokenList tokens;
    int numberOfStrings = lexedSimpleProgram.size();

    for (int i = 0; i < numberOfStrings; i++) {

    }

    return tokens;
}
