#ifndef SPA_ABSTRACTQUERY_H
#define SPA_ABSTRACTQUERY_H

#include "ClauseVector.h"
#include "DeclarationTable.h"
#include "Errorable.h"
#include "ResultSynonymVector.h"

class AbstractQuery: public Errorable {
private:
    const ResultSynonymVector resultSynonyms; // Empty Vector but valid AbstractQuery => Select BOOLEAN
    ClauseVector clauses;
    DeclarationTable declarationTable;
    Boolean isToReturnFalseResult = false;

public:
    AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage, Boolean returnFalseResult);
    AbstractQuery(ResultSynonymVector synonym, DeclarationTable& declarations);
    AbstractQuery(ResultSynonymVector synonym, DeclarationTable& declarations, ClauseVector& clauseVector);
    const Vector<ResultSynonym> getSelectSynonym() const;
    const Vector<ResultSynonym> getSynonyms() const;
    const ClauseVector& getClauses() const;
    DeclarationTable getDeclarationTable() const;
    Boolean toReturnFalseResult() const;
    ErrorMessage getErrorMessage() const;
    Boolean operator==(const AbstractQuery& abstractQuery);
};

#endif // SPA_ABSTRACTQUERY_H
