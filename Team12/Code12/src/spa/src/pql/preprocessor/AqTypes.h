/**
 * This class defines an AbstractQuery (and necessary
 * supporting substructures), which is used to represent
 * a PQL query, and passed to the query evaluator, for
 * further processing.
 */

#ifndef SPA_PQL_AQTYPES_H
#define SPA_PQL_AQTYPES_H

#include <cstdint>
#include <utility>

#include "Types.h"
#include "Util.h"
#include "ast/AstTypes.h"

typedef std::pair<String, String> StringPair;
typedef String Synonym;

enum DesignEntityType : uint8_t {
    // statement types: smallest bits are 01
    StmtType = 1,    // 0000 0001
    ReadType = 5,    // 0000 0101
    PrintType = 9,   // 0000 1001
    CallType = 13,   // 0000 1101
    WhileType = 17,  // 0001 0001
    IfType = 21,     // 0001 0101
    AssignType = 25, // 0001 1001
    // other types: smallest bits are 00
    VariableType = 0,    // 0000 0000
    ConstantType = 4,    // 0000 0100
    ProcedureType = 8,   // 0000 1000
    NonExistentType = 12 // 0000 1100
};

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

enum ClauseType : char { SuchThatClauseType = 0, PatternClauseType = 1 };

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
};

typedef String ReferenceValue;

class Reference {
protected:
    ReferenceType referenceType;
    ReferenceValue referenceValue;
    //    Boolean isProcedureType;
    DesignEntity designEntity;
    Boolean hasError;
    Reference();

public:
    Reference(ReferenceType refType, ReferenceValue refValue);
    Reference(ReferenceType refType, ReferenceValue refValue, DesignEntity designEnt);
    ReferenceType getReferenceType();
    DesignEntity getDesignEntity();
    ReferenceValue getValue();
    Boolean isValidEntityRef();
    Boolean isValidStatementRef();
    Boolean isInvalid();
    Boolean isProcedure();
    Boolean isWildCard();
    Boolean isNonStatementSynonym();
    static Reference invalidReference();
    Boolean operator==(const Reference& reference);
};

enum RelationshipReferenceType : uint16_t {
    FollowsType = 0,
    FollowsStarType = 1,
    ParentType = 2,
    ParentStarType = 4,
    UsesType = 8, // placeholder type for UsesStatement and UsesProcedure
    UsesStatementType = 16,
    UsesProcedureType = 32,
    ModifiesType = 64, // placeholder type for ModifiesStatement and ModifiesProcedure
    ModifiesStatementType = 128,
    ModifiesProcedureType = 256,
    InvalidRelationshipType = 512
};

class Relationship {
private:
    RelationshipReferenceType relationshipReference;
    Reference leftReference;
    Reference rightReference;
    Boolean hasError;

public:
    Relationship(String relationshipRef, Reference leftRef, Reference rightRef);
    RelationshipReferenceType getRelationship();
    Reference getLeftRef();
    Reference getRightRef();
    Boolean isInvalid();
    static RelationshipReferenceType getRelRefType(String relRef);
    static Boolean validateRelationshipSemantics(RelationshipReferenceType relationshipReferenceType, Reference leftRef,
                                                 Reference rightRef);
    Boolean operator==(const Relationship& relationship);
};

class SuchThatClause: public Clause {
private:
    Relationship relationship;

public:
    explicit SuchThatClause(Relationship& r);
    Relationship getRelationship();
    Boolean operator==(const SuchThatClause& suchThatClause);
};

enum ExpressionSpecType : char {
    WildcardExpressionType = 0,          // _
    LiteralExpressionType = 1,           // _"x + y"_
    ExtendableLiteralExpressionType = 2, // "x + y"
    InvalidExpressionType = 4
};

class ExpressionSpec {
private:
    ExpressionSpecType expressionSpecType;
    std::unique_ptr<Expression> expression;
    bool hasError;

public:
    ExpressionSpec();
    explicit ExpressionSpec(ExpressionSpecType exprSpecType);
    ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType);
    Expression* getExpression();
    Boolean isInvalid() const;
    static ExpressionSpec invalidExpressionSpec();
    Boolean operator==(const ExpressionSpec& expressionSpec);
};

enum PatternStatementType : char { AssignPatternType = 0 };

class PatternClause: public Clause {
private:
    Synonym patternSynonym;
    PatternStatementType patternStatementType;
    Reference entityReference;
    ExpressionSpec expressionSpec;

public:
    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec);
    PatternStatementType getStatementType();
    Reference getEntRef();
    ExpressionSpec getExprSpec();
    Synonym getPatternSynonym();
    Boolean operator==(const PatternClause& patternClause);
};

class DeclarationTable {
private:
    std::unordered_map<Synonym, DesignEntity> table;
    bool hasError = false;

public:
    Void addDeclaration(Synonym s, DesignEntity& designEntity);
    DesignEntity getDesignEntityOfSynonym(Synonym s) const;
    Boolean isInvalid();
    Boolean hasSynonym(Synonym s);
    static DeclarationTable invalidDeclarationTable();
    Boolean operator==(const DeclarationTable& declarationTable);
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
    Synonym selectSynonym;
    ClauseVector clauses;
    DeclarationTable declarationTable;
    Boolean hasError;
    AbstractQuery(Boolean hasError);

public:
    AbstractQuery(Synonym synonym, DeclarationTable& declarations);
    AbstractQuery(Synonym synonym, DeclarationTable& declarations, ClauseVector& clauseVector);
    static AbstractQuery invalidAbstractQuery();
    Synonym getSelectSynonym() const;
    const ClauseVector& getClauses() const;
    DeclarationTable getDeclarationTable() const;
    Boolean isInvalid() const;
    Boolean operator==(const AbstractQuery& abstractQuery);
    AbstractQuery();
};

// Utils

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
