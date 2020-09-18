/**
 * PatternMatcher holds logic required for Query Evaluator
 * to match a pattern clause in a Program Query Language query.
 */

#ifndef SPA_PQL_PATTERN_MATCHER_H
#define SPA_PQL_PATTERN_MATCHER_H

#include <vector>

#include "ResultsTable.h"
#include "Types.h"
#include "pql/preprocessor/AqTypes.h"

/**
 * Evaluates a pattern clause in the query, storing the results
 * of evaluation in a provided ResultsTable.
 *
 * @param pnClause The pattern clause in the query.
 * @param resultsTable The results table to store the results in.
 */
Void evaluatePattern(PatternClause* pnClause, ResultsTable* resultsTable);

#endif // SPA_PQL_PATTERN_MATCHER_H
