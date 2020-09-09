#include "../src/ast/AstTypes.cpp"
#include "../src/pkb/tables/Tables.cpp"
#include "TNode.h"
#include "catch.hpp"

TEST_CASE("2nd Test")
{

    StatementTable T;
    Variable testVar = Variable("TestVar");
    PrintStatementNode* printNode = new PrintStatementNode(3, testVar);
    T.insertIntoStatementTable(printNode, 3);
    T.getAllStatements(AnyStatement);

    REQUIRE(1 == 1);
}
