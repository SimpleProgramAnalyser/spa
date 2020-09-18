/**
 * SuchThatMatcher holds logic required for Query Evaluator
 * to match a such that clause in a Program Query Language query.
 */

#ifndef SPA_PQL_SUCH_THAT_MATCHER_H
#define SPA_PQL_SUCH_THAT_MATCHER_H

#include <vector>

#include "ResultsTable.h"
#include "Types.h"
#include "pql/preprocessor/AqTypes.h"

/*
 * Processes a single such that clause in a PQL query, and
 * stores the results in the given ResultsTable.
 *
 * This method also interacts with the PKB, if needed
 * to obtain the results for the such that clause.
 *
 * @param stClause The SuchThatClause to evaluate.
 * @param resultsTable The table of results.
 */
Void evaluateSuchThat(SuchThatClause* stClause, ResultsTable* resultsTable);

#endif // SPA_PQL_SUCH_THAT_MATCHER_H
