/**
 * Implementation of the PQL's logic manager.
 */

#include "PqlManager.h"

#include "pql/evaluator/Evaluator.h"
#include "pql/optimiser/Optimiser.h"
#include "pql/preprocessor/Preprocessor.h"
#include "pql/projector/Projector.h"

FormattedQueryResult PqlManager::executeQuery(const String& query, QueryResultFormatType format, Ui& ui)
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
        formattedQueryResult = projector.formatAutotester(rawQueryResult, ui);
    } else if (format == UiFormat) {
        formattedQueryResult = projector.formatUI(rawQueryResult, ui);
    }

    // Finally, return the formatted result
    return formattedQueryResult;
}
