/**
 * IfMatcher holds logic required for Query Evaluator to match
 * an if pattern clause in a Program Query Language query.
 */

#ifndef SPA_PQL_IF_MATCHER_H
#define SPA_PQL_IF_MATCHER_H

#include "pql/evaluator/ResultsTable.h"
#include "pql/preprocessor/AqTypes.h"

/**
 * Given an if pattern clause, retrieves the SIMPLE
 * program from the Program Knowledge Base (root node of
 * Abstract Syntax Tree) and tries to find the if statement
 * by matching the variable in the clause.
 *
 * @param pnClause A pattern clause in the query.
 * @param resultsTable The results table to store the results in.
 */
Void evaluateIfPattern(PatternClause* pnClause, ResultsTable* resultsTable);

#endif // SPA_PQL_IF_MATCHER_H
