/**
 * This class represents the PQL query evaluator
 * and encapsulates logic necessary for further
 * processing a PQL query (e.g, optimisation), and
 * finally interfacing with the PKB component,
 * for the results of the query.
 */

#ifndef SPA_PQL_EVALUATOR_H
#define SPA_PQL_EVALUATOR_H

#include "ResultsTable.h"
#include "pql/preprocessor/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

class Evaluator {
private:
    const AbstractQuery& query;
    ResultsTable resultsTable;

    RawQueryResult evaluateSyntacticallyValidQuery();
    Void evaluateClause(Clause* clause);

public:
    /**
     * Constructor for a Evaluator for an abstract query.
     */
    explicit Evaluator(const AbstractQuery& abstractQuery);

    /**
     * Evaluates the query stored in this Evaluator.
     *
     * @return A list of results from evaluation.
     */
    RawQueryResult evaluateQuery();
};

/**
 * Evaluates a PQL query, and interacts with the PKB,
 * for the results of that query.
 *
 * @param query The PQL query.
 *
 * @return RawQueryResult, representing the PQL
 * query results (Note: If either PQL query invalid, or is
 * valid but yields no result, an empty RawQueryResult
 * would be returned).
 */
RawQueryResult evaluateQuery(const AbstractQuery& abstractQuery);

#endif // SPA_PQL_EVALUATOR_H
