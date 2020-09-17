#include "AqTypes.h"

ExpressionSpec::ExpressionSpec(): hasError{false}
{
    expressionSpecType = InvalidExpressionType;
    Expression* expressionPtr = nullptr;
    expression = expressionPtr;
}

ExpressionSpec::ExpressionSpec(ExpressionSpecType exprSpecType): expressionSpecType{exprSpecType}, hasError{false}
{
    Expression* expressionPtr = nullptr;
    expression = expressionPtr;
}

ExpressionSpec::ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType):
    expressionSpecType{exprSpecType}, expression(expr), hasError{false}
{}

Expression* ExpressionSpec::getExpression()
{
    return expression;
}

Boolean ExpressionSpec::isInvalid() const
{
    return hasError;
}

ExpressionSpec ExpressionSpec::invalidExpressionSpec()
{
    auto* eS = new ExpressionSpec();
    eS->hasError = true;
    return *eS;
}

Boolean ExpressionSpec::operator==(const ExpressionSpec& expressionSpec)
{

    Boolean equalExpression = false;

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
