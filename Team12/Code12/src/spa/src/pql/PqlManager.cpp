/**
 * Implementation of the PQL's logic manager.
 */

#include "PqlManager.h"

#include "pql/evaluator/Evaluator.h"
#include "pql/optimiser/Optimiser.h"
#include "pql/preprocessor/Preprocessor.h"
#include "pql/projector/Projector.h"

/**
 * Executes a PQL query, and interacts with the PKB,
 * for the results of that query.
 *
 * @param query The PQL query.
 * @param format The format, to format the results.
 *
 * @return FormattedQueryResult, representing the PQL
 * query results (Note: If either PQL query invalid, or is
 * valid but yields no result, an empty FormattedQueryResult
 * would be returned).
 */
FormattedQueryResult PqlManager::executeQuery(const String& query, QueryResultFormatType format)
{
    // Call the Preprocessor to parse the query
    AbstractQuery abstractQuery = Preprocessor::processQuery(query);

    // Optimise the query
    optimiseQuery(abstractQuery);

    /*
     * Pass the parsed query (AbstractQuery) to the PQL
     * query evaluator
     */
    RawQueryResult rawQueryResult = evaluateQuery(abstractQuery);

    // Once, we have the result, format it then return the formatted results
    Projector projector;
    FormattedQueryResult formattedQueryResult = FormattedQueryResult::emptyFormattedQueryResult();

    if (format == AutotesterFormat) {
        formattedQueryResult = projector.formatAutotester(rawQueryResult);
    } else if (format == UiFormat) {
        formattedQueryResult = projector.formatUI(rawQueryResult);
    }

    // Finally, return the formatted result
    return formattedQueryResult;
}
