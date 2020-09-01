/**
 * Implementation of Abstract Syntax Tree library.
 */

#include "AstLibrary.h"


RelationalExpression* createRelExpr(Expression* leftRelFactor, Expression* rightRelFactor, RelationalOperator relOpr);

ArithmeticExpression* createArithExpr(Expression* leftExpr, Expression* rightExpr, ExpressionOperator exprOpr);
