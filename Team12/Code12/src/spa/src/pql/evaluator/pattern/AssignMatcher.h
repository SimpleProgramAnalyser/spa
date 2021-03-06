/**
 * AssignMatcher holds logic required for Query Evaluator to match
 * an assign pattern clause in a Program Query Language query.
 */

#ifndef SPA_PQL_ASSIGN_MATCHER_H
#define SPA_PQL_ASSIGN_MATCHER_H

#include "pql/evaluator/ResultsTable.h"
#include "pql/preprocessor/AqTypes.h"

/**
 * Given an assignment pattern clause, retrieves the SIMPLE
 * program from the Program Knowledge Base (root node of
 * Abstract Syntax Tree) and tries to find the assignment
 * statements, or assigned to variables, that match the
 * expression pattern specified.
 *
 * @param pnClause A pattern clause in the query.
 * @param resultsTable The results table to store the results in.
 */
Void evaluateAssignPattern(PatternClause* pnClause, ResultsTable* resultsTable);

#endif // SPA_PQL_ASSIGN_MATCHER_H
