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

SCENARIO("Iteration 1 toy example Modifies", "[uses][pkb]")
{
    ModifiesTable usesTable = ModifiesTable();
    GIVEN("some Modifies relationships")
    {
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
                REQUIRE(!usesTable.getAllModifiesProcedures().empty());
                REQUIRE(!usesTable.getAllModifiesVariablesFromProgram().empty());
                REQUIRE(!usesTable.getAllModifiesVariablesFromStatementType(AnyStatement).empty());
                REQUIRE(!usesTable.getAllModifiesVariablesFromStatementType(AssignmentStatement).empty());
                REQUIRE(!usesTable.getAllModifiesVariablesFromStatementType(ReadStatement).empty());
                REQUIRE(!usesTable.getAllModifiesStatements(AnyStatement).empty());
                REQUIRE(!usesTable.getAllModifiesStatements(AssignmentStatement).empty());
                REQUIRE(!usesTable.getAllModifiesStatements(ReadStatement).empty());
            }

            THEN("irrelevant tables should be empty")
            {
                REQUIRE(usesTable.getAllModifiesStatements(StatementType::CallStatement).empty());
                REQUIRE(usesTable.getAllModifiesStatements(StatementType::WhileStatement).empty());
                REQUIRE(usesTable.getAllModifiesStatements(StatementType::IfStatement).empty());
                REQUIRE(usesTable.getAllModifiesVariablesFromStatementType(StatementType::CallStatement).empty());
                REQUIRE(usesTable.getAllModifiesVariablesFromStatementType(StatementType::WhileStatement).empty());
                REQUIRE(usesTable.getAllModifiesVariablesFromStatementType(StatementType::IfStatement).empty());
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
                REQUIRE(usesTable.getModifiesStatements("num1", StatementType::AnyStatement) == Vector<Integer>{1});
                REQUIRE(usesTable.getModifiesStatements("num2", StatementType::AnyStatement) == Vector<Integer>{2});
                REQUIRE(usesTable.getModifiesStatements("num3", StatementType::AnyStatement) == Vector<Integer>{3});
                REQUIRE(usesTable.getModifiesStatements("num1", StatementType::ReadStatement) == Vector<Integer>{1});
                REQUIRE_FALSE(usesTable.getModifiesStatements("num1", StatementType::AssignmentStatement)
                              == Vector<Integer>{1});
                REQUIRE(usesTable.getModifiesStatements("sum", StatementType::AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getModifiesStatements("ave", StatementType::AnyStatement) == Vector<Integer>{5});
                REQUIRE(usesTable.getModifiesStatements("ave", StatementType::AssignmentStatement)
                        == Vector<Integer>{5});
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
