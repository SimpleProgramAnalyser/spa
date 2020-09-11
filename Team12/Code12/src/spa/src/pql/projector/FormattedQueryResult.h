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
