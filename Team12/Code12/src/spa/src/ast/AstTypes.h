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
    virtual ~StatementNode() = default;
    StatementNode(const StatementNode&) = delete;
    StatementNode& operator=(const StatementNode&) = delete;
    StatementNode(StatementNode&&) = delete;
    StatementNode& operator=(StatementNode&&) = delete;

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
    ProcedureNode(const ProcedureNode&) = delete;
    ProcedureNode& operator=(const ProcedureNode&) = delete;
    ProcedureNode(ProcedureNode&&) = delete;
    ProcedureNode& operator=(ProcedureNode&&) = delete;
};

class ProgramNode {
public:
    const Name programName;
    List<ProcedureNode> procedureList;
    ProgramNode(Name n, List<ProcedureNode> procLst);
};

class BasicDataType {
public:
    virtual ~BasicDataType() = default;
    virtual String toString() = 0;
    virtual Boolean isConstant() noexcept = 0;

protected:
    BasicDataType() = default;
    BasicDataType(const BasicDataType&) = default;
    BasicDataType& operator=(const BasicDataType&) = default;
    BasicDataType(BasicDataType&&) = default;
    BasicDataType& operator=(BasicDataType&&) = default;
};

class Constant: public BasicDataType {
public:
    const Integer value;

    explicit Constant(Integer val);
    ~Constant() override = default;
    Constant(const Constant&) = default;
    Constant& operator=(const Constant&) = delete;
    Constant(Constant&&) = default;
    Constant& operator=(Constant&&) = delete;

    String toString() override;
    Boolean isConstant() noexcept override;
};

class Variable: public BasicDataType {
public:
    const Name varName;

    explicit Variable(Name n);
    ~Variable() override = default;
    Variable(const Variable&) = default;
    Variable& operator=(const Variable&) = delete;
    Variable(Variable&&) = default;
    Variable& operator=(Variable&&) = delete;

    String toString() override;
    Boolean isConstant() noexcept override;
};

class ReadStatementNode: public StatementNode {
public:
    const Variable var;
    ReadStatementNode(StatementNumber stmtNum, Variable v);
    StatementType getStatementType() override;
};

class PrintStatementNode: public StatementNode {
public:
    const Variable var;
    PrintStatementNode(StatementNumber stmtNum, Variable v);
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
    virtual ~ConditionalExpression() = default;
    virtual ConditionalExpressionType getConditionalType() noexcept = 0;

protected:
    ConditionalExpression() = default;
    ConditionalExpression(const ConditionalExpression&) = default;
    ConditionalExpression& operator=(const ConditionalExpression&) = default;
    ConditionalExpression(ConditionalExpression&&) = default;
    ConditionalExpression& operator=(ConditionalExpression&&) = default;
};

class NotExpression: public ConditionalExpression {
public:
    const ConditionalExpression* expression;

    explicit NotExpression(const ConditionalExpression* exp);
    ~NotExpression() override;
    NotExpression(const NotExpression&) = delete;
    NotExpression& operator=(const NotExpression&) = delete;
    NotExpression(NotExpression&&) = delete;
    NotExpression& operator=(NotExpression&&) = delete;

    ConditionalExpressionType getConditionalType() noexcept override;
};

class AndExpression: public ConditionalExpression {
public:
    const ConditionalExpression* const leftExpression;
    const ConditionalExpression* const rightExpression;

    AndExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp);
    ~AndExpression() override;
    AndExpression(const AndExpression&) = delete;
    AndExpression& operator=(const AndExpression&) = delete;
    AndExpression(AndExpression&&) = delete;
    AndExpression& operator=(AndExpression&&) = delete;

    ConditionalExpressionType getConditionalType() noexcept override;
};

class OrExpression: public ConditionalExpression {
public:
    const ConditionalExpression* const leftExpression;
    const ConditionalExpression* const rightExpression;

    ~OrExpression() override;
    OrExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp);
    OrExpression(const OrExpression&) = delete;
    OrExpression& operator=(const OrExpression&) = delete;
    OrExpression(OrExpression&&) = delete;
    OrExpression& operator=(OrExpression&&) = delete;

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
    virtual ~Expression() = default;
    virtual Boolean isArithmetic() noexcept = 0;

protected:
    Expression() = default;
    Expression(const Expression&) = default;
    Expression& operator=(const Expression&) = default;
    Expression(Expression&&) = default;
    Expression& operator=(Expression&&) = default;
};

class ArithmeticExpression: public Expression {
public:
    const Expression* const leftFactor;
    const Expression* const rightFactor;
    const ExpressionOperator opr;

    ArithmeticExpression(const Expression* left, const Expression* right, ExpressionOperator op);
    ~ArithmeticExpression() override;
    ArithmeticExpression(const ArithmeticExpression&) = delete;
    ArithmeticExpression& operator=(const ArithmeticExpression&) = delete;
    ArithmeticExpression(ArithmeticExpression&&) = delete;
    ArithmeticExpression& operator=(ArithmeticExpression&&) = delete;

    Boolean isArithmetic() noexcept override;
};

class ReferenceExpression: public Expression {
public:
    const BasicDataType* const basicData;

    explicit ReferenceExpression(const BasicDataType* bd);
    ~ReferenceExpression() override;
    ReferenceExpression(const ReferenceExpression&) = delete;
    ReferenceExpression& operator=(const ReferenceExpression&) = delete;
    ReferenceExpression(ReferenceExpression&&) = delete;
    ReferenceExpression& operator=(ReferenceExpression&&) = delete;

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
    RelationalExpression(const RelationalExpression&) = delete;
    RelationalExpression& operator=(const RelationalExpression&) = delete;
    RelationalExpression(RelationalExpression&&) = delete;
    RelationalExpression& operator=(RelationalExpression&&) = delete;

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
    IfStatementNode(const IfStatementNode&) = delete;
    IfStatementNode& operator=(const IfStatementNode&) = delete;
    IfStatementNode(IfStatementNode&&) = delete;
    IfStatementNode& operator=(IfStatementNode&&) = delete;

    StatementType getStatementType() override;
};

class WhileStatementNode: public StatementNode {
public:
    const ConditionalExpression* const predicate;
    const StmtlstNode* const statementList;

    WhileStatementNode(StatementNumber stmtNum, const ConditionalExpression* pred, const StmtlstNode* stmts);
    ~WhileStatementNode() override;
    WhileStatementNode(const WhileStatementNode&) = delete;
    WhileStatementNode& operator=(const WhileStatementNode&) = delete;
    WhileStatementNode(WhileStatementNode&&) = delete;
    WhileStatementNode& operator=(WhileStatementNode&&) = delete;

    StatementType getStatementType() override;
};

class AssignmentStatementNode: public StatementNode {
public:
    const Variable variable;
    const Expression* expression;

    AssignmentStatementNode(StatementNumber stmtNum, const Variable& v, const Expression* expr);
    ~AssignmentStatementNode() override;
    AssignmentStatementNode(const AssignmentStatementNode&) = delete;
    AssignmentStatementNode& operator=(const AssignmentStatementNode&) = delete;
    AssignmentStatementNode(AssignmentStatementNode&&) = delete;
    AssignmentStatementNode& operator=(AssignmentStatementNode&&) = delete;

    StatementType getStatementType() override;
};

typedef List<ProcedureNode> ProcedureNodeList;
typedef List<StatementNode> StatementNodeList;

#endif // SPA_AST_TYPES_H
