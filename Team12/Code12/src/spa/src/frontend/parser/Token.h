//
// Created by Alden on 2/9/2020.
//

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
    CallKeywordTag = 'a',
    ConstantTag = 'c',
    IfKeywordTag = 'i',
    IdentifierTag = 'd',
    ProcedureTag = 'o',
    PrintKeywordTag = 'p',
    ReadKeywordTag = 'r',
    WhileKeywordTag = 'w',
    SemicolonTag = ';',
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

typedef List<Token> TokenList;

TokenList tokeniseSimple(StringList lexedSimpleProgram);

}

#endif // SPA_FRONTEND_TOKEN_H
