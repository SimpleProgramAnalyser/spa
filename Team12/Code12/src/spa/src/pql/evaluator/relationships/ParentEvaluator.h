/**
 * Methods to evaluate such that clauses
 * with Parent relationships
 */
#ifndef SPA_QUERY_PARENT_EVALUATOR_H
#define SPA_QUERY_PARENT_EVALUATOR_H

#include "pql/evaluator/ResultsTable.h"

/**
 * Processes a single Parent clause in a PQL query.
 *
 * @param synonym The name that is to be selected.
 * @param stClause The Parent clause to evaluate.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @return Results for the synonym in the Parent clause.
 */
Void evaluateParentClause(const Reference& leftRef, const Reference& rightRef, Boolean isStar,
                          ResultsTable* resultsTable);

#endif // SPA_QUERY_PARENT_EVALUATOR_H
