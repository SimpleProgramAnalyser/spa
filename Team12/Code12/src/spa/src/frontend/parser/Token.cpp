//
// Created by Alden on 2/9/2020.
//

#include "Token.h"

frontend::Token::Token(String s, frontend::Tag t): rawString(std::move(s)), tokenTag(t) {}
