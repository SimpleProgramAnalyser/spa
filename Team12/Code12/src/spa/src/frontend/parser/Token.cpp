/**
 * Implementation of methods used for tokenising
 * strings of SIMPLE programs.
 */

#include "Token.h"

using namespace frontend;

Token::Token(String s, frontend::Tag t): rawString(std::move(s)), tokenTag(t) {}

Boolean isPossibleIdentifier(Tag tag)
{
    // match the bit pattern 010xxxxx
    uint8_t firstThreeBits = static_cast<uint8_t>(tag) >> 5u;
    return firstThreeBits == 0x02;
}

TokenList tokeniseSimple(StringList lexedSimpleProgram)
{
    TokenList tokens;
    int numberOfStrings = lexedSimpleProgram.size();

    for (int i = 0; i < numberOfStrings; i++) {
    }

    return tokens;
}
