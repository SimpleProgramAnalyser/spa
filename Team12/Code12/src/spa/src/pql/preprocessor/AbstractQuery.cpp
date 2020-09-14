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

Boolean AbstractQuery::isInvalid() const
{
    return hasError;
}

AbstractQuery::AbstractQuery():
    clauses{*(new ClauseVector())}, declarationTable{*(new DeclarationTable())}, hasError{false}
{}

AbstractQuery AbstractQuery::invalidAbstractQuery()
{
    auto* aq = new AbstractQuery();
    (*aq).hasError = true;
    return *aq;
}

Boolean AbstractQuery::operator==(const AbstractQuery& abstractQuery)
{
    return this->selectSynonym == abstractQuery.selectSynonym && this->clauses == abstractQuery.clauses
           && this->declarationTable == abstractQuery.declarationTable;
}
