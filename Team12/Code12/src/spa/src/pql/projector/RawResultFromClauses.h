/**
 * This class represents the raw query results from all
 * the clauses in the PQL query, with respect to a particular
 * synonym (the synonym in question, won't be stored in this
 * class or any raw query results class).
 *
 * Note: Please do not confuse this class with the
 * RawResultFromClauses class (please read the
 * documentation of that class for more details).
 */

#ifndef SPA_PQL_RAW_RESULT_FROM_CLAUSES_H
#define SPA_PQL_RAW_RESULT_FROM_CLAUSES_H

#include "Types.h"
#include "RawResultFromClause.h"

class RawResultFromClauses {
public:
    RawResultFromClause(Vector<RawResultFromClause>);

    Boolean isEmpty();

    RawResultFromClause get(Integer index);

    Integer count();

    Boolean operator==(const RawResultFromClauses& rawResultFromClauses) const;

private:
    // RawResultFromClauses();
    Vector<RawResultFromClause> results;
};

#endif // SPA_PQL_RAW_RESULT_FROM_CLAUSES_H
