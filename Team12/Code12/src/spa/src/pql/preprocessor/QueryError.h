//
// Created by Chester Sim on 15/10/20.
//

#ifndef SPA_QUERYERROR_H
#define SPA_QUERYERROR_H

#include "QueryErrorType.h"
#include "Types.h"

class QueryError {
protected:
    Boolean isSemanticError = false;
    Boolean isSyntaxError = false;
    String errorMessage;

public:
    Boolean isSyntacticallyInvalid();
    Boolean isSemanticallyInvalid();
    Boolean isInvalid();
    Void setSemanticsError();
    Void setSyntaxError();
    Void setError(QueryErrorType queryErrorType);
    Void setError(QueryErrorType queryErrorType, String message);
    String getErrorMessage();
};

#endif // SPA_QUERYERROR_H
