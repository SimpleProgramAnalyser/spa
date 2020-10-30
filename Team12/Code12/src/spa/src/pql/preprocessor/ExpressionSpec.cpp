/**
 * Implementation of ExpressionSpec, representation
 * of an Expression in a pattern clause.
 */

#include "ExpressionSpec.h"

#include <utility>

#include "AqTypes.h"
#include "frontend/parser/Parser.h"

ExpressionSpec::ExpressionSpec():
    Errorable(), expression(std::unique_ptr<Expression>()), expressionSpecType{InvalidExpressionType}
{}

ExpressionSpec::ExpressionSpec(ExpressionSpecType exprSpecType):
    Errorable(), expression(std::unique_ptr<Expression>()), expressionSpecType{exprSpecType}
{}

ExpressionSpec::ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType):
    Errorable(), expression(expr), expressionSpecType{exprSpecType}
{}

ExpressionSpec::ExpressionSpec(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable{queryErrorType, std::move(errorMessage)}, expression(), expressionSpecType{InvalidExpressionType}
{}

ExpressionSpec ExpressionSpec::createExpressionSpec(const String& exprSpecString)
{
    if (exprSpecString == "_") {
        ExpressionSpec expressionSpec{WildcardExpressionType};
        return expressionSpec;
    }

    if (util::isEnclosedWith(exprSpecString, '_', '_')) {
        String possibleLiteral = util::removeCharFromBothEnds(exprSpecString);
        if (!util::isEnclosedWith(possibleLiteral, '\"', '\"')) {
            return ExpressionSpec(QuerySyntaxError, "Extendable ExpressionSpec is not enclosed with double quotes");
        }

        Expression* expression = createExpression(util::removeCharFromBothEnds(possibleLiteral));
        if (!expression) {
            return ExpressionSpec(QuerySyntaxError,
                                  "Invalid Expression"); // TODO: Implement error type for invalid Expression
        }

        ExpressionSpec expressionSpec{expression, ExtendableLiteralExpressionType};
        return expressionSpec;
    }

    if (!util::isEnclosedWith(exprSpecString, '\"', '\"')) {
        return ExpressionSpec(QuerySyntaxError, "ExpressionSpec is not enclosed with double quotes");
    }

    String expressionString = util::removeCharFromBothEnds(exprSpecString);
    Expression* expression = createExpression(expressionString);
    if (!expression) {
        return ExpressionSpec(QuerySyntaxError,
                              "Invalid Expression"); // TODO: Implement error type for invalid Expression
    }

    ExpressionSpec expressionSpec{expression, LiteralExpressionType};
    return expressionSpec;
}

Expression* ExpressionSpec::getExpression() const
{
    return expression.get();
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
