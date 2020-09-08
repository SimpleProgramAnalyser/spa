/**
 * Functions for the Abstract Syntax Tree library,
 * namely for creation of Abstract Syntax Tree nodes.
 */

#ifndef SPA_AST_LIBRARY_H
#define SPA_AST_LIBRARY_H

#include "AstTypes.h"

// Nodes
ProgramNode* createProgramNode(Name programName, ProcedureNodeList& procedureNodes, StatementNumber totalStmts);
ProcedureNode* createProcedureNode(Name procedureName, StmtlstNode* stmtlstNode);
StmtlstNode* createStmtlstNode(StatementNodeList& statementNodes);
AssignmentStatementNode* createAssignNode(StatementNumber sn, const Variable& var, Expression* expr);
ReadStatementNode* createReadNode(StatementNumber sn, const Variable& var);
PrintStatementNode* createPrintNode(StatementNumber sn, const Variable& var);
WhileStatementNode* createWhileNode(StatementNumber sn, ConditionalExpression* predicate, StmtlstNode* statementList);
IfStatementNode* createIfNode(StatementNumber sn, ConditionalExpression* predicate, StmtlstNode* leftStatementList,
                              StmtlstNode* rightStatementList);
CallStatementNode* createCallNode(StatementNumber sn, Name procName);

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
ReferenceExpression* createRefExpr(String variable);
ReferenceExpression* createRefExpr(Integer constant);

// Other useful functions
inline Boolean isContainerStatement(StatementType stmtType);

#endif // SPA_AST_LIBRARY_H
