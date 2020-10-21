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

// Program 7: computeCentroid, program with multiple procedures, with if, while, read, print, call, assign
String getProgram7String_computeCentroid();
// Get a syntactically incorrect version of program 7, with mismatched braces
String getProgram7StringMismatchedBraces_computeCentroid();
// Get a syntactically incorrect version of program 7, with mismatched brackets
String getProgram7StringMismatchedBrackets_computeCentroid();
// Program 7: computeCentroid, program with multiple procedures, with if, while, read, print, call, assign
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
StringVector getProgram15StringList_complicatedConditional();
// Program 15: Complicated conditional in while
ProgramNode* getProgram15Tree_complicatedConditional();

// Program 16: Keywords used as identifier
String getProgram16String_keywordsAsIdentifier();
// Program 16: Keywords used as identifier
ProgramNode* getProgram16Tree_keywordsAsIdentifier();

// Program 17: Variable with same name as procedure
String getProgram17String_sameVariableAndProcedureName();
// Program 17: Variable with same name as procedure
ProgramNode* getProgram17Tree_sameVariableAndProcedureName();

// Program 18: Procedure ending with while statement, with multiple assign
String getProgram18String_endWithWhile();
// Program 18: Procedure ending with while statement, with multiple assign
ProgramNode* getProgram18Tree_endWithWhile();

// Program 19: mutipleProcedures, program with multiple procedures, with if, while, read, print, call, assign
String getProgram19String_multipleProcedures();
// Program 19: mutipleProcedures, program with multiple procedures, with if, while, read, print, call, assign
ProgramNode* getProgram19Tree_multipleProcedures();

// Program 20: mutipleProcedures, program with multiple procedures, with if, while, read, print, call, assign
String getProgram20String_multipleProceduresSpheresdf();
// Program 20: mutipleProcedures, program with multiple procedures, with if, while, read, print, call, assign
ProgramNode* getProgram20Tree_multipleProceduresSpheresdf();

// Program 21: program with if, while, read, print, call, assign
String getProgram21String_endWithIf();
// Program 21: program with if, while, read, print, call, assign
ProgramNode* getProgram21Tree_endWithIf();

// Program 22: program with if, nested whiles, read, print, call, assign
String getProgram22String_whileNestedInWhile();
// Program 22: program with if, nested whiles, read, print, call, assign
ProgramNode* getProgram22Tree_whileNestedInWhile();

// Program 23: program with nested whiles and if
String getProgram23String_nestedWhileIfs();
// Program 23: program with nested whiles and if
ProgramNode* getProgram23Tree_nestedWhileIfs();

// Program 24: program with nested ifs
String getProgram24String_nestedIfs();
// Program 24: program with nested ifs
ProgramNode* getProgram24Tree_nestedIfs();

// Program 25: program with nested ifs in a while
String getProgram25String_nestedIfsInWhile();
// Program 25: program with nested ifs in a while
ProgramNode* getProgram25Tree_nestedIfsInWhile();

// Program 26: program with interleaving nested ifs in a while
String getProgram26String_nestedInterleavingIfsInWhile();
// Program 26: program with interleaving nested ifs in a while
ProgramNode* getProgram26Tree_nestedInterleavingIfsInWhile();

#endif // UNIT_TESTING_AST_UTILS_H
