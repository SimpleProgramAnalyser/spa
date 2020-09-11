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

#include "Types.h"
#include "ast/AstTypes.h"

// Program 1: Basic program with read, assign, print
String getProgram1String();
// Program 1: Basic program with read, assign, print
ProgramNode* getProgram1Tree();

// Program 2: Program with "if" nested in "while"
String getProgram2String();
// Program 2: Program with "if" nested in "while"
ProgramNode* getProgram2Tree();

// Program 3: computeAverage, program with read, assign, print
String getProgram3String();
// Program 3: computeAverage, program with read, assign, print
ProgramNode* getProgram3Tree();

// Program 4: printAscending, program with read, assign, if/else, print
String getProgram4String();
// Program 4: printAscending, program with read, assign, if/else, print
ProgramNode* getProgram4Tree();

// Program 5: sumDigit, program with read, assign, while, print
String getProgram5String();
// Program 5: sumDigit, program with read, assign, while, print
ProgramNode* getProgram5Tree();

// Program 6: sumDigitsDuplicate, program with duplicate procedure
String getProgram6String();
// Program 6: sumDigitsDuplicate, program with duplicate procedure
ProgramNode* getProgram6Tree();

// Program 7: computeCentroid, program with multiple procedures
String getProgram7String();
// Program 7: computeCentroid, program with multiple procedures
ProgramNode* getProgram7Tree();

// Program 8: sumDigit but with a cyclic call (2 procedures)
String getProgram8String();
// Program 8: sumDigit but with a cyclic call (2 procedures)
ProgramNode* getProgram8Tree();

// Program 9: sumDigit but with a cyclic call (3 procedures)
String getProgram9String();
// Program 9: sumDigit but with a cyclic call (3 procedures)
ProgramNode* getProgram9Tree();

// Program 10: Calling a non-existent procedure
String getProgram10String();
// Program 10: Calling a non-existent procedure
ProgramNode* getProgram10Tree();

// Program 11: Cyclic calls within nested if and while
String getProgram11String();
// Program 11: Cyclic calls within nested if and while
ProgramNode* getProgram11Tree();

// Program 12: Recursive program
String getProgram12String();
// Program 12: Recursive program
ProgramNode* getProgram12Tree();

#endif // UNIT_TESTING_AST_UTILS_H
