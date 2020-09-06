#ifndef SPA_INPUT_ERROR_H
#define SPA_INPUT_ERROR_H

#include <Types.h>

class InputLocation {
public:
    const Integer line;
    const Integer column;

    InputLocation(Integer lineNumber, Integer columnNumber);
};

class InputError {
public:
    const String message;
    const Integer line;
    const Integer column;
    const ErrorSource source;
    const ErrorType type;

    InputError(String msg, Integer lineNumber, Integer columnNumber, ErrorSource src, ErrorType errorType);
    
    String getMessage();
    InputLocation* getLocation();
    ErrorSource getSource();
    ErrorType getType();
};

#endif // SPA_INPUT_ERROR_H