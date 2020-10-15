/**
 * WhileMatcher holds logic required for Query Evaluator to match
 * a while pattern clause in a Program Query Language query.
 */

#ifndef SPA_PQL_WHILE_MATCHER_H
#define SPA_PQL_WHILE_MATCHER_H

#include "PatternMatcherUtil.h"

/**
 * Given a while pattern clause, retrieves the SIMPLE
 * program from the Program Knowledge Base (root node of
 * Abstract Syntax Tree) and tries to find the while
 * statement by matching the variable in the clause.
 *
 * @param pnClause A pattern clause in the query.
 * @param resultsTable The results table to store the results in.
 */
Void evaluateWhilePattern(PatternClause* pnClause, ResultsTable* resultsTable);

#endif // SPA_PQL_WHILE_MATCHER_H
