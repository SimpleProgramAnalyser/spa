#ifndef SPA_RESULTSYNONYMVECTOR_H
#define SPA_RESULTSYNONYMVECTOR_H

#include "ResultSynonym.h"

class ResultSynonymVector: public Errorable {
private:
    Vector<ResultSynonym> resultSynonyms; // Empty Vector but valid AbstractQuery => Select BOOLEAN

public:
    ResultSynonymVector() = default;
    explicit ResultSynonymVector(const ResultSynonym& synonym);
    ResultSynonymVector(Vector<ResultSynonym> synonyms);
    ResultSynonymVector(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    Void add(const ResultSynonym& resultSynonym);
    Vector<ResultSynonym> getSynonyms() const;
    Boolean isSelectBoolean();
    QueryErrorType getErrorType() const;
    ErrorMessage getErrorMessage() const;
    Boolean operator==(const ResultSynonymVector& resultSynonymVector) const;
};

#endif // SPA_RESULTSYNONYMVECTOR_H
