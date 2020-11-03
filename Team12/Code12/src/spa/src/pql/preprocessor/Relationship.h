/**
 * Relationships decide how a such that clause is
 * evaluated, and correspond to certain SIMPLE program
 * design abstractions like Parent, Follows, Next.
 */

#ifndef SPA_PQL_PREPROCESSOR_RELATIONSHIP_H
#define SPA_PQL_PREPROCESSOR_RELATIONSHIP_H

#include "AqTypesUtils.h"
#include "Reference.h"

// All the Relationship Types.
enum RelationshipType : char {
    FollowsType,
    FollowsStarType,
    ParentType,
    ParentStarType,
    UsesType, // placeholder type for UsesStatement and UsesProcedure
    UsesStatementType,
    UsesProcedureType,
    ModifiesType, // placeholder type for ModifiesStatement and ModifiesProcedure
    ModifiesStatementType,
    ModifiesProcedureType,
    CallsType,
    CallsStarType,
    AffectsType,
    AffectsStarType,
    NextType,
    NextStarType,
    // branch into procedure (BIP) types
    AffectsBipType,
    AffectsBipStarType,
    NextBipType,
    NextBipStarType,
    InvalidRelationshipType
};

// Hash function for RelationshipType
template <>
struct std::hash<RelationshipType> {
    std::size_t operator()(const RelationshipType& relRefType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(relRefType));
    }
};

class Relationship: public Errorable {
private:
    // Map of string to its corresponding RelationshipType.
    // If string does not have any corresponding match, an
    // InvalidRelationshipType will be returned.
    static std::unordered_map<String, RelationshipType> relationshipTypeMap;

    // Map of the RelationshipType to the valid DesignEntityType of the left Synonym Reference.
    static std::unordered_map<RelationshipType, std::unordered_set<DesignEntityType>>
        leftReferenceSynonymValidationTable;

    // Map of the RelationshipType to the valid DesignEntityType of the right Synonym Reference.
    static std::unordered_map<RelationshipType, std::unordered_set<DesignEntityType>>
        rightReferenceSynonymValidationTable;

    // Map of the RelationshipType to the valid ReferenceType of the left Reference.
    static std::unordered_map<RelationshipType, std::unordered_set<ReferenceType>> leftReferenceTypeValidationTable;

    // Map of the RelationshipType to the valid ReferenceType of the right Reference.
    static std::unordered_map<RelationshipType, std::unordered_set<ReferenceType>> rightReferenceTypeValidationTable;

    RelationshipType relationshipType;
    Reference leftReference;
    Reference rightReference;

    // Checks the validity of the Relationship and the left and right References
    // using the validity maps.
    static Boolean validateRelationshipSemantics(RelationshipType relRefType, const Reference& leftRef,
                                                 const Reference& rightRef);

    // Instantiate a Relationship with the given RelationshipType, and the left and right References.
    Relationship(RelationshipType relRefType, Reference leftRef, Reference rightRef);

public:
    // Validates andd creates a Relationship based given RelationshipType,
    // and the left and right References.
    static Relationship createRelationship(RelationshipType relRefType, Reference leftRef, const Reference& rightRef);

    // Converts the given string into a RelationshipType.
    static RelationshipType getRelRefType(const String& relRef);

    // Instantiates an erroneous Relationship with the given QueryErrorType
    // and ErrorMessage.
    Relationship(QueryErrorType queryErrorType, ErrorMessage errorMessage);

    // Retrieves the RelationshipType of the Relationship.
    RelationshipType getType();

    // Retrieves the left Reference of the Relationship.
    Reference getLeftRef();

    // Retrieves the right Reference of the Relationship.
    Reference getRightRef();

    Boolean operator==(const Relationship& relationship);
};

#endif // SPA_PQL_PREPROCESSOR_RELATIONSHIP_H
