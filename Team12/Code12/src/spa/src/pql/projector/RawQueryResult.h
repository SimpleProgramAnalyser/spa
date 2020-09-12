/**
 * This class represents a RawQueryResult, which is an
 * immediate encoding PQL results after interaction with
 * the PKB (the interaction, is of course done by other
 * classes).
 */

#ifndef SPA_PQL_RAW_QUERY_RESULT_H
#define SPA_PQL_RAW_QUERY_RESULT_H

#include "Types.h"
#include "pkb/PkbTypes.h"

class RawQueryResult {
public:
    RawQueryResult(Vector<Vector<Vector<String>>> results);
    static RawQueryResult emptyRawQueryResult();
    /*
     * TODO: Don't return the whole Vector<Vector<String>>,
     * implement it like Chester's, get by index only.
     */
    Vector<Vector<Vector<String>>> getResults();

    Boolean isEmpty();

    Boolean operator==(const RawQueryResult& rawQueryResult) const;

private:
    RawQueryResult();
    Vector<Vector<Vector<String>>> results;
};

#endif // SPA_PQL_RAW_QUERY_RESULT_H
