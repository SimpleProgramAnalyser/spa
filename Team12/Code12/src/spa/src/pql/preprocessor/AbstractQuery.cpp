#include "AqTypes.h"

AbstractQuery::AbstractQuery(Synonym synonym, DeclarationTable& declarations, ClauseVector& clauseVector):
    selectSynonym(std::move(synonym)), clauses(clauseVector), declarationTable(declarations), hasError(false)
{}

Synonym AbstractQuery::getSelectSynonym()
{
    return selectSynonym;
}
ClauseVector AbstractQuery::getClauses()
{
    return clauses;
}
DeclarationTable AbstractQuery::getDeclarationTable()
{
    return declarationTable;
}

Boolean AbstractQuery::isInvalid()
{
    return hasError;
}

AbstractQuery::AbstractQuery():
    hasError{false}, selectSynonym{""}, clauses{*(new ClauseVector())}, declarationTable{*(new DeclarationTable())}
{}

AbstractQuery AbstractQuery::invalidAbstractQuery()
{
    AbstractQuery* aq = new AbstractQuery();
    (*aq).hasError = true;
    return *aq;
}

Boolean AbstractQuery::operator==(const AbstractQuery& abstractQuery)
{
    return this->selectSynonym == abstractQuery.selectSynonym && this->clauses == abstractQuery.clauses
           && this->declarationTable == abstractQuery.declarationTable;
}
