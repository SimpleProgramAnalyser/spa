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

    FormattedQueryResult formattedQueryResult(formattedResults);

    return formattedQueryResult;
}

FormattedQueryResult Projector::formatUI(RawQueryResult rawQueryResult)
{
    String formattedResults = "";

    FormattedQueryResult formattedQueryResult(formattedResults);

    return formattedQueryResult;
}
