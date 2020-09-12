/**
 * Implementation of the AbstractQuery, and
 * supporting substructures and their public methods.
 */

#include "AqTypes.h"

#include "Util.h"

DesignEntity::DesignEntity(DesignEntityType designEntityType)
{
    type = designEntityType;
}

DesignEntity::DesignEntity(const String& stringType)
{
    if (stringType == "statement") {
        type = StatementType;
    } else if (stringType == "read") {
        type = ReadType;
    } else if (stringType == "print") {
        type = PrintType;
    } else if (stringType == "call") {
        type = CallType;
    } else if (stringType == "while") {
        type = WhileType;
    } else if (stringType == "if") {
        type = IfType;
    } else if (stringType == "assign") {
        type = AssignType;
    } else if (stringType == "variable") {
        type = VariableType;
    } else if (stringType == "constant") {
        type = ConstantType;
    } else if (stringType == "procedure") {
        type = ProcedureType;
    } else {
        type = NonExistentType;
    }
}

DesignEntityType DesignEntity::getType()
{
    return type;
}

Clause::Clause(ClauseType clauseType)
{
    type = clauseType;
}

ClauseType Clause::getType()
{
    return type;
}

Reference::Reference(ReferenceType refType, ReferenceValue& refValue)
{
    referenceType = refType;
    referenceValue = refValue;
}

ReferenceType Reference::getReferenceType()
{
    return referenceType;
}

ReferenceValue Reference::getValue()
{
    return referenceValue;
}

EntityReference::EntityReference(ReferenceValue& refValue): Reference(ENTITY_REF, refValue) {}

StatementReference::StatementReference(ReferenceValue& refValue): Reference(STATEMENT_REF, refValue) {}

Relationship::Relationship(RelationshipReference relationshipRef, Reference leftRef, Reference rightRef):
    leftReference(leftRef), rightReference(rightRef)
{
    relationshipReference = relationshipRef;
}

RelationshipReference Relationship::getRelationship()
{
    return relationshipReference;
}

Reference Relationship::getLeftRef()
{
    return leftReference;
}

Reference Relationship::getRightRef()
{
    return rightReference;
}

SuchThatClause::SuchThatClause(Relationship& r): Clause(SUCH_THAT_CLAUSE), relationship{r} {}

Relationship SuchThatClause::getRelationship()
{
    return relationship;
}

ExpressionSpec::ExpressionSpec(String expr, bool any, bool beforeOrAfter)
{
    expression = expr;
    isAny = any;
    hasBeforeOrAfter = beforeOrAfter;
}

bool ExpressionSpec::checkIsAny()
{
    return isAny;
}

bool ExpressionSpec::checkHasBeforeOrAfter()
{
    return hasBeforeOrAfter;
}

Expression ExpressionSpec::getExpression()
{
    return expression;
}

PatternClause::PatternClause(PatternStatementType statementType, EntityReference entRef, ExpressionSpec exprSpec):
    Clause(PATTERN_CLAUSE), patternStatementType(statementType), entityReference(entRef), expressionSpec(exprSpec)
{}

PatternStatementType PatternClause::getStatementType()
{
    return patternStatementType;
}

EntityReference PatternClause::getEntRef()
{
    return entityReference;
}

ExpressionSpec PatternClause::getExprSpec()
{
    return expressionSpec;
}

void DeclarationTable::addDeclaration(Synonym s, DesignEntity& designEntity)
{
    table.insert({s, designEntity});
}

void DeclarationTable::setInvalidDeclaration()
{
    isInvalid = true;
}
Boolean DeclarationTable::hasInvalidDeclaration()
{
    return isInvalid;
}
Boolean DeclarationTable::hasSynonym(Synonym s)
{
    std::unordered_map<Synonym, DesignEntity>::const_iterator got = table.find(s);
    if (got == table.end()) {
        return false;
    }

    return true;
}

DesignEntity DeclarationTable::getDesignEntityOfSynonym(Synonym s)
{
    std::unordered_map<Synonym, DesignEntity>::const_iterator got = table.find(s);
    if (got == table.end()) {
        DesignEntity nonExistentType("nonExistentType");
        return nonExistentType;
    } else {
        return got->second;
    }
}

AbstractQuery::AbstractQuery(Synonym synonym, ClauseVector& clauseList):
    selectSynonym(std::move(synonym)), clauses(clauseList), hasError(false)
{}

Synonym AbstractQuery::getSelectSynonym()
{
    return selectSynonym;
}
ClauseVector AbstractQuery::getClauses()
{
    return clauses;
}
DeclarationTable AbstractQuery::getDeclarationTable()
{
    return declarationTable;
}
AbstractQuery::AbstractQuery(): hasError(false){};

void AbstractQuery::setToInvalid()
{
    hasError = true;
}
Boolean AbstractQuery::isInvalid()
{
    return hasError;
}
AbstractQuery AbstractQuery::invalidAbstractQuery()
{
    AbstractQuery aq;
    aq.setToInvalid();
    return aq;
}

Boolean isValidSynonym(String testString)
{
    return util::isPossibleIdentifier(testString);
}
