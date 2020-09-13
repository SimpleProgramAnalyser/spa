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
 * program number. For example, getProgram1String_compute()
 * will return the raw SIMPLE program, while
 * getProgram1Tree_compute() will return the Abstract
 * Syntax Tree representing the program returned in
 * getProgram1String_compute().
 */

#ifndef UNIT_TESTING_AST_UTILS_H
#define UNIT_TESTING_AST_UTILS_H

#include "Types.h"
#include "ast/AstTypes.h"

// Program 1: Basic program with read, assign, print
String getProgram1String_compute();
// Program 1: Basic program with read, assign, print
ProgramNode* getProgram1Tree_compute();

// Program 2: Program with "if" nested in "while"
String getProgram2String_factorials();
// Program 2: Program with "if" nested in "while"
ProgramNode* getProgram2Tree_factorials();

// Program 3: computeAverage, program with read, assign, print
String getProgram3String_computeAverage();
// Program 3: computeAverage, program with read, assign, print
ProgramNode* getProgram3Tree_computeAverage();

// Program 4: printAscending, program with read, assign, if/else, print
String getProgram4String_printAscending();
// Program 4: printAscending, program with read, assign, if/else, print
ProgramNode* getProgram4Tree_printAscending();

// Program 5: sumDigit, program with read, assign, while, print
String getProgram5String_sumDigit();
// Program 5: sumDigit, program with read, assign, while, print
ProgramNode* getProgram5Tree_sumDigit();

// Program 6: sumDigitsDuplicate, program with duplicate procedure
String getProgram6String_sumDigitDuplicate();
// Program 6: sumDigitsDuplicate, program with duplicate procedure
ProgramNode* getProgram6Tree_sumDigitDuplicate();

// Program 7: computeCentroid, program with multiple procedures
String getProgram7String_computeCentroid();
// Program 7: computeCentroid, program with multiple procedures
ProgramNode* getProgram7Tree_computeCentroid();

// Program 8: sumDigit but with a cyclic call (2 procedures)
String getProgram8String_sumDigitPlusCyclicCall();
// Program 8: sumDigit but with a cyclic call (2 procedures)
ProgramNode* getProgram8Tree_sumDigitPlusCyclicCall();

// Program 9: sumDigit but with a cyclic call (3 procedures)
String getProgram9String_sumDigitPlusMultiplyCyclicCall();
// Program 9: sumDigit but with a cyclic call (3 procedures)
ProgramNode* getProgram9Tree_sumDigitPlusMultiplyCyclicCall();

// Program 10: Calling a non-existent procedure
String getProgram10String_sumDigitPlusNonexistentProcedureCall();
// Program 10: Calling a non-existent procedure
ProgramNode* getProgram10Tree_sumDigitPlusNonexistentProcedureCall();

// Program 11: Cyclic calls within nested if and while
String getProgram11String_sumDigitPlusMultiplyNestedWhileIfCyclicCall();
// Program 11: Cyclic calls within nested if and while
ProgramNode* getProgram11Tree_sumDigitPlusMultiplyNestedWhileIfCyclicCall();

// Program 12: Recursive program
String getProgram12String_recursivePrintAscending();
// Program 12: Recursive program
ProgramNode* getProgram12Tree_recursivePrintAscending();

// Program 13: If statement and assign
String getProgram13String_ifExample();
// Program 13: If statement and assign
ProgramNode* getProgram13Tree_ifExample();

// Program 14: While statement and assign
String getProgram14String_whileExample();
// Program 14: While statement and assign
ProgramNode* getProgram14Tree_whileExample();

// Program 15: Complicated conditional in while
String getProgram15String_complicatedConditional();
// Program 15: Complicated conditional in while
StringList* getProgram15StringList_complicatedConditional();
// Program 15: Complicated conditional in while
ProgramNode* getProgram15Tree_complicatedConditional();

#endif // UNIT_TESTING_AST_UTILS_H
