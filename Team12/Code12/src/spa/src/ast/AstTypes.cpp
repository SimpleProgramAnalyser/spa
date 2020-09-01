/**
 * Implementation of Abstract Syntax Tree
 * classes and methods.
 */

#include "AstTypes.h"

#include <string>

ProcedureNode::~ProcedureNode()
{
    delete statementList;
}

String Constant::toString()
{
    return std::to_string(static_cast<int>(value));
}

String Variable::toString()
{
    return static_cast<String>(varName);
}

StatementType ReadStatementNode::getStatementType()
{
    return ReadStatement;
}

StatementNumber ReadStatementNode::getStatementNumber()
{
    return stmtNum;
}

StatementType PrintStatementNode::getStatementType()
{
    return PrintStatement;
}

StatementNumber PrintStatementNode::getStatementNumber()
{
    return stmtNum;
}

StatementType CallStatementNode::getStatementType()
{
    return CallStatement;
}

StatementNumber CallStatementNode::getStatementNumber()
{
    return stmtNum;
}

NotExpression::~NotExpression()
{
    delete expression;
}

ConditionalExpressionType NotExpression::getConditionalType() noexcept
{
    return NotConditionalExpression;
}

AndExpression::~AndExpression()
{
    delete leftExpression;
    delete rightExpression;
}

ConditionalExpressionType AndExpression::getConditionalType() noexcept
{
    return AndConditionalExpression;
}

OrExpression::~OrExpression()
{
    delete leftExpression;
    delete rightExpression;
}

ConditionalExpressionType OrExpression::getConditionalType() noexcept
{
    return OrConditionalExpression;
}

ArithmeticExpression::ArithmeticExpression(Expression* const left, Expression* const right, ExpressionOperator op):
    leftFactor(left), rightFactor(right), op(op)
{}

ArithmeticExpression::~ArithmeticExpression()
{
    delete leftFactor;
    delete rightFactor;
}

bool ArithmeticExpression::isArithmetic() noexcept
{
    return true;
}

ReferenceExpression::~ReferenceExpression()
{
    delete basicData;
}

bool ReferenceExpression::isArithmetic() noexcept
{
    return false;
}

ConditionalExpressionType RelationalExpression::getConditionalType() noexcept
{
    return RelationalConditionalExpression;
}

RelationalExpression::~RelationalExpression()
{
    delete leftFactor;
    delete rightFactor;
}

IfStatementNode::~IfStatementNode()
{
    delete ifStatementList;
    delete elseStatementList;
}

StatementType IfStatementNode::getStatementType()
{
    return IfStatement;
}

StatementNumber IfStatementNode::getStatementNumber()
{
    return stmtNum;
}

WhileStatementNode::~WhileStatementNode()
{
    delete statementList;
}

StatementType WhileStatementNode::getStatementType()
{
    return WhileStatement;
}

StatementNumber WhileStatementNode::getStatementNumber()
{
    return stmtNum;
}

AssignmentStatementNode::~AssignmentStatementNode() {
    delete expression;
}

StatementType AssignmentStatementNode::getStatementType()
{
    return AssignmentStatement;
}

StatementNumber AssignmentStatementNode::getStatementNumber()
{
    return stmtNum;
}
