/**
 * This class represents the reduced raw query results from all
 * the clauses in the PQL query, with respect every synonym
 * in the PQL query (the synonym in question won't be stored in this
 * class or any raw results class).
 *
 * The reduction in this case, would be to condense/merge
 * similar results from adjacent RawResultFromClauses
 * (which is directly done by ReducedRawResultFromClauses).
 *
 * This class is usually called by Projector.h class to
 * obtain a reduction in the raw results.
 */

#ifndef SPA_PQL_REDUCED_RAW_QUERY_RESULT_H
#define SPA_PQL_REDUCED_RAW_QUERY_RESULT_H

#include "Types.h"
#include "pkb/PkbTypes.h"
#include "RawQueryResult.h"
#include "ReducedRawResultFromClauses.h"

class ReducedRawQueryResult {
public:
    ReducedRawQueryResult(RawQueryResult results);

    // static ReducedRawQueryResult emptyReducedRawQueryResult();

    Boolean isEmpty();

    ReducedRawResultFromClauses get(Integer index);

    Integer count();

    Boolean operator==(const ReducedRawQueryResult& reducedRawQueryResult) const;
private:
    // ReducedRawQueryResult();
    Vector<ReducedRawResultFromClauses> results;
};

#endif // SPA_PQL_REDUCED_RAW_QUERY_RESULT_H
