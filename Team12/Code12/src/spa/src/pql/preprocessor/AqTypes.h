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

typedef String Synonym;

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

enum Attribute : char { ProcNameType, VarNameType, ValueType, StmtNumberType, NoAttribute, InvalidAttribute };

class ResultSynonym: public Error {
private:
    Synonym synonym;
    Attribute attribute;

    static std::unordered_map<String, Attribute> attributeMap;

public:
    explicit ResultSynonym(Boolean hasError);
    explicit ResultSynonym(Synonym syn);
    ResultSynonym(Synonym syn, const String& attr);
    Synonym getSynonym() const;
    Attribute getAttribute();
    Boolean operator==(const ResultSynonym& resultSynonym);
    Boolean operator!=(const ResultSynonym& resultSynonym);
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
    DesignEntityType type;

public:
    DesignEntity();
    explicit DesignEntity(DesignEntityType designEntityType);
    explicit DesignEntity(const String& stringType);
    DesignEntityType getType();
    Boolean operator==(const DesignEntity& designEntity);
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

enum ClauseType : char { SuchThatClauseType = 0, PatternClauseType = 1, NonExistentClauseType = 2 };

class Clause {
protected:
    ClauseType type;
    Boolean hasError;

public:
    explicit Clause(ClauseType clauseType);
    static Clause* invalidClause(ClauseType clauseType);
    ClauseType getType();
    Boolean isInvalid();
    Clause(const Clause&) = default;
    Clause operator=(const Clause&) = delete;
    Clause(Clause&&) = default;
    Clause& operator=(Clause&&) = delete;
    virtual ~Clause() = default;
    virtual Boolean operator==(Clause& clause);
};

enum ReferenceType : char {
    SynonymRefType = 0,
    WildcardRefType = 1,
    LiteralRefType = 2,
    IntegerRefType = 4,
    InvalidRefType = 8
};

typedef std::unordered_set<ReferenceType> ReferenceTypeSet;

typedef String ReferenceValue;

class Reference {
protected:
    ReferenceType referenceType;
    ReferenceValue referenceValue;
    DesignEntity designEntity;
    Boolean hasError;

public:
    explicit Reference(Boolean hasError);
    Reference(ReferenceType refType, ReferenceValue refValue);
    Reference(ReferenceType refType, ReferenceValue refValue, DesignEntity designEnt);
    ReferenceType getReferenceType() const;
    DesignEntity getDesignEntity();
    ReferenceValue getValue() const;
    Boolean isValidEntityRef();
    Boolean isValidStatementRef();
    Boolean isInvalid() const;
    Boolean isProcedure();
    Boolean isWildCard() const;
    Boolean isNonStatementSynonym();
    static Reference createReference(String ref, DeclarationTable& declarationTable);
    Boolean operator==(const Reference& reference);
    Reference();
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
    RelationshipReferenceType getRelationship();
    Reference getLeftRef();
    Reference getRightRef();
    Boolean isInvalid() const;
    static Relationship createRelationship(RelationshipReferenceType relRefType, Reference leftRef, Reference rightRef);
    static RelationshipReferenceType getRelRefType(String relRef);
    Boolean operator==(const Relationship& relationship);
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
    WildcardExpressionType = 0,          // _
    LiteralExpressionType = 1,           // "x + y"
    ExtendableLiteralExpressionType = 2, // _"x + y"_
    InvalidExpressionType = 4
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

enum PatternStatementType : char {
    AssignPatternType = 0,
    WhilePatternType = 1, // Advanced SPA
    IfPatternType = 2     // Advanced SPA
};

class PatternClause: public Clause {
private:
    Synonym patternSynonym;
    PatternStatementType patternStatementType;
    Reference entityReference;
    ExpressionSpec expressionSpec;

public:
    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec);
    PatternStatementType getStatementType() const;
    Reference getEntRef() const;
    const ExpressionSpec& getExprSpec() const;
    Synonym getPatternSynonym() const;
    static Clause* createPatternClause(String clauseConstraint, DeclarationTable& declarationTable);
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
    //    Synonym selectSynonym;
    Vector<ResultSynonym> resultSynonyms;
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
