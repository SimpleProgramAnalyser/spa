/**
 * Methods to evaluate such that clauses
 * with Modifies relationships
 */
#ifndef SPA_QUERY_MODIFIES_EVALUATOR_H
#define SPA_QUERY_MODIFIES_EVALUATOR_H

#include "pql/evaluator/ResultsTable.h"

Void evaluateModifiesClause(Reference leftRef, Reference rightRef, ResultsTable* resultsTable);

#endif // SPA_QUERY_MODIFIES_EVALUATOR_H
