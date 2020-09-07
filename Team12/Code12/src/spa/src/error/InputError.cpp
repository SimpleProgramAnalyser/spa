/**
 * Implementation of the Input Error Library.
 */
#include "InputError.h"

InputLocation::InputLocation(Integer lineNumber, Integer columnNumber):
    line(std::move(lineNumber)), column(std::move(columnNumber))
{}

InputError::InputError(String msg, Integer lineNumber, Integer columnNumber, ErrorSource src, ErrorType errorType):
    message(std::move(msg)), line(std::move(lineNumber)), column(std::move(columnNumber)), source(std::move(src)),
    type(std::move(errorType))
{}

String InputError::getMessage()
{
    return message;
}

InputLocation* InputError::getLocation()
{
    InputLocation* inputLocation = new InputLocation(line, column);
    return inputLocation;
}

ErrorSource InputError::getSource()
{
    return source;
}

ErrorType InputError::getType()
{
    return type;
}
