#include <utility>

#include "AqTypes.h"

/************************/
/** Static Members      */
/************************/

std::unordered_set<DesignEntityType> PatternClause::designEntityTypeValidationSet{AssignType, IfType, WhileType};

/************************/
/** Constructors        */
/************************/

PatternClause::PatternClause(Synonym s, PatternStatementType statementType, Reference entRef):
    Clause(PatternClauseType), patternSynonym(std::move(s)), patternStatementType(statementType),
    entityReference(std::move(entRef))
{}

PatternClause::PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec):
    Clause(PatternClauseType), patternSynonym(std::move(s)), patternStatementType(statementType),
    entityReference(std::move(entRef)), expressionSpec(std::move(exprSpec))
{}

/************************/
/** Static Methods      */
/************************/

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
Clause* PatternClause::createPatternClause(const String& clauseConstraint, DeclarationTable& declarationTable)
{
    StringPair pair = util::splitByFirstDelimiter(clauseConstraint, '(');

    Synonym patternSynonym = pair.first;
    DesignEntityType synonymDesignEntityType = declarationTable.getDesignEntityOfSynonym(patternSynonym).getType();
    Boolean isValidDesignEntityType
        = designEntityTypeValidationSet.find(synonymDesignEntityType) != designEntityTypeValidationSet.end();
    if (!isValidDesignEntityType) {
        return Clause::invalidClause(PatternClauseType);
    }

    String constraintVariablesString
        = pair.second.substr(0, pair.second.size() - 1); // remove the last char which is a close parentheses
    StringVector constraintVariables = splitByDelimiter(constraintVariablesString, ",");

    if (synonymDesignEntityType == AssignType) {
        return processAssignPatternClause(patternSynonym, constraintVariables, declarationTable);
    } else if (synonymDesignEntityType) {
        return processIfWhilePatternClause(patternSynonym, synonymDesignEntityType, constraintVariables,
                                           declarationTable);
    }

    return Clause::invalidClause(PatternClauseType);
}

Clause* PatternClause::processAssignPatternClause(Synonym patternSynonym, StringVector constraints,
                                                  DeclarationTable& declarationTable)
{
    if (constraints.size() != 2) {
        return Clause::invalidClause(PatternClauseType);
    }

    String firstConstraintString = constraints.at(0);
    String secondConstraintString = constraints.at(1);

    Reference firstReference = Reference::createReference(firstConstraintString, declarationTable);
    if (!isValidVariableEntityRef(firstReference)) {
        return Clause::invalidClause(PatternClauseType);
    }

    ExpressionSpec rightExpressionSpec = ExpressionSpec::createExpressionSpec(secondConstraintString);
    if (rightExpressionSpec.isInvalid()) {
        return Clause::invalidClause(PatternClauseType);
    }

    return new PatternClause(std::move(patternSynonym), AssignPatternType, firstReference,
                             std::move(rightExpressionSpec));
}

Clause* PatternClause::processIfWhilePatternClause(Synonym patternSynonym, DesignEntityType synonymDesignEntityType,
                                                   StringVector constraints, DeclarationTable& declarationTable)
{
    assert(synonymDesignEntityType == IfType // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
           || synonymDesignEntityType == WhileType);

    if ((constraints.size() != 3 && synonymDesignEntityType == IfType)
        || (constraints.size() != 2 && synonymDesignEntityType == WhileType)) {
        return Clause::invalidClause(PatternClauseType);
    }

    String firstConstraintString = constraints.at(0);
    String secondConstraintString = constraints.at(1);
    String thirdConstraintString;
    if (synonymDesignEntityType == IfType) {
        thirdConstraintString = constraints.at(2);
    }

    Reference firstReference = Reference::createReference(firstConstraintString, declarationTable);
    if (!isValidVariableEntityRef(firstReference)) {
        return Clause::invalidClause(PatternClauseType);
    }

    if (secondConstraintString != "_" || (synonymDesignEntityType == IfType && thirdConstraintString != "_")) {
        return Clause::invalidClause(PatternClauseType);
    }

    PatternStatementType patternType = synonymDesignEntityType == IfType ? IfPatternType : WhilePatternType;

    return new PatternClause(std::move(patternSynonym), patternType, firstReference);
}

Boolean PatternClause::isValidVariableEntityRef(Reference ref)
{
    if (ref.isInvalid()) {
        return false;
    }

    Boolean isVariableType
        = ref.getReferenceType() == SynonymRefType && ref.getDesignEntity().getType() == VariableType;
    Boolean isWildcard = ref.getReferenceType() == WildcardRefType;
    Boolean isLiteral = ref.getReferenceType() == LiteralRefType;

    return isVariableType || isWildcard || isLiteral;
}

/************************/
/** Instance Methods    */
/************************/

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
