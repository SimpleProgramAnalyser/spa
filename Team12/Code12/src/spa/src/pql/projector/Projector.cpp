/**
 * Implementation of the PQL results projector/formatter.
 */

#include "Projector.h"

#include "Types.h"

/*
 * Formats the raw query results into a format suitable and conforming to
 * the autotester.
 *
 * @param rawQueryResult The raw query result as returned by the
 * evaluator.
 *
 * @return FormattedQueryResult, which respresents the result
 * in a foramt ready for the autotester/UI.
 */
FormattedQueryResult Projector::formatAutotester(RawQueryResult rawQueryResult, Ui& ui)
{
    String formattedResults;
    if (rawQueryResult.hasError) {
        ui.postUiError(rawQueryResult.errorMessage);
    }
    if (rawQueryResult.isEmpty()) {
        return FormattedQueryResult::emptyFormattedQueryResult();
    }

    Integer len = rawQueryResult.count();

    for (int i = 0; i < len - 1; ++i) {
        String result = rawQueryResult.get(i);

        /*
         * Recall that the RawResultFromClauses, already
         * merge adjacent similar results from clauses
         * (with respect to a given synonym) (please
         * read up the documentation for the RawResultFromClauses
         * class, for more details).
         *
         * Hence, we don't have to do anything here to
         * RawResultFromClauses.
         */
        formattedResults.append(result).append(CommaStr);
    }
    formattedResults.append(rawQueryResult.get(len - 1) /* remove trailing comma */);
    return FormattedQueryResult(formattedResults);
}

FormattedQueryResult Projector::formatUI(RawQueryResult rawQueryResult, Ui& ui)
{
    if (rawQueryResult.hasError) {
        ui.postUiError(rawQueryResult.errorMessage);
    }
    if (rawQueryResult.isEmpty()) {
        return FormattedQueryResult::emptyFormattedQueryResult();
    }
    Integer len = rawQueryResult.count();
    String formattedResults;
    rawQueryResult.sort();
    for (int i = 0; i < len - 1; ++i) {
        String result = rawQueryResult.get(i);
        /*
         * Recall that the RawResultFromClauses, already
         * merge adjacent similar results from clauses
         * (with respect to a given synonym) (please
         * read up the documentation for the RawResultFromClauses
         * class, for more details).
         *
         * Hence, we don't have to do anything here to
         * RawResultFromClauses.
         */
        formattedResults.append(result).append(CommaStr);
    }
    formattedResults.append(rawQueryResult.get(len - 1) /* remove trailing comma */);
    return FormattedQueryResult(formattedResults);
}
