#include "AqTypes.h"

template <typename T>
Boolean isValidInTable(std::unordered_map<RelationshipReferenceType, std::unordered_set<T>> table,
                       RelationshipReferenceType relRefType, T type);

std::unordered_map<String, RelationshipReferenceType> Relationship::relationshipReferenceTypeMap{
    {"Follows", FollowsType}, {"Follows*", FollowsStarType}, {"Parent", ParentType}, {"Parent*", ParentStarType},
    {"Uses", UsesType},       {"Modifies", ModifiesType}};

// Hash function for RelationshipReferenceType
template <>
struct std::hash<RelationshipReferenceType> {
    std::size_t operator()(const RelationshipReferenceType& pv) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(pv));
    }
};

// Hash function for DesignEntityType
template <>
struct std::hash<DesignEntityType> {
    std::size_t operator()(const DesignEntityType& pv) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(pv));
    }
};

// Hash function for ReferenceType
template <>
struct std::hash<ReferenceType> {
    std::size_t operator()(const ReferenceType& pv) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(pv));
    }
};

std::unordered_map<RelationshipReferenceType, DesignEntityTypeSet> Relationship::leftReferenceSynonymValidationTable{
    {FollowsType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {FollowsStarType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {ParentType, DesignEntityTypeSet{StmtType, WhileType, IfType, Prog_LineType}},
    {ParentStarType, DesignEntityTypeSet{StmtType, WhileType, IfType, Prog_LineType}},
    {UsesType, DesignEntityTypeSet{StmtType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {ModifiesType, DesignEntityTypeSet{StmtType, ReadType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
};

std::unordered_map<RelationshipReferenceType, DesignEntityTypeSet> Relationship::rightReferenceSynonymValidationTable{
    {FollowsType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {FollowsStarType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {ParentType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {ParentStarType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {UsesType, DesignEntityTypeSet{VariableType}},
    {ModifiesType, DesignEntityTypeSet{VariableType}},
};

std::unordered_map<RelationshipReferenceType, ReferenceTypeSet> Relationship::leftReferenceTypeValidationTable{
    {FollowsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {FollowsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {UsesType, ReferenceTypeSet{SynonymRefType, LiteralRefType, IntegerRefType}},
    {ModifiesType, ReferenceTypeSet{SynonymRefType, LiteralRefType, IntegerRefType}},
};

std::unordered_map<RelationshipReferenceType, ReferenceTypeSet> Relationship::rightReferenceTypeValidationTable{
    {FollowsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {FollowsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {UsesType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {ModifiesType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
};

Relationship::Relationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef):
    relationshipReferenceType(relRefType), leftReference(leftRef), rightReference(rightRef), hasError(false)
{}

Relationship::Relationship(Boolean hasError): relationshipReferenceType{InvalidRelationshipType}, hasError{hasError} {}

Relationship Relationship::createRelationship(String relationshipRef, Reference leftRef, Reference rightRef)
{
    // Validate relationship reference type
    RelationshipReferenceType relationshipReferenceType = getRelRefType(std::move(relationshipRef));
    if (relationshipReferenceType == InvalidRelationshipType) {
        return Relationship(true);
    }

    // Validate semantics for relationships
    if (!validateRelationshipSemantics(relationshipReferenceType, leftRef, rightRef)) {
        return Relationship(true);
    }

    // Split Statement and Procedure type for Uses and Modifies
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
    std::unordered_map<String, RelationshipReferenceType>::const_iterator got
        = relationshipReferenceTypeMap.find(relRef);

    if (got == relationshipReferenceTypeMap.end()) {
        return InvalidRelationshipType;
    }

    return got->second;
}

Boolean Relationship::isInvalid() const
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


Boolean Relationship::validateRelationshipSemantics(RelationshipReferenceType relRefType, Reference leftRef,
                                                    Reference rightRef)
{
    assert(relRefType != InvalidRelationshipType);

    ReferenceType leftReferenceType = leftRef.getReferenceType();
    Boolean isValidLeftReferenceType = isValidInTable(leftReferenceTypeValidationTable, relRefType, leftReferenceType);
    if (!isValidLeftReferenceType) {
        return false;
    }

    Boolean isLeftSynonymRefType = leftReferenceType == SynonymRefType;
    if (isLeftSynonymRefType) {
        DesignEntityType leftDesignEntityType = leftRef.getDesignEntity().getType();
        Boolean isValidLeftReferenceSynonym
            = isValidInTable(leftReferenceSynonymValidationTable, relRefType, leftDesignEntityType);
        if (!isValidLeftReferenceSynonym) {
            return false;
        }
    }

    ReferenceType rightReferenceType = rightRef.getReferenceType();
    Boolean isValidRightReferenceType
        = isValidInTable(rightReferenceTypeValidationTable, relRefType, rightReferenceType);
    if (!isValidRightReferenceType) {
        return false;
    }

    Boolean isRightSynonymRefType = rightReferenceType == SynonymRefType;
    if (isRightSynonymRefType) {
        DesignEntityType rightDesignEntityType = rightRef.getDesignEntity().getType();
        Boolean isValidRightReferenceSynonym
            = isValidInTable(rightReferenceSynonymValidationTable, relRefType, rightDesignEntityType);
        if (!isValidRightReferenceSynonym) {
            return false;
        }
    }

    return true;
}

template <typename T>
Boolean isValidInTable(std::unordered_map<RelationshipReferenceType, std::unordered_set<T>> table,
                       RelationshipReferenceType relRefType, T type)
{
    auto validationSet = table.find(relRefType)->second;
    Boolean isValid = validationSet.find(type) != validationSet.end();

    return isValid;
}

Boolean Relationship::operator==(const Relationship& relationship)
{
    return this->relationshipReferenceType == relationship.relationshipReferenceType
           && this->leftReference == relationship.leftReference && this->rightReference == relationship.rightReference;
}