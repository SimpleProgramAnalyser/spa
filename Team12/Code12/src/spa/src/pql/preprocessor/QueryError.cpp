#include "QueryError.h"

/************************/
/** Instance Methods    */
/************************/

Boolean QueryError::isSyntacticallyInvalid()
{
    return isSyntaxError;
}

Boolean QueryError::isSemanticallyInvalid()
{
    return isSemanticError;
}

Boolean QueryError::isInvalid()
{
    return isSyntaxError || isSemanticError;
}

Void QueryError::setSemanticsError()
{
    isSemanticError = true;
}

Void QueryError::setSyntaxError()
{
    isSyntaxError = true;
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
        isSemanticError = true;
        break;
    case QuerySyntaxError:
        isSyntaxError = true;
        break;
    default:
        break;
    }
}

String QueryError::getErrorMessage() {
    return errorMessage;
}
