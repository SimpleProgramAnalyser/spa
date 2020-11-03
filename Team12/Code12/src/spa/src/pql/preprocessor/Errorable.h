/**
 * This class represents an object that can hold an Error.
 * Since Preprocessor identifies the syntax and semantic
 * errors in a query, Errorable is highly utilised.
 */

#ifndef SPA_PQL_PREPROCESSOR_ERRORABLE_H
#define SPA_PQL_PREPROCESSOR_ERRORABLE_H

#include "QueryErrorType.h"
#include "Types.h"

typedef String ErrorMessage;

class Errorable {
protected:
    QueryErrorType errorType;
    ErrorMessage errorMessage;

public:
    Errorable();

    // Instantiate an Errorable with the given QueryErrorType.
    explicit Errorable(QueryErrorType queryErrorType);

    // Instantiate an Errorable with the given QueryErrorType and ErrorMessage.
    Errorable(QueryErrorType queryErrorType, ErrorMessage message);

    // Checks if the Errorable is syntactically invalid.
    Boolean isSyntacticallyInvalid() const;

    // Checks if the Errorable is semantically invalid.
    Boolean isSemanticallyInvalid() const;

    // Checks if the Errorable is either semantically
    // or syntactically invalid.
    Boolean isInvalid() const;

    // Sets the Errorable to be of QuerySemanticsError.
    Void setSemanticsError();

    // Sets the Errorable to be of QuerySyntaxError.
    Void setSyntaxError();

    // Sets the errorType of the Errorable to the given QueryErrorType.
    Void setError(QueryErrorType queryErrorType);

    // Sets the Errorable to the given QueryErrorType and ErrorMessage.
    Void setError(QueryErrorType queryErrorType, ErrorMessage message);

    // Retrieves the ErrorMessage.
    String getErrorMessage();

    // Retrieves the QueryErrorType.
    QueryErrorType getErrorType();
};

#endif // SPA_PQL_PREPROCESSOR_ERRORABLE_H
