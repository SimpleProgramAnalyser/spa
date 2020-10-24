#ifndef SPA_EXPRESSIONSPEC_H
#define SPA_EXPRESSIONSPEC_H

#include "ast/AstTypes.h"
#include "AqTypesUtils.h"

// All the types of ExpressionSpec.
enum ExpressionSpecType : char {
    WildcardExpressionType,          // _
    LiteralExpressionType,           // "x + y"
    ExtendableLiteralExpressionType, // _"x + y"_
    NoExpressionType,
    InvalidExpressionType
};

class ExpressionSpec: public Errorable {
private:
    std::unique_ptr<Expression> expression;

public:
    ExpressionSpecType expressionSpecType;
    ExpressionSpec();

    // Instantiate an erroneous ExpressionSpec with the given QueryErrorType
    // and ErrorMessage.
    ExpressionSpec(QueryErrorType queryErrorType, ErrorMessage errorMessage);

    // Instantiate an ExpressionSpec with the given ExpressionSpecType.
    // Mainly used for WildcardExpressionType, since there is no
    // Expression needed.
    explicit ExpressionSpec(ExpressionSpecType exprSpecType);

    // Instantiate an ExpressionSpec with the given Expression
    // and ExpressionSpecType.
    ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType);

    // Retrieves the Expression.
    Expression* getExpression() const;

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
    static ExpressionSpec createExpressionSpec(const String& exprSpecString);

    Boolean operator==(const ExpressionSpec& expressionSpec);
};

#endif // SPA_EXPRESSIONSPEC_H
