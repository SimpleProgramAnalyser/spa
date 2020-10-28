/**
 * An array that holds the error messages for the SPA Parser.
 * The error codes correspond to the index of the message in the array.
 */

#include <array>

#include "Parser.h"

std::array<String, maxErrorCode> errorMessages
    = {"",
       "Expected \"procedure\" keyword.",
       "Procedure parsing failed, is procedure name given?",
       "Statement list parsing failed, is statement list empty, or are braces mismatched?",
       "Statement parsing failed, reached end of program.",
       "Statement parsing failed, unrecognised statement.",
       "Call statement parsing failed, is procedure name missing?",
       "Print statement parsing failed, is variable name missing?",
       "Read statement parsing failed, is variable name missing?",
       "If statement parsing failed, is \"then\" missing?",
       "If statement parsing failed, expected statement list after \"then\"",
       "If statement parsing failed, reached end of program.",
       "If statement parsing failed, is \"else\" missing?",
       "If statement parsing failed, is the condition missing?",
       "While statement parsing failed, does condition have a closing bracket?",
       "While statement parsing failed, is the condition missing?",
       "Assignment statement parsing failed, is the semicolon missing?",
       "Assignment statement parsing failed, is the equal sign missing?",
       "Reference expression parsing failed, missing constant or variable reference.",
       "Reference expression parsing failed, expression is not a constant or variable.",
       "Arithmetic expression parsing failed, reached end of expression.",
       "Arithmetic expression parsing failed, is an arithmetic operator missing?",
       "Expression parsing failed, reached end of bounds.",
       "Expression parsing failed, expected constant or variable reference.",
       "Relational expression parsing failed, reached end of bounds.",
       "Relational expression parsing failed, is a relational operator missing?",
       "Conditional expression parsing failed, reached end of bounds.",
       "Conditional expression parsing failed, does condition have a closing bracket?",
       "Conditional expression parsing failed, exceeded closing bracket while parsing.",
       "Conditional expression parsing failed, expected conditional or relational operator after closing bracket.",
       "Conditional expression parsing failed, unrecognised expression.",
       "Conditional expression parsing failed, expected closing bracket for !( ... )",
       "Conditional expression parsing failed, expected closing bracket for (...) opr (...)",
       "Conditional expression parsing failed, unexpected token after (...) opr (...)",
       "Statement list parsing failed, expected closing brace after statement list."};
