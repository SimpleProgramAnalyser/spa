/**
 * The main logic manager for PQL, invokes serveral components
 * within PQL, and interfaces with UI too.
 */

#ifndef SPA_PQL_PQL_MANAGER_H
#define SPA_PQL_PQL_MANAGER_H

#include "Types.h"
#include "Ui.h"
#include "projector/FormattedQueryResult.h"
#include "projector/QueryResultFormatType.h"

class PqlManager {
public:
    /**
     * Executes a PQL query, and interacts with the PKB,
     * for the results of that query.
     *
     * @param query The PQL query.
     * @param format The format, to format the results.
     * @param ui The UI to display errors to.
     * @param optimise Whether Query Optimiser should be used
     *                 to reorder clauses in this query.
     *
     * @return FormattedQueryResult, representing the PQL
     * query results (Note: If either PQL query invalid, or is
     * valid but yields no result, an empty FormattedQueryResult
     * would be returned).
     */
    static FormattedQueryResult executeQuery(const String& query, QueryResultFormatType format, Ui& ui,
                                             Boolean optimise);
};

#endif // SPA_PQL_PQL_MANAGER_H
