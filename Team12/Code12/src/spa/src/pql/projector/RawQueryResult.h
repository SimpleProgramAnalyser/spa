/**
 * This class represents the raw query results from all
 * the clauses in the PQL query, with respect every synonym
 * in the PQL query (the synonym in question won't be stored in this
 * class or any raw results class).
 *
 * Note: Please read the documentation of the RawResultFromClause
 * and RawResultFromClauses classes to get a better understanding
 * of the information this class stores.
 *
 * Hierachically, this class is basically like a giant container,
 * encapsulating all (raw) results of the query.
 */

#ifndef SPA_PQL_RAW_QUERY_RESULT_H
#define SPA_PQL_RAW_QUERY_RESULT_H

#include "Types.h"
#include "pkb/PkbTypes.h"
#include "RawResultFromClauses.h"

class RawQueryResult {
public:
    RawQueryResult(Vector<RawResultFromClauses> results);

    static RawQueryResult emptyRawQueryResult();

    Boolean isEmpty();

    RawResultFromClauses get(Integer index);

    Integer count();

    Boolean operator==(const RawQueryResult& rawQueryResult) const;
private:
    RawQueryResult();
    Vector<RawResultFromClauses> results;
};

#endif // SPA_PQL_RAW_QUERY_RESULT_H
