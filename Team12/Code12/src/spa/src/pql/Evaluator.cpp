/**
 * Implemenetation of the PQL query evaluator.
 */

#include "Evaluator.h"
#include "AqTypes.h"
#include "RawQueryResult.h"

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
    RawQueryResult rawQueryResult;

    return rawQueryResult;
}
