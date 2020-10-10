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
    c->hasError = true;
    return c;
}

Boolean Clause::operator==(Clause& clause)
{
    if (this->type != clause.type) {
        return false;
    }

    if (this->type == SuchThatClauseType) {
        return dynamic_cast<SuchThatClause&>(*this) == dynamic_cast<SuchThatClause&>(clause);
    } else if (this->type == PatternClauseType) {
        return dynamic_cast<PatternClause&>(*this) == dynamic_cast<PatternClause&>(clause);
    } else if (this->type == WithClauseType) {
        return dynamic_cast<WithClause&>(*this) == dynamic_cast<WithClause&>(clause);
    } else {
        return false;
    }
}
