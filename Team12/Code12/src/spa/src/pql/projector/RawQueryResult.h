/**
 * This class represents a RawQueryResult, which is an
 * immediate encoding PQL results after interaction with
 * the PKB (the interaction, is of course done by other
 * classes).
 */

#ifndef SPA_PQL_RAW_QUERY_RESULT_H
#define SPA_PQL_RAW_QUERY_RESULT_H

#include "pkb/PkbTypes.h"
#include "Types.h"


class RawQueryResult {
public:
    RawQueryResult(Vector<Vector<String>> results);
    static RawQueryResult emptyRawQueryResult();
    Vector<Vector<String>> getResults();

    Boolean isEmpty();
private:
    RawQueryResult();
    Vector<Vector<String>> results;
};

#endif // SPA_PQL_RAW_QUERY_RESULT_H
