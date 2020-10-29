#include "PatternClause.h"

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

Clause* PatternClause::createPatternClause(const String& clauseConstraint, DeclarationTable& declarationTable)
{
    StringPair pair = util::splitByFirstDelimiter(clauseConstraint, '(');

    Synonym patternSynonym = pair.first;
    DesignEntityType synonymDesignEntityType = declarationTable.getDesignEntityOfSynonym(patternSynonym).getType();
    Boolean isValidDesignEntityType
        = designEntityTypeValidationSet.find(synonymDesignEntityType) != designEntityTypeValidationSet.end();
    if (!isValidDesignEntityType) {
        return new Clause(PatternClauseType, QuerySyntaxError,
                          DesignEntity::INVALID_DESIGN_ENTITY + "used in PatternClause " + clauseConstraint);
    }

    String constraintVariablesString
        = pair.second.substr(0, pair.second.size() - 1); // remove the last char which is a close parentheses
    StringVector constraintVariables = splitByDelimiter(constraintVariablesString, ",");

    if (synonymDesignEntityType == AssignType) {
        return processAssignPatternClause(patternSynonym, constraintVariables, declarationTable);
    } else {
        // Process IfType or WhileType
        return processIfWhilePatternClause(patternSynonym, synonymDesignEntityType, constraintVariables,
                                           declarationTable);
    }
}

Clause* PatternClause::processAssignPatternClause(Synonym patternSynonym, StringVector constraints,
                                                  DeclarationTable& declarationTable)
{
    if (constraints.size() != 2) {
        return new Clause(PatternClauseType, QuerySyntaxError, "Incorrect number of arguments for PatternClause");
    }

    String firstConstraintString = constraints.at(0);
    String secondConstraintString = constraints.at(1);

    Reference firstReference = Reference::createReference(firstConstraintString, declarationTable);
    if (firstReference.isInvalid()) {
        return new Clause(PatternClauseType, firstReference.getErrorType(), firstReference.getErrorMessage());
    }

    QueryErrorType errorType = checkValidityOfVariable(firstReference);
    if (errorType != NoQueryErrorType) {
        return new Clause(PatternClauseType, errorType,
                          "Invalid variable in Assign PatternClause: " + firstReference.getValue());
    }

    ExpressionSpec rightExpressionSpec = ExpressionSpec::createExpressionSpec(secondConstraintString);
    if (rightExpressionSpec.isInvalid()) {
        return new Clause(PatternClauseType, rightExpressionSpec.getErrorType(),
                          "Invalid ExpressionSpec used in assign PatternClause: " + secondConstraintString);
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
        return new Clause(PatternClauseType, QuerySyntaxError, "Incorrect number of arguments for PatternClause");
    }

    String firstConstraintString = constraints.at(0);
    String secondConstraintString = constraints.at(1);
    String thirdConstraintString;
    if (synonymDesignEntityType == IfType) {
        thirdConstraintString = constraints.at(2);
    }

    Reference firstReference = Reference::createReference(firstConstraintString, declarationTable);
    if (firstReference.isInvalid()) {
        return new Clause(PatternClauseType, firstReference.getErrorType(), firstReference.getErrorMessage());
    }

    QueryErrorType errorType = checkValidityOfVariable(firstReference);
    if (errorType != NoQueryErrorType) {
        return new Clause(PatternClauseType, errorType,
                          "Invalid variable in If/While PatternClause: " + firstReference.getValue());
    }

    if (secondConstraintString != "_" || (synonymDesignEntityType == IfType && thirdConstraintString != "_")) {
        return new Clause(PatternClauseType, QuerySyntaxError,
                          "Second or third argument of if/while PatternClause is not a wildcard");
    }

    PatternStatementType patternType = synonymDesignEntityType == IfType ? IfPatternType : WhilePatternType;

    return new PatternClause(std::move(patternSynonym), patternType, firstReference);
}

QueryErrorType PatternClause::checkValidityOfVariable(const Reference& ref)
{
    Boolean isSynonym = ref.getReferenceType() == SynonymRefType;

    if (isSynonym) {
        return ref.getDesignEntity().getType() == VariableType ? NoQueryErrorType : QuerySemanticsError;
    }

    Boolean isWildcard = ref.getReferenceType() == WildcardRefType;
    Boolean isLiteral = ref.getReferenceType() == LiteralRefType;

    return isWildcard || isLiteral ? NoQueryErrorType : QuerySyntaxError;
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
