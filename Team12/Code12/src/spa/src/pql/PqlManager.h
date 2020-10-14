/**
 * The main logic manager for PQL, invokes serveral components
 * within PQL, and interfaces with UI too.
 */

#ifndef SPA_PQL_PQL_MANAGER_H
#define SPA_PQL_PQL_MANAGER_H

#include <Types.h>

#include "projector/FormattedQueryResult.h"
#include "projector/QueryResultFormatType.h"

class PqlManager {
public:
    static FormattedQueryResult executeQuery(const String& query, QueryResultFormatType format);
};

#endif // SPA_PQL_PQL_MANAGER_H
