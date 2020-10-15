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

#include "QueryError.h"
#include "Types.h"
#include "Util.h"
#include "ast/AstTypes.h"
#include "frontend/parser/Parser.h"
#include "lexer/Lexer.h"

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
    static const ErrorMessage INVALID_DESIGN_ENTITY;

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

class ResultSynonym: public QueryError {
private:
    static std::unordered_map<AttributeType, DesignEntityTypeSet> attributeDesignEntityTypeValidationMap;

    Synonym synonym;
    Attribute attribute;

public:
    static const ErrorMessage INVALID_SYNONYM_MESSAGE;

    explicit ResultSynonym(QueryErrorType queryErrorType);
    explicit ResultSynonym(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    explicit ResultSynonym(Synonym syn);
    ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity);
    Synonym getSynonym() const;
    Attribute getAttribute() const;
    Boolean operator==(const ResultSynonym& resultSynonym);
    Boolean operator!=(const ResultSynonym& resultSynonym);
};

class ResultSynonymVector: public QueryError {
private:
    Vector<ResultSynonym> resultSynonyms; // Empty Vector but valid AbstractQuery => Select BOOLEAN

public:
    ResultSynonymVector() = default;
    explicit ResultSynonymVector(ResultSynonym synonym);
    ResultSynonymVector(Vector<ResultSynonym> synonyms);
    ResultSynonymVector(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    Void add(ResultSynonym resultSynonym);
    Vector<ResultSynonym> getSynonyms();
    Boolean isSelectBoolean();
    Boolean operator==(const ResultSynonymVector& resultSynonymVector);
};

class DeclarationTable: public QueryError {
private:
    std::unordered_map<Synonym, DesignEntity> table;

public:
    static const String INVALID_DECLARATION_SYNTAX;

    DeclarationTable();
    DeclarationTable(QueryErrorType queryErrorType, String errorMessage);
    Void addDeclaration(const Synonym& s, DesignEntity& designEntity);
    DesignEntity getDesignEntityOfSynonym(Synonym s) const;
    Boolean hasSynonym(Synonym s);
    Boolean operator==(const DeclarationTable& declarationTable) const;
};

enum ClauseType : char { SuchThatClauseType, PatternClauseType, WithClauseType, NonExistentClauseType };

class Clause: public QueryError {
protected:
    ClauseType type;

public:
    explicit Clause(ClauseType clauseType);
    Clause(ClauseType clauseType, QueryErrorType queryErrorType, ErrorMessage errorMessage);

    ClauseType getType();
    // The rest of the methods are required to allow virtual operator==
    Clause(const Clause&) = default;
    Clause operator=(const Clause&) = delete;
    Clause(Clause&&) = default;
    Clause& operator=(Clause&&) = delete;
    virtual ~Clause() = default;
    virtual Boolean operator==(Clause& clause);
};

// Hash function for ClauseType
template <>
struct std::hash<ClauseType> {
    std::size_t operator()(const ClauseType& clauseType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(clauseType));
    }
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

class Reference: public QueryError {
protected:
    ReferenceType referenceType;
    ReferenceValue referenceValue;
    DesignEntity designEntity;
    Attribute attribute;

public:
    static Reference createReference(String ref, DeclarationTable& declarationTable);

    Reference();
    Reference(QueryErrorType queryErrorType, ErrorMessage errorMessage);
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
    Boolean isProcedure();
    Boolean isWildCard() const;
    Boolean operator==(const Reference& reference);
};

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

class Relationship: public QueryError {
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

class ExpressionSpec: public QueryError {
private:
    std::unique_ptr<Expression> expression;

public:
    ExpressionSpecType expressionSpecType;
    ExpressionSpec();
    ExpressionSpec(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    explicit ExpressionSpec(ExpressionSpecType exprSpecType);
    ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType);
    Expression* getExpression() const;
    static ExpressionSpec createExpressionSpec(const String& exprSpecString);
    Boolean operator==(const ExpressionSpec& expressionSpec);
};

enum PatternStatementType : char { AssignPatternType, WhilePatternType, IfPatternType };

// Hash function for RelationshipReferenceType
template <>
struct std::hash<PatternStatementType> {
    std::size_t operator()(const PatternStatementType& patternStatementType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(patternStatementType));
    }
};

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

class ClauseVector: public QueryError {
private:
    List<Clause> clauses;

public:
    ClauseVector();
    ClauseVector(QueryErrorType queryErrorType, ErrorMessage);
    Void add(Clause* clause);
    Clause* get(Integer index) const;
    Integer count() const;
    Boolean operator==(const ClauseVector& clauseVector);
    ~ClauseVector() = default;
    ClauseVector(const ClauseVector&) = delete;
    ClauseVector operator=(const ClauseVector&) = delete;
    ClauseVector(ClauseVector&&) = default;
    ClauseVector& operator=(ClauseVector&&) = default;
};

class AbstractQuery: public QueryError {
private:
    ResultSynonymVector resultSynonyms; // Empty Vector but valid AbstractQuery => Select BOOLEAN
    ClauseVector clauses;
    DeclarationTable declarationTable;
    Boolean isToReturnFalseResult = false;

public:
    AbstractQuery();
    AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage, Boolean returnFalseResult);
    AbstractQuery(ResultSynonymVector synonym, DeclarationTable& declarations);
    AbstractQuery(ResultSynonymVector synonym, DeclarationTable& declarations, ClauseVector& clauseVector);
    Vector<ResultSynonym> getSelectSynonym();
    Vector<ResultSynonym> getSynonyms();
    const ClauseVector& getClauses() const;
    DeclarationTable getDeclarationTable() const;
    Boolean toReturnFalseResult();
    Boolean operator==(const AbstractQuery& abstractQuery);
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
