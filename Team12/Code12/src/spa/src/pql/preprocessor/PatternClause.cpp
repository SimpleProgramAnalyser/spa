#include "AqTypes.h"

PatternClause::PatternClause(PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec):
    Clause(PatternClauseType), patternStatementType(statementType), entityReference(entRef), expressionSpec(exprSpec)
{}

PatternStatementType PatternClause::getStatementType()
{
    return patternStatementType;
}

Reference PatternClause::getEntRef()
{
    return entityReference;
}

ExpressionSpec PatternClause::getExprSpec()
{
    return expressionSpec;
}

Boolean PatternClause::operator==(const PatternClause& patternClause)
{
    return this->patternStatementType == patternClause.patternStatementType
           && this->entityReference == patternClause.entityReference
           && this->expressionSpec == patternClause.expressionSpec;
}
