#ifndef SPA_EXPRESSIONSPEC_H
#define SPA_EXPRESSIONSPEC_H

#include "ast/AstTypes.h"
#include "AqTypesUtils.h"

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
    ExpressionSpec(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    explicit ExpressionSpec(ExpressionSpecType exprSpecType);
    ExpressionSpec(Expression* expr, ExpressionSpecType exprSpecType);
    Expression* getExpression() const;
    static ExpressionSpec createExpressionSpec(const String& exprSpecString);
    Boolean operator==(const ExpressionSpec& expressionSpec);
};

#endif // SPA_EXPRESSIONSPEC_H
