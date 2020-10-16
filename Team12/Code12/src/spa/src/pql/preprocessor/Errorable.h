//
// Created by Chester Sim on 15/10/20.
//

#ifndef SPA_QUERYERROR_H
#define SPA_QUERYERROR_H

#include "QueryErrorType.h"
#include "Types.h"

typedef String ErrorMessage;

class Errorable {
protected:
    QueryErrorType errorType = NoQueryErrorType;
    ErrorMessage errorMessage;

public:
    Boolean isSyntacticallyInvalid();
    Boolean isSemanticallyInvalid();
    Boolean isInvalid() const;
    Void setSemanticsError();
    Void setSyntaxError();
    Void setError(QueryErrorType queryErrorType);
    Void setError(QueryErrorType queryErrorType, ErrorMessage message);
    String getErrorMessage();
    QueryErrorType getErrorType();
};

#endif // SPA_QUERYERROR_H
