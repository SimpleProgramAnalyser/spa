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
    virtual bool operator==(const StatementNode& sn) const;

    virtual StatementType getStatementType() const = 0;
    virtual StatementNumber getStatementNumber() const;

protected:
    explicit StatementNode(StatementNumber n);
    virtual bool compare(const StatementNode& sn) const = 0;

private:
    StatementNumber stmtNum;
};

class StmtlstNode {
public:
    List<StatementNode> statementList;
    explicit StmtlstNode(List<StatementNode> stmtLst);
    bool operator==(const StmtlstNode& sln) const;
};

class ProcedureNode {
public:
    const Name procedureName;
    const StmtlstNode* const statementListNode;

    ProcedureNode(Name n, const StmtlstNode* stmtLst);
    ~ProcedureNode();
    ProcedureNode(const ProcedureNode&) = delete;
    ProcedureNode& operator=(const ProcedureNode&) = delete;
    ProcedureNode(ProcedureNode&&) = delete;
    ProcedureNode& operator=(ProcedureNode&&) = delete;
    bool operator==(const ProcedureNode& pcn) const;
};

class ProgramNode {
public:
    const Name programName;
    List<ProcedureNode> procedureList;
    ProgramNode(Name n, List<ProcedureNode> procLst);
    bool operator==(const ProgramNode& pgn) const;
};

class BasicDataType {
public:
    virtual ~BasicDataType() = default;
    virtual String toString() = 0;
    virtual Boolean isConstant() const noexcept = 0;
    virtual bool operator==(const BasicDataType& bdt) const;

protected:
    BasicDataType() = default;
    BasicDataType(const BasicDataType&) = default;
    BasicDataType& operator=(const BasicDataType&) = default;
    BasicDataType(BasicDataType&&) = default;
    BasicDataType& operator=(BasicDataType&&) = default;
    virtual bool compare(const BasicDataType& bdt) const = 0;
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
    Boolean isConstant() const noexcept override;
    bool operator==(const BasicDataType& c) const override;

protected:
    bool compare(const BasicDataType& c) const override;
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
    Boolean isConstant() const noexcept override;
    bool operator==(const BasicDataType& v) const override;

protected:
    bool compare(const BasicDataType& c) const override;
};

class ReadStatementNode: public StatementNode {
public:
    const Variable var;
    ReadStatementNode(StatementNumber stmtNum, Variable v);
    bool operator==(const StatementNode& sn) const override;
    StatementType getStatementType() const override;

protected:
    bool compare(const StatementNode& sn) const override;
};

class PrintStatementNode: public StatementNode {
public:
    const Variable var;
    PrintStatementNode(StatementNumber stmtNum, Variable v);
    bool operator==(const StatementNode& sn) const override;
    StatementType getStatementType() const override;

protected:
    bool compare(const StatementNode& sn) const override;
};

class CallStatementNode: public StatementNode {
public:
    const Name procedureName;
    CallStatementNode(StatementNumber stmtNum, Name n);
    bool operator==(const StatementNode& sn) const override;
    StatementType getStatementType() const override;

protected:
    bool compare(const StatementNode& sn) const override;
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
    virtual ConditionalExpressionType getConditionalType() const noexcept = 0;
    virtual bool operator==(const ConditionalExpression& ce) const;

protected:
    ConditionalExpression() = default;
    ConditionalExpression(const ConditionalExpression&) = default;
    ConditionalExpression& operator=(const ConditionalExpression&) = default;
    ConditionalExpression(ConditionalExpression&&) = default;
    ConditionalExpression& operator=(ConditionalExpression&&) = default;

    virtual bool compare(const ConditionalExpression& ce) const = 0;
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
    bool operator==(const ConditionalExpression& ce) const override;

    ConditionalExpressionType getConditionalType() const noexcept override;

protected:
    bool compare(const ConditionalExpression& ce) const override;
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
    bool operator==(const ConditionalExpression& ce) const override;

    ConditionalExpressionType getConditionalType() const noexcept override;

protected:
    bool compare(const ConditionalExpression& ce) const override;
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
    bool operator==(const ConditionalExpression& ce) const override;

    ConditionalExpressionType getConditionalType() const noexcept override;

protected:
    bool compare(const ConditionalExpression& ce) const override;
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
    virtual Boolean isArithmetic() const noexcept = 0;
    virtual bool operator==(const Expression& ex) const;

protected:
    Expression() = default;
    Expression(const Expression&) = default;
    Expression& operator=(const Expression&) = default;
    Expression(Expression&&) = default;
    Expression& operator=(Expression&&) = default;

    virtual bool compare(const Expression& ex) const = 0;
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
    bool operator==(const Expression& ex) const override;

    Boolean isArithmetic() const noexcept override;

protected:
    bool compare(const Expression& ex) const override;
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

    Boolean isArithmetic() const noexcept override;
    bool operator==(const Expression& re) const override;

protected:
    bool compare(const Expression& ex) const override;
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
    bool operator==(const ConditionalExpression& ce) const override;

    ConditionalExpressionType getConditionalType() const noexcept override;

protected:
    bool compare(const ConditionalExpression& ce) const override;
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
    bool operator==(const StatementNode& sn) const override;

    StatementType getStatementType() const override;

protected:
    bool compare(const StatementNode& sn) const override;
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
    bool operator==(const StatementNode& sn) const override;

    StatementType getStatementType() const override;

protected:
    bool compare(const StatementNode& sn) const override;
};

class AssignmentStatementNode: public StatementNode {
public:
    const Variable variable;
    const Expression* expression;

    AssignmentStatementNode(StatementNumber stmtNum, Variable v, const Expression* expr);
    ~AssignmentStatementNode() override;
    AssignmentStatementNode(const AssignmentStatementNode&) = delete;
    AssignmentStatementNode& operator=(const AssignmentStatementNode&) = delete;
    AssignmentStatementNode(AssignmentStatementNode&&) = delete;
    AssignmentStatementNode& operator=(AssignmentStatementNode&&) = delete;
    bool operator==(const StatementNode& sn) const override;

    StatementType getStatementType() const override;

protected:
    bool compare(const StatementNode& sn) const override;
};

typedef List<ProcedureNode> ProcedureNodeList;
typedef List<StatementNode> StatementNodeList;

#endif // SPA_AST_TYPES_H
