#include "AqTypes.h"

ResultSynonymVector::ResultSynonymVector(ResultSynonym synonym)
{
    resultSynonyms.push_back(synonym);
}

ResultSynonymVector::ResultSynonymVector(Vector<ResultSynonym> synonyms): resultSynonyms{synonyms} {}

ResultSynonymVector::ResultSynonymVector(QueryErrorType queryErrorType, ErrorMessage errorMessage)
{
    this->setError(queryErrorType, errorMessage);
}

Vector<ResultSynonym> ResultSynonymVector::getSynonyms() const
{
    return resultSynonyms;
}

Void ResultSynonymVector::add(ResultSynonym resultSynonym)
{
    resultSynonyms.push_back(resultSynonym);
}

Boolean ResultSynonymVector::isSelectBoolean()
{
    return resultSynonyms.empty();
}

Boolean ResultSynonymVector::operator==(const ResultSynonymVector& resultSynonymVector) const
{
    // Check equality of resultSynonyms
    if (this->resultSynonyms.size() != resultSynonymVector.resultSynonyms.size()) {
        return false;
    }

    size_t length = this->resultSynonyms.size();
    for (size_t i = 0; i < length; i++) {
        if (resultSynonyms.at(i) != resultSynonymVector.resultSynonyms.at(i)) {
            return false;
        }
    }

    return true;
};
