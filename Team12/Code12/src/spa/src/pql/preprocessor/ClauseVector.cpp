#include "AqTypes.h"

ClauseVector::ClauseVector() noexcept: hasError{false} {}

ClauseVector ClauseVector::invalidClauseVector()
{
    auto* cV = new ClauseVector();
    (*cV).hasError = true;
    return *cV;
}

Void ClauseVector::add(Clause* clause)
{
    clauses.push_back(clause);
}

Clause* ClauseVector::get(Integer index)
{
    return clauses.at(index);
}

Integer ClauseVector::count()
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
