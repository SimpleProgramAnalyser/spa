#include "AqTypes.h"

Relationship::Relationship(String relationshipRef, Reference leftRef, Reference rightRef):
    leftReference(leftRef), rightReference(rightRef), hasError(false)
{
    // Validate relationship reference type
    relationshipReference = getRelRefType(relationshipRef);
    if (relationshipReference == InvalidRelationshipType) {
        hasError = true;
        return;
    }

    // Validate semantics for relationships
    if (!validateRelationshipSemantics(relationshipReference, leftRef, rightRef)) {
        hasError = true;
        return;
    }

    if (relationshipReference == UsesType || relationshipReference == ModifiesType) {
        if (leftRef.isProcedure() || util::isLiteralIdent(leftRef.getValue())) {
            relationshipReference = relationshipReference == UsesType ? UsesProcedureType : ModifiesProcedureType;
        } else {
            relationshipReference = relationshipReference == UsesType ? UsesStatementType : ModifiesStatementType;
        }
    }

    leftReference = leftRef;
    rightReference = rightRef;
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

Boolean Relationship::operator==(const Relationship& relationship)
{
    return this->relationshipReference == relationship.relationshipReference
           && this->leftReference == relationship.leftReference && this->rightReference == relationship.rightReference;
}

// TODO: Consider to abstract the validation to their own classes?
Boolean Relationship::validateRelationshipSemantics(RelationshipReferenceType relRefType, Reference leftRef,
                                                    Reference rightRef)
{
    if (relRefType == FollowsType || relRefType == FollowsStarType || relRefType == ParentType
        || relRefType == ParentStarType) {
        return Relationship::validateStmtAndStmtRelationshipSemantics(leftRef, rightRef);
    } else if (relRefType == UsesType || relRefType == ModifiesType) {
        return Relationship::validateUsesAndModifiesSemantics(relRefType, leftRef, rightRef);
    }

    return false;
}

Boolean Relationship::validateStmtAndStmtRelationshipSemantics(Reference leftRef, Reference rightRef)
{
    Boolean bothAreValidStatementRefs = leftRef.isValidStatementRef() && rightRef.isValidStatementRef();
    Boolean eitherIsNonStatementSynonym = leftRef.isNonStatementSynonym() || rightRef.isNonStatementSynonym();

    if (!bothAreValidStatementRefs || eitherIsNonStatementSynonym) {
        return false;
    }

    return true;
}

Boolean Relationship::validateUsesAndModifiesSemantics(RelationshipReferenceType relRefType, Reference leftRef,
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