//
// Created by Chester Sim on 4/9/20.
//

#include "AqTypes.h"

#include <iostream>
#include <utility>

DesignEntity::DesignEntity(DesignEntityType designEntityType)
{
    type = designEntityType;
}

DesignEntityType DesignEntity::getType() { return type; }

Clause::Clause(ClauseType clauseType)
{
    type = clauseType;
}

ClauseType Clause::getType() { return type; }

Reference::Reference(ReferenceType refType, ReferenceValue& refValue)
{
    referenceType = refType;
    referenceValue = refValue;
}

ReferenceType Reference::getReferenceType(){ return referenceType; }
ReferenceValue Reference::getValue(){ return referenceValue; }

EntityReference::EntityReference(ReferenceValue& refValue)
    : Reference(ENTITY_REF, refValue)
{}

StatementReference::StatementReference(ReferenceValue& refValue)
    : Reference(STATEMENT_REF, refValue)
{}

Relationship::Relationship(RelationshipReference relationshipRef, Reference leftRef, Reference rightRef)
    : leftReference(leftRef), rightReference(rightRef)
{
    relationshipReference = relationshipRef;
}

RelationshipReference Relationship::getRelationship(){ return relationshipReference; }
Reference Relationship::getLeftRef(){ return leftReference; }
Reference Relationship::getRightRef(){ return rightReference; }

SuchThatClause::SuchThatClause(Relationship& r)
    : Clause(SUCH_THAT_CLAUSE), relationship{ r }
{}

Relationship SuchThatClause::getRelationship(){ return relationship; }

ExpressionSpec::ExpressionSpec(String expr, bool any, bool beforeOrAfter)
{
    expression = expr;
    isAny = any;
    hasBeforeOrAfter = beforeOrAfter;
}

bool ExpressionSpec::checkIsAny(){ return isAny; }
bool ExpressionSpec::checkHasBeforeOrAfter(){ return hasBeforeOrAfter; }
Expression ExpressionSpec::getExpression(){ return expression; }

PatternClause::PatternClause(PatternStatementType statementType, EntityReference entRef, ExpressionSpec exprSpec)
    : Clause(PATTERN_CLAUSE), entityReference(entRef), expressionSpec(exprSpec)
{}

PatternStatementType PatternClause::getStatementType(){ return patternStatementType; }
EntityReference PatternClause::getEntRef(){ return entityReference; }
ExpressionSpec PatternClause::getExprSpec(){ return expressionSpec; }

void DeclarationTable::addDeclaration(Synonym s, DesignEntity& designEntity)
{
    table.insert({ s, designEntity });
}

DesignEntity DeclarationTable::getDesignEntityOfSynonym(Synonym s)
{
    std::unordered_map<Synonym, DesignEntity>::const_iterator got = table.find(s);
    if (got == table.end()) {
        std::cout << "Synonym not found in Declaration Table";
    } else {
        return got->second;
    }
}

AbstractQuery::AbstractQuery(Synonym synonym, ClauseList& clauseList)
{
    selectSynonym = std::move(synonym);
    clauses = clauseList;
}

Synonym AbstractQuery::getSelectSynonym(){ return selectSynonym; }
ClauseList AbstractQuery::getClauses(){ return clauses; }
DeclarationTable AbstractQuery::getDeclarationTable(){ return declarationTable; }
AbstractQuery::AbstractQuery() = default;
