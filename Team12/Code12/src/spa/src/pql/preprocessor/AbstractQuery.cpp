#include "AqTypes.h"

AbstractQuery::AbstractQuery(Boolean hasError): hasError(hasError) {}

AbstractQuery::AbstractQuery(Synonym synonym, DeclarationTable& declarations):
    selectSynonym(std::move(synonym)), declarationTable(declarations), hasError(false)
{}

AbstractQuery::AbstractQuery(Synonym synonym, DeclarationTable& declarations, ClauseVector& clauseVector):
    selectSynonym(std::move(synonym)), clauses(std::move(clauseVector)), declarationTable(declarations), hasError(false)
{}

Synonym AbstractQuery::getSelectSynonym() const
{
    return selectSynonym;
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
    return this->selectSynonym == abstractQuery.selectSynonym && this->clauses == abstractQuery.clauses
           && this->declarationTable == abstractQuery.declarationTable;
}
