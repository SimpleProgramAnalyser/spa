#include "ast/AstLibrary.h"
#include "catch.hpp"

TEST_CASE("createRefExpr creates correct nodes")
{
    String rawVariable = "testVariable123";
    Integer rawConstant = 12345;
    auto* var = new Variable(rawVariable);
    auto* cons = new Constant(rawConstant);
    REQUIRE(ReferenceExpression(var) == *createRefExpr(rawVariable));
    REQUIRE(ReferenceExpression(cons) == *createRefExpr(rawConstant));
}
