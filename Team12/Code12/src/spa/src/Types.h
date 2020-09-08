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

enum StatementType {
    AnyStatement = 0,
    AssignmentStatement,
    CallStatement,
    IfStatement,
    PrintStatement,
    ReadStatement,
    WhileStatement,
    STATEMENT_TYPE_COUNT
};

// PQL

#endif // SPA_TYPES_H
