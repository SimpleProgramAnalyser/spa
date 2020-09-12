/**
 * Implementation of the PQL's logic manager.
 */

#include "PqlManager.h"

#include "pql/evaluator/Evaluator.h"
#include "pql/preprocessor/AqTypes.h"
#include "pql/preprocessor/Preprocessor.h"
#include "pql/projector/FormattedQueryResult.h"
#include "pql/projector/Projector.h"
#include "pql/projector/QueryResultFormatType.h"

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
FormattedQueryResult PqlManager::executeQuery(String query, QueryResultFormatType format)
{
    // Call the Preprocessor to parse the query
    Preprocessor preprocessor;
    AbstractQuery abstractQuery = preprocessor.processQuery(query);

    /*
     * Pass the parsed query (AbstractQueryType to the PQL
     * query evaluator
     */
    Evaluator evaluator;
    RawQueryResult rawQueryResult = evaluator.evaluateQuery(abstractQuery);

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
