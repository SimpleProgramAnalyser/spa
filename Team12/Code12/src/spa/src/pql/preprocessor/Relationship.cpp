//
// Created by Chester Sim on 12/9/20.
//

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

    // Validate semantics for Follows, Follows*, Parent, Parent*
    if (relationshipReference == FollowsType || relationshipReference == FollowsStarType
        || relationshipReference == ParentType || relationshipReference == ParentStarType) {
        Boolean bothAreValidStatementRefs = leftRef.isValidStatementRef() && rightRef.isValidStatementRef();
        Boolean leftRefIsProcedure = leftRef.isProcedure();

        if (!bothAreValidStatementRefs || leftRefIsProcedure) {
            hasError = true;
            return;
        }
    }

    if (relationshipReference == UsesType || relationshipReference == ModifiesType) {
        if (leftRef.isWildCard() || !rightRef.isValidEntityRef()) {
            hasError = true;
            return;
        }

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
