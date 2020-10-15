#include <utility>


#include "AqTypes.h"

/************************/
/** Constructors        */
/************************/

Clause::Clause(ClauseType clauseType)
{
    type = clauseType;
}

Clause::Clause(ClauseType clauseType, QueryErrorType queryErrorType, ErrorMessage errorMessage)
{
    type = clauseType;
    this->setError(queryErrorType, std::move(errorMessage));
}

/************************/
/** Instance Methods    */
/************************/

ClauseType Clause::getType()
{
    return type;
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
