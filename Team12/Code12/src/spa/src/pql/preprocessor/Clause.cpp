#include "Clause.h"

#include <utility>

#include "PatternClause.h"
#include "SuchThatClause.h"
#include "WithClause.h"

/************************/
/** Constructors        */
/************************/

Clause::Clause(ClauseType clauseType): Errorable(), type(clauseType) {}

Clause::Clause(ClauseType clauseType, QueryErrorType queryErrorType, ErrorMessage message):
    Errorable(queryErrorType, std::move(message)), type(clauseType)
{}

/************************/
/** Instance Methods    */
/************************/

ClauseType Clause::getType()
{
    return type;
}

QueryErrorType Clause::getErrorType() const
{
    return errorType;
}

ErrorMessage Clause::getErrorMessage() const
{
    return errorMessage;
}

Boolean Clause::operator==(Clause& clause)
{
    if (this->type != clause.type) {
        return false;
    }

    if (this->type == SuchThatClauseType && clause.type == SuchThatClauseType) {
        return dynamic_cast<SuchThatClause&>(*this) == dynamic_cast<SuchThatClause&>(clause);
    } else if (this->type == PatternClauseType && clause.type == PatternClauseType) {
        return dynamic_cast<PatternClause&>(*this) == dynamic_cast<PatternClause&>(clause);
    } else if (this->type == WithClauseType && clause.type == WithClauseType) {
        return dynamic_cast<WithClause&>(*this) == dynamic_cast<WithClause&>(clause);
    } else {
        return false;
    }
}
