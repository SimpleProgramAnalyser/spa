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
    RawQueryResult evaluateSelectSynonym();
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
 * Converts a paired result to a vector of strings, where
 * each string is a tuple with elements separated by spaces.
 *
 * Example: Select <a, p> ... returns {{"a", "b"}, {"c", "d"}}
 * This method will return {"a b", "c d"}.
 *
 * @param resultPairs The pairs of results to be converted.
 * @return A vector of strings with each tuple as a single string.
 */
Vector<String> convertToTupleString(const PairedResult& resultPairs);

/**
 * Converts a n-tupled result to a vector of strings, where
 * each string is a tuple with elements separated by spaces.
 *
 * Example: Select <a, p, x> ... returns {{"e", "1", "n"}, {"f", "2", "m"}}
 * This method will return {"e 1 n", "f 2 m"}.
 *
 * @param resultTuples The n-tuples of results to be converted.
 * @return A vector of strings with each tuple as a single string.
 */
Vector<String> convertToTupleString(const NtupledResult& resultTuples);

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
