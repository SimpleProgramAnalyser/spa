/**
 * Implementation of a class representing a list
 * of Clauses in an AbstractQuery.
 */

#include "ClauseVector.h"

#include <utility>

/************************/
/** Constructors        */
/************************/

ClauseVector::ClauseVector(): Errorable(), clauses() {}

ClauseVector::ClauseVector(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, std::move(errorMessage)), clauses()
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

Clause* ClauseVector::remove(Integer index)
{
    // We do not remove the empty unique_pointer to retain size of the ClauseVector.
    // The whole ClauseVector will be useless after this operation, it is the user's job to clean it up.
    return clauses.at(index).release();
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

Boolean ClauseVector::operator==(const ClauseVector& clauseVector) const
{
    return util::checkVectorOfPointersEqual(this->clauses, clauseVector.clauses);
}

List<Clause>& ClauseVector::getAllUnsafe()
{
    return clauses;
}
