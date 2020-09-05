/**
 * Implementation of utility functions for SPA.
 */

#include "Util.h"

template <typename T>
bool util::checkListValuesEqual(List<T> first, List<T> second)
{
    if (first.size() != second.size()) {
        return false;
    }

    bool allMatch = true;
    size_t len = first.statementList.size();
    for (size_t i = 0; i < len; i++) {
        if (*(first.statementList.at(i)) == *(second.statementList.at(i))) {
            allMatch = false;
            break;
        }
    }
    return allMatch;
}
