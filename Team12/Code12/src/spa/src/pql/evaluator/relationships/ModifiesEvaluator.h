/**
 * Methods to evaluate such that clauses
 * with Modifies relationships
 */
#ifndef SPA_QUERY_MODIFIES_EVALUATOR_H
#define SPA_QUERY_MODIFIES_EVALUATOR_H

#include "pql/evaluator/ResultsTable.h"

/**
 * Processes a single Modifies clause in a PQL query.
 *
 * @param leftRef The left reference in the Modifies relationship.
 * @param rightRef The right reference in the Modifies relationship.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @param resultsTable Table containing past results, to store
 *                     the results of evaluating this clause
 */
Void evaluateModifiesClause(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable);

#endif // SPA_QUERY_MODIFIES_EVALUATOR_H
