/**
 * Methods to generate Abstract Syntax Trees for SIMPLE
 * programs. These methods are meant to be used in the
 * unit tests of the Simple Program Analyser Frontend.
 *
 * Overall, these methods form a database of parsed
 * and unparsed SIMPLE programs, easily accessible
 * for unit testing purposes.
 *
 * In the methods, the same programs will share the same
 * program number. For example, getProgram1String() will
 * return the raw SIMPLE program, while getProgram1Tree()
 * will return the Abstract Syntax Tree representing
 * the program returned in getProgram1String().
 */

#ifndef UNIT_TESTING_AST_UTILS_H
#define UNIT_TESTING_AST_UTILS_H

#include "ast/AstTypes.h"
#include "Types.h"

// Program 1: Basic program with read, assign, print
String getProgram1String();
// Program 1: Basic program with read, assign, print
ProgramNode* getProgram1Tree();
// Program 2: Program with "if" nested in "while"
String getProgram2String();
// Program 2: Program with "if" nested in "while"
ProgramNode* getProgram2Tree();



#endif // UNIT_TESTING_AST_UTILS_H
