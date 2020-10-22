/**
 * Methods to generate Control Flow Graph for SIMPLE
 * programs. These methods are meant to be used in the
 * unit tests of the Simple Program Analyser Frontend.
 *
 * Overall, these methods form a database of CFGs of 
 * SIMPLE programs, easily accessible
 * for unit testing purposes.
 *
 * In the methods, the same programs will share the same
 * program number with the AST in AstUtils. 
 * For example, getProgram1Cfg_compute() will return the 
 * Abstract Syntax Tree representing the program returned 
 * in getProgram1String_compute() where as 
 * getProgram1Cfg_compute() will return the Control 
 * Flow Graph representing the same program.
 */

#ifndef UNIT_TESTING_CFG_UTILS_H
#define UNIT_TESTING_CFG_UTILS_H

#include "Types.h"
#include "cfg/CfgBuilder.h"

// Program 1: Basic program with read, assign, print
std::pair<CfgNode*, size_t> getProgram1Cfg_compute();

// Program 2: Program with "if" nested in "while"
std::pair<CfgNode*, size_t> getProgram2Cfg_factorials();

// Program 3: computeAverage, program with read, assign, print
std::pair<CfgNode*, size_t> getProgram3Cfg_computeAverage();

// Program 4: printAscending, program with read, assign, if/else, print
std::pair<CfgNode*, size_t> getProgram4Cfg_printAscending();

// Program 5: sumDigit, program with read, assign, while, print
std::pair<CfgNode*, size_t> getProgram5Cfg_sumDigit();

// Program 6: sumDigitsDuplicate, program with duplicate procedure - Not Applicable for CFG

// Program 7: computeCentroid, program with multiple procedures, with if, while, read, print, call, assign
std::pair<CfgNode*, size_t> getProgram7Cfg_computeCentroid();

// Program 8: sumDigit but with a cyclic call (2 procedures) - Not Applicable for CFG

// Program 9: sumDigit but with a cyclic call (3 procedures) - Not Applicable for CFG

// Program 10: Calling a non-existent procedure - Not Applicable for CFG

// Program 11: Cyclic calls within nested if and while - Not Applicable for CFG

// Program 12: Recursive program - Not Applicable for CFG

// Program 13: If statement and assign
std::pair<CfgNode*, size_t> getProgram13Cfg_ifExample();

// Program 14: While statement and assign
std::pair<CfgNode*, size_t> getProgram14Cfg_whileExample();

// Program 15: Complicated conditional in while
std::pair<CfgNode*, size_t> getProgram15Cfg_complicatedConditional();

// Program 16: Keywords used as identifier - Not Applicable for CFG

// Program 17: Variable with same name as procedure - Not Applicable for CFG

// Program 18: Procedure ending with while statement, with multiple assign
std::pair<CfgNode*, size_t> getProgram18Cfg_endWithWhile();

// Program 19: mutipleProcedures, program with multiple procedures, with if, while, read, print, call, assign - Not Applicable for CFG

/** CFG BIP Builder**/
//Program 20: mutipleProcedures, program with multiple procedures, with if, while, read, print, call, assign
std::pair<CfgNode*, size_t> getProgram20Cfg_main();
std::pair<CfgNode*, size_t> getProgram20Cfg_raymarch();
std::pair<CfgNode*, size_t> getProgram20Cfg_spheresdf();
std::pair<CfgNode*, size_t> getProgram20CfgBip_multipleProceduresSpheresdf();


#endif // UNIT_TESTING_CFG_UTILS_H
