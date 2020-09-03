/**
 * Implementation of methods used for tokenising
 * strings of SIMPLE programs.
 */

#include "Token.h"

#include <regex>

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
str_match::Trie<Tag>* generateSimpleTrie()
{
    auto* trie = new str_match::Trie<Tag>();
    // populate trie with SIMPLE program keywords
    // statement syntax
    Tag nonEndNodeValue = NullTag;
    trie->addEntryToTrie("=", AssignmentTag, nonEndNodeValue);
    trie->addEntryToTrie("{", BracesOpenTag, nonEndNodeValue);
    trie->addEntryToTrie("}", BracesClosedTag, nonEndNodeValue);
    trie->addEntryToTrie("(", BracketOpenTag, nonEndNodeValue);
    trie->addEntryToTrie(")", BracketClosedTag, nonEndNodeValue);
    trie->addEntryToTrie(";", SemicolonTag, nonEndNodeValue);
    // keywords
    trie->addEntryToTrie("call", CallKeywordTag, nonEndNodeValue);
    trie->addEntryToTrie("else", ElseKeywordTag, nonEndNodeValue);
    trie->addEntryToTrie("if", IfKeywordTag, nonEndNodeValue);
    trie->addEntryToTrie("procedure", ProcedureKeywordTag, nonEndNodeValue);
    trie->addEntryToTrie("print", PrintKeywordTag, nonEndNodeValue);
    trie->addEntryToTrie("read", ReadKeywordTag, nonEndNodeValue);
    trie->addEntryToTrie("then", ThenKeywordTag, nonEndNodeValue);
    trie->addEntryToTrie("while", WhileKeywordTag, nonEndNodeValue);
    // expression operators
    trie->addEntryToTrie(">", GtTag, nonEndNodeValue);
    trie->addEntryToTrie(">=", GteTag, nonEndNodeValue);
    trie->addEntryToTrie("<", LtTag, nonEndNodeValue);
    trie->addEntryToTrie("<=", LteTag, nonEndNodeValue);
    trie->addEntryToTrie("!=", NeqTag, nonEndNodeValue);
    trie->addEntryToTrie("==", EqTag, nonEndNodeValue);
    trie->addEntryToTrie("+", PlusTag, nonEndNodeValue);
    trie->addEntryToTrie("-", MinusTag, nonEndNodeValue);
    trie->addEntryToTrie("*", TimesTag, nonEndNodeValue);
    trie->addEntryToTrie("/", DivideTag, nonEndNodeValue);
    trie->addEntryToTrie("%", ModuloTag, nonEndNodeValue);
    trie->addEntryToTrie("!", NotConditionalTag, nonEndNodeValue);
    trie->addEntryToTrie("&&", AndConditionalTag, nonEndNodeValue);
    trie->addEntryToTrie("||", OrConditionalTag, nonEndNodeValue);
    return trie;
}

Boolean frontend::isIdentifierTag(Tag tag)
{
    // match the bit pattern 010xxxxx
    uint8_t firstThreeBits = static_cast<uint8_t>(tag) >> 5u;
    return firstThreeBits == 0x02;
}

Boolean isMatchingRegex(const String& rawInput, const String& regex)
{
    return std::regex_match(rawInput, std::regex(regex));
}

Boolean frontend::isPossibleIdentifier(const String& str)
{
    return isMatchingRegex(str, "[A-Za-z]([A-Za-z]|[\\d])*");
}

Boolean frontend::isPossibleConstant(const String& str)
{
    return isMatchingRegex(str, "\\d+");
}

TokenList* frontend::tokeniseSimple(StringList* lexedSimpleProgram)
{
    auto* tokens = new TokenList();
    int numberOfStrings = lexedSimpleProgram->size();
    str_match::Trie<Tag>* lookupTrie = generateSimpleTrie();

    for (int i = 0; i < numberOfStrings; i++) {
        String currentString = *lexedSimpleProgram->at(i);
        Tag tokenTag = lookupTrie->matchString(currentString, NullTag);
        if (tokenTag == NullTag) {
            if (isPossibleIdentifier(currentString)) {
                tokens->push_back(std::unique_ptr<Token>(new Token(currentString, IdentifierTag)));
            } else if (isPossibleConstant(currentString)) {
                tokens->push_back(std::unique_ptr<Token>(new Token(currentString, ConstantTag)));
            } else {
                // tokeniser is not sure what the item is
                tokens->push_back(std::unique_ptr<Token>(new Token(currentString, UnknownTag)));
            }
        } else {
            tokens->push_back(std::unique_ptr<Token>(new Token(currentString, tokenTag)));
        }
    }

    delete lookupTrie;
    return tokens;
}
