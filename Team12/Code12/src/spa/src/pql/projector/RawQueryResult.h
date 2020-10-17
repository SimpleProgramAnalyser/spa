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

#include "error/InputError.h"

#include "Types.h"
#include "pkb/PkbTypes.h"

class RawQueryResult {
public:
    explicit RawQueryResult(Vector<String> results);
    Boolean operator==(const RawQueryResult& rawQueryResult) const;

    const Boolean hasError;
    const InputError errorMessage;

    static RawQueryResult getSyntaxError(String errorMessage);
    static RawQueryResult getSemanticError(String errorMessage);
    static RawQueryResult getFalseResultWithSemanticError(String errorMessage);

    Boolean isEmpty();
    String get(Integer index);
    size_t count();
    void sort();

private:
    explicit RawQueryResult(String errorMessage, Boolean isSyntaxError);
    explicit RawQueryResult(String errorMessage, Vector<String> rawQueryResult);
    Vector<String> results;
};

#endif // SPA_PQL_RAW_QUERY_RESULT_H
