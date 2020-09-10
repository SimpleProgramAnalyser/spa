/**
 * This class defines an AbstractQuery (and necessary
 * supporting substructures), which is used to represent
 * a PQL query, and passed to the query evaluator, for
 * further processing.
 */

#ifndef SPA_AQTYPES_H
#define SPA_AQTYPES_H

#include <Types.h>
#include <Util.h>
#include <utility>

typedef std::pair<String, String> StringPair;

enum DesignEntityType {
    StatementType = 0,
    ReadType = 1,
    PrintType = 2,
    CallType = 4,
    WhileType = 8,
    IfType = 16,
    AssignType = 32,
    VariableType = 64,
    ConstantType = 128,
    ProcedureType = 256,
    NonExistentType = 512
};

class DesignEntity {
private:
    DesignEntityType type;

public:
    explicit DesignEntity(DesignEntityType designEntityType);
    explicit DesignEntity(const String& stringType);
    DesignEntityType getType();
};

typedef String Synonym;

enum ClauseType { SuchThatClauseType = 0, PatternClauseType = 1 };

class Clause {
protected:
    ClauseType type;
    Boolean hasError;

public:
    virtual ~Clause() = default;
    Clause(const Clause&) = delete;
    Clause operator=(const Clause&) = delete;
    Clause(Clause&&) = delete;
    Clause& operator=(Clause&&) = delete;
    explicit Clause(ClauseType clauseType);
    static Clause* invalidClause(ClauseType clauseType);
    ClauseType getType();
    Boolean isInvalid();
};

enum ReferenceType { StatementRefType = 0, EntityRefType = 1, AnyRefType = 2, InvalidRefType = 3 };

typedef String ReferenceValue; // TODO: more thought needs to be done on the implementation of ReferenceValue

class Reference {
protected:
    ReferenceType referenceType;
    ReferenceValue referenceValue;
    Boolean isProcedureType;

public:
    Reference(ReferenceType refType, ReferenceValue refValue);
    Reference(ReferenceType refType, ReferenceValue refValue, Boolean isProc);
    ReferenceType getReferenceType();
    ReferenceValue getValue();
    Boolean isValidEntityRef();
    Boolean isValidStatementRef();
    Boolean isWildCard();
    Boolean isInvalid();
    Boolean isProcedure();
    static Reference invalidReference();
};

enum RelationshipReferenceType {
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
};

class SuchThatClause: public Clause {
private:
    Relationship relationship;

public:
    SuchThatClause(Relationship& r);
    Relationship getRelationship();
};

typedef String Expression; // TODO: more thought is needed for the implementation of this

class ExpressionSpec {
private:
    bool isAny;
    bool hasBeforeOrAfter;
    Expression expression;

public:
    ExpressionSpec(String expr, bool any = false, bool beforeOrAfter = false);
    bool checkIsAny();
    bool checkHasBeforeOrAfter();
    Expression getExpression();
};

enum PatternStatementType { ASSIGN = 0 };

class PatternClause: public Clause {
private:
    PatternStatementType patternStatementType;
    Reference entityReference;
    ExpressionSpec expressionSpec;

public:
    PatternClause(PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec);
    PatternStatementType getStatementType();
    Reference getEntRef();
    ExpressionSpec getExprSpec();
};

class DeclarationTable {
private:
    std::unordered_map<Synonym, DesignEntity> table;
    bool hasError = false;

public:
    void addDeclaration(Synonym s, DesignEntity& designEntity);
    DesignEntity getDesignEntityOfSynonym(Synonym s);
    Boolean isInvalid();
    Boolean hasSynonym(Synonym s);
    static DeclarationTable invalidDeclarationTable();
};

class ClauseVector {
private:
    std::vector<Clause*> clauses;
    Boolean hasError;

public:
    static ClauseVector invalidClauseVector();
    void add(Clause* clause);
    Clause get(Integer index);
    Boolean isInvalid();
};

class AbstractQuery {
private:
    Synonym selectSynonym;
    ClauseVector clauses;
    DeclarationTable declarationTable;
    Boolean hasError;

public:
    AbstractQuery(); // TODO: To be removed in the future
    AbstractQuery(Synonym synonym, DeclarationTable& declarations, ClauseVector& clauseVector);
    static AbstractQuery invalidAbstractQuery();
    Synonym getSelectSynonym();
    ClauseVector getClauses();
    DeclarationTable getDeclarationTable();
    Boolean isInvalid();
};

// Utils

Boolean isValidSynonym(String s);

#endif // SPA_AQTYPES_H
