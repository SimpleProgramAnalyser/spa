/**
 * Implementation of utility functions for SPA.
 */

#include "Util.h"

template <typename T>
bool util::checkListValuesEqual(const List<T>& first, const List<T>& second)
{
    if (first.size() != second.size()) {
        return false;
    }

    bool allMatch = true;
    size_t len = first.size();
    for (size_t i = 0; i < len; i++) {
        if (*(first.at(i)) == *(second.at(i))) {
            continue;
        } else {
            allMatch = false;
            break;
        }
    }
    return allMatch;
}
