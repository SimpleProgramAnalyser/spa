/**
 * Concrete implementations of abstract types.
 */

#ifndef SPA_TYPES_H
#define SPA_TYPES_H

#include <memory>
#include <string>
#include <vector>

typedef std::string String;
typedef std::vector<std::unique_ptr<std::string>> StringList;

#endif // SPA_TYPES_H
