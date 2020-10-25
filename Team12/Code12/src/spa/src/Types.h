/**
 * Concrete implementations of abstract types.
 */

#ifndef SPA_TYPES_H
#define SPA_TYPES_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef bool Boolean;
typedef int32_t Integer;
typedef std::string String;
typedef void Void;

template <typename T>
using List = std::vector<std::unique_ptr<T>>;

template <typename T>
using Vector = std::vector<T>;

typedef std::pair<String, String> StringPair;
typedef Vector<String> StringVector;

enum StatementType : char {
    AnyStatement = 0,
    AssignmentStatement = 1,
    CallStatement = 2,
    IfStatement = 3,
    PrintStatement = 4,
    ReadStatement = 5,
    WhileStatement = 6,
    STATEMENT_TYPE_COUNT = 7
};

// PQL

enum class ErrorType : char { Syntax = 0, Semantic = 1, Unknown = 2 };

enum class ErrorSource : char { SimpleProgram = 0, Query = 1, Unknown = 2 };

#endif // SPA_TYPES_H
