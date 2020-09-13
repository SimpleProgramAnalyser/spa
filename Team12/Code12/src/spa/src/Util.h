/**
 * Utility functions for SPA.
 */

#ifndef SPA_UTIL_H
#define SPA_UTIL_H

#include "Types.h"

namespace util {

// template functions have to be known in the header
template <typename T>
bool checkListValuesEqual(const List<T>& first, const List<T>& second)
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

template<typename T>
Boolean checkVectorOfPointersEqual(const std::vector<T>& first, const std::vector<T>& second) {
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
};

Boolean isPossibleIdentifier(const String& str);
Boolean isPossibleConstant(const String& str);
Boolean isRelationshipReference(const String& str);
Boolean isLiteralIdent(const String& str);
} // namespace util

#endif // SPA_UTIL_H
