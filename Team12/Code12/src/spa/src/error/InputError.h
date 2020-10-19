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

    explicit InputError();
    InputError(String msg, Integer lineNumber, Integer columnNumber, ErrorSource src, ErrorType errorType);

    bool operator==(const InputError& error) const;

    String getMessage() const;
    InputLocation getLocation() const;
    ErrorSource getSource() const;
    ErrorType getType() const;
    String getSourceString() const;
    String getTypeString() const;
};

#endif // SPA_INPUT_ERROR_H
