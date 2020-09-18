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

SCENARIO("Iteration 1 toy example Uses", "[uses][pkb]")
{
    UsesTable usesTable = UsesTable();
    GIVEN("some Uses relationships")
    {
        WHEN("relationships are added to Uses table")
        {
            usesTable.addUsesRelationships(4, StatementType::AssignmentStatement,
                                           Vector<String>{"num1", "num2", "num3"});
            usesTable.addUsesRelationships(5, StatementType::AssignmentStatement, Vector<String>{"sum"});
            usesTable.addUsesRelationships(6, StatementType::PrintStatement, Vector<String>{"ave"});
            usesTable.addUsesRelationships("compute", Vector<String>{"num1", "num2", "num3", "ave"});

            THEN("tables should be populated")
            {
                REQUIRE(!usesTable.getAllUsesProcedures().empty());
                REQUIRE(!usesTable.getAllUsesVariablesFromProgram().empty());
                REQUIRE(!usesTable.getAllUsesVariablesFromStatementType(StatementType::AnyStatement).empty());
                REQUIRE(!usesTable.getAllUsesVariablesFromStatementType(StatementType::AssignmentStatement).empty());
                REQUIRE(!usesTable.getAllUsesVariablesFromStatementType(StatementType::PrintStatement).empty());
                REQUIRE(!usesTable.getAllUsesStatements(StatementType::AnyStatement).empty());
                REQUIRE(!usesTable.getAllUsesStatements(StatementType::AssignmentStatement).empty());
                REQUIRE(!usesTable.getAllUsesStatements(StatementType::PrintStatement).empty());
            }

            THEN("irrelevant tables should be empty")
            {
                REQUIRE(usesTable.getAllUsesStatements(StatementType::CallStatement).empty());
                REQUIRE(usesTable.getAllUsesStatements(StatementType::WhileStatement).empty());
                REQUIRE(usesTable.getAllUsesStatements(StatementType::IfStatement).empty());
                REQUIRE(usesTable.getAllUsesVariablesFromStatementType(StatementType::CallStatement).empty());
                REQUIRE(usesTable.getAllUsesVariablesFromStatementType(StatementType::WhileStatement).empty());
                REQUIRE(usesTable.getAllUsesVariablesFromStatementType(StatementType::IfStatement).empty());
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
                REQUIRE_THAT(usesTable.getAllUsesVariablesFromStatementType(StatementType::AnyStatement),
                             Catch::UnorderedEquals(ans4));
                auto ans5 = Vector<String>{"num1", "num2", "num3", "sum"};
                REQUIRE_THAT(usesTable.getAllUsesVariablesFromStatementType(StatementType::AssignmentStatement),
                             Catch::UnorderedEquals(ans5));
                REQUIRE_THAT(usesTable.getAllUsesVariablesFromStatementType(StatementType::PrintStatement),
                             Catch::UnorderedEquals(ans3));
            }

            THEN("correct variable-statement relationships are retrieved")
            {
                REQUIRE(usesTable.getUsesStatements("num1", StatementType::AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("num2", StatementType::AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("num3", StatementType::AnyStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("num1", StatementType::AssignmentStatement) == Vector<Integer>{4});
                REQUIRE_FALSE(usesTable.getUsesStatements("num1", StatementType::PrintStatement) == Vector<Integer>{4});
                REQUIRE(usesTable.getUsesStatements("sum", StatementType::AnyStatement) == Vector<Integer>{5});
                REQUIRE(usesTable.getUsesStatements("ave", StatementType::AnyStatement) == Vector<Integer>{6});
                REQUIRE(usesTable.getUsesStatements("ave", StatementType::PrintStatement) == Vector<Integer>{6});
            }

            THEN("correct procedure-variable relationships are retrieved")
            {
                auto ans = Vector<String>{"num1", "num2", "num3", "ave"};
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