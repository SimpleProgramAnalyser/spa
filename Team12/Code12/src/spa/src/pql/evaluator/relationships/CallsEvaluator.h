/**
 * Method to evaluate such that clauses
 * with Calls relationships
 */
#ifndef SPA_PQL_CALLS_EVALUATOR_H
#define SPA_PQL_CALLS_EVALUATOR_H

#include "pql/evaluator/ResultsTable.h"

/*
 * Processes a single Calls clause in a PQL query. All results
 * obtained from the clause will be stored in the results table.
 *
 * @param leftRef The left reference in the Calls relationship.
 * @param rightRef The right reference in the Calls relationship.
 * @param isStar Whether the Calls clause comes with an
 *               asterisk, indicating the transitive closure.
 * @param resultsTable The results table that would store
 *                     the results of evaluating this clause
 */
Void evaluateCallsClause(const Reference& leftRef, const Reference& rightRef, Boolean isStar,
                         ResultsTable* resultsTable);

#endif // SPA_PQL_CALLS_EVALUATOR_H
