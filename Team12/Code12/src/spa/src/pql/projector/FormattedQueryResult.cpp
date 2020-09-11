#include "FormattedQueryResult.h"

FormattedQueryResult::FormattedQueryResult(String results):
    results(results)
{}

FormattedQueryResult::FormattedQueryResult()
{}

FormattedQueryResult FormattedQueryResult::emptyFormattedQueryResult()
{
    FormattedQueryResult* formattedQueryResult = new FormattedQueryResult();

    return *formattedQueryResult;
}


String FormattedQueryResult::getResults()
{
    return results;
}
