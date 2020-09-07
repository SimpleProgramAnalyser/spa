/**
 * This class defines an AbstractQueryType (and necessary
 * supporting substructures), which is used to represent
 * a PQL query, and passed to the query evaluator, for
 * further processing.
 */

#ifndef SPA_AQTYPES_H
#define SPA_AQTYPES_H

#include <Types.h>

enum DesignEntityType {
    STMT_TYPE = 0,
    READ_TYPE = 1,
    PRINT_TYPE = 2,
    CALL_TYPE = 4,
    WHILE_TYPE = 8,
    IF_TYPE = 16,
    ASSIGN_TYPE = 32,
    VARIABLE_TYPE = 64,
    CONSTANT_TYPE = 128,
    PROCEDURE_TYPE = 256
};

class DesignEntity {
    private:
        DesignEntityType type;
    public:
        explicit DesignEntity(DesignEntityType type);
        DesignEntityType getType();
};

typedef String Synonym;

enum ClauseType {
    SUCH_THAT_CLAUSE = 0,
    PATTERN_CLAUSE = 1
};

class Clause {
protected:
    ClauseType type;
public:
    explicit Clause(ClauseType clauseType);
    ClauseType getType();
};

enum ReferenceType {
    STATEMENT_REF = 0,
    ENTITY_REF = 1
};

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

class EntityReference : public Reference {
    public:
        EntityReference(ReferenceValue& refValue);
};

class StatementReference : public Reference {
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
        ExpressionSpec(String expr, bool any=false, bool beforeOrAfter=false);
        bool checkIsAny();
        bool checkHasBeforeOrAfter();
        Expression getExpression();
};

enum PatternStatementType {
    ASSIGN = 0
};

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
    public:
        void addDeclaration(Synonym s, DesignEntity& designEntity);
        DesignEntity getDesignEntityOfSynonym(Synonym s);
};

typedef std::vector<Clause> ClauseList; // TODO: Adding unique_ptr causes compilation error

class AbstractQuery {
    private:
        Synonym selectSynonym;
        ClauseList clauses;
        DeclarationTable declarationTable;
    public:
        AbstractQuery(Synonym synonym, ClauseList& clauseList);
        Synonym getSelectSynonym();
        ClauseList getClauses();
        DeclarationTable getDeclarationTable();
};

#endif // SPA_AQTYPES_H
