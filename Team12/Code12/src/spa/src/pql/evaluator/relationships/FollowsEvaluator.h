/**
 * Methods to evaluate such that clauses
 * with Follows relationships
 */
#ifndef SPA_PQL_FOLLOWS_EVALUATOR_H
#define SPA_PQL_FOLLOWS_EVALUATOR_H

#include "pql/evaluator/ResultsTable.h"

/*
 * Processes a single (such that, Follows) clause in a PQL query, with
 * respect to two given references. All results obtained from the clauses
 * will be stored in the results table. This method also
 * interacts with the PKB to obtain the results to the query.
 *
 * @param leftRef The left reference in the Follows relationship.
 * @param rightRef The right reference in the Follows relationship.
 * @param isStar Whether the Follows clause comes with an
 *               asterisk, indicating the transitive closure.
 * @param resultsTable Table containing past results, to store
 *                     the results of evaluating this clause
 */
Void evaluateFollowsClause(const Reference& leftRef, const Reference& rightRef, Boolean isStar,
                           ResultsTable* resultsTable);

#endif // SPA_PQL_FOLLOWS_EVALUATOR_H
