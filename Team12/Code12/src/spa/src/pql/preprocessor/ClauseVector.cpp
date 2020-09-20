#include "AqTypes.h"

ClauseVector::ClauseVector(Boolean hasError) noexcept: hasError(hasError) {}

ClauseVector::ClauseVector() noexcept: hasError{false} {}

ClauseVector ClauseVector::invalidClauseVector()
{
    return ClauseVector(true);
}

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

Boolean ClauseVector::isInvalid() const
{
    return hasError;
}

Boolean ClauseVector::operator==(const ClauseVector& clauseVector)
{
    return util::checkVectorOfPointersEqual(this->clauses, clauseVector.clauses)
           && this->hasError == clauseVector.hasError;
}
