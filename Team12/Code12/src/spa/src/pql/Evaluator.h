#ifndef SPA_EVALUATOR_H
#define SPA_EVALUATOR_H

#include "AqTypes.h"
#include "RawQueryResult.h"

class Evaluator {
    public:
        RawQueryResult evaluateQuery(AbstractQuery abstractQuery);
};

#endif // SPA_EVALUATOR_H
