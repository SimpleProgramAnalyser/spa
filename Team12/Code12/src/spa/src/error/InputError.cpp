/**
 * Implementation of the Input Error Library.
 */
#include "InputError.h"

InputLocation::InputLocation(Integer lineNumber, Integer columnNumber): line(lineNumber), column(columnNumber) {}

InputError::InputError(): line(-1), column(-1), source(ErrorSource::Unknown), type(ErrorType::Unknown) {}

InputError::InputError(String msg, Integer lineNumber, Integer columnNumber, ErrorSource src, ErrorType errorType):
    message(std::move(msg)), line(lineNumber), column(columnNumber), source(src), type(errorType)
{}

bool InputError::operator==(const InputError& error) const
{
    return this->message == error.message && this->line == error.line && this->column == error.column
           && this->source == error.source && this->type == error.type;
}

String InputError::getMessage() const
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

String InputError::getSourceString() const
{
    switch (source) {
    case ErrorSource::SimpleProgram:
        return "SIMPLE Program";
    case ErrorSource::Query:
        return "PQL Query";
    default:
        return "Location Unknown";
    }
}

String InputError::getTypeString() const
{
    switch (type) {
    case ErrorType::Syntax:
        return "Syntax Error";
    case ErrorType::Semantic:
        return "Semantic Error";
    default:
        return "Unknown Error";
    }
}
