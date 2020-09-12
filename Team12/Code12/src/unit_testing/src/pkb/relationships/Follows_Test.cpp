#include <catch.hpp>
#include <pkb/relationships/Follows.h>

/* Toy example SIMPLE program
procedure compute {
1.  read num1;
2.  read num2;
3.  read num3;
4.  sum = (num1 + num2) * num3;
5.  ave = sum / 3;
6.  print ave;
}
*/

SCENARIO("Iteration 1 toy example Follows", "[follows][pkb]")
{
    FollowsTable followsTable = FollowsTable();
    GIVEN("some Follows relationships")
    {
        Vector<Pair<Integer, StatementType>> stmtList{
            std::make_pair(1, ReadStatement),       std::make_pair(2, ReadStatement),
            std::make_pair(3, ReadStatement),       std::make_pair(4, AssignmentStatement),
            std::make_pair(5, AssignmentStatement), std::make_pair(6, PrintStatement),
        };
        WHEN("relationships are added to Follows table")
        {
            for (auto it = stmtList.begin(); it != stmtList.end() - 1; it++) {
                followsTable.addFollowsRelationships(it->first, it->second, (it + 1)->first, (it + 1)->second);
                followsTable.addFollowsRelationshipsStar(it->first, it->second,
                                                         Vector<Pair<Integer, StatementType>>(it+1, stmtList.end()));
            }
            THEN("tables should be populated")
            {
                REQUIRE_FALSE(followsTable.getAllBeforeStatementsStar(6, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllAfterStatementsStar(1, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllAfterStatementsTyped(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllBeforeStatementsTyped(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, AnyStatement).empty());
            }

            THEN("irrelevant tables should be empty")
            {
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(IfStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(WhileStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(WhileStatement, CallStatement).empty());
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(IfStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(WhileStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(WhileStatement, CallStatement).empty());
            }
        }
    }
}
