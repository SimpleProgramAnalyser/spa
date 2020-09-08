/**
 * This class represents the PQL query evaluator
 * and encapsulates logic necessary for further
 * processing a PQL query (e.g, optimisation), and
 * finally interfacing with the PKB component,
 * for the results of the query.
 */

#ifndef SPA_EVALUATOR_H
#define SPA_EVALUATOR_H

#include "AqTypes.h"
#include "RawQueryResult.h"

class Evaluator {
public:
    RawQueryResult evaluateQuery(AbstractQuery abstractQuery);
};

#endif // SPA_EVALUATOR_H
