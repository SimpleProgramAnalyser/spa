/**
 * Implemenetation of the PQL query evaluator.
 */

#include "Evaluator.h"

#include "pkb/PKB.h"
#include "pql/preprocessor/AqTypes.h"
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

    // Boolean queryVacuouslyTrue = isQueryVacuouslyTrue(query);

    // RawQueryResult result = processQuery(query);

    /*
     * If query is vacuously true, retrieve all entity
     * related to that synonym, from Tables API
     */

    RawQueryResult result = RawQueryResult::emptyRawQueryResult();
    return result;
}
