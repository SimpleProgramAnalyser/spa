/**
 * Implementation of Abstract Query, a class that
 * represents a parsed Program Query Language query.
 */

#include "AbstractQuery.h"

#include <utility>

/************************/
/** Constructors        */
/************************/

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, std::move(errorMessage)), resultSynonyms(), clauses(), declarationTable(),
    isToReturnFalseResult(false)
{}

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage, Boolean returnFalseResult):
    Errorable(queryErrorType, std::move(errorMessage)), resultSynonyms(), clauses(), declarationTable(),
    isToReturnFalseResult(returnFalseResult)
{}

AbstractQuery::AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations):
    Errorable(), resultSynonyms(std::move(synonyms)), clauses(), declarationTable(declarations),
    isToReturnFalseResult(false)
{}

AbstractQuery::AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations, ClauseVector& clauseVector):
    Errorable(), resultSynonyms(std::move(synonyms)), clauses(std::move(clauseVector)), declarationTable(declarations),
    isToReturnFalseResult(false)
{}

/*************************/
/** Instance Methods    */
/************************/

Vector<ResultSynonym> AbstractQuery::getSelectedSynonyms() const
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

Boolean AbstractQuery::toReturnFalseResult() const
{
    return isToReturnFalseResult;
}

ErrorMessage AbstractQuery::getErrorMessage() const
{
    return errorMessage;
}

Boolean AbstractQuery::operator==(const AbstractQuery& abstractQuery) const
{
    return this->resultSynonyms == abstractQuery.resultSynonyms && this->clauses == abstractQuery.clauses
           && this->declarationTable == abstractQuery.declarationTable
           && this->isToReturnFalseResult == abstractQuery.isToReturnFalseResult;
}
