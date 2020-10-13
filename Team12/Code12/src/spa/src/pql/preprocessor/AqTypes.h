/**
 * This class defines an AbstractQuery (and necessary
 * supporting substructures), which is used to represent
 * a PQL query, and passed to the query evaluator, for
 * further processing.
 */

#ifndef SPA_PQL_AQTYPES_H
#define SPA_PQL_AQTYPES_H

#include <assert.h>
#include <cstdint>
#include <unordered_set>
#include <utility>

#include "Types.h"
#include "Util.h"
#include "ast/AstTypes.h"
#include "frontend/parser/Parser.h"
#include "lexer/Lexer.h"

class Error {
private:
    Boolean hasError = false;

public:
    Boolean isInvalid() const
    {
        return hasError;
    }
    void setError(Boolean error)
    {
        hasError = error;
    }
};

enum DesignEntityType : unsigned char {
    // statement types: smallest bits are 01
    StmtType = 1,       // 0000 0001
    ReadType = 5,       // 0000 0101
    PrintType = 9,      // 0000 1001
    CallType = 13,      // 0000 1101
    WhileType = 17,     // 0001 0001
    IfType = 21,        // 0001 0101
    AssignType = 25,    // 0001 1001
    Prog_LineType = 29, // 0001 1101
    // other types: smallest bits are 00
    VariableType = 0,    // 0000 0000
    ConstantType = 4,    // 0000 0100
    ProcedureType = 8,   // 0000 1000
    NonExistentType = 12 // 0000 1100
};

typedef std::unordered_set<DesignEntityType> DesignEntityTypeSet;

class DesignEntity {
private:
    static std::unordered_map<String, DesignEntityType> designEntityMap;

    DesignEntityType type;

public:
    DesignEntity();
    explicit DesignEntity(DesignEntityType designEntityType);
    explicit DesignEntity(const String& stringType);
    DesignEntityType getType();
    Boolean operator==(const DesignEntity& designEntity);
};

// Hash function for DesignEntityType
template <>
struct std::hash<DesignEntityType> {
    std::size_t operator()(const DesignEntityType& de) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(de));
    }
};

typedef String Synonym;

enum AttributeType : char {
    ProcNameType,
    VarNameType,
    ValueType,
    StmtNumberType,
    NoAttributeType,
    InvalidAttributeType
};
enum AttributeValueType : char { IntegerValueType, NameValueType, InvalidValueType };

// Hash function for Attribute
template <>
struct std::hash<AttributeType> {
    std::size_t operator()(const AttributeType& attributeType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(attributeType));
    }
};

class Attribute {
private:
    AttributeType type;

public:
    static std::unordered_map<String, AttributeType> attributeMap;
    static std::unordered_map<AttributeType, DesignEntityTypeSet> attributeDesignEntityMap;

    static Boolean validateDesignEntityAttributeSemantics(DesignEntityType designEntityType,
                                                          AttributeType attributeType);

    Attribute();
    explicit Attribute(AttributeType attributeType);
    explicit Attribute(String attributeTypeString);
    AttributeType getType();
    Boolean operator==(const Attribute& attribute);
    Boolean operator!=(const Attribute& attribute);
};

class ResultSynonym: public Error {
private:
    Synonym synonym;
    Attribute attribute;

    static std::unordered_map<AttributeType, DesignEntityTypeSet> attributeDesignEntityTypeValidationMap;

public:
    explicit ResultSynonym(Boolean hasError);
    explicit ResultSynonym(Synonym syn);
    ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity);
    Synonym getSynonym() const;
    Attribute getAttribute();
    Boolean operator==(const ResultSynonym& resultSynonym);
    Boolean operator!=(const ResultSynonym& resultSynonym);
};

class DeclarationTable: public Error {
private:
    std::unordered_map<Synonym, DesignEntity> table;

public:
    Void addDeclaration(const Synonym& s, DesignEntity& designEntity);
    DesignEntity getDesignEntityOfSynonym(Synonym s) const;
    Boolean hasSynonym(Synonym s);
    static DeclarationTable invalidDeclarationTable();
    Boolean operator==(const DeclarationTable& declarationTable) const;
};

enum ClauseType : char { SuchThatClauseType, PatternClauseType, WithClauseType, NonExistentClauseType };

class Clause {
protected:
    ClauseType type;
    Boolean hasError;

public:
    explicit Clause(ClauseType clauseType);
    static Clause* invalidClause(ClauseType clauseType);
    ClauseType getType();
    Boolean isInvalid();
    // The rest of the methods are required to allow virtual operator==
    Clause(const Clause&) = default;
    Clause operator=(const Clause&) = delete;
    Clause(Clause&&) = default;
    Clause& operator=(Clause&&) = delete;
    virtual ~Clause() = default;
    virtual Boolean operator==(Clause& clause);
};

enum ReferenceType : char {
    SynonymRefType,
    WildcardRefType,
    LiteralRefType,
    IntegerRefType,
    AttributeRefType,
    InvalidRefType
};

typedef std::unordered_set<ReferenceType> ReferenceTypeSet;

typedef String ReferenceValue;

class Reference {
protected:
    ReferenceType referenceType;
    ReferenceValue referenceValue;
    DesignEntity designEntity;
    Attribute attribute;
    Boolean hasError;

public:
    static Reference createReference(String ref, DeclarationTable& declarationTable);

    Reference();
    explicit Reference(Boolean hasError);
    Reference(ReferenceType refType, ReferenceValue refValue);
    Reference(ReferenceType refType, ReferenceValue refValue,
              DesignEntity
                  designEnt); // TODO: Refactor to remove refType parameter (not needed, will always be SynonymRefType)
    Reference(ReferenceValue refValue, DesignEntity designEnt, Attribute attr);
    ReferenceType getReferenceType() const;
    DesignEntity getDesignEntity();
    ReferenceValue getValue() const;
    Attribute getAttribute();
    AttributeValueType getAttributeValueType();
    Boolean isValidEntityRef();
    Boolean isInvalid() const;
    Boolean isProcedure();
    Boolean isWildCard() const;
    Boolean operator==(const Reference& reference);
};

enum RelationshipReferenceType : uint16_t {
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

class Relationship {
private:
    RelationshipReferenceType relationshipReferenceType;
    Reference leftReference;
    Reference rightReference;
    Boolean hasError;
    static std::unordered_map<String, RelationshipReferenceType> relationshipReferenceTypeMap;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<DesignEntityType>>
        leftReferenceSynonymValidationTable;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<DesignEntityType>>
        rightReferenceSynonymValidationTable;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<ReferenceType>>
        leftReferenceTypeValidationTable;
    static std::unordered_map<RelationshipReferenceType, std::unordered_set<ReferenceType>>
        rightReferenceTypeValidationTable;

    static Boolean validateRelationshipSemantics(RelationshipReferenceType relRefType, Reference leftRef,
                                                 Reference rightRef);

public:
    Relationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef);
    explicit Relationship(Boolean hasError);
    RelationshipReferenceType getType();
    Reference getLeftRef();
    Reference getRightRef();
    Boolean isInvalid() const;
    static Relationship createRelationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef);
    static RelationshipReferenceType getRelRefType(String relRef);
    Boolean operator==(const Relationship& relationship);
};

class WithClause: public Clause {
private:
    Reference leftReference;
    Reference rightReference;

public:
    static Clause* createWithClause(const String& clauseConstraint, DeclarationTable& declarationTable);

    WithClause(Reference& leftRef, Reference& rightRef);
    Reference getLeftReference();
    Reference getRightReference();
    Boolean operator==(const WithClause& withClause);
};

class SuchThatClause: public Clause {
private:
    Relationship relationship;

public:
    static Clause* createSuchThatClause(const String& clauseConstraint, DeclarationTable& declarationTable);
    explicit SuchThatClause(Relationship& r);
    Relationship getRelationship();
    Boolean operator==(const SuchThatClause& suchThatClause);
};

enum ExpressionSpecType : char {
    WildcardExpressionType,          // _
    LiteralExpressionType,           // "x + y"
    ExtendableLiteralExpressionType, // _"x + y"_
    NoExpressionType,
    InvalidExpressionType
};

class ExpressionSpec {
private:
    std::unique_ptr<Expression> expression;
    bool hasError;

public:
    ExpressionSpecType expressionSpecType;
    ExpressionSpec();
    explicit ExpressionSpec(Boolean hasError);
    explicit ExpressionSpec(ExpressionSpecType exprSpecType);
    ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType);
    Expression* getExpression() const;
    Boolean isInvalid() const;
    static ExpressionSpec createExpressionSpec(const String& exprSpecString);
    Boolean operator==(const ExpressionSpec& expressionSpec);
};

enum PatternStatementType : char { AssignPatternType, WhilePatternType, IfPatternType };

class PatternClause: public Clause {
private:
    static std::unordered_set<DesignEntityType> designEntityTypeValidationSet;
    static Clause* processAssignPatternClause(Synonym patternSynonym, StringVector constraints,
                                              DeclarationTable& declarationTable);
    static Clause* processIfWhilePatternClause(Synonym patternSynonym, DesignEntityType synonymDesignEntityType,
                                               StringVector constraints, DeclarationTable& declarationTable);
    static Boolean isValidVariableEntityRef(Reference ref);

    Synonym patternSynonym;
    PatternStatementType patternStatementType;
    Reference entityReference;
    ExpressionSpec expressionSpec;

public:
    static Clause* createPatternClause(const String& clauseConstraint, DeclarationTable& declarationTable);

    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef);
    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec);
    PatternStatementType getStatementType() const;
    Reference getEntRef() const;
    const ExpressionSpec& getExprSpec() const;
    Synonym getPatternSynonym() const;
    Boolean operator==(const PatternClause& patternClause);
};

class ClauseVector {
private:
    List<Clause> clauses;
    Boolean hasError;
    explicit ClauseVector(Boolean hasError) noexcept;

public:
    ClauseVector() noexcept;
    ~ClauseVector() = default;
    ClauseVector(const ClauseVector&) = delete;
    ClauseVector operator=(const ClauseVector&) = delete;
    ClauseVector(ClauseVector&&) = default;
    ClauseVector& operator=(ClauseVector&&) = default;

    static ClauseVector invalidClauseVector();
    Void add(Clause* clause);
    Clause* get(Integer index) const;
    Integer count() const;
    Boolean isInvalid() const;
    Boolean operator==(const ClauseVector& clauseVector);
};

class AbstractQuery {
private:
    Vector<ResultSynonym> resultSynonyms; // Empty Vector but valid Abstract Query => Select BOOLEAN
    ClauseVector clauses;
    DeclarationTable declarationTable;
    Boolean hasError;

public:
    explicit AbstractQuery(Boolean hasError);
    AbstractQuery(const Vector<ResultSynonym>& synonym, DeclarationTable& declarations);
    AbstractQuery(const Vector<ResultSynonym>& synonym, DeclarationTable& declarations, ClauseVector& clauseVector);
    static AbstractQuery invalidAbstractQuery();
    Synonym getSelectSynonym() const;
    Vector<ResultSynonym> getSynonyms();
    const ClauseVector& getClauses() const;
    DeclarationTable getDeclarationTable() const;
    Boolean isInvalid() const;
    Boolean operator==(const AbstractQuery& abstractQuery);
    AbstractQuery();
};

// Utils

/**
 * Returns an Expression pointer after constructing the
 * Expression object using the given literal.
 * @param literal   String to be converted into Expression
 * @return          Pointer of the constructed Expression
 */
Expression* createExpression(const String& literal);

Boolean isValidSynonym(String s);

/**
 * Check if the DesignEntityType represents
 * a SIMPLE statement of some sort.
 *
 * @param type DesignEntityType to be checked.
 * @return True, if DesignEntityType is one of
 *         the 6 SIMPLE statement types. False,
 *         if DesignEntityType is other entities
 *         like variable or procedure.
 */
Boolean isStatementDesignEntity(DesignEntityType type);

#endif // SPA_PQL_AQTYPES_H
