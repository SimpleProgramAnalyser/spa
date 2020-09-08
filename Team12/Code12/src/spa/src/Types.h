/**
 * Concrete implementations of abstract types.
 */

#ifndef SPA_TYPES_H
#define SPA_TYPES_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

typedef bool Boolean;
typedef int32_t Integer;
typedef std::string String;
typedef void Void;

template <typename T>
using List = std::vector<std::unique_ptr<T>>;

typedef List<String> StringList;

enum StatementType {
    AnyStatement = 0,
    AssignmentStatement = 1,
    CallStatement = 2,
    IfStatement = 4,
    PrintStatement = 8,
    ReadStatement = 16,
    WhileStatement = 32
};

enum ErrorType { Syntax = 0, Semantic = 1 };

enum ErrorSource { SimpleProgram = 0, Query = 1 };

#endif // SPA_TYPES_H