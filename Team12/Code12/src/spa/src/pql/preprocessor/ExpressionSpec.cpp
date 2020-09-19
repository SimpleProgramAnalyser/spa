#include "AqTypes.h"

ExpressionSpec::ExpressionSpec():
    expressionSpecType(InvalidExpressionType), expression(std::unique_ptr<Expression>()), hasError{false}
{}

ExpressionSpec::ExpressionSpec(ExpressionSpecType exprSpecType):
    expressionSpecType{exprSpecType}, expression(std::unique_ptr<Expression>()), hasError{false}
{}

ExpressionSpec::ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType):
    expression(expr), hasError{false}, expressionSpecType{exprSpecType}
{}

Expression* ExpressionSpec::getExpression()
{
    return expression.get();
}

Boolean ExpressionSpec::isInvalid() const
{
    return hasError;
}

ExpressionSpec ExpressionSpec::invalidExpressionSpec()
{
    ExpressionSpec es;
    es.hasError = true;
    return es;
}

Boolean ExpressionSpec::operator==(const ExpressionSpec& expressionSpec)
{

    Boolean equalExpression;

    // Check if either expression is NULL
    if (!(this->expression) || !(expressionSpec.expression)) {
        if (this->expression || expressionSpec.expression) {
            return false;
        } else {
            equalExpression = true;
        }
    } else {
        equalExpression = *(this->expression) == *(expressionSpec.expression);
    }

    return this->expressionSpecType == expressionSpec.expressionSpecType && equalExpression;
}
