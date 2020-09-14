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
 * Hierarchically, this class is basically like a giant container,
 * encapsulating all (raw) results of the query.
 */

#ifndef SPA_PQL_RAW_QUERY_RESULT_H
#define SPA_PQL_RAW_QUERY_RESULT_H

#include "Types.h"
#include "pkb/PkbTypes.h"
#include "pql/evaluator/RawResultFromClause.h"

class RawQueryResult {
public:
    explicit RawQueryResult(Vector<RawResultFromClause> results);
    Boolean operator==(const RawQueryResult& rawQueryResult) const;

    const Boolean isSyntaxError;
    const String errorMessage;

    static RawQueryResult getSyntaxError(String errorMessage);

    Boolean isEmpty();
    RawResultFromClause get(Integer index);
    Integer count();

private:
    explicit RawQueryResult(String errorMessage);
    const Vector<RawResultFromClause> results;
};

#endif // SPA_PQL_RAW_QUERY_RESULT_H
