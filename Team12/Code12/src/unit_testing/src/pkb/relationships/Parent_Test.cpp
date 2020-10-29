#include <catch.hpp>
#include <pkb/relationships/Parent.h>

#define p(x, y) std::make_pair((x), (y))
#define AS AssignmentStatement
#define RS ReadStatement
#define PS PrintStatement
#define WS WhileStatement
#define IS IfStatement
#define CS CallStatement

/*
Annotated with statement numbers:
procedure main {
1.     read steps;
2.     call raymarch;
3.     print depth; }
procedure raymarch {
4.     ro = 13;
5.     rd = 19;
6.     read depth;
7.     while (count < steps) {
8.            print depth;
9.            po = ro + rd * depth;
10.          call spheresdf;
11.          if (dist < epsilon) then {
12.                  done = depth; }
            else {
13.                  depth = depth + dist;}
14.          count = count + 1; }}
procedure spheresdf {
15.     dist = x * x + y * y + z * z;
16.     x = dist;
17.     depth = depth;
18.     read p;
19.     while (x != p) {
20.            p = x;
21.            x = (dist / x + x) / 2; }
22.      dist = x - 1;
23.      x = x * x + y * y / 2; }
*/

// Parent
// 7: 8,9,10,11,14
// 11: 12 13
// 19: 20,21
SCENARIO("Iteration 1 Parent", "[follows][pkb]")
{
    ParentTable parentTable = ParentTable();
    GIVEN("some Parent relationships")
    {
        // for easy retrieval of statement type
        HashMap<Integer, StatementType> statements
            = {p(1, RS),  p(2, CS),  p(3, PS),  p(4, AS),  p(5, AS),  p(6, RS),  p(7, WS),  p(8, PS),
               p(9, AS),  p(10, CS), p(11, IS), p(12, AS), p(13, AS), p(14, AS), p(15, AS), p(16, AS),
               p(17, AS), p(18, AS), p(19, WS), p(20, AS), p(21, AS), p(22, AS), p(23, AS)};
        // parent
        Vector<Pair<Integer, Vector<Integer>>> toAdd
            = {p(7, (Vector<Integer>{8, 9, 10, 11, 14})), p(11, (Vector<Integer>{12, 13})),
               p(19, (Vector<Integer>{20, 21}))};
        // parent*
        Vector<Pair<Integer, Vector<Integer>>> toAddStar
            = {p(7, (Vector<Integer>{8, 9, 10, 11, 12, 13, 14})), p(11, (Vector<Integer>{12, 13})),
               p(19, (Vector<Integer>{20, 21}))};
        WHEN("relationships are added to Parent table")
        {
            for (const auto& parentChildren : toAdd) {
                for (auto child : parentChildren.second) {
                    auto parent = parentChildren.first;
                    parentTable.addParentRelationships(parent, statements[parent], child, statements[child]);
                }
            }
            for (const auto& parentChildren : toAddStar) {
                Integer parent = parentChildren.first;
                Vector<Integer> children = parentChildren.second;
                Vector<Pair<Integer, StatementType>> childrenWithType;
                std::transform(children.begin(), children.end(), std::back_inserter(childrenWithType),
                               [statements](Integer i) { return p(i, statements.find(i)->second); });
                parentTable.addParentRelationshipsStar(parent, statements[parent], childrenWithType);
            }

            THEN("tables should be populated")
            {
                REQUIRE_FALSE(parentTable.getAllParentStatementsStar(14, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentStatementsStar(12, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentStatementsStar(20, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllChildStatementsStar(7, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllChildStatementsStar(11, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllChildStatementsStar(19, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllChildStatementsTyped(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentStatementsTyped(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllChildStatementsTypedStar(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentStatementsTypedStar(AnyStatement, AnyStatement).empty());
                // tuples
                Vector<StatementType> containerTypes = {IfStatement, WhileStatement};
                REQUIRE_FALSE(parentTable.getAllParentTuple(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentTuple(WhileStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentTuple(IfStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentTupleStar(AnyStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentTupleStar(WhileStatement, AnyStatement).empty());
                REQUIRE_FALSE(parentTable.getAllParentTupleStar(IfStatement, AnyStatement).empty());
            }

            THEN("irrelevant tables should be empty")
            {
                Vector<StatementType> nonContainerTypes
                    = {CallStatement, ReadStatement, PrintStatement, AssignmentStatement};
                for (StatementType nonContainerType : nonContainerTypes) {
                    for (int i = 0; i < StatementTypeCount; i++) {
                        REQUIRE(parentTable.getAllParentStatementsTyped(nonContainerType, StatementType(i)).empty());
                        REQUIRE(parentTable.getAllChildStatementsTyped(nonContainerType, StatementType(i)).empty());
                        REQUIRE(
                            parentTable.getAllParentStatementsTypedStar(nonContainerType, StatementType(i)).empty());
                        REQUIRE(parentTable.getAllChildStatementsTypedStar(nonContainerType, StatementType(i)).empty());
                        REQUIRE(parentTable.getAllParentTuple(nonContainerType, StatementType(i)).empty());
                        REQUIRE(parentTable.getAllParentTupleStar(nonContainerType, StatementType(i)).empty());
                    }
                }
            }

            THEN("first-order relationships are correct")
            {
                for (const auto& parentChildren : toAdd) {
                    for (auto child : parentChildren.second) {
                        auto parent = parentChildren.first;
                        parentTable.checkIfParentHolds(parent, child);
                    }
                }
                for (const auto& parentChildren : toAddStar) {
                    Integer parent = parentChildren.first;
                    for (auto child : parentChildren.second) {
                        parentTable.checkIfParentHoldsStar(parent, child);
                    }
                }
            }

            THEN("second-order relationships are correct (no star)")
            {
                // select s such that Parent(s, s1): 7,11,19
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(AnyStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7, 11, 19}));
                // select s1 such that Parent(s, s1):
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(AnyStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{8, 9, 10, 11, 14, 12, 13, 20, 21}));

                // select w such that Parent(w, s):
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(WhileStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7, 19}));
                // select s such that Parent(w, s)
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(WhileStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{8, 9, 10, 11, 14, 20, 21}));

                // select w such that Parent(w, a):
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(WhileStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7, 19}));
                // select a such that Parent(w, a)
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(WhileStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{9, 14, 20, 21}));

                // select w such that Parent(w, p)
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(WhileStatement, PrintStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7}));
                // select p such that Parent(w, p)
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(WhileStatement, PrintStatement),
                             Catch::UnorderedEquals(Vector<Integer>{8}));

                // select w such that Parent(w, c)
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7}));
                // select c such that Parent(w, c)
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{10}));

                // select w such that Parent(w, i)
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7}));
                // select i such that Parent(w, i)
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{11}));

                // select i such that Parent(i, s)
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(IfStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{11}));
                // select s such that Parent(i, s)
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(IfStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{12, 13}));

                // select i such that Parent(i, a)
                REQUIRE_THAT(parentTable.getAllParentStatementsTyped(IfStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{11}));
                // select a such that Parent(i, a)
                REQUIRE_THAT(parentTable.getAllChildStatementsTyped(IfStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{12, 13}));
            }

            THEN("second-order relationships are correct (star)")
            {
                // select s such that Parent*(s, s1): 7,11,19
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(AnyStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7, 11, 19}));
                // select s1 such that Parent*(s, s1):
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(AnyStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{8, 9, 10, 11, 14, 12, 13, 20, 21}));

                // select w such that Parent*(w, s):
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(WhileStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7, 19}));
                // select s such that Parent*(w, s)
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(WhileStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{8, 9, 10, 11, 14, 12, 13, 20, 21}));

                // select w such that Parent*(w, a):
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(WhileStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7, 19}));
                // select a such that Parent*(w, a)
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(WhileStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{9, 12, 13, 14, 20, 21}));

                // select w such that Parent*(w, p)
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(WhileStatement, PrintStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7}));
                // select p such that Parent*(w, p)
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(WhileStatement, PrintStatement),
                             Catch::UnorderedEquals(Vector<Integer>{8}));

                // select w such that Parent*(w, c)
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7}));
                // select c such that Parent*(w, c)
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{10}));

                // select w such that Parent*(w, i)
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{7}));
                // select i such that Parent*(w, i)
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(Vector<Integer>{11}));

                // select i such that Parent*(i, s)
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(IfStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{11}));
                // select s such that Parent*(i, s)
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(IfStatement, AnyStatement),
                             Catch::UnorderedEquals(Vector<Integer>{12, 13}));

                // select i such that Parent*(i, a)
                REQUIRE_THAT(parentTable.getAllParentStatementsTypedStar(IfStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{11}));
                // select a such that Parent*(i, a)
                REQUIRE_THAT(parentTable.getAllChildStatementsTypedStar(IfStatement, AssignmentStatement),
                             Catch::UnorderedEquals(Vector<Integer>{12, 13}));
            }

            // Parent
            // w7: 8,9,10,11,14
            // i11: 12 13
            // w19: 20,21
            THEN("second-order tuple relationships are correct (no star)")
            {
                // select <s,s1> such that Parent(s, s1): 7,11,19
                Vector<Pair<Integer, Integer>> ss1
                    = {p(7, 8), p(7, 9), p(7, 10), p(7, 11), p(7, 14), p(11, 12), p(11, 13), p(19, 20), p(19, 21)};
                REQUIRE_THAT(parentTable.getAllParentTuple(AnyStatement, AnyStatement), Catch::UnorderedEquals(ss1));

                // select <w,s> such that Parent(w, s):
                Vector<Pair<Integer, Integer>> ws
                    = {p(7, 8), p(7, 9), p(7, 10), p(7, 11), p(7, 14), p(19, 20), p(19, 21)};
                REQUIRE_THAT(parentTable.getAllParentTuple(WhileStatement, AnyStatement), Catch::UnorderedEquals(ws));

                // select <w,a> such that Parent(w, a):
                Vector<Pair<Integer, Integer>> wa = {p(7, 9), p(7, 14), p(19, 20), p(19, 21)};
                REQUIRE_THAT(parentTable.getAllParentTuple(WhileStatement, AssignmentStatement),
                             Catch::UnorderedEquals(wa));

                // select <w,p> such that Parent(w, p)
                Vector<Pair<Integer, Integer>> wp = {p(7, 8)};
                REQUIRE_THAT(parentTable.getAllParentTuple(WhileStatement, PrintStatement), Catch::UnorderedEquals(wp));

                // select <w,c> such that Parent(w, c)
                Vector<Pair<Integer, Integer>> wc = {p(7, 10)};
                REQUIRE_THAT(parentTable.getAllParentTuple(WhileStatement, CallStatement), Catch::UnorderedEquals(wc));

                // select <w,i> such that Parent(w, i)
                Vector<Pair<Integer, Integer>> wi = {p(7, 11)};
                REQUIRE_THAT(parentTable.getAllParentTuple(WhileStatement, CallStatement), Catch::UnorderedEquals(wi));

                // select <i,s> such that Parent(i, s)
                Vector<Pair<Integer, Integer>> is = {p(11, 12), p(11, 13)};
                REQUIRE_THAT(parentTable.getAllParentTuple(IfStatement, AnyStatement), Catch::UnorderedEquals(is));

                // select i such that Parent(i, a)
                REQUIRE_THAT(parentTable.getAllParentTuple(IfStatement, AssignmentStatement),
                             Catch::UnorderedEquals(is));
            }

            THEN("second-order tuple relationships are correct (star)")
            {
                // select <s,s1> such that Parent*(s, s1): 7,11,19
                Vector<Pair<Integer, Integer>> ss1 = {p(7, 8),  p(7, 9),   p(7, 10),  p(7, 11),  p(7, 12), p(7, 13),
                                                      p(7, 14), p(11, 12), p(11, 13), p(19, 20), p(19, 21)};
                REQUIRE_THAT(parentTable.getAllParentTupleStar(AnyStatement, AnyStatement),
                             Catch::UnorderedEquals(ss1));

                // select <w,s> such that Parent*(w, s):
                Vector<Pair<Integer, Integer>> ws
                    = {p(7, 8), p(7, 9), p(7, 10), p(7, 11), p(7, 12), p(7, 13), p(7, 14), p(19, 20), p(19, 21)};
                REQUIRE_THAT(parentTable.getAllParentTupleStar(WhileStatement, AnyStatement),
                             Catch::UnorderedEquals(ws));

                // select <w,a> such that Parent*(w, a):
                Vector<Pair<Integer, Integer>> wa = {p(7, 9), p(7, 12), p(7, 13), p(7, 14), p(19, 20), p(19, 21)};
                REQUIRE_THAT(parentTable.getAllParentTupleStar(WhileStatement, AssignmentStatement),
                             Catch::UnorderedEquals(wa));

                // select <w,p> such that Parent*(w, p)
                Vector<Pair<Integer, Integer>> wp = {p(7, 8)};
                REQUIRE_THAT(parentTable.getAllParentTupleStar(WhileStatement, PrintStatement),
                             Catch::UnorderedEquals(wp));

                // select <w,c> such that Parent*(w, c)
                Vector<Pair<Integer, Integer>> wc = {p(7, 10)};
                REQUIRE_THAT(parentTable.getAllParentTupleStar(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(wc));

                // select <w,i> such that Parent*(w, i)
                Vector<Pair<Integer, Integer>> wi = {p(7, 11)};
                REQUIRE_THAT(parentTable.getAllParentTupleStar(WhileStatement, CallStatement),
                             Catch::UnorderedEquals(wi));

                // select <i,s> such that Parent*(i, s)
                Vector<Pair<Integer, Integer>> is = {p(11, 12), p(11, 13)};
                REQUIRE_THAT(parentTable.getAllParentTupleStar(IfStatement, AnyStatement), Catch::UnorderedEquals(is));

                // select i such that Parent*(i, a)
                REQUIRE_THAT(parentTable.getAllParentTupleStar(IfStatement, AssignmentStatement),
                             Catch::UnorderedEquals(is));
            }
        }
    }
}
