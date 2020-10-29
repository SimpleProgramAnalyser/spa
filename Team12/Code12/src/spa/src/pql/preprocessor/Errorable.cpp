#include "Errorable.h"

#include <utility>

/************************/
/** Constructors        */
/************************/

Errorable::Errorable(): errorType(NoQueryErrorType), errorMessage() {}

Errorable::Errorable(QueryErrorType queryErrorType): errorType(queryErrorType), errorMessage() {}

Errorable::Errorable(QueryErrorType queryErrorType, ErrorMessage message):
    errorType(queryErrorType), errorMessage(std::move(message))
{}

/************************/
/** Instance Methods    */
/************************/

Boolean Errorable::isSyntacticallyInvalid() const
{
    return errorType == QuerySyntaxError;
}

Boolean Errorable::isSemanticallyInvalid() const
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
    this->errorMessage = std::move(message);
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