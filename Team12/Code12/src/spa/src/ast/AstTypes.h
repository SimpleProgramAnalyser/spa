/**
 * Class structure of Abstract Syntax Tree,
 * types, fields and methods.
 */

#ifndef SPA_AST_TYPES_H
#define SPA_AST_TYPES_H

#include <Types.h>

typedef String Name;
typedef Integer StatementNumber;

enum StatementType {
    AnyStatement = 0,
    AssignmentStatement = 1,
    CallStatement = 2,
    IfStatement = 4,
    PrintStatement = 8,
    ReadStatement = 16,
    WhileStatement = 32
};

class StatementNode {
public:
    virtual ~StatementNode() = 0;
    virtual StatementType getStatementType() = 0;
    virtual StatementNumber getStatementNumber() = 0;
};

class StatementListNode {
public:
    List<StatementNode> statementList;
};

class ProcedureNode {
public:
    const Name procedureName;
    const StatementListNode* const statementList;
    ~ProcedureNode();
};

class ProgramNode {
public:
    const Name programName;
    List<ProcedureNode> procedureList;
};

class BasicDataType {
public:
    virtual ~BasicDataType() = 0;
    virtual String toString() = 0;
};

class Constant: public BasicDataType {
public:
    const Integer value;
    String toString() override;
    ~Constant() override = default;
};

class Variable: public BasicDataType {
public:
    const Name varName;
    String toString() override;
    ~Variable() override = default;
};

class ReadStatementNode: public StatementNode {
public:
    const Variable var;
    StatementType getStatementType() override;
    StatementNumber getStatementNumber() override;

private:
    StatementNumber stmtNum;
};

class PrintStatementNode: public StatementNode {
public:
    const Variable var;
    StatementType getStatementType() override;
    StatementNumber getStatementNumber() override;

private:
    StatementNumber stmtNum;
};

class CallStatementNode: public StatementNode {
public:
    const Name procedureName;
    StatementType getStatementType() override;
    StatementNumber getStatementNumber() override;

private:
    StatementNumber stmtNum;
};

enum ConditionalExpressionType {
    RelationalConditionalExpression = 0,
    NotConditionalExpression = 1,
    AndConditionalExpression = 2,
    OrConditionalExpression = 4
};

class ConditionalExpression {
public:
    virtual ~ConditionalExpression() = 0;
    virtual ConditionalExpressionType getConditionalType() noexcept = 0;
};

class NotExpression: public ConditionalExpression {
public:
    ~NotExpression() override;
    const ConditionalExpression* expression;
    ConditionalExpressionType getConditionalType() noexcept override;
};

class AndExpression: public ConditionalExpression {
public:
    ~AndExpression() override;
    const ConditionalExpression* const leftExpression;
    const ConditionalExpression* const rightExpression;
    ConditionalExpressionType getConditionalType() noexcept override;
};

class OrExpression: public ConditionalExpression {
public:
    ~OrExpression() override;
    const ConditionalExpression* const leftExpression;
    const ConditionalExpression* const rightExpression;
    ConditionalExpressionType getConditionalType() noexcept override;
};

enum ExpressionOperator : char {
    PlusOperator = '+',
    MinusOperator = '-',
    TimesOperator = '*',
    DivideOperator = '/',
    ModuloOperator = '%'
};

class Expression {
public:
    virtual ~Expression() = 0;
    virtual bool isArithmetic() noexcept = 0;
};

class ArithmeticExpression: public Expression {
public:
    const Expression* const leftFactor;
    const Expression* const rightFactor;
    const ExpressionOperator opr;
    ArithmeticExpression(Expression* left, Expression* right, ExpressionOperator op);
    ~ArithmeticExpression() override;
    bool isArithmetic() noexcept override;
};

class ReferenceExpression: public Expression {
public:
    const BasicDataType* const basicData;
    ~ReferenceExpression() override;
    bool isArithmetic() noexcept override;
};

enum RelationalOperator : char {
    GreaterThanOperator = '>',
    GreaterThanOrEqualToOperator = '}',
    LessThanOperator = '<',
    LessThanOrEqualToOperator = '{',
    NotEqualToOperator = '!',
    EqualToOperator = '='
};

class RelationalExpression: public ConditionalExpression {
public:
    const Expression* const leftFactor;
    const Expression* const rightFactor;
    const RelationalOperator opr;
    ~RelationalExpression() override;
    ConditionalExpressionType getConditionalType() noexcept override;
};

class IfStatementNode: public StatementNode {
public:
    const ConditionalExpression* const predicate;
    const StatementListNode* const ifStatementList;
    const StatementListNode* const elseStatementList;
    ~IfStatementNode() override;
    StatementType getStatementType() override;
    StatementNumber getStatementNumber() override;

private:
    StatementNumber stmtNum;
};

class WhileStatementNode: public StatementNode {
public:
    const ConditionalExpression* const predicate;
    const StatementListNode* const statementList;
    ~WhileStatementNode() override;
    StatementType getStatementType() override;
    StatementNumber getStatementNumber() override;

private:
    StatementNumber stmtNum;
};

class AssignmentStatementNode: public StatementNode {
public:
    const Variable variable;
    const Expression* expression;
    ~AssignmentStatementNode() override;
    StatementType getStatementType() override;
    StatementNumber getStatementNumber() override;

private:
    StatementNumber stmtNum;
};

#endif // SPA_AST_TYPES_H
