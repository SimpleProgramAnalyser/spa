#include "Errorable.h"

/************************/
/** Instance Methods    */
/************************/

Boolean Errorable::isSyntacticallyInvalid()
{
    return errorType == QuerySyntaxError;
}

Boolean Errorable::isSemanticallyInvalid()
{
    return errorType == QuerySemanticsError;
}

Boolean Errorable::isInvalid() const
{
    return errorType != NoQueryErrorType;
}

Void Errorable::setSemanticsError()
{
    errorType = QuerySemanticsError;
}

Void Errorable::setSyntaxError()
{
    errorType = QuerySyntaxError;
}

Void Errorable::setError(QueryErrorType queryErrorType, String message)
{
    setError(queryErrorType);
    this->errorMessage = message;
}

Void Errorable::setError(QueryErrorType queryErrorType)
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

String Errorable::getErrorMessage()
{
    return errorMessage;
}

QueryErrorType Errorable::getErrorType()
{
    return errorType;
}