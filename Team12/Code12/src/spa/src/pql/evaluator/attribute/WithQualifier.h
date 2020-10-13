/**
 * WithQualifier holds logic required for Query Evaluator
 * to qualify the arguments in a with clause.
 */

#ifndef SPA_PQL_WITH_QUALIFIER_H
#define SPA_PQL_WITH_QUALIFIER_H

#include "Types.h"
#include "pql/evaluator/ResultsTable.h"
#include "pql/preprocessor/AqTypes.h"

/**
 * Evaluates a with clause in the query, storing the results
 * of evaluation in a provided ResultsTable.
 *
 * @param whClause The with clause in the query.
 * @param resultsTable The results table to store the results in.
 */
Void evaluateWith(WithClause* whClause, ResultsTable* resultsTable);

#endif // SPA_PQL_WITH_QUALIFIER_H
