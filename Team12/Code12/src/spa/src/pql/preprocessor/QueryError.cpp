#include "QueryError.h"

/************************/
/** Instance Methods    */
/************************/

Boolean QueryError::isSyntacticallyInvalid()
{
    return errorType == QuerySyntaxError;
}

Boolean QueryError::isSemanticallyInvalid()
{
    return errorType == QuerySemanticsError;
}

Boolean QueryError::isInvalid()
{
    return errorType != NoQueryErrorType;
}

Void QueryError::setSemanticsError()
{
    errorType = QuerySemanticsError;
}

Void QueryError::setSyntaxError()
{
    errorType = QuerySyntaxError;
}

Void QueryError::setError(QueryErrorType queryErrorType, String message)
{
    setError(queryErrorType);
    this->errorMessage = message;
}

Void QueryError::setError(QueryErrorType queryErrorType)
{
    switch (queryErrorType) {
    case QuerySemanticsError:
        errorType = QuerySemanticsError;
        break;
    case QuerySyntaxError:
        errorType = QuerySyntaxError;
        break;
    default:
        break;
    }
}

String QueryError::getErrorMessage()
{
    return errorMessage;
}

QueryErrorType QueryError::getErrorType()
{
    return errorType;
}