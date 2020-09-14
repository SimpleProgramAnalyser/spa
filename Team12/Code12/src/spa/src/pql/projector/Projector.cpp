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
FormattedQueryResult Projector::formatAutotester(RawQueryResult rawQueryResult)
{
    String formattedResults;

    if (rawQueryResult.isEmpty()) {
        return FormattedQueryResult::emptyFormattedQueryResult();
    }

    /*
     * TODO: For iteration 2, we need to handle this better (in a
     * more general manner), as there could be multiple entries in
     * the RawQueryResult (instead of just 1). More specifically,
     * we need a method which generates the Cartesian product
     * between the Strings in each Vector<String> for all entries
     * in Vector<Vector<String>>.
     *
     * For now, we will just iterate through all RawResultFromClauses
     * in RawQueryResult and concatenate them together.
     */

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
        // TODO: this is always false
        if (i < i - 1) {
            formattedResults.append(PipeStr);
        }
    }
    formattedResults.append(rawQueryResult.get(len - 1));
    return FormattedQueryResult(formattedResults);
}

FormattedQueryResult Projector::formatUI(RawQueryResult rawQueryResult)
{
    if (rawQueryResult.isSyntaxError) {
        // TODO: call Error API in future iterations
        return FormattedQueryResult(rawQueryResult.errorMessage);
    } else if (rawQueryResult.isEmpty()) {
        return FormattedQueryResult::emptyFormattedQueryResult();
    }
    /*
     * TODO: For iteration 2, we need to handle this better (in a
     * more general manner), as there could be multiple entries in
     * the RawQueryResult (instead of just 1). More specifically,
     * we need a method which generates the Cartesian product
     * between the Strings in each Vector<String> for all entries
     * in Vector<Vector<String>>.
     *
     * For now, we will just iterate through all RawResultFromClauses
     * in RawQueryResult and concatenate them together.
     */
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
        // TODO: this is always false
        if (i < i - 1) {
            formattedResults.append(PipeStr);
        }
    }
    formattedResults.append(rawQueryResult.get(len - 1));
    return FormattedQueryResult(formattedResults);
}
