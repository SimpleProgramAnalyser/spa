#include "AqTypes.h"

Clause::Clause(ClauseType clauseType)
{
    type = clauseType;
}

ClauseType Clause::getType()
{
    return type;
}

Clause* Clause::invalidClause(ClauseType clauseType, QueryErrorType queryErrorType, ErrorMessage errorMessage)
{
    auto* c = new Clause(clauseType);
    c->setError(queryErrorType, errorMessage);
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
