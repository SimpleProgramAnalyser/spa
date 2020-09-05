/**
 * Implementation of Abstract Syntax Tree
 * classes and methods.
 */

#include "AstTypes.h"

StatementNumber StatementNode::getStatementNumber() const
{
    return stmtNum;
}

StatementNode::StatementNode(StatementNumber n): stmtNum(n) {}

bool StatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StmtlstNode::StmtlstNode(List<StatementNode> stmtLst): statementList(std::move(stmtLst)) {}

ProcedureNode::ProcedureNode(Name n, const StmtlstNode* stmtLst): procedureName(std::move(n)), statementList(stmtLst) {}

ProcedureNode::~ProcedureNode()
{
    delete statementList;
}

ProgramNode::ProgramNode(Name n, List<ProcedureNode> procLst):
    programName(std::move(n)), procedureList(std::move(procLst))
{}

bool BasicDataType::operator==(const BasicDataType& bdt) const
{
    return this->compare(bdt);
}

Constant::Constant(Integer val): value(val) {}

String Constant::toString()
{
    return std::to_string(static_cast<int>(value));
}

Boolean Constant::isConstant() const noexcept
{
    return true;
}

bool Constant::operator==(const BasicDataType& c) const
{
    return this->compare(c);
}

bool Constant::compare(const BasicDataType& c) const
{
    if (c.isConstant()) {
        // NOLINTNEXTLINE
        return static_cast<const Constant&>(c).value == this->value;
    } else {
        return false;
    }
}

Variable::Variable(Name n): varName(std::move(n)) {}

String Variable::toString()
{
    return static_cast<String>(varName);
}

Boolean Variable::isConstant() const noexcept
{
    return false;
}

bool Variable::operator==(const BasicDataType& v) const
{
    return this->compare(v);
}

bool Variable::compare(const BasicDataType& v) const
{
    if (!v.isConstant()) {
        // NOLINTNEXTLINE
        return static_cast<const Variable&>(v).varName == this->varName;
    } else {
        return false;
    }
}

ReadStatementNode::ReadStatementNode(StatementNumber stmtNum, Variable v): StatementNode(stmtNum), var(std::move(v)) {}

bool ReadStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType ReadStatementNode::getStatementType() const
{
    return ReadStatement;
}

bool ReadStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == ReadStatement) {
        return this->getStatementNumber() == sn.getStatementNumber()
               // NOLINTNEXTLINE
               && static_cast<const ReadStatementNode&>(sn).var == this->var;
    } else {
        return false;
    }
}

PrintStatementNode::PrintStatementNode(StatementNumber stmtNum, Variable v): StatementNode(stmtNum), var(std::move(v))
{}

bool PrintStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType PrintStatementNode::getStatementType() const
{
    return PrintStatement;
}

bool PrintStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == PrintStatement) {
        return this->getStatementNumber() == sn.getStatementNumber()
               // NOLINTNEXTLINE
               && static_cast<const PrintStatementNode&>(sn).var == this->var;
    } else {
        return false;
    }
}

CallStatementNode::CallStatementNode(StatementNumber stmtNum, Name n):
    StatementNode(stmtNum), procedureName(std::move(n))
{}

bool CallStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType CallStatementNode::getStatementType() const
{
    return CallStatement;
}

bool CallStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == CallStatement) {
        return this->getStatementNumber() == sn.getStatementNumber()
            // NOLINTNEXTLINE
            && static_cast<const CallStatementNode&>(sn).procedureName == this->procedureName;
    } else {
        return false;
    }
}

NotExpression::NotExpression(const ConditionalExpression* exp): expression(exp) {}

NotExpression::~NotExpression()
{
    delete expression;
}

ConditionalExpressionType NotExpression::getConditionalType() noexcept
{
    return NotConditionalExpression;
}

AndExpression::AndExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp):
    leftExpression(leftExp), rightExpression(rightExp)
{}

AndExpression::~AndExpression()
{
    delete leftExpression;
    delete rightExpression;
}

ConditionalExpressionType AndExpression::getConditionalType() noexcept
{
    return AndConditionalExpression;
}

OrExpression::OrExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp):
    leftExpression(leftExp), rightExpression(rightExp)
{}

OrExpression::~OrExpression()
{
    delete leftExpression;
    delete rightExpression;
}

ConditionalExpressionType OrExpression::getConditionalType() noexcept
{
    return OrConditionalExpression;
}

ArithmeticExpression::ArithmeticExpression(const Expression* left, const Expression* right, ExpressionOperator op):
    leftFactor(left), rightFactor(right), opr(op)
{}

ArithmeticExpression::~ArithmeticExpression()
{
    delete leftFactor;
    delete rightFactor;
}

Boolean ArithmeticExpression::isArithmetic() noexcept
{
    return true;
}

ReferenceExpression::ReferenceExpression(const BasicDataType* bd): basicData(bd) {}

ReferenceExpression::~ReferenceExpression()
{
    delete basicData;
}

Boolean ReferenceExpression::isArithmetic() noexcept
{
    return false;
}

bool ReferenceExpression::operator==(const ReferenceExpression& re) const
{
    return *(this->basicData) == *(re.basicData);
}

RelationalExpression::RelationalExpression(const Expression* left, const Expression* right, RelationalOperator ro):
    leftFactor(left), rightFactor(right), opr(ro)
{}

RelationalExpression::~RelationalExpression()
{
    delete leftFactor;
    delete rightFactor;
}

ConditionalExpressionType RelationalExpression::getConditionalType() noexcept
{
    return RelationalConditionalExpression;
}

IfStatementNode::IfStatementNode(StatementNumber stmtNum, const ConditionalExpression* pred, const StmtlstNode* ifs,
                                 const StmtlstNode* elses):
    StatementNode(stmtNum),
    predicate(pred), ifStatementList(ifs), elseStatementList(elses)
{}

IfStatementNode::~IfStatementNode()
{
    delete predicate;
    delete ifStatementList;
    delete elseStatementList;
}

bool IfStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType IfStatementNode::getStatementType() const
{
    return IfStatement;
}

WhileStatementNode::WhileStatementNode(StatementNumber stmtNum, const ConditionalExpression* pred,
                                       const StmtlstNode* stmts):
    StatementNode(stmtNum),
    predicate(pred), statementList(stmts)
{}

WhileStatementNode::~WhileStatementNode()
{
    delete predicate;
    delete statementList;
}

bool WhileStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType WhileStatementNode::getStatementType() const
{
    return WhileStatement;
}

AssignmentStatementNode::AssignmentStatementNode(StatementNumber stmtNum, const Variable& v, const Expression* expr):
    StatementNode(stmtNum), variable(v), expression(expr)
{}

AssignmentStatementNode::~AssignmentStatementNode()
{
    delete expression;
}

bool AssignmentStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType AssignmentStatementNode::getStatementType() const
{
    return AssignmentStatement;
}
