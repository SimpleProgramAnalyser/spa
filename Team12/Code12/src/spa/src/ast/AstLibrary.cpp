/**
 * Implementation of Abstract Syntax Tree library.
 */

#include "AstLibrary.h"

ProgramNode* createProgramNode(Name programName, ProcedureNodeList& procedureNodes)
{
    return new ProgramNode(std::move(programName), procedureNodes);
}

ProcedureNode* createProcedureNode(Name procedureName, StmtlstNode* stmtlstNode)
{
    return new ProcedureNode(std::move(procedureName), stmtlstNode);
}

StmtlstNode* createStmtlstNode(StatementNodeList& statementNodes)
{
    return new StmtlstNode(statementNodes);
}

AssignmentStatementNode* createAssignNode(StatementNumber sn, const Variable& var, Expression* expr)
{
    return new AssignmentStatementNode(sn, var, expr);
}

ReadStatementNode* createReadNode(StatementNumber sn, const Variable& var)
{
    return new ReadStatementNode(sn, var);
}

PrintStatementNode* createPrintNode(StatementNumber sn, const Variable& var)
{
    return new PrintStatementNode(sn, var);
}

WhileStatementNode* createWhileNode(StatementNumber sn, ConditionalExpression* predicate, StmtlstNode* statementList)
{
    return new WhileStatementNode(sn, predicate, statementList);
}

IfStatementNode* createIfNode(StatementNumber sn, ConditionalExpression* predicate, StmtlstNode* leftStatementList,
                              StmtlstNode* rightStatementList)
{
    return new IfStatementNode(sn, predicate, leftStatementList, rightStatementList);
}

CallStatementNode* createCallNode(StatementNumber sn, Name procName)
{
    return new CallStatementNode(sn, std::move(procName));
}

RelationalExpression* createRelExpr(Expression* leftRelFactor, Expression* rightRelFactor, RelationalOperator relOpr)
{
    return new RelationalExpression(leftRelFactor, rightRelFactor, relOpr);
}

RelationalExpression* createGtExpr(Expression* leftRelFactor, Expression* rightRelFactor)
{
    return createRelExpr(leftRelFactor, rightRelFactor, GreaterThanOperator);
}

RelationalExpression* createGteExpr(Expression* leftRelFactor, Expression* rightRelFactor)
{
    return createRelExpr(leftRelFactor, rightRelFactor, GreaterThanOrEqualToOperator);
}

RelationalExpression* createLtExpr(Expression* leftRelFactor, Expression* rightRelFactor)
{
    return createRelExpr(leftRelFactor, rightRelFactor, LessThanOperator);
}

RelationalExpression* createLteExpr(Expression* leftRelFactor, Expression* rightRelFactor)
{
    return createRelExpr(leftRelFactor, rightRelFactor, LessThanOrEqualToOperator);
}

RelationalExpression* createEqExpr(Expression* leftRelFactor, Expression* rightRelFactor)
{
    return createRelExpr(leftRelFactor, rightRelFactor, EqualToOperator);
}

RelationalExpression* createNeqExpr(Expression* leftRelFactor, Expression* rightRelFactor)
{
    return createRelExpr(leftRelFactor, rightRelFactor, NotEqualToOperator);
}

NotExpression* createNotExpr(ConditionalExpression* negatedExpression)
{
    return new NotExpression(negatedExpression);
}

AndExpression* createAndExpr(ConditionalExpression* leftExpr, ConditionalExpression* rightExpr)
{
    return new AndExpression(leftExpr, rightExpr);
}

OrExpression* createOrExpr(ConditionalExpression* leftExpr, ConditionalExpression* rightExpr)
{
    return new OrExpression(leftExpr, rightExpr);
}

ArithmeticExpression* createArithExpr(Expression* leftExpr, Expression* rightExpr, ExpressionOperator exprOpr)
{
    return new ArithmeticExpression(leftExpr, rightExpr, exprOpr);
}

ArithmeticExpression* createPlusExpr(Expression* leftExpr, Expression* rightExpr)
{
    return createArithExpr(leftExpr, rightExpr, PlusOperator);
}

ArithmeticExpression* createMinusExpr(Expression* leftExpr, Expression* rightExpr)
{
    return createArithExpr(leftExpr, rightExpr, MinusOperator);
}

ArithmeticExpression* createTimesExpr(Expression* leftExpr, Expression* rightExpr)
{
    return createArithExpr(leftExpr, rightExpr, TimesOperator);
}

ArithmeticExpression* createDivExpr(Expression* leftExpr, Expression* rightExpr)
{
    return createArithExpr(leftExpr, rightExpr, DivideOperator);
}

ArithmeticExpression* createModExpr(Expression* leftExpr, Expression* rightExpr)
{
    return createArithExpr(leftExpr, rightExpr, ModuloOperator);
}

ReferenceExpression* createRefExpr(BasicDataType* basicData)
{
    return new ReferenceExpression(basicData);
}
