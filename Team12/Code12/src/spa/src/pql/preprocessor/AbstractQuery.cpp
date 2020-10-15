#include <utility>
#include "AqTypes.h"

/************************/
/** Constructors        */
/************************/

AbstractQuery::AbstractQuery()= default;

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage) {
    this->setError(queryErrorType, std::move(errorMessage));
}

AbstractQuery::AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage, Boolean isSelectBoolean) { // TODO:
    this->setError(queryErrorType, std::move(errorMessage));
}

AbstractQuery::AbstractQuery(const Vector<ResultSynonym>& synonyms, DeclarationTable& declarations):
    resultSynonyms(synonyms), declarationTable(declarations)
{}

AbstractQuery::AbstractQuery(const Vector<ResultSynonym>& synonyms, DeclarationTable& declarations,
                             ClauseVector& clauseVector):
    resultSynonyms(synonyms),
    clauses(std::move(clauseVector)), declarationTable(declarations)
{}

/*************************/
/** Instance Methods    */
/************************/

Vector<ResultSynonym> AbstractQuery::getSelectSynonym() const
{
    return resultSynonyms;
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
