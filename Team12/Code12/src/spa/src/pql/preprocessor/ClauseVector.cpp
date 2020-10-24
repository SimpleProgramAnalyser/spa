#include "ClauseVector.h"

/************************/
/** Constructors        */
/************************/

ClauseVector::ClauseVector(): Errorable() {}

ClauseVector::ClauseVector(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, errorMessage)
{}

/************************/
/** Instance Methods    */
/************************/

Void ClauseVector::add(Clause* clause)
{
    clauses.push_back(std::unique_ptr<Clause>(clause));
}

Clause* ClauseVector::get(Integer index) const
{
    return clauses.at(index).get();
}

Integer ClauseVector::count() const
{
    return clauses.size();
}

QueryErrorType ClauseVector::getErrorType() const
{
    return errorType;
}

ErrorMessage ClauseVector::getErrorMessage() const
{
    return errorMessage;
}

Boolean ClauseVector::operator==(const ClauseVector& clauseVector)
{
    return util::checkVectorOfPointersEqual(this->clauses, clauseVector.clauses);
}
