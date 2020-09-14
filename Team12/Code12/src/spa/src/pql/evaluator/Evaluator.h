/**
 * This class represents the PQL query evaluator
 * and encapsulates logic necessary for further
 * processing a PQL query (e.g, optimisation), and
 * finally interfacing with the PKB component,
 * for the results of the query.
 */

#ifndef SPA_PQL_EVALUATOR_H
#define SPA_PQL_EVALUATOR_H

#include "Types.h"
#include "pql/preprocessor/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

class Evaluator {
public:
    RawQueryResult evaluateQuery(const AbstractQuery& abstractQuery);
};

#endif // SPA_PQL_EVALUATOR_H
