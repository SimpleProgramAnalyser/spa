#include "AqTypes.h"

Relationship::Relationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef):
    leftReference(leftRef), rightReference(rightRef), hasError(false), relationshipReferenceType(relRefType)
{}

Relationship::Relationship(Boolean hasError): hasError{hasError}, relationshipReferenceType{InvalidRelationshipType} {}

Relationship Relationship::createRelationship(String relationshipRef, Reference leftRef, Reference rightRef)
{
    // Validate relationship reference type
    RelationshipReferenceType relationshipReferenceType = getRelRefType(relationshipRef);
    if (relationshipReferenceType == InvalidRelationshipType) {
        return Relationship(true);
    }

    // Validate semantics for relationships
    if (!validateRelationshipSemantics(relationshipReferenceType, leftRef, rightRef)) {
        return Relationship(true);
    }

    // Split Statement and Prodedure type for Uses and Modifies
    if (relationshipReferenceType == UsesType || relationshipReferenceType == ModifiesType) {
        if (leftRef.isProcedure() || util::isLiteralIdent(leftRef.getValue())) {
            relationshipReferenceType
                = relationshipReferenceType == UsesType ? UsesProcedureType : ModifiesProcedureType;
        } else {
            relationshipReferenceType
                = relationshipReferenceType == UsesType ? UsesStatementType : ModifiesStatementType;
        }
    }

    return Relationship(relationshipReferenceType, leftRef, rightRef);
}

RelationshipReferenceType Relationship::getRelRefType(String relRef)
{
    if (relRef == "Follows") {
        return FollowsType;
    } else if (relRef == "Follows*") {
        return FollowsStarType;
    } else if (relRef == "Parent") {
        return ParentType;
    } else if (relRef == "Parent*") {
        return ParentStarType;
    } else if (relRef == "Uses") {
        return UsesType;
    } else if (relRef == "Modifies") {
        return ModifiesType;
    }

    return InvalidRelationshipType;
}

Boolean Relationship::isInvalid()
{
    return hasError;
}

RelationshipReferenceType Relationship::getRelationship()
{
    return relationshipReferenceType;
}

Reference Relationship::getLeftRef()
{
    return leftReference;
}

Reference Relationship::getRightRef()
{
    return rightReference;
}

// TODO: Consider to abstract into hash table
Boolean Relationship::validateRelationshipSemantics(RelationshipReferenceType relRefType, Reference leftRef,
                                                    Reference rightRef)
{
    if (relRefType == FollowsType || relRefType == FollowsStarType || relRefType == ParentType
        || relRefType == ParentStarType) {
        return Relationship::validateStmtAndStmtSemantics(leftRef, rightRef);
    } else if (relRefType == UsesType || relRefType == ModifiesType) {
        return Relationship::validateStmtProcAndVarSemantics(relRefType, leftRef, rightRef);
    }

    return false;
}

Boolean Relationship::validateStmtAndStmtSemantics(Reference leftRef, Reference rightRef)
{
    Boolean bothAreValidStatementRefs = leftRef.isValidStatementRef() && rightRef.isValidStatementRef();
    Boolean eitherIsNonStatementSynonym = leftRef.isNonStatementSynonym() || rightRef.isNonStatementSynonym();

    if (!bothAreValidStatementRefs || eitherIsNonStatementSynonym) {
        return false;
    }

    return true;
}

Boolean Relationship::validateStmtProcAndVarSemantics(RelationshipReferenceType relRefType, Reference leftRef,
                                                      Reference rightRef)
{
    if (relRefType != ModifiesType && relRefType != UsesType) {
        return false;
    }

    DesignEntityType leftRefDesignEntityType = leftRef.getDesignEntity().getType();
    DesignEntityType rightRefDesignEntityType = rightRef.getDesignEntity().getType();
    ReferenceType leftReferenceType = leftRef.getReferenceType();
    ReferenceType rightReferenceType = rightRef.getReferenceType();
    Boolean isLeftRefStatementOrProcedureType = isStatementDesignEntity(leftRefDesignEntityType)
                                                || leftRefDesignEntityType == ProcedureType
                                                || leftReferenceType == LiteralRefType;
    Boolean isLeftRefTypeInvalid
        = relRefType == UsesType ? leftRefDesignEntityType == ReadType : leftRefDesignEntityType == PrintType;
    Boolean isRightRefVariableType = rightRefDesignEntityType == VariableType || rightReferenceType == LiteralRefType
                                     || rightReferenceType == WildcardRefType;

    if (!isLeftRefStatementOrProcedureType || isLeftRefTypeInvalid || !isRightRefVariableType) {
        return false;
    }

    return true;
}

Boolean Relationship::operator==(const Relationship& relationship)
{
    return this->relationshipReferenceType == relationship.relationshipReferenceType
           && this->leftReference == relationship.leftReference && this->rightReference == relationship.rightReference;
}