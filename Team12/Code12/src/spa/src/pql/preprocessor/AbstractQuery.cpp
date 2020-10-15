#include <utility>

#include "AqTypes.h"

/************************/
/** Constructors        */
/************************/

AbstractQuery::AbstractQuery() = default;

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage)
{
    this->setError(queryErrorType, std::move(errorMessage));
}

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage, Boolean returnFalseResult):
    isToReturnFalseResult(returnFalseResult)
{
    this->setError(queryErrorType, std::move(errorMessage));
}

AbstractQuery::AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations):
    resultSynonyms(std::move(synonyms)), declarationTable(declarations)
{}

AbstractQuery::AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations, ClauseVector& clauseVector):
    resultSynonyms(std::move(synonyms)), clauses(std::move(clauseVector)), declarationTable(declarations)
{}

/*************************/
/** Instance Methods    */
/************************/

Vector<ResultSynonym> AbstractQuery::getSelectSynonym()
{
    return resultSynonyms.getSynonyms();
}

Vector<ResultSynonym> AbstractQuery::getSynonyms()
{
    return resultSynonyms.getSynonyms();
}

const ClauseVector& AbstractQuery::getClauses() const
{
    return std::move(clauses);
}

DeclarationTable AbstractQuery::getDeclarationTable() const
{
    return declarationTable;
}

Boolean AbstractQuery::operator==(const AbstractQuery& abstractQuery)
{
    return this->resultSynonyms == abstractQuery.resultSynonyms && this->clauses == abstractQuery.clauses
           && this->declarationTable == abstractQuery.declarationTable;
}
