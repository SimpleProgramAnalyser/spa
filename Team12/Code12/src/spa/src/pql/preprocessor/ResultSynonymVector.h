#ifndef SPA_RESULTSYNONYMVECTOR_H
#define SPA_RESULTSYNONYMVECTOR_H

#include "ResultSynonym.h"

class ResultSynonymVector: public Errorable {
private:
    Vector<ResultSynonym> resultSynonyms; // Empty Vector but valid AbstractQuery => Select BOOLEAN

public:
    ResultSynonymVector();

    // Instantiate a ResultSynonymVector with the one given ResultSynonym.
    explicit ResultSynonymVector(const ResultSynonym& synonym);

    // Instantiate a ResultSynonymVector with the given synonyms.
    explicit ResultSynonymVector(Vector<ResultSynonym> synonyms);

    // Instantiate an erroneous ResultSynonymVector with the given QueryErrorType
    // and ErrorMessage.s
    ResultSynonymVector(QueryErrorType queryErrorType, ErrorMessage errorMessage);

    // Add a ResultSynonym to the vector of ResultSynonyms.
    Void add(const ResultSynonym& resultSynonym);

    // Retrieves the vector of ResultsSynonyms.
    Vector<ResultSynonym> getSynonyms() const;

    // Checks if the vector of ResultSynonyms is empty, indicating that the
    // the query selects a BOOLEAN.
    Boolean isSelectBoolean() const;

    // Retrieves the QueryErrorType of the ResultSynonymsVector.
    QueryErrorType getErrorType() const;

    // Retrieves the ErrorMessage of the ResultSynonymsVector.
    ErrorMessage getErrorMessage() const;

    Boolean operator==(const ResultSynonymVector& resultSynonymVector) const;
};

#endif // SPA_RESULTSYNONYMVECTOR_H
