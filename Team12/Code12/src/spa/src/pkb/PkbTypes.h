/**
 * For PKB's internal use only.
 * Type definitions and shorthands that are used in various components of PKB.
 */
#include <ast/AstTypes.h>
#include <unordered_map>
#include <unordered_set>

#ifndef SPA_PKBTYPES_H
#define SPA_PKBTYPES_H

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

template <typename T>
using HashSet = std::unordered_set<T>;

template <typename T>
using Vector = std::vector<T>;

typedef struct {
    std::array<Vector<Integer>, STATEMENT_TYPE_COUNT> byType;
} StatementNumVectorsByType;

#endif // SPA_PKBTYPES_H