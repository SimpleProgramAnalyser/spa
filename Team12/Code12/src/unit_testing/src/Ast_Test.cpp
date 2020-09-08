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

TEST_CASE("Assign nodes with different names are different")
{
    AssignmentStatementNode* asn1 = createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3")));
    AssignmentStatementNode* asn2 = createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num4")));
    REQUIRE(!(*(asn1) == *(asn2)));
    AssignmentStatementNode* asn3 = createAssignNode(
        4, Variable("summ"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3")));
    AssignmentStatementNode* asn4 = createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3")));
    REQUIRE(!(*(asn3) == *(asn4)));
}
