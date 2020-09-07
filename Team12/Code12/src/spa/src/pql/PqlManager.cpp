/**
 * Implementation of the PQL's logic manager.
 */

#include "PqlManager.h"
#include "FormattedQueryResult.h"

/**
 * Evaluates a PQL query, and interacts with the PKB,
 * for the results of the query.
 *
 * @param query The PQL query.
 *
 * @return FormattedQueryResult, representing the PQL
 * query results (Note: If either PQL query invalid, or is
 * valid but yields no result, an empty FormattedQueryResult
 * would be returned).
 */
FormattedQueryResult PqlManager::evaluateQuery(String query)
{
    // Call the Preprocessor to parse the query
    FormattedQueryResult formattedQueryResult(query);
    return formattedQueryResult;
}

