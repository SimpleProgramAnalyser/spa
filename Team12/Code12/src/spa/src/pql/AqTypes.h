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

enum ClauseType { SUCH_THAT_CLAUSE = 0, PATTERN_CLAUSE = 1 };

class Clause {
protected:
    ClauseType type;

public:
    explicit Clause(ClauseType clauseType);
    ClauseType getType();
};

enum ReferenceType { STATEMENT_REF = 0, ENTITY_REF = 1 };

typedef String ReferenceValue; // TODO: more thought needs to be done on the implementation of ReferenceValue

class Reference {
protected:
    ReferenceType referenceType;
    ReferenceValue referenceValue;

public:
    Reference(ReferenceType refType, ReferenceValue& refValue);
    ReferenceType getReferenceType();
    ReferenceValue getValue();
};

class EntityReference: public Reference {
public:
    EntityReference(ReferenceValue& refValue);
};

class StatementReference: public Reference {
public:
    StatementReference(ReferenceValue& refValue);
};

enum RelationshipReference {
    FOLLOWS = 0,
    FOLLOWS_T = 1,
    PARENT = 2,
    PARENT_T = 4,
    USES_S = 8,
    USES_P = 16,
    MODIFIES_S = 32,
    MODIFIES_P = 64
};

class Relationship {
private:
    RelationshipReference relationshipReference;
    Reference leftReference;
    Reference rightReference;

public:
    Relationship(RelationshipReference relationshipRef, Reference leftRef, Reference rightRef);
    RelationshipReference getRelationship();
    Reference getLeftRef();
    Reference getRightRef();
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
    EntityReference entityReference;
    ExpressionSpec expressionSpec;

public:
    PatternClause(PatternStatementType statementType, EntityReference entRef, ExpressionSpec exprSpec);
    PatternStatementType getStatementType();
    EntityReference getEntRef();
    ExpressionSpec getExprSpec();
};

class DeclarationTable {
private:
    std::unordered_map<Synonym, DesignEntity> table;
    bool isInvalid = false;

public:
    void addDeclaration(Synonym s, DesignEntity& designEntity);
    DesignEntity getDesignEntityOfSynonym(Synonym s);
    void setInvalidDeclaration();
    Boolean hasInvalidDeclaration();
    Boolean hasSynonym(Synonym s);
};

typedef std::vector<Clause> ClauseVector; // TODO: Adding unique_ptr causes compilation error

class AbstractQuery {
private:
    Synonym selectSynonym;
    ClauseVector clauses;
    DeclarationTable declarationTable;
    Boolean hasError;

public:
    AbstractQuery(); // TODO: To be removed in the future
    AbstractQuery(Synonym synonym, ClauseVector& clauseList);
    Synonym getSelectSynonym();
    ClauseVector getClauses();
    DeclarationTable getDeclarationTable();
    void setToInvalid();
    Boolean isInvalid();
    static AbstractQuery invalidAbstractQuery();
};

// Utils

Boolean isValidSynonym(String s);

#endif // SPA_AQTYPES_H
