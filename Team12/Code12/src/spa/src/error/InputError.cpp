/**
 * Implementation of the Input Error Library.
 */
#include "InputError.h"

InputLocation::InputLocation(Integer lineNumber, Integer columnNumber): line(lineNumber), column(columnNumber) {}

InputError::InputError(String msg, Integer lineNumber, Integer columnNumber, ErrorSource src, ErrorType errorType):
    message(std::move(msg)), line(lineNumber), column(columnNumber), source(src), type(errorType)
{}

String InputError::getMessage()
{
    return message;
}

InputLocation InputError::getLocation() const
{
    return {line, column};
}

ErrorSource InputError::getSource() const
{
    return source;
}

ErrorType InputError::getType() const
{
    return type;
}
