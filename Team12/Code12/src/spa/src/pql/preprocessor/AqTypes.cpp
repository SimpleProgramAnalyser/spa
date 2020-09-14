/**
 * Implementation of the AbstractQuery, and
 * supporting substructures and their public methods.
 */

#include "AqTypes.h"

// DesignEntity::DesignEntity(DesignEntityType designEntityType)
//{
//    type = designEntityType;
//}
//
// DesignEntity::DesignEntity(const String& stringType)
//{
//    /*
//     * Suggestion: Declare some variables on top, to hold
//     * these values, don't use 'magic' values.
//     */
//    if (stringType == "stmt") {
//        type = StmtType;
//    } else if (stringType == "read") {
//        type = ReadType;
//    } else if (stringType == "print") {
//        type = PrintType;
//    } else if (stringType == "call") {
//        type = CallType;
//    } else if (stringType == "while") {
//        type = WhileType;
//    } else if (stringType == "if") {
//        type = IfType;
//    } else if (stringType == "assign") {
//        type = AssignType;
//    } else if (stringType == "variable") {
//        type = VariableType;
//    } else if (stringType == "constant") {
//        type = ConstantType;
//    } else if (stringType == "procedure") {
//        type = ProcedureType;
//    } else {
//        type = NonExistentType;
//    }
//}
//
// DesignEntityType DesignEntity::getType()
//{
//    return type;
//}
//
// Clause::Clause(ClauseType clauseType): hasError{false}
//{
//    type = clauseType;
//}
//
// ClauseType Clause::getType()
//{
//    return type;
//}
//
// Boolean Clause::isInvalid()
//{
//    return hasError;
//}
//
// Clause* Clause::invalidClause(ClauseType clauseType)
//{
//    auto* c = new Clause(clauseType);
//    (*c).hasError = true;
//    return c;
//}

// Reference::Reference(ReferenceType refType, ReferenceValue refValue):
//    referenceType{refType}, referenceValue{refValue}, isProcedureType{false}
//{}
//
// Reference::Reference(ReferenceType refType, ReferenceValue refValue, Boolean isProc):
//    referenceType{refType}, referenceValue{refValue}, isProcedureType{isProc}
//{}
//
// Reference Reference::invalidReference()
//{
//    Reference r(InvalidRefType, "");
//    return r;
//}
//
// Boolean Reference::isInvalid()
//{
//    return referenceType == InvalidRefType;
//}
//
// Boolean Reference::isValidEntityRef()
//{
//    return util::isLiteralIdent(referenceValue) || util::isPossibleIdentifier(referenceValue) || referenceValue ==
//    "_";
//}
//
// Boolean Reference::isValidStatementRef()
//{
//    return util::isPossibleConstant(referenceValue) || util::isPossibleIdentifier(referenceValue)
//           || referenceValue == "_";
//}
//
// Boolean Reference::isWildCard()
//{
//    return referenceValue == "_";
//}
//
// Boolean Reference::isProcedure()
//{
//    return isProcedureType;
//}
//
// ReferenceType Reference::getReferenceType()
//{
//    return referenceType;
//}
//
// ReferenceValue Reference::getValue()
//{
//    return referenceValue;
//}

// Relationship::Relationship(String relationshipRef, Reference leftRef, Reference rightRef):
//    leftReference(leftRef), rightReference(rightRef), hasError(false)
//{
//    // Validate relationship reference type
//    relationshipReference = getRelRefType(relationshipRef);
//    if (relationshipReference == InvalidRelationshipType) {
//        hasError = true;
//        return;
//    }
//
//    // Validate semantics for Follows, Follows*, Parent, Parent*
//    if (relationshipReference == FollowsType || relationshipReference == FollowsStarType
//        || relationshipReference == ParentType || relationshipReference == ParentStarType) {
//        Boolean bothAreValidStatementRefs = leftRef.isValidStatementRef() && rightRef.isValidStatementRef();
//        Boolean leftRefIsProcedure = leftRef.isProcedure();
//
//        if (!bothAreValidStatementRefs || leftRefIsProcedure) {
//            hasError = true;
//            return;
//        }
//    }
//
//    if (relationshipReference == UsesType || relationshipReference == ModifiesType) {
//        if (leftRef.isWildCard() || !rightRef.isValidEntityRef()) {
//            hasError = true;
//            return;
//        }
//
//        if (leftRef.isProcedure() || util::isLiteralIdent(leftRef.getValue())) {
//            relationshipReference = relationshipReference == UsesType ? UsesProcedureType : ModifiesProcedureType;
//        } else {
//            relationshipReference = relationshipReference == UsesType ? UsesStatementType : ModifiesStatementType;
//        }
//    }
//
//    leftReference = leftRef;
//    rightReference = rightRef;
//}
//
// RelationshipReferenceType Relationship::getRelRefType(String relRef)
//{
//    if (relRef == "Follows") {
//        return FollowsType;
//    } else if (relRef == "Follows*") {
//        return FollowsStarType;
//    } else if (relRef == "Parent") {
//        return ParentType;
//    } else if (relRef == "Parent*") {
//        return ParentStarType;
//    } else if (relRef == "Uses") {
//        return UsesType;
//    } else if (relRef == "Modifies") {
//        return ModifiesType;
//    }
//
//    return InvalidRelationshipType;
//}
//
// Boolean Relationship::isInvalid()
//{
//    return hasError;
//}
//
// RelationshipReferenceType Relationship::getRelationship()
//{
//    return relationshipReference;
//}
//
// Reference Relationship::getLeftRef()
//{
//    return leftReference;
//}
//
// Reference Relationship::getRightRef()
//{
//    return rightReference;
//}
//
// SuchThatClause::SuchThatClause(Relationship& r): Clause(SuchThatClauseType), relationship{r} {}
//
// Relationship SuchThatClause::getRelationship()
//{
//    return relationship;
//}

// ExpressionSpec::ExpressionSpec(): hasError{false}
//{
//    expressionSpecType = InvalidExpressionType;
//    Expression* expressionPtr = nullptr;
//    expression = expressionPtr;
//}
//
// ExpressionSpec::ExpressionSpec(ExpressionSpecType exprSpecType): expressionSpecType{exprSpecType}, hasError{false}
//{
//    Expression* expressionPtr = nullptr;
//    expression = expressionPtr;
//}
//
// ExpressionSpec::ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType):
//    expression(expr), expressionSpecType{exprSpecType}, hasError{false}
//{}
//
// Expression* ExpressionSpec::getExpression()
//{
//    return expression;
//}
//
// Boolean ExpressionSpec::isInvalid()
//{
//    return hasError;
//}
//
// ExpressionSpec ExpressionSpec::invalidExpressionSpec()
//{
//    ExpressionSpec* eS = new ExpressionSpec();
//    eS->hasError = true;
//    return *eS;
//}

// PatternClause::PatternClause(PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec):
//    Clause(PatternClauseType), patternStatementType(statementType), entityReference(entRef), expressionSpec(exprSpec)
//{}
//
// PatternStatementType PatternClause::getStatementType()
//{
//    return patternStatementType;
//}
//
// Reference PatternClause::getEntRef()
//{
//    return entityReference;
//}
//
// ExpressionSpec PatternClause::getExprSpec()
//{
//    return expressionSpec;
//}

// Void DeclarationTable::addDeclaration(Synonym s, DesignEntity& designEntity)
//{
//    table.insert({s, designEntity});
//}
//
// Boolean DeclarationTable::isInvalid()
//{
//    return hasError;
//}
// Boolean DeclarationTable::hasSynonym(Synonym s)
//{
//    std::unordered_map<Synonym, DesignEntity>::const_iterator got = table.find(s);
//    if (got == table.end()) {
//        return false;
//    }
//
//    return true;
//}
//
// DesignEntity DeclarationTable::getDesignEntityOfSynonym(Synonym s)
//{
//    std::unordered_map<Synonym, DesignEntity>::const_iterator got = table.find(s);
//    if (got == table.end()) {
//        DesignEntity nonExistentType("NonExistentType");
//        return nonExistentType;
//    } else {
//        return got->second;
//    }
//}
//
// DeclarationTable DeclarationTable::invalidDeclarationTable()
//{
//    DeclarationTable dT;
//    dT.hasError = true;
//    return dT;
//}

// ClauseVector::ClauseVector() noexcept: hasError{false} {}
//
// ClauseVector ClauseVector::invalidClauseVector()
//{
//    ClauseVector* cV = new ClauseVector();
//    (*cV).hasError = true;
//    return *cV;
//}
//
// Void ClauseVector::add(Clause* clause)
//{
//    clauses.push_back(clause);
//}
//
// Clause* ClauseVector::get(Integer index)
//{
//    return clauses.at(index);
//}
//
// Integer ClauseVector::totalNumClauses()
//{
//    return clauses.size();
//}
//
// Boolean ClauseVector::isInvalid()
//{
//    return hasError;
//}
//
// AbstractQuery::AbstractQuery(Synonym synonym, DeclarationTable& declarations, ClauseVector& clauseVector):
//    selectSynonym(std::move(synonym)), clauses(clauseVector), declarationTable(declarations), hasError(false)
//{}
//
// Synonym AbstractQuery::getSelectSynonym()
//{
//    return selectSynonym;
//}
// ClauseVector AbstractQuery::getClauses()
//{
//    return clauses;
//}
// DeclarationTable AbstractQuery::getDeclarationTable()
//{
//    return declarationTable;
//}
//
// Boolean AbstractQuery::isInvalid()
//{
//    return hasError;
//}
//
// AbstractQuery::AbstractQuery():
//    hasError{false}, selectSynonym{""}, clauses{*(new ClauseVector())}, declarationTable{*(new DeclarationTable())}
//{}
//
// AbstractQuery AbstractQuery::invalidAbstractQuery()
//{
//    AbstractQuery* aq = new AbstractQuery();
//    (*aq).hasError = true;
//    return *aq;
//}

// Utils

Boolean isValidSynonym(String testString)
{
    return util::isPossibleIdentifier(testString);
}

Boolean isStatementDesignEntity(DesignEntityType type)
{
    return static_cast<uint8_t>(type << 6u) == 0x40;
}
