#include <utility>

#include "AqTypes.h"

/************************/
/** Constructors        */
/************************/

ClauseVector::ClauseVector() {}

ClauseVector::ClauseVector(QueryErrorType queryErrorType, ErrorMessage errorMessage)
{
    this->setError(queryErrorType, std::move(errorMessage));
}

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

Boolean ClauseVector::operator==(const ClauseVector& clauseVector)
{
    return util::checkVectorOfPointersEqual(this->clauses, clauseVector.clauses);
}
