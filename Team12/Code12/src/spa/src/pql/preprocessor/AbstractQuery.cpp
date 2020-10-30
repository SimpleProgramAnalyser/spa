#include "AbstractQuery.h"

/************************/
/** Constructors        */
/************************/

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, errorMessage)
{}

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage, Boolean returnFalseResult):
    isToReturnFalseResult(returnFalseResult), Errorable(queryErrorType, errorMessage)
{}

AbstractQuery::AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations):
    resultSynonyms(std::move(synonyms)), declarationTable(declarations)
{}

AbstractQuery::AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations, ClauseVector& clauseVector):
    resultSynonyms(std::move(synonyms)), clauses(std::move(clauseVector)), declarationTable(declarations)
{}

/*************************/
/** Instance Methods    */
/************************/

const Vector<ResultSynonym> AbstractQuery::getSelectedSynonyms() const
{
    return resultSynonyms.getSynonyms();
}

const ClauseVector& AbstractQuery::getClauses() const
{
    return std::move(clauses);
}

ClauseVector AbstractQuery::getClausesUnsafe()
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

Boolean AbstractQuery::operator==(const AbstractQuery& abstractQuery)
{
    return this->resultSynonyms == abstractQuery.resultSynonyms && this->clauses == abstractQuery.clauses
           && this->declarationTable == abstractQuery.declarationTable
           && this->isToReturnFalseResult == abstractQuery.isToReturnFalseResult;
}

Void AbstractQuery::setClauses(ClauseVector& clauseVector)
{
    clauses = std::move(clauseVector);
}
