/**
 * Implementation of the PQL's logic manager.
 */

#include "AqTypes.h"
#include "Evaluator.h"
#include "FormattedQueryResult.h"
#include "PqlManager.h"
#include "Projector.h"
#include "QueryResultFormatType.h"

/**
 * Executes a PQL query, and interacts with the PKB,
 * for the results of that query.
 *
 * @param query The PQL query.
 * @param The format, to format the results.
 *
 * @return FormattedQueryResult, representing the PQL
 * query results (Note: If either PQL query invalid, or is
 * valid but yields no result, an empty FormattedQueryResult
 * would be returned).
 */
FormattedQueryResult PqlManager::executeQuery(String query, QueryResultFormatType format)
{
    // Call the Preprocessor to parse the query
    AbstractQuery abstractQuery;

    /*
     * Pass the parsed query (AbstractQueryType to the PQL
     * query evaluator
     */
    Evaluator evaluator;

    RawQueryResult rawQueryResult = evaluator.evaluateQuery(abstractQuery);

    // Once, we have the result, format it then return the formatted results
    Projector projector;
    FormattedQueryResult formattedQueryResult;

    if (format == AutotesterFormat) {
        formattedQueryResult = projector
            .formatAutotester(rawQueryResult);
    } else if (format == UiFormat) {
        formattedQueryResult = projector
            .formatUI(rawQueryResult);
    }

    // Finally, return the formatted result
    return formattedQueryResult;
}

