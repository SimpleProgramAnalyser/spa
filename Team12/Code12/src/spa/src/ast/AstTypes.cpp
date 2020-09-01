/**
 * Implementation of Abstract Syntax Tree
 * classes and methods.
 */

#include "AstTypes.h"

#include <string>

String Constant::toString()
{
    return std::to_string(static_cast<int>(value));
}

String Variable::toString()
{
    return static_cast<String>(varName);
}
