/**
 * Error stub
 */

#ifndef SPA_ERROR_H
#define SPA_ERROR_H

#include <exception>

typedef std::exception Error;
class SyntaxError: public Error {}

#endif // SPA_ERROR_H
