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
    Boolean isSyntacticallyInvalid() const;
    Boolean isSemanticallyInvalid() const;
    Boolean isInvalid() const;
    Void setSemanticsError();
    Void setSyntaxError();
    Void setError(QueryErrorType queryErrorType);
    Void setError(QueryErrorType queryErrorType, ErrorMessage message);
    String getErrorMessage() const;
    QueryErrorType getErrorType() const;
};

#endif // SPA_QUERYERROR_H
