/**
 * Utility functions for SPA.
 */

#ifndef SPA_UTIL_H
#define SPA_UTIL_H

#include <regex>

#include "Types.h"

namespace util {

template <typename T>
Boolean checkListValuesEqual(const List<T>& first, const List<T>& second);
Boolean isPossibleIdentifier(const String& str);
Boolean isPossibleConstant(const String& str);
Boolean isRelationshipReference(const String& str);
Boolean isLiteralIdent(const String& str);
} // namespace util

#endif // SPA_UTIL_H
