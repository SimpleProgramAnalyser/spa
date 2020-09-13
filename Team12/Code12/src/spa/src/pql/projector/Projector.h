/**
 * Formats the raw query result into a format, suitable
 * for the frontend and autotester.
 */

#ifndef SPA_PQL_PROJECTOR_H
#define SPA_PQL_PROJECTOR_H

#include "FormattedQueryResult.h"
#include "RawQueryResult.h"

class Projector {
public:
    FormattedQueryResult formatAutotester(RawQueryResult rawQueryResult);
    FormattedQueryResult formatUI(RawQueryResult rawQueryResult);

private:
    const String CommaStr = ",";
    const String PipeStr = "|";
};

#endif // SPA_PQL_PROJECTOR_H
