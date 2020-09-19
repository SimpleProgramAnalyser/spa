/**
 * Methods to evaluate such that clauses
 * with Uses relationships
 */
#ifndef SPA_PQL_USES_EVALUATOR_H
#define SPA_PQL_USES_EVALUATOR_H

#include "pql/evaluator/ResultsTable.h"

/**
 * Processes a single Uses clause in a PQL query.
 *
 * @param leftRef The left reference in the Uses relationship.
 * @param rightRef The right reference in the Uses relationship.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @param resultsTable Table containing past results, to store
 *                     the results of evaluating this clause
 */
Void evaluateUsesClause(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable);

#endif // SPA_PQL_USES_EVALUATOR_H
