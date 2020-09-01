/**
 * Functions for the Abstract Syntax Tree library,
 * namely for creation of Abstract Syntax Tree nodes.
 */

#ifndef SPA_AST_LIBRARY_H
#define SPA_AST_LIBRARY_H

#include "AstTypes.h"

// Nodes
ProgramNode* createProgramNode(ProcedureNodeList& procedureNodes);
ProcedureNode* createProcedureNode(StmtlstNode* stmtlstNode);
StmtlstNode* createStmtlstNode(StatementNodeList& statementNodes);
AssignmentStatementNode* createAssignNode(Variable var, Expression* expr);
ReadStatementNode* createReadNode(Variable var);
PrintStatementNode* createPrintNode(Variable var);
WhileStatementNode* createWhileNode(ConditionalExpression* predicate, StmtlstNode* statementList);
IfStatementNode* createIfNode(ConditionalExpression* predicate, StmtlstNode* leftStatementList,
                              StmtlstNode* rightStatementList);
CallStatementNode* createCallNode(Name procName);

// Expressions
RelationalExpression* createGtExpr(Expression* leftRelFactor, Expression* rightRelFactor);
RelationalExpression* createGteExpr(Expression* leftRelFactor, Expression* rightRelFactor);
RelationalExpression* createLtExpr(Expression* leftRelFactor, Expression* rightRelFactor);
RelationalExpression* createLteExpr(Expression* leftRelFactor, Expression* rightRelFactor);
RelationalExpression* createEqExpr(Expression* leftRelFactor, Expression* rightRelFactor);
RelationalExpression* createNeqExpr(Expression* leftRelFactor, Expression* rightRelFactor);
NotExpression* createNotExpr(ConditionalExpression* negatedExpression);
AndExpression* createAndExpr(ConditionalExpression* leftExpr, ConditionalExpression* rightExpr);
OrExpression* createOrExpr(ConditionalExpression* leftExpr, ConditionalExpression* rightExpr);
ArithmeticExpression* createPlusExpr(Expression* leftExpr, Expression* rightExpr);
ArithmeticExpression* createMinusExpr(Expression* leftExpr, Expression* rightExpr);
ArithmeticExpression* createTimesExpr(Expression* leftExpr, Expression* rightExpr);
ArithmeticExpression* createDivExpr(Expression* leftExpr, Expression* rightExpr);
ArithmeticExpression* createModExpr(Expression* leftExpr, Expression* rightExpr);
ReferenceExpression* createRefExpr(BasicDataType& basicData);

#endif // SPA_AST_LIBRARY_H
