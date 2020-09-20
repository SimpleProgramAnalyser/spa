#include "AqTypes.h"

ExpressionSpec::ExpressionSpec():
    expression(std::unique_ptr<Expression>()), hasError{false}, expressionSpecType(InvalidExpressionType)
{}

ExpressionSpec::ExpressionSpec(ExpressionSpecType exprSpecType):
    expression(std::unique_ptr<Expression>()), hasError{false}, expressionSpecType{exprSpecType}
{}

ExpressionSpec::ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType):
    expression(expr), hasError{false}, expressionSpecType{exprSpecType}
{}

Expression* ExpressionSpec::getExpression() const
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
