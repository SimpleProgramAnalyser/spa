/**
 * Formats the raw query result into a format, suitable
 * for the frontend and autotester.
 */

#ifndef SPA_PQL_PROJECTOR_H
#define SPA_PQL_PROJECTOR_H

#include "FormattedQueryResult.h"
#include "RawQueryResult.h"
#include "Ui.h"

class Projector {
public:
    FormattedQueryResult formatAutotester(RawQueryResult rawQueryResult, Ui& ui);
    FormattedQueryResult formatUI(RawQueryResult rawQueryResult, Ui& ui);

private:
    const String CommaStr = ", ";
    const String PipeStr = "|";
};

#endif // SPA_PQL_PROJECTOR_H
