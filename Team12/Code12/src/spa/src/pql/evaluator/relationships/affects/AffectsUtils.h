/**
 * Utility classes and methods used by Affects Evaluator
 * as well as its related subclasses and dependencies.
 */

#ifndef SPA_PQL_AFFECTS_UTILS_H
#define SPA_PQL_AFFECTS_UTILS_H

#include "pql/preprocessor/Reference.h"

/**
 * Checks whether a particular Reference is a WildcardRefType,
 * or the Reference's DesignEntityType could
 * possibly have any Affects/Affects* relationships.
 *
 * @param ref Reference to check.
 * @return True, if the type is either "_", "assign _;" or "stmt _;".
 */
inline bool isAffectable(const Reference& ref)
{
    if (ref.getReferenceType() == WildcardRefType) {
        return true;
    }

    DesignEntityType type = ref.getDesignEntity().getType();
    return type == AssignType || type == StmtType || type == Prog_LineType;
}

#endif // SPA_PQL_AFFECTS_UTILS_H
