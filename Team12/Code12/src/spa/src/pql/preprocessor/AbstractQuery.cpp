#include "AqTypes.h"

AbstractQuery::AbstractQuery(Boolean hasError): hasError(hasError) {}

AbstractQuery::AbstractQuery(const Vector<ResultSynonym>& synonyms, DeclarationTable& declarations):
    resultSynonyms(synonyms), declarationTable(declarations), hasError(false)
{}

AbstractQuery::AbstractQuery(const Vector<ResultSynonym>& synonyms, DeclarationTable& declarations,
                             ClauseVector& clauseVector):
    resultSynonyms(synonyms),
    clauses(std::move(clauseVector)), declarationTable(declarations), hasError(false)
{}

Synonym AbstractQuery::getSelectSynonym() const
{
    return resultSynonyms.at(0).getSynonym();
}

Vector<ResultSynonym> AbstractQuery::getSynonyms()
{
    return resultSynonyms;
}

const ClauseVector& AbstractQuery::getClauses() const
{
    return std::move(clauses);
}

DeclarationTable AbstractQuery::getDeclarationTable() const
{
    return declarationTable;
}

Boolean AbstractQuery::isInvalid() const
{
    return hasError;
}

AbstractQuery::AbstractQuery(): hasError{false} {}

AbstractQuery AbstractQuery::invalidAbstractQuery()
{
    return AbstractQuery(true);
}

Boolean AbstractQuery::operator==(const AbstractQuery& abstractQuery)
{
    // Check equality of resultSynonyms
    if (this->resultSynonyms.size() != abstractQuery.resultSynonyms.size()) {
        return false;
    }

    size_t length = this->resultSynonyms.size();
    for (size_t i = 0; i < length; i++) {
        if (resultSynonyms.at(i) != abstractQuery.resultSynonyms.at(i)) {
            return false;
        }
    }

    return this->clauses == abstractQuery.clauses && this->declarationTable == abstractQuery.declarationTable;
}
