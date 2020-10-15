#include "AqTypes.h"
#include "frontend/parser/Parser.h"
#include "lexer/Lexer.h"

ExpressionSpec::ExpressionSpec(): expression(std::unique_ptr<Expression>()), expressionSpecType{InvalidExpressionType}
{}

ExpressionSpec::ExpressionSpec(ExpressionSpecType exprSpecType):
    expression(std::unique_ptr<Expression>()), expressionSpecType{exprSpecType}
{}

ExpressionSpec::ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType):
    expression(expr), expressionSpecType{exprSpecType}
{}

ExpressionSpec::ExpressionSpec(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    expressionSpecType{InvalidExpressionType}
{
    this->setError(queryErrorType, errorMessage);
}

/**
 * Creates an ExpressionSpec of based on the given
 * exprSpecString. It will determine the ExpressionSpecType
 * and call the parser from frontend to parse the expression
 * string into an Expression.
 *
 * If the exprSpecString is an invalid form of
 * ExpressionSpec, an invalid ExpressionSpec will be
 * returned.
 *
 * @param exprSpecString    The string to be parsed into an ExpressionSpec.
 * @return                  The ExpressionSpec constructed using exprSpectString.
 */
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
