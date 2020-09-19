#include "AqTypes.h"

PatternClause::PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec):
    Clause(PatternClauseType), patternSynonym(s), patternStatementType(statementType), entityReference(entRef),
    expressionSpec(std::move(exprSpec))
{}

PatternStatementType PatternClause::getStatementType() const
{
    return patternStatementType;
}

Reference PatternClause::getEntRef() const
{
    return entityReference;
}

const ExpressionSpec& PatternClause::getExprSpec() const
{
    return expressionSpec;
}

Synonym PatternClause::getPatternSynonym() const
{
    return patternSynonym;
}

Boolean PatternClause::operator==(const PatternClause& patternClause)
{
    return this->patternSynonym == patternClause.patternSynonym
           && this->patternStatementType == patternClause.patternStatementType
           && this->entityReference == patternClause.entityReference
           && this->expressionSpec == patternClause.expressionSpec;
}
