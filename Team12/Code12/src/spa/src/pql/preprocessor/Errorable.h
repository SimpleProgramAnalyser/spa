//
// Created by Chester Sim on 15/10/20.
//

#ifndef SPA_QUERYERROR_H
#define SPA_QUERYERROR_H

#include "Types.h"
#include "QueryErrorType.h"

typedef String ErrorMessage;

class Errorable {
protected:
    QueryErrorType errorType = NoQueryErrorType;
    ErrorMessage errorMessage;

public:
    Errorable()=default;
    Errorable(QueryErrorType queryErrorType);
    Errorable(QueryErrorType queryErrorType, ErrorMessage message);
    Boolean isSyntacticallyInvalid() const;
    Boolean isSemanticallyInvalid() const;
    Boolean isInvalid() const;
    Void setSemanticsError();
    Void setSyntaxError();
    Void setError(QueryErrorType queryErrorType);
    Void setError(QueryErrorType queryErrorType, ErrorMessage message);
    String getErrorMessage();
    QueryErrorType getErrorType();
};

#endif // SPA_QUERYERROR_H
