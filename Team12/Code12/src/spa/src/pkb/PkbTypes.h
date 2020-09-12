/**
 * For PKB's internal use only.
 * Type definitions and shorthands that are used in various components of PKB.
 */
#include <ast/AstTypes.h>
#include <array>
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

template <typename T, std::size_t n>
using Array = std::array<T, n>;

template <typename K, typename V>
using Pair = std::pair<K, V>;

typedef struct {
    Array<Vector<Integer>, STATEMENT_TYPE_COUNT> byType;
} StatementNumVectorsByType;

typedef struct {
    Array<HashSet<Integer>, STATEMENT_TYPE_COUNT> byType;
} StatementNumSetsByType;
#endif // SPA_PKBTYPES_H
