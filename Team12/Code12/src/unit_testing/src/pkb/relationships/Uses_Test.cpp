#include "catch.hpp"
#include "pkb/relationships/Uses.h"

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

#define p(x, y) std::make_pair((x), (y))

SCENARIO("Iteration 1 toy example Uses", "[uses][pkb]")
{
    UsesTable usesTable = UsesTable();
    GIVEN("some Uses relationships")
    {
        Vector<Pair<Integer, String>> assignmentTuples{p(4, "num1"), p(4, "num2"), p(4, "num3"), p(5, "sum")};
        Vector<Pair<Integer, String>> printTuples{p(6, "ave")};
        Vector<Pair<Integer, String>> allTuples = assignmentTuples;
        allTuples.insert(allTuples.end(), printTuples.begin(), printTuples.end());
        Vector<Pair<String, String>> procTuples = {p("compute", "num1"), p("compute", "num2"), p("compute", "num3"),
                                                   p("compute", "sum"), p("compute", "ave")};

        WHEN("relationships are added to Uses table")
        {
            usesTable.addUsesRelationships(4, AssignmentStatement, Vector<String>{"num1", "num2", "num3"});
            usesTable.addUsesRelationships(5, AssignmentStatement, Vector<String>{"sum"});
            usesTable.addUsesRelationships(6, PrintStatement, Vector<String>{"ave"});
            usesTable.addUsesRelationships("compute", Vector<String>{"num1", "num2", "num3", "ave", "sum"});

            THEN("tables should be populated")
            {
                REQUIRE_FALSE(usesTable.getAllUsesProcedures().empty());
                REQUIRE_FALSE(usesTable.getAllUsesVariablesFromProgram().empty());
                REQUIRE_FALSE(usesTable.getAllUsesVariablesFromStatementType(AnyStatement).empty());
                REQUIRE_FALSE(usesTable.getAllUsesVariablesFromStatementType(AssignmentStatement).empty());
                REQUIRE_FALSE(usesTable.getAllUsesVariablesFromStatementType(PrintStatement).empty());
                REQUIRE_FALSE(usesTable.getAllUsesStatements(AnyStatement).empty());
                REQUIRE_FALSE(usesTable.getAllUsesStatements(AssignmentStatement).empty());
                REQUIRE_FALSE(usesTable.getAllUsesStatements(PrintStatement).empty());
                // tuples
                REQUIRE_FALSE(usesTable.getAllUsesStatementTuple(AnyStatement).empty());
                REQUIRE_FALSE(usesTable.getAllUsesStatementTuple(AssignmentStatement).empty());
                REQUIRE_FALSE(usesTable.getAllUsesStatementTuple(PrintStatement).empty());
            }

            THEN("irrelevant tables should be empty")
            {
                REQUIRE(usesTable.getAllUsesStatements(CallStatement).empty());
                REQUIRE(usesTable.getAllUsesStatements(WhileStatement).empty());
                REQUIRE(usesTable.getAllUsesStatements(IfStatement).empty());
                REQUIRE(usesTable.getAllUsesVariablesFromStatementType(CallStatement).empty());
                REQUIRE(usesTable.getAllUsesVariablesFromStatementType(WhileStatement).empty());
                REQUIRE(usesTable.getAllUsesVariablesFromStatementType(IfStatement).empty());
                // tuples
                REQUIRE(usesTable.getAllUsesStatementTuple(CallStatement).empty());
                REQUIRE(usesTable.getAllUsesStatementTuple(WhileStatement).empty());
                REQUIRE(usesTable.getAllUsesStatementTuple(IfStatement).empty());
            }

            THEN("non-existent procedures should have negative results")
            {
                REQUIRE_FALSE(usesTable.checkIfProcedureUses("wrong_proc", "num1"));
                REQUIRE(usesTable.getUsesVariablesFromProcedure("wrong_proc").empty());
            }

            THEN("non-existent statements should have negative results")
            {
                REQUIRE_FALSE(usesTable.checkIfStatementUses(100, "num1"));
                REQUIRE(usesTable.getUsesVariablesFromStatement(100).empty());
            }

            THEN("non-existent variables should have negative results")
            {
                REQUIRE_FALSE(usesTable.checkIfProcedureUses("compute", "wrong_var"));
                REQUIRE_FALSE(usesTable.checkIfStatementUses(4, "wrong_var"));
            }

            THEN("correct statement-variable relationships are retrieved")
            {
                auto ans1 = Vector<String>{"num1", "num2", "num3"};
                REQUIRE_THAT(usesTable.getUsesVariablesFromStatement(4), Catch::UnorderedEquals(ans1));
                auto ans2 = Vector<String>{"sum"};
                REQUIRE(usesTable.getUsesVariablesFromStatement(5) == ans2);
                auto ans3 = Vector<String>{"ave"};
                REQUIRE(usesTable.getUsesVariablesFromStatement(6) == ans3);
                auto ans4 = Vector<String>{"num1", "num2", "num3", "ave", "sum"};
                REQUIRE_THAT(usesTable.getAllUsesVariablesFromStatementType(AnyStatement),
                             Catch::UnorderedEquals(ans4));
                auto ans5 = Vector<String>{"num1", "num2", "num3", "sum"};
                REQUIRE_THAT(usesTable.getAllUsesVariablesFromStatementType(AssignmentStatement),
                             Catch::UnorderedEquals(ans5));
                REQUIRE_THAT(usesTable.getAllUsesVariablesFromStatementType(PrintStatement),
                             Catch::UnorderedEquals(ans3));
            }

            THEN("correct variable-statement relationships are retrieved")
            {
                REQUIRE(usesTable.getUsesStatements("num1", AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("num2", AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("num3", AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("num1", AssignmentStatement) == Vector<Integer>{4});
                REQUIRE_FALSE(usesTable.getUsesStatements("num1", PrintStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("sum", AnyStatement) == Vector<Integer>{5});
                REQUIRE(usesTable.getUsesStatements("ave", AnyStatement) == Vector<Integer>{6});
                REQUIRE(usesTable.getUsesStatements("ave", PrintStatement) == Vector<Integer>{6});
            }

            // procedure compute {
            //    1.  read num1;
            //    2.  read num2;
            //    3.  read num3;
            //    4.  sum = (num1 + num2) * num3;
            //    5.  ave = sum / 3;
            //    6.  print ave;
            //}
            THEN("correct tuples are retrieved")
            {
                REQUIRE_THAT(usesTable.getAllUsesStatementTuple(AnyStatement), Catch::UnorderedEquals(allTuples));
                REQUIRE_THAT(usesTable.getAllUsesStatementTuple(AssignmentStatement),
                             Catch::UnorderedEquals(assignmentTuples));
                REQUIRE_THAT(usesTable.getAllUsesStatementTuple(PrintStatement), Catch::UnorderedEquals(printTuples));
                REQUIRE_THAT(usesTable.getAllUsesProcedureTuple(), Catch::UnorderedEquals(procTuples));
            }

            THEN("correct procedure-variable relationships are retrieved")
            {
                auto ans = Vector<String>{"num1", "num2", "num3", "ave", "sum"};
                REQUIRE_THAT(usesTable.getUsesVariablesFromProcedure("compute"), Catch::UnorderedEquals(ans));
            }

            THEN("correct procedure-variable relationships are retrieved")
            {
                auto ans = Vector<String>{"num1", "num2", "num3", "ave"};
                for (const auto& var : ans) {
                    REQUIRE(usesTable.getUsesProcedures(var) == Vector<String>{"compute"});
                }
            }
        }
    }
}