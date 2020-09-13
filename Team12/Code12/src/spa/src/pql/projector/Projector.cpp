/**
 * Implementation of the PQL results projector/formatter.
 */

#include "Projector.h"

#include "FormattedQueryResult.h"
#include "RawQueryResult.h"
#include "Types.h"
#include "frontend/parser/StringMatcher.h"

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
    String formattedResults = "";

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

    for (int i = 0; i < len; ++i) {
        RawResultFromClauses result = rawQueryResult.get(i);

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
        formattedResults += result.toString(CommaStr);

        if (i < i - 1) {
            formattedResults += PipeStr;
        }
    }


    FormattedQueryResult formattedQueryResult(formattedResults);

    return formattedQueryResult;
}

FormattedQueryResult Projector::formatUI(RawQueryResult rawQueryResult)
{
    String formattedResults = "";

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

    for (int i = 0; i < len; ++i) {
        RawResultFromClauses result = rawQueryResult.get(i);

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
        formattedResults += result.toString(CommaStr);

        if (i < i - 1) {
            formattedResults += PipeStr;
        }
    }


    FormattedQueryResult formattedQueryResult(formattedResults);

    return formattedQueryResult;
}

