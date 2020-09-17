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

Boolean Clause::operator==(Clause& clause)
{
    if (this->type != clause.type) {
        return false;
    }

    if (this->type == SuchThatClauseType) {
        return dynamic_cast<SuchThatClause&>(*this) == dynamic_cast<SuchThatClause&>(clause);
    } else {
        return dynamic_cast<PatternClause&>(*this) == dynamic_cast<PatternClause&>(clause);
    }
}
