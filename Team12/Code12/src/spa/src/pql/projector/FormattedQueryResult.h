/**
 * This class represents a FormattedQueryResult, which is an
 * final encoding PQL results after formatting from
 * the RawQueryResult form, by the results projector.
 */

#ifndef SPA_PQL_FORMATTED_QUERY_RESULT_H
#define SPA_PQL_FORMATTED_QUERY_RESULT_H

#include "Types.h"

/*
 * TODO: Formatted result should be split into 2 classes,
 * one representing the formatted results for the UI,
 * and the other for the autotester, as these 2
 * classes might potentially require different
 * formats.
 */
class FormattedQueryResult {
public:
    FormattedQueryResult(String results);
    static FormattedQueryResult emptyFormattedQueryResult();
    String getResults();

    Boolean operator==(const FormattedQueryResult& formattedQueryResult) const;

private:
    FormattedQueryResult();
    String results;
};

#endif // SPA_PQL_FORMATTED_QUERY_RESULT_H
