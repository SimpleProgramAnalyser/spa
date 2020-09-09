/**
 * Utility functions for SPA.
 */

#ifndef SPA_UTIL_H
#define SPA_UTIL_H

#include "Types.h"

#include <regex>

namespace util {

template <typename T>
Boolean checkListValuesEqual(const List<T>& first, const List<T>& second);
Boolean isPossibleIdentifier(const String& str);
Boolean isPossibleConstant(const String& str);
}

#endif // SPA_UTIL_H
