//
// Created by Chester Sim on 12/9/20.
//

#include "AqTypes.h"

Clause::Clause(ClauseType clauseType): hasError{false}
{
    type = clauseType;
}

ClauseType Clause::getType()
{
    return type;
}

Boolean Clause::isInvalid()
{
    return hasError;
}

Clause* Clause::invalidClause(ClauseType clauseType)
{
    auto* c = new Clause(clauseType);
    (*c).hasError = true;
    return c;
}

Boolean Clause::operator==(const Clause& clause)
{
    return this->type == clause.type && this->hasError == clause.hasError;
}