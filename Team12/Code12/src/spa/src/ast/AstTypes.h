/**
 * Class structure of Abstract Syntax Tree,
 * types, fields and methods.
 */

#ifndef SPA_AST_TYPES_H
#define SPA_AST_TYPES_H

#include <Types.h>
#include <utility>

typedef String Name;
typedef Integer StatementNumber;

class StatementNode {
public:
    virtual ~StatementNode() = 0;
    virtual StatementType getStatementType() = 0;
    virtual StatementNumber getStatementNumber();

protected:
    explicit StatementNode(StatementNumber n);

private:
    StatementNumber stmtNum;
};

class StmtlstNode {
public:
    List<StatementNode> statementList;
    explicit StmtlstNode(List<StatementNode> stmtLst);
};

class ProcedureNode {
public:
    const Name procedureName;
    const StmtlstNode* const statementList;
    ProcedureNode(Name n, const StmtlstNode* stmtLst);
    ~ProcedureNode();
};

class ProgramNode {
public:
    const Name programName;
    List<ProcedureNode> procedureList;
    ProgramNode(Name n, List<ProcedureNode> procLst);
};

class BasicDataType {
public:
    virtual ~BasicDataType() = 0;
    virtual String toString() = 0;
    virtual Boolean isConstant() noexcept = 0;
};

class Constant: public BasicDataType {
public:
    const Integer value;
    explicit Constant(Integer val);
    ~Constant() override = default;
    String toString() override;
    Boolean isConstant() noexcept override;
};

class Variable: public BasicDataType {
public:
    const Name varName;
    explicit Variable(Name n);
    ~Variable() override = default;
    String toString() override;
    Boolean isConstant() noexcept override;
};

class ReadStatementNode: public StatementNode {
public:
    const Variable var;
    ReadStatementNode(StatementNumber stmtNum, const Variable& v);
    StatementType getStatementType() override;
};

class PrintStatementNode: public StatementNode {
public:
    const Variable var;
    PrintStatementNode(StatementNumber stmtNum, const Variable& v);
    StatementType getStatementType() override;
};

class CallStatementNode: public StatementNode {
public:
    const Name procedureName;
    CallStatementNode(StatementNumber stmtNum, Name n);
    StatementType getStatementType() override;
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
    const ConditionalExpression* expression;
    explicit NotExpression(const ConditionalExpression* exp);
    ~NotExpression() override;
    ConditionalExpressionType getConditionalType() noexcept override;
};

class AndExpression: public ConditionalExpression {
public:
    const ConditionalExpression* const leftExpression;
    const ConditionalExpression* const rightExpression;
    AndExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp);
    ~AndExpression() override;
    ConditionalExpressionType getConditionalType() noexcept override;
};

class OrExpression: public ConditionalExpression {
public:
    const ConditionalExpression* const leftExpression;
    const ConditionalExpression* const rightExpression;
    ~OrExpression() override;
    OrExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp);
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
    virtual Boolean isArithmetic() noexcept = 0;
};

class ArithmeticExpression: public Expression {
public:
    const Expression* const leftFactor;
    const Expression* const rightFactor;
    const ExpressionOperator opr;
    ArithmeticExpression(const Expression* left, const Expression* right, ExpressionOperator op);
    ~ArithmeticExpression() override;
    Boolean isArithmetic() noexcept override;
};

class ReferenceExpression: public Expression {
public:
    const BasicDataType* const basicData;
    explicit ReferenceExpression(const BasicDataType* bd);
    ~ReferenceExpression() override;
    Boolean isArithmetic() noexcept override;
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
    RelationalExpression(const Expression* left, const Expression* right, RelationalOperator ro);
    ~RelationalExpression() override;
    ConditionalExpressionType getConditionalType() noexcept override;
};

class IfStatementNode: public StatementNode {
public:
    const ConditionalExpression* const predicate;
    const StmtlstNode* const ifStatementList;
    const StmtlstNode* const elseStatementList;
    IfStatementNode(StatementNumber stmtNum, const ConditionalExpression* pred, const StmtlstNode* ifs,
                    const StmtlstNode* elses);
    ~IfStatementNode() override;
    StatementType getStatementType() override;
};

class WhileStatementNode: public StatementNode {
public:
    const ConditionalExpression* const predicate;
    const StmtlstNode* const statementList;
    WhileStatementNode(StatementNumber stmtNum, const ConditionalExpression* pred, const StmtlstNode* stmts);
    ~WhileStatementNode() override;
    StatementType getStatementType() override;
};

class AssignmentStatementNode: public StatementNode {
public:
    const Variable variable;
    const Expression* expression;
    AssignmentStatementNode(StatementNumber stmtNum, const Variable& v, const Expression* expr);
    ~AssignmentStatementNode() override;
    StatementType getStatementType() override;
};

typedef List<ProcedureNode> ProcedureNodeList;
typedef List<StatementNode> StatementNodeList;

#endif // SPA_AST_TYPES_H
