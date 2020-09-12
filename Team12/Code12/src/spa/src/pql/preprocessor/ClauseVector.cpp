//
// Created by Chester Sim on 13/9/20.
//

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

Integer ClauseVector::totalNumClauses()
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