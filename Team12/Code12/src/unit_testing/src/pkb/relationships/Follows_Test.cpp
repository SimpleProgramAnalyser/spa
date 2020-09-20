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

Vector<Pair<Integer, Integer>> makeTupleVector(Vector<Integer> vec)
{
    Vector<Pair<Integer, Integer>> toReturn;
    for (Integer i : vec) {
        toReturn.push_back(std::make_pair(i, i + 1));
    }
    return toReturn;
}

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
                                                         Vector<Pair<Integer, StatementType>>(it + 1, stmtList.end()));
            }
            THEN("tables should be populated")
            {
                REQUIRE_FALSE(followsTable.getAllBeforeStatementsStar(6, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllAfterStatementsStar(1, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllAfterStatementsTyped(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllBeforeStatementsTyped(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, AnyStatement).empty());
                // tuples
                REQUIRE_FALSE(followsTable.getAllFollowsTuple(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(followsTable.getAllFollowsTupleStar(AnyStatement, AnyStatement).empty());
            }

            THEN("irrelevant tables should be empty")
            {
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(IfStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(WhileStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(WhileStatement, CallStatement).empty());
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(IfStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(WhileStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(WhileStatement, CallStatement).empty());
                // tuples
                REQUIRE(followsTable.getAllFollowsTupleStar(IfStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllFollowsTupleStar(WhileStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllFollowsTupleStar(WhileStatement, CallStatement).empty());
                REQUIRE(followsTable.getAllFollowsTupleStar(IfStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllFollowsTupleStar(WhileStatement, IfStatement).empty());
                REQUIRE(followsTable.getAllFollowsTupleStar(WhileStatement, CallStatement).empty());
            }

            THEN("first-order relationships are correct")
            {
                for (int i = 1; i < 6; i++) {
                    REQUIRE(followsTable.checkIfFollowsHolds(i, i + 1));
                    for (int j = i + 1; j <= 6; j++) {
                        REQUIRE(followsTable.checkIfFollowsHoldsStar(i, j));
                    }
                }
            }

            THEN("second-order relationships are correct (no star)")
            {
                // select s such that Follows(s,s1): 1,2,3,4,5
                REQUIRE(followsTable.getAllBeforeStatementsTyped(AnyStatement, AnyStatement)
                        == Vector<Integer>{1, 2, 3, 4, 5});
                // select s1 such that Follows(s,s1): 1,2,3,4,5
                REQUIRE(followsTable.getAllAfterStatementsTyped(AnyStatement, AnyStatement)
                        == Vector<Integer>{2, 3, 4, 5, 6});

                // select a such that Follows(a,a1): 4
                REQUIRE(followsTable.getAllBeforeStatementsTyped(AssignmentStatement, AssignmentStatement)
                        == Vector<Integer>{4});
                // select a1 such that Follows(a,a1): 5
                REQUIRE(followsTable.getAllAfterStatementsTyped(AssignmentStatement, AssignmentStatement)
                        == Vector<Integer>{5});

                // select re such that Follows(re,re1): 1,2
                REQUIRE(followsTable.getAllBeforeStatementsTyped(ReadStatement, ReadStatement)
                        == Vector<Integer>{1, 2});
                // select re1 such that Follows(re,re1): 2,3
                REQUIRE(followsTable.getAllAfterStatementsTyped(ReadStatement, ReadStatement) == Vector<Integer>{2, 3});

                // select s such that Follows(s,a): 3,4
                REQUIRE(followsTable.getAllBeforeStatementsTyped(AnyStatement, AssignmentStatement)
                        == Vector<Integer>{3, 4});
                // select a such that Follows(s,a): 4,5
                REQUIRE(followsTable.getAllAfterStatementsTyped(AnyStatement, AssignmentStatement)
                        == Vector<Integer>{4, 5});
                // select a such that Follows(a,s): 4,5
                REQUIRE(followsTable.getAllBeforeStatementsTyped(AssignmentStatement, AnyStatement)
                        == Vector<Integer>{4, 5});
                // select s such that Follows(a,s): 5,6
                REQUIRE(followsTable.getAllAfterStatementsTyped(AssignmentStatement, AnyStatement)
                        == Vector<Integer>{5, 6});

                // select s such that Follows(s,r): 1,2
                REQUIRE(followsTable.getAllBeforeStatementsTyped(AnyStatement, ReadStatement) == Vector<Integer>{1, 2});
                // select r such that Follows(s,r): 2,3
                REQUIRE(followsTable.getAllAfterStatementsTyped(AnyStatement, ReadStatement) == Vector<Integer>{2, 3});
                // select r such that Follows(r,s): 1,2,3
                REQUIRE(followsTable.getAllBeforeStatementsTyped(ReadStatement, AnyStatement)
                        == Vector<Integer>{1, 2, 3});
                // select s such that Follows(r,s): 2,3,4
                REQUIRE(followsTable.getAllAfterStatementsTyped(ReadStatement, AnyStatement)
                        == Vector<Integer>{2, 3, 4});

                // select s such that Follows(s,p): 5
                REQUIRE(followsTable.getAllBeforeStatementsTyped(AnyStatement, PrintStatement) == Vector<Integer>{5});
                // select p such that Follows(s,p): 6
                REQUIRE(followsTable.getAllAfterStatementsTyped(AnyStatement, PrintStatement) == Vector<Integer>{6});
                // select s such that Follows(p,s): None
                REQUIRE(followsTable.getAllBeforeStatementsTyped(PrintStatement, AnyStatement).empty());
                // select p such that Follows(p,s): None
                REQUIRE(followsTable.getAllAfterStatementsTyped(PrintStatement, AnyStatement).empty());
            }

            // procedure compute {
            //    1.  read num1;
            //    2.  read num2;
            //    3.  read num3;
            //    4.  sum = (num1 + num2) * num3;
            //    5.  ave = sum / 3;
            //    6.  print ave;
            //}
            THEN("second-order relationships are correct (star)")
            {
                // select s such that Follows*(s,s1): 1,2,3,4,5
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, AnyStatement)
                        == Vector<Integer>{1, 2, 3, 4, 5});
                // select s1 such that Follows*(s,s1): 1,2,3,4,5
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, AnyStatement)
                        == Vector<Integer>{2, 3, 4, 5, 6});

                // select a such that Follows*(a,a1): 4
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AssignmentStatement, AssignmentStatement)
                        == Vector<Integer>{4});
                // select a1 such that Follows*(a,a1): 5
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AssignmentStatement, AssignmentStatement)
                        == Vector<Integer>{5});

                // select re such that Follows*(re,re1): 1,2
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(ReadStatement, ReadStatement)
                        == Vector<Integer>{1, 2});
                // select re1 such that Follows*(re,re1): 2,3
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(ReadStatement, ReadStatement)
                        == Vector<Integer>{2, 3});

                // select s such that Follows*(s,a): 3,4
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, AssignmentStatement)
                        == Vector<Integer>{1, 2, 3, 4});
                // select a such that Follows*(s,a): 4,5
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, AssignmentStatement)
                        == Vector<Integer>{4, 5});
                // select a such that Follows*(a,s): 4,5
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AssignmentStatement, AnyStatement)
                        == Vector<Integer>{4, 5});
                // select s such that Follows*(a,s): 5,6
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AssignmentStatement, AnyStatement)
                        == Vector<Integer>{5, 6});

                // select s such that Follows*(s,r): 1,2
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, ReadStatement)
                        == Vector<Integer>{1, 2});
                // select r such that Follows*(s,r): 2,3
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, ReadStatement)
                        == Vector<Integer>{2, 3});
                // select r such that Follows*(r,s): 1,2,3
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(ReadStatement, AnyStatement)
                        == Vector<Integer>{1, 2, 3});
                // select s such that Follows*(r,s): 2,3,4
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(ReadStatement, AnyStatement)
                        == Vector<Integer>{2, 3, 4, 5, 6});

                // select s such that Follows*(s,p): 5
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, PrintStatement)
                        == Vector<Integer>{1, 2, 3, 4, 5});
                // select p such that Follows*(s,p): 6
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, PrintStatement)
                        == Vector<Integer>{6});
                // select s such that Follows*(p,s): None
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(PrintStatement, AnyStatement).empty());
                // select p such that Follows*(p,s): None
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(PrintStatement, AnyStatement).empty());
            }

            THEN("second-order tuple relationships are correct (no star)")
            {
                auto v12345 = makeTupleVector(Vector<Integer>{1, 2, 3, 4, 5});
                // select s such that Follows(s,s1): 1,2,3,4,5
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AnyStatement, AnyStatement),
                             Catch::UnorderedEquals(v12345));
                // select s1 such that Follows(s,s1): 1,2,3,4,5
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AnyStatement, AnyStatement),
                             Catch::UnorderedEquals(v12345));

                auto v4 = makeTupleVector(Vector<Integer>{4});
                // select a such that Follows(a,a1): 4
                REQUIRE(followsTable.getAllFollowsTuple(AssignmentStatement, AssignmentStatement) == v4);
                // select a1 such that Follows(a,a1): 5
                REQUIRE(followsTable.getAllFollowsTuple(AssignmentStatement, AssignmentStatement) == v4);

                auto v12 = makeTupleVector(Vector<Integer>{1, 2});
                // select re such that Follows(re,re1): 1,2
                REQUIRE_THAT(followsTable.getAllFollowsTuple(ReadStatement, ReadStatement),
                             Catch::UnorderedEquals(v12));
                // select re1 such that Follows(re,re1): 2,3
                REQUIRE_THAT(followsTable.getAllFollowsTuple(ReadStatement, ReadStatement),
                             Catch::UnorderedEquals(v12));

                auto v34 = makeTupleVector(Vector<Integer>{3, 4});
                // select s such that Follows(s,a): 3,4
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AnyStatement, AssignmentStatement),
                             Catch::UnorderedEquals(v34));
                // select a such that Follows(s,a): 4,5
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AnyStatement, AssignmentStatement),
                             Catch::UnorderedEquals(v34));

                auto v45 = makeTupleVector(Vector<Integer>{4, 5});
                // select a such that Follows(a,s): 4,5
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AssignmentStatement, AnyStatement),
                             Catch::UnorderedEquals(v45));
                // select s such that Follows(a,s): 5,6
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AssignmentStatement, AnyStatement),
                             Catch::UnorderedEquals(v45));

                // select s such that Follows(s,r): 1,2
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AnyStatement, ReadStatement), Catch::UnorderedEquals(v12));
                // select r such that Follows(s,r): 2,3
                REQUIRE_THAT(followsTable.getAllFollowsTuple(AnyStatement, ReadStatement), Catch::UnorderedEquals(v12));

                auto v123 = makeTupleVector(Vector<Integer>{1, 2, 3});
                // select r such that Follows(r,s): 1,2,3
                REQUIRE_THAT(followsTable.getAllFollowsTuple(ReadStatement, AnyStatement),
                             Catch::UnorderedEquals(v123));
                // select s such that Follows(r,s): 2,3,4
                REQUIRE_THAT(followsTable.getAllFollowsTuple(ReadStatement, AnyStatement),
                             Catch::UnorderedEquals(v123));

                auto v5 = makeTupleVector(Vector<Integer>{5});
                // select s such that Follows(s,p): 5
                REQUIRE(followsTable.getAllFollowsTuple(AnyStatement, PrintStatement) == v5);
                // select p such that Follows(s,p): 6
                REQUIRE(followsTable.getAllFollowsTuple(AnyStatement, PrintStatement) == v5);
                // select s such that Follows(p,s): None
                REQUIRE(followsTable.getAllFollowsTuple(PrintStatement, AnyStatement).empty());
                // select p such that Follows(p,s): None
                REQUIRE(followsTable.getAllFollowsTuple(PrintStatement, AnyStatement).empty());
            }

            // procedure compute {
            //    1.  read num1;
            //    2.  read num2;
            //    3.  read num3;
            //    4.  sum = (num1 + num2) * num3;
            //    5.  ave = sum / 3;
            //    6.  print ave;
            //}
            THEN("second-order tuple relationships are correct (star)")
            {
                // select s such that Follows*(s,s1): 1,2,3,4,5
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, AnyStatement)
                        == Vector<Integer>{1, 2, 3, 4, 5});
                // select s1 such that Follows*(s,s1): 1,2,3,4,5
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, AnyStatement)
                        == Vector<Integer>{2, 3, 4, 5, 6});

                // select a such that Follows*(a,a1): 4
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AssignmentStatement, AssignmentStatement)
                        == Vector<Integer>{4});
                // select a1 such that Follows*(a,a1): 5
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AssignmentStatement, AssignmentStatement)
                        == Vector<Integer>{5});

                // select re such that Follows*(re,re1): 1,2
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(ReadStatement, ReadStatement)
                        == Vector<Integer>{1, 2});
                // select re1 such that Follows*(re,re1): 2,3
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(ReadStatement, ReadStatement)
                        == Vector<Integer>{2, 3});

                // select s such that Follows*(s,a): 3,4
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, AssignmentStatement)
                        == Vector<Integer>{1, 2, 3, 4});
                // select a such that Follows*(s,a): 4,5
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, AssignmentStatement)
                        == Vector<Integer>{4, 5});
                // select a such that Follows*(a,s): 4,5
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AssignmentStatement, AnyStatement)
                        == Vector<Integer>{4, 5});
                // select s such that Follows*(a,s): 5,6
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AssignmentStatement, AnyStatement)
                        == Vector<Integer>{5, 6});

                // select s such that Follows*(s,r): 1,2
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, ReadStatement)
                        == Vector<Integer>{1, 2});
                // select r such that Follows*(s,r): 2,3
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, ReadStatement)
                        == Vector<Integer>{2, 3});
                // select r such that Follows*(r,s): 1,2,3
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(ReadStatement, AnyStatement)
                        == Vector<Integer>{1, 2, 3});
                // select s such that Follows*(r,s): 2,3,4
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(ReadStatement, AnyStatement)
                        == Vector<Integer>{2, 3, 4, 5, 6});

                // select s such that Follows*(s,p): 5
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(AnyStatement, PrintStatement)
                        == Vector<Integer>{1, 2, 3, 4, 5});
                // select p such that Follows*(s,p): 6
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(AnyStatement, PrintStatement)
                        == Vector<Integer>{6});
                // select s such that Follows*(p,s): None
                REQUIRE(followsTable.getAllBeforeStatementsTypedStar(PrintStatement, AnyStatement).empty());
                // select p such that Follows*(p,s): None
                REQUIRE(followsTable.getAllAfterStatementsTypedStar(PrintStatement, AnyStatement).empty());
            }
        }
    }
}
