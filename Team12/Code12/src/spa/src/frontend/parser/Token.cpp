/**
 * Implementation of methods used for tokenising
 * strings of SIMPLE programs.
 */

#include "Token.h"

#include "StringMatcher.h"

using namespace frontend;

Token::Token(String s, Tag t): rawString(std::move(s)), tokenTag(t) {}



/**
 * Generates a trie for SIMPLE program keywords.
 * The caller of this function has to remember
 * to delete the trie created, after usage.
 *
 * @return A trie that can be used to identify SIMPLE
 *         program keywords efficiently.
 */
str_match::Trie<Tag>* generateSimpleTrie() {
    auto* trie = new str_match::Trie<Tag>();
//    // populate trie with SIMPLE program keywords
//    // statement syntax
//    AssignmentTag = '=',
//    BracesOpenTag = '{',
//    BracesClosedTag = '}',
//    BracketOpenTag = '(',
//    BracketClosedTag = ')',
//    ConstantTag = 'c',
//    SemicolonTag = ';',
//    // keywords
//    CallKeywordTag = 'C',
//    ElseKeywordTag = 'E',
//    IfKeywordTag = 'F',
//    ProcedureKeywordTag = 'O',
//    PrintKeywordTag = 'P',
//    ReadKeywordTag = 'R',
//    ThenKeywordTag = 'T',
//    WhileKeywordTag = 'W',
//    // expression operators
//    GtTag = '>',
//    GteTag = 'g',
//    LtTag = '<',
//    LteTag = 'l',
//    NeqTag = 'n',
//    EqTag = 'q',
//    PlusTag = '+',
//    MinusTag = '-',
//    TimesTag = '*',
//    DivideTag = '/',
//    ModuloTag = '%',
//    NotConditionalTag = '!',
//    AndConditionalTag = '&',
//    OrConditionalTag = '|',
    return trie;
}

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
