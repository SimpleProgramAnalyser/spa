#include "catch.hpp"
#include "pkb/relationships/Modifies.h"

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

SCENARIO("Iteration 1 toy example Modifies", "[uses][pkb]")
{
    ModifiesTable usesTable = ModifiesTable();
    GIVEN("some Modifies relationships")
    {
        Vector<Pair<Integer, String>> readTuples{p(1, "num1"), p(2, "num2"), p(3, "num3")};
        Vector<Pair<Integer, String>> assignmentTuples{p(4, "sum"), p(5, "ave")};
        Vector<Pair<Integer, String>> allTuples = readTuples;
        allTuples.insert(allTuples.end(), assignmentTuples.begin(), assignmentTuples.end());
        Vector<Pair<String, String>> procTuples{p("compute", "num1"), p("compute", "num2"), p("compute", "num3"),
                                                p("compute", "sum"), p("compute", "ave")};

        WHEN("relationships are added to Modifies table")
        {
            usesTable.addModifiesRelationships(1, ReadStatement, Vector<String>{"num1"});
            usesTable.addModifiesRelationships(2, ReadStatement, Vector<String>{"num2"});
            usesTable.addModifiesRelationships(3, ReadStatement, Vector<String>{"num3"});
            usesTable.addModifiesRelationships(4, AssignmentStatement, Vector<String>{"sum"});
            usesTable.addModifiesRelationships(5, AssignmentStatement, Vector<String>{"ave"});
            usesTable.addModifiesRelationships("compute", Vector<String>{"num1", "num2", "num3", "sum", "ave"});

            THEN("tables should be populated")
            {
                REQUIRE_FALSE(usesTable.getAllModifiesProcedures().empty());
                REQUIRE_FALSE(usesTable.getAllModifiesVariablesFromProgram().empty());
                REQUIRE_FALSE(usesTable.getAllModifiesVariablesFromStatementType(AnyStatement).empty());
                REQUIRE_FALSE(usesTable.getAllModifiesVariablesFromStatementType(AssignmentStatement).empty());
                REQUIRE_FALSE(usesTable.getAllModifiesVariablesFromStatementType(ReadStatement).empty());
                REQUIRE_FALSE(usesTable.getAllModifiesStatements(AnyStatement).empty());
                REQUIRE_FALSE(usesTable.getAllModifiesStatements(AssignmentStatement).empty());
                REQUIRE_FALSE(usesTable.getAllModifiesStatements(ReadStatement).empty());
                // tuples
                REQUIRE_FALSE(usesTable.getAllModifiesStatementTuple(AnyStatement).empty());
                REQUIRE_FALSE(usesTable.getAllModifiesStatementTuple(AssignmentStatement).empty());
                REQUIRE_FALSE(usesTable.getAllModifiesStatementTuple(ReadStatement).empty());
            }

            THEN("irrelevant tables should be empty")
            {
                REQUIRE(usesTable.getAllModifiesStatements(CallStatement).empty());
                REQUIRE(usesTable.getAllModifiesStatements(WhileStatement).empty());
                REQUIRE(usesTable.getAllModifiesStatements(IfStatement).empty());
                REQUIRE(usesTable.getAllModifiesVariablesFromStatementType(CallStatement).empty());
                REQUIRE(usesTable.getAllModifiesVariablesFromStatementType(WhileStatement).empty());
                REQUIRE(usesTable.getAllModifiesVariablesFromStatementType(IfStatement).empty());
                // tuple
                REQUIRE(usesTable.getAllModifiesStatementTuple(CallStatement).empty());
                REQUIRE(usesTable.getAllModifiesStatementTuple(WhileStatement).empty());
                REQUIRE(usesTable.getAllModifiesStatementTuple(IfStatement).empty());
            }

            THEN("non-existent procedures should have negative results")
            {
                REQUIRE_FALSE(usesTable.checkIfProcedureModifies("wrong_proc", "num1"));
                REQUIRE(usesTable.getModifiesVariablesFromProcedure("wrong_proc").empty());
            }

            THEN("non-existent statements should have negative results")
            {
                REQUIRE_FALSE(usesTable.checkIfStatementModifies(100, "num1"));
                REQUIRE(usesTable.getModifiesVariablesFromStatement(100).empty());
            }

            THEN("non-existent variables should have negative results")
            {
                REQUIRE_FALSE(usesTable.checkIfProcedureModifies("compute", "wrong_var"));
                REQUIRE_FALSE(usesTable.checkIfStatementModifies(4, "wrong_var"));
            }

            THEN("correct statement-variable relationships are retrieved")
            {
                auto ans1 = Vector<String>{"num1"};
                REQUIRE(usesTable.getModifiesVariablesFromStatement(1) == (ans1));
                auto ans2 = Vector<String>{"num2"};
                REQUIRE(usesTable.getModifiesVariablesFromStatement(2) == (ans2));
                auto ans3 = Vector<String>{"num3"};
                REQUIRE(usesTable.getModifiesVariablesFromStatement(3) == (ans3));
                auto ans4 = Vector<String>{"sum"};
                REQUIRE(usesTable.getModifiesVariablesFromStatement(4) == ans4);
                auto ans5 = Vector<String>{"ave"};
                REQUIRE(usesTable.getModifiesVariablesFromStatement(5) == ans5);
                auto ans6 = Vector<String>{"num1", "num2", "num3", "ave", "sum"};
                REQUIRE_THAT(usesTable.getAllModifiesVariablesFromStatementType(AnyStatement),
                             Catch::UnorderedEquals(ans6));
                auto ans7 = Vector<String>{"sum", "ave"};
                REQUIRE_THAT(usesTable.getAllModifiesVariablesFromStatementType(AssignmentStatement),
                             Catch::UnorderedEquals(ans7));
                auto ans8 = Vector<String>{"num1", "num2", "num3"};
                REQUIRE_THAT(usesTable.getAllModifiesVariablesFromStatementType(ReadStatement),
                             Catch::UnorderedEquals(ans8));
            }

            THEN("correct variable-statement relationships are retrieved")
            {
                REQUIRE(usesTable.getModifiesStatements("num1", AnyStatement) == Vector<Integer>{1});
                REQUIRE(usesTable.getModifiesStatements("num2", AnyStatement) == Vector<Integer>{2});
                REQUIRE(usesTable.getModifiesStatements("num3", AnyStatement) == Vector<Integer>{3});
                REQUIRE(usesTable.getModifiesStatements("num1", ReadStatement) == Vector<Integer>{1});
                REQUIRE_FALSE(usesTable.getModifiesStatements("num1", AssignmentStatement) == Vector<Integer>{1});
                REQUIRE(usesTable.getModifiesStatements("sum", AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getModifiesStatements("ave", AnyStatement) == Vector<Integer>{5});
                REQUIRE(usesTable.getModifiesStatements("ave", AssignmentStatement) == Vector<Integer>{5});
            }

            THEN("correct tuples are retrieved")
            {
                REQUIRE_THAT(usesTable.getAllModifiesStatementTuple(AnyStatement), Catch::UnorderedEquals(allTuples));
                REQUIRE_THAT(usesTable.getAllModifiesStatementTuple(AssignmentStatement),
                             Catch::UnorderedEquals(assignmentTuples));
                REQUIRE_THAT(usesTable.getAllModifiesStatementTuple(ReadStatement), Catch::UnorderedEquals(readTuples));
                REQUIRE_THAT(usesTable.getAllModifiesProcedureTuple(), Catch::UnorderedEquals(procTuples));
            }

            THEN("correct procedure-variable relationships are retrieved")
            {
                auto ans = Vector<String>{"num1", "num2", "num3", "ave", "sum"};
                REQUIRE_THAT(usesTable.getModifiesVariablesFromProcedure("compute"), Catch::UnorderedEquals(ans));
                REQUIRE_THAT(usesTable.getAllModifiesVariablesFromProgram(), Catch::UnorderedEquals(ans));
            }

            THEN("correct procedure-variable relationships are retrieved")
            {
                auto ans = Vector<String>{"num1", "num2", "num3", "sum", "ave"};
                for (const auto& var : ans) {
                    REQUIRE(usesTable.getModifiesProcedures(var) == Vector<String>{"compute"});
                }
            }
        }
    }
}
