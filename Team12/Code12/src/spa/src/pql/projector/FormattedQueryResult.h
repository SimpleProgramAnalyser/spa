/**
 * This class represents a FormattedQueryResult, which is an
 * final encoding PQL results after formatting from
 * the RawQueryResult form, by the results projector.
 */

#ifndef SPA_PQL_FORMATTED_QUERY_RESULT_H
#define SPA_PQL_FORMATTED_QUERY_RESULT_H

#include "Types.h"

class FormattedQueryResult {
public:
    FormattedQueryResult(String results);
    static FormattedQueryResult emptyFormattedQueryResult();
    String getResults();

private:
    FormattedQueryResult();
    String results;
};

#endif // SPA_PQL_FORMATTED_QUERY_RESULT_H
