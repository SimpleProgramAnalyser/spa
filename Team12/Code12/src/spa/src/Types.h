/**
 * Concrete implementations of abstract types.
 */

#ifndef SPA_TYPES_H
#define SPA_TYPES_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

typedef bool Boolean;
typedef int32_t Integer;
typedef std::string String;
typedef void Void;

template <typename T>
using List = std::vector<std::unique_ptr<T>>;

typedef List<String> StringList;

typedef std::vector<String> StringVector;

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

enum ErrorType : char { Syntax = 0, Semantic = 1 };

enum ErrorSource : char { SimpleProgram = 0, Query = 1 };

#endif // SPA_TYPES_H
