#include "ResultSynonymVector.h"

#include <utility>

ResultSynonymVector::ResultSynonymVector(const ResultSynonym& synonym)
{
    resultSynonyms.push_back(synonym);
}

ResultSynonymVector::ResultSynonymVector(Vector<ResultSynonym> synonyms): resultSynonyms{std::move(synonyms)} {}

ResultSynonymVector::ResultSynonymVector(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, std::move(errorMessage))
{}

Vector<ResultSynonym> ResultSynonymVector::getSynonyms() const
{
    return resultSynonyms;
}

Void ResultSynonymVector::add(const ResultSynonym& resultSynonym)
{
    resultSynonyms.push_back(resultSynonym);
}

Boolean ResultSynonymVector::isSelectBoolean()
{
    return resultSynonyms.empty();
}

QueryErrorType ResultSynonymVector::getErrorType() const
{
    return errorType;
}

ErrorMessage ResultSynonymVector::getErrorMessage() const
{
    return errorMessage;
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
}
