#ifndef SPA_FORMATTEDQUERYRESULT_H
#define SPA_FORMATTEDQUERYRESULT_H

#include <Types.h>

class FormattedQueryResult {
    public:
        FormattedQueryResult(String results);
        String getResult();
    private:
        String result;
};

#endif // SPA_RAW_QUERY_RESULT_H
