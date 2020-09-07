/**
 * Formats the raw query result into a format, suitable
 * for the frontend and autotester.
 */

#ifndef SPA_PROJECTOR_H
#define SPA_PROJECTOR_H

#include "FormattedQueryResult.h"
#include "RawQueryResult.h"

class Projector {
    public:
        FormattedQueryResult formatAutotester(RawQueryResult rawQueryResult);
        FormattedQueryResult formatUI(RawQueryResult rawQueryResult);
};

#endif // SPA_PROJECTOR_H

