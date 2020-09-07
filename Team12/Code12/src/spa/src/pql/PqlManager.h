/**
 * The main logic manager for PQL, invokes serveral components
 * within PQL, and interfaces with UI too.
 */

#ifndef SPA_PQLMANAGER_H
#define SPA_PQLMANAGER_H

#include <Types.h>
#include "FormattedQueryResult.h"

class PqlManager {
    public:
        FormattedQueryResult evaluateQuery(String query);
};

#endif // SPA_PQLMANAGER_H
