/**
 * This class represents the raw query results from a single
 * the clauses in the PQL query, with respect to a particular
 * synonym (the synonym in question however, won't be stored
 * in this class or any raw results class).
 *
 * Note: Please do not confuse this class with the
 * RawResultFromClauses class (please read the
 * documentation of that class for more details).
 */

#ifndef SPA_PQL_RAW_RESULT_FROM_CLAUSE_H
#define SPA_PQL_RAW_RESULT_FROM_CLAUSE_H

#include "Types.h"
#include "pkb/PkbTypes.h"

class RawResultFromClause {
public:
    RawResultFromClause(Vector<String> results, Boolean isClauseRelatedToSynonym);

    static RawResultFromClause emptyRawResultFromClause();

    static Vector<String> convertToStringVect(Vector<Integer> intList);

    Boolean isEmpty();

    String get(Integer index);

    Integer count();

    Boolean checkIsClauseRelatedToSynonym();

    Boolean operator==(const RawResultFromClause& rawResultFromClause) const;
private:
    RawResultFromClause();
    Boolean isClauseRelatedToSynonym;
    Vector<String> results;
};

#endif // SPA_PQL_RAW_RESULT_FROM_CLAUSE_H
