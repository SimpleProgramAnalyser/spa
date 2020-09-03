/**
 * Types and methods used for tokenising of
 * SIMPLE program strings.
 */

#ifndef SPA_FRONTEND_TOKEN_H
#define SPA_FRONTEND_TOKEN_H

#include "Types.h"

namespace frontend {

enum Tag: char {
    // statement syntax
    AssignmentTag = '=',
    BracesOpenTag = '{',
    BracesClosedTag = '}',
    BracketOpenTag = '(',
    BracketClosedTag = ')',
    ConstantTag = 'c',
    SemicolonTag = ';',
    // keywords 010xxxxx
    CallKeywordTag = 'C',
    ElseKeywordTag = 'E',
    IfKeywordTag = 'F',
    ProcedureKeywordTag = 'O',
    PrintKeywordTag = 'P',
    ReadKeywordTag = 'R',
    ThenKeywordTag = 'T',
    WhileKeywordTag = 'W',
    // identifiers
    IdentifierTag = 'I',
    // expression operators
    GtTag = '>',
    GteTag = 'g',
    LtTag = '<',
    LteTag = 'l',
    NeqTag = 'n',
    EqTag = 'q',
    PlusTag = '+',
    MinusTag = '-',
    TimesTag = '*',
    DivideTag = '/',
    ModuloTag = '%',
    NotConditionalTag = '!',
    AndConditionalTag = '&',
    OrConditionalTag = '|',
};

class Token {
public:
    const String rawString;
    const Tag tokenTag;
    Token(String s, Tag t);
};

Boolean isPossibleIdentifier(Tag tag);

typedef List<Token> TokenList;
TokenList tokeniseSimple(StringList lexedSimpleProgram);

}

#endif // SPA_FRONTEND_TOKEN_H
