#ifndef SPA_RELATIONSHIPREFERENCETYPE_H
#define SPA_RELATIONSHIPREFERENCETYPE_H

#include "AqTypesUtils.h"
#include "Reference.h"

enum RelationshipReferenceType : char {
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
    NextType,
    NextStarType,
    AffectsType,
    AffectsStarType,
    InvalidRelationshipType
};

// Hash function for RelationshipReferenceType
template <>
struct std::hash<RelationshipReferenceType> {
    std::size_t operator()(const RelationshipReferenceType& relRefType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(relRefType));
    }
};

class Relationship: public Errorable {
private:
    static std::unordered_map<String, RelationshipReferenceType> relationshipReferenceTypeMap;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<DesignEntityType>>
        leftReferenceSynonymValidationTable;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<DesignEntityType>>
        rightReferenceSynonymValidationTable;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<ReferenceType>>
        leftReferenceTypeValidationTable;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<ReferenceType>>
        rightReferenceTypeValidationTable;

    RelationshipReferenceType relationshipReferenceType;
    Reference leftReference;
    Reference rightReference;

    static Boolean validateRelationshipSemantics(RelationshipReferenceType relRefType, Reference leftRef,
                                                 Reference rightRef);

public:
    static Relationship createRelationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef);
    static RelationshipReferenceType getRelRefType(String relRef);

    Relationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef);
    Relationship(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    RelationshipReferenceType getType();
    Reference getLeftRef();
    Reference getRightRef();
    Boolean operator==(const Relationship& relationship);
};

#endif // SPA_RELATIONSHIPREFERENCETYPE_H
