/**
 * Concrete implementations of abstract types.
 */

#ifndef SPA_TYPES_H
#define SPA_TYPES_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

typedef int32_t Integer;
typedef std::string String;

template <typename T>
using List = std::vector<std::unique_ptr<T>>;

typedef List<String> StringList;

#endif // SPA_TYPES_H
