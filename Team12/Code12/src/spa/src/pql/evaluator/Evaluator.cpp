/**
 * Implemenetation of the PQL query evaluator.
 */

#include "Evaluator.h"

#include "pql/parser/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

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
RawQueryResult Evaluator::evaluateQuery(AbstractQuery query)
{
    /*
     * First check if PQL query is semantically valid.
     * If invalid, don't continue evaluating query.
     */
    if (query.isInvalid()) {
        return RawQueryResult::emptyRawQueryResult();
    }

    /*
     * Otherwise, at this point, the PQL query,
     * should be semantically (and of course
     * syntatically) valid.
     */

    if (isQueryVacuouslyTrue(query)) {
    }

    Vector<Vector<String>> results;

    RawQueryResult rawQueryResult(results);

    return rawQueryResult;
}

/*
 * Checks if the PQL query is vacously true,
 * where the synonym (in select) is not directly
 * used in the clauses. An example of such queries
 * include; variable v; statement s; Select v such
 * that Follows(3, s).
 *
 * @return True if vacuously true, false otherwise.
 */
Boolean Evaluator::isQueryVacuouslyTrue(AbstractQuery query)
{
    return true;
}
