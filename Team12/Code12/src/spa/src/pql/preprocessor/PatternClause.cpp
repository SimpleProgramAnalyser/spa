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

/**
 * Processes the clause constraint string for a
 * PatternClause, by abstracting it into its relevant
 * PatternTypeStatement, Reference and ExpressionSpec.
 *
 * @param clauseConstraint  String of the clause constraint with all
 *                          whitespaces removed.
 * @return                  A Clause pointer of the PatternClause
 *                          that was constructed.
 */
Clause* PatternClause::createPatternClause(String clauseConstraint, DeclarationTable& declarationTable)
{
    StringPair pair = util::splitByFirstDelimiter(clauseConstraint, '(');

    Synonym patternSynonym = pair.first;
    DesignEntity synonymDesignEntity = declarationTable.getDesignEntityOfSynonym(patternSynonym);
    if (synonymDesignEntity.getType() != AssignType) {
        return Clause::invalidClause(PatternClauseType);
    }

    String constraintVariablesString
        = pair.second.substr(0, pair.second.size() - 1); // remove the last char which is a close parentheses
    StringPair constraintVariablesPair = util::splitByFirstDelimiter(constraintVariablesString, ',');
    String leftConstraintString = constraintVariablesPair.first;
    String rightConstraintString = constraintVariablesPair.second;

    Reference leftReference = Reference::createReference(leftConstraintString, declarationTable);
    Boolean isLeftRefInvalid = !leftReference.isValidEntityRef()
                               || (leftReference.getReferenceType() == SynonymRefType
                                   && leftReference.getDesignEntity().getType() != VariableType);
    if (leftReference.isInvalid() || isLeftRefInvalid) {
        return Clause::invalidClause(PatternClauseType);
    }

    ExpressionSpec rightExpressionSpec = ExpressionSpec::createExpressionSpec(rightConstraintString);
    if (rightExpressionSpec.isInvalid()) {
        return Clause::invalidClause(PatternClauseType);
    }

    return new PatternClause(patternSynonym, AssignPatternType, leftReference, std::move(rightExpressionSpec));
}
