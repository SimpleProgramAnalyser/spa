#include "Relationship.h"

template <typename T>
Boolean isValidInTable(std::unordered_map<RelationshipReferenceType, std::unordered_set<T>> table,
                       RelationshipReferenceType relRefType, T type);

std::unordered_map<String, RelationshipReferenceType> Relationship::relationshipReferenceTypeMap{
    {"Follows", FollowsType}, {"Follows*", FollowsStarType}, {"Parent", ParentType},   {"Parent*", ParentStarType},
    {"Uses", UsesType},       {"Modifies", ModifiesType},    {"Calls", CallsType},     {"Calls*", CallsStarType},
    {"Next", NextType},       {"Next*", NextStarType},       {"Affects", AffectsType}, {"Affects*", AffectsStarType}};

// Hash function for ReferenceType
template <>
struct std::hash<ReferenceType> {
    std::size_t operator()(const ReferenceType& rt) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(rt));
    }
};

std::unordered_map<RelationshipReferenceType, ReferenceTypeSet> Relationship::leftReferenceTypeValidationTable{
    {FollowsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {FollowsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {UsesType, ReferenceTypeSet{SynonymRefType, LiteralRefType, IntegerRefType}},
    {UsesStatementType, ReferenceTypeSet{SynonymRefType, IntegerRefType}},
    {UsesProcedureType, ReferenceTypeSet{SynonymRefType, LiteralRefType}},
    {ModifiesType, ReferenceTypeSet{SynonymRefType, LiteralRefType, IntegerRefType}},
    {ModifiesStatementType, ReferenceTypeSet{SynonymRefType, IntegerRefType}},
    {ModifiesProcedureType, ReferenceTypeSet{SynonymRefType, LiteralRefType}},
    {CallsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {CallsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {NextType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {NextStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {AffectsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {AffectsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}}};

std::unordered_map<RelationshipReferenceType, ReferenceTypeSet> Relationship::rightReferenceTypeValidationTable{
    {FollowsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {FollowsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {ParentStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {UsesType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {UsesStatementType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {UsesProcedureType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {ModifiesType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {ModifiesStatementType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {ModifiesProcedureType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {CallsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {CallsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, LiteralRefType}},
    {NextType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {NextStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {AffectsType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}},
    {AffectsStarType, ReferenceTypeSet{SynonymRefType, WildcardRefType, IntegerRefType}}};

std::unordered_map<RelationshipReferenceType, DesignEntityTypeSet> Relationship::leftReferenceSynonymValidationTable{
    {FollowsType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {FollowsStarType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {ParentType, DesignEntityTypeSet{StmtType, WhileType, IfType, Prog_LineType}},
    {ParentStarType, DesignEntityTypeSet{StmtType, WhileType, IfType, Prog_LineType}},
    {UsesType,
     DesignEntityTypeSet{StmtType, PrintType, CallType, WhileType, IfType, AssignType, ProcedureType, Prog_LineType}},
    {UsesStatementType,
     DesignEntityTypeSet{StmtType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {UsesProcedureType, DesignEntityTypeSet{ProcedureType}},
    {ModifiesType,
     DesignEntityTypeSet{StmtType, ReadType, CallType, WhileType, IfType, AssignType, ProcedureType, Prog_LineType}},
    {ModifiesStatementType,
     DesignEntityTypeSet{StmtType, ReadType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {ModifiesProcedureType, DesignEntityTypeSet{ProcedureType}},
    {CallsType, DesignEntityTypeSet{ProcedureType}},
    {CallsStarType, DesignEntityTypeSet{ProcedureType}},
    {NextType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {NextStarType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {AffectsType, DesignEntityTypeSet{StmtType, AssignType, Prog_LineType}},
    {AffectsStarType, DesignEntityTypeSet{StmtType, AssignType, Prog_LineType}}};

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
    {CallsType, DesignEntityTypeSet{ProcedureType}},
    {CallsStarType, DesignEntityTypeSet{ProcedureType}},
    {NextType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {NextStarType,
     DesignEntityTypeSet{StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType, Prog_LineType}},
    {AffectsType, DesignEntityTypeSet{StmtType, AssignType, Prog_LineType}},
    {AffectsStarType, DesignEntityTypeSet{StmtType, AssignType, Prog_LineType}}};

/************************/
/** Constructors        */
/************************/

Relationship::Relationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef):
    relationshipReferenceType(relRefType), leftReference(leftRef), rightReference(rightRef)
{}

Relationship::Relationship(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    relationshipReferenceType{InvalidRelationshipType}
{
    this->setError(queryErrorType, errorMessage);
}

/************************/
/** Instance Methods    */
/************************/

Relationship Relationship::createRelationship(RelationshipReferenceType relRefType, Reference leftRef,
                                              Reference rightRef)
{

    assert( // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        leftReferenceTypeValidationTable.find(relRefType) != leftReferenceTypeValidationTable.end());

    // Validate semantics for relationships
    if (!validateRelationshipSemantics(relRefType, leftRef, rightRef)) {
        return Relationship(QuerySemanticsError, "Invalid Relationship");
    }

    // Split Statement and Procedure type for Uses and Modifies
    if (relRefType == UsesType || relRefType == ModifiesType) {
        if (leftRef.isProcedure() || util::isLiteralIdent(leftRef.getValue())) {
            relRefType = relRefType == UsesType ? UsesProcedureType : ModifiesProcedureType;
        } else {
            relRefType = relRefType == UsesType ? UsesStatementType : ModifiesStatementType;
        }
    }

    return Relationship(relRefType, leftRef, rightRef);
}

Boolean Relationship::validateRelationshipSemantics(RelationshipReferenceType relRefType, Reference leftRef,
                                                    Reference rightRef)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
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

/************************/
/** Instance Methods    */
/************************/

RelationshipReferenceType Relationship::getRelRefType(String relRef)
{
    auto got = relationshipReferenceTypeMap.find(relRef);

    if (got == relationshipReferenceTypeMap.end()) {
        return InvalidRelationshipType;
    }

    return got->second;
}

RelationshipReferenceType Relationship::getType()
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

Boolean Relationship::operator==(const Relationship& relationship)
{
    return this->relationshipReferenceType == relationship.relationshipReferenceType
           && this->leftReference == relationship.leftReference && this->rightReference == relationship.rightReference;
}

template <typename T>
Boolean isValidInTable(std::unordered_map<RelationshipReferenceType, std::unordered_set<T>> table,
                       RelationshipReferenceType relRefType, T type)
{
    auto validationSet = table.find(relRefType)->second;
    Boolean isValid = validationSet.find(type) != validationSet.end();

    return isValid;
}
