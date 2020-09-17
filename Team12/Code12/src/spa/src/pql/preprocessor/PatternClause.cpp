#include "AqTypes.h"

PatternClause::PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec):
    Clause(PatternClauseType), patternSynonym(s), patternStatementType(statementType), entityReference(entRef),
    expressionSpec(exprSpec)
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

Synonym PatternClause::getPatternSynonym()
{
    return patternSynonym;
}

Boolean PatternClause::operator==(const PatternClause& patternClause)
{
    ExpressionSpec thisES = this->expressionSpec;
    ExpressionSpec thatES = patternClause.expressionSpec;
    Boolean equalsES = thisES == thatES;

    return this->patternSynonym == patternClause.patternSynonym
           && this->patternStatementType == patternClause.patternStatementType
           && this->entityReference == patternClause.entityReference
           && this->expressionSpec == patternClause.expressionSpec;
}
