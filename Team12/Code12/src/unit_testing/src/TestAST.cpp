
#include "TNode.h"
#include "../src/ast/AstTypes.h"
#include "../src/ast/AstTypes.cpp"
#include "../src/tables/Tables.h"
#include "../src/tables/Tables.cpp"
#include "catch.hpp"


using namespace std;
void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("1st Test") {

    TNode T;
	
	
	
    require(1 == 1);
}

TEST_CASE("2nd Test")
{

    StatementTable T;
    Variable testVar = Variable("TestVar");
    PrintStatementNode* printNode = new PrintStatementNode(3, testVar);
    T.insertIntoStatementTable(printNode, 3);
    T.getAllStatements(AnyStatement);

    require(1 == 1);
}


