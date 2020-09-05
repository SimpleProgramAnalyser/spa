#include "catch.hpp"
#include "ast/AstLibrary.h"

TEST_CASE("createRefExpr creates correct nodes")
{
    auto* var = new Variable("testVariable123");
    auto* cons = new Constant(12345);
    REQUIRE(ReferenceExpression(var) == *createRefExpr(var));
    REQUIRE(ReferenceExpression(cons) == *createRefExpr(cons));
}

