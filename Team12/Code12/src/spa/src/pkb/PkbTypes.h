/**
 * For PKB internal use, feel free to use but be careful of template redefinition.
 * Type definitions and shorthands that are used in various components of PKB.
 */
#include <array>
#include <ast/AstTypes.h>
#include <unordered_map>
#include <unordered_set>

#ifndef SPA_PKB_TYPES_H
#define SPA_PKB_TYPES_H

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

typedef Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> ArrayArrayList;

typedef Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> ArrayArraySet;

typedef Pair<Integer, StatementType> StatementNumWithType;

#endif // SPA_PKB_TYPES_H
