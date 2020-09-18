/**
 * Integration tests between Frontend and PKB,
 * for the Modifies relationship.
 */

#include "../../unit_testing/src/ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/FrontendManager.h"
#include "pkb/PKB.h"

TEST_CASE("Multiple procedures Spheresdf Modifies")
{
    parseSimple(getProgram20String_multipleProceduresSpheresdf());
    SECTION("Modifies relationships stored correctly for test program - getModifiesStatements")
    {
        // Variable name - depth, StmtType - AnyStatement type
        std::vector<Integer> actualModifiesStatement = getModifiesStatements("depth", AnyStatement);
        std::unordered_set<Integer> actualModifiesStatementSet(actualModifiesStatement.begin(),
                                                               actualModifiesStatement.end());
        std::unordered_set<Integer> expectedModifiesStatementSet = {2, 6, 7, 10, 11, 13, 17};
        REQUIRE(actualModifiesStatementSet == expectedModifiesStatementSet);

        // Variable name - print (non-existent, keyword), StmtType - AnyStatement type
        std::vector<Integer> actualModifiesPrintAnyStatement = getModifiesStatements("print", AnyStatement);
        std::unordered_set<Integer> actualModifiesPrintAnyStatementSet(actualModifiesPrintAnyStatement.begin(),
                                                                       actualModifiesPrintAnyStatement.end());
        std::unordered_set<Integer> expectedModifiesPrintAnyStatementSet = {};
        REQUIRE(actualModifiesPrintAnyStatementSet == expectedModifiesPrintAnyStatementSet);
    }

    SECTION("Modifies relationships stored correctly for test program - getModifiesProcedures")
    {
        // Variable name - depth
        std::vector<String> actualModifiesProcedure = getModifiesProcedures("depth");
        std::unordered_set<String> actualModifiesProcedureSet(actualModifiesProcedure.begin(),
                                                              actualModifiesProcedure.end());
        std::unordered_set<String> expectedModifiesProcedureSet = {"main", "raymarch", "spheresdf"};
        REQUIRE(actualModifiesProcedureSet == expectedModifiesProcedureSet);

        // Variable name - raymarch (non-existent, procedure name)
        std::vector<String> actualModifiesProcedureRaymarch = getModifiesProcedures("raymarch");
        std::unordered_set<String> actualModifiesProcedureRaymarchSet(actualModifiesProcedureRaymarch.begin(),
                                                                      actualModifiesProcedureRaymarch.end());
        std::unordered_set<String> expectedModifiesProcedureRaymarchSet = {};
        REQUIRE(actualModifiesProcedureRaymarchSet == expectedModifiesProcedureRaymarchSet);
    }

    SECTION("Modifies relationships stored correctly for test program - getModifiesVariablesFromStatement")
    {
        // StmtNumber - 7 (While statement with nested If statement)
        std::vector<String> actualModifiesVariablesFromStatement7 = getModifiesVariablesFromStatement(7);
        std::unordered_set<String> actualModifiesVariablesFromStatement7Set(
            actualModifiesVariablesFromStatement7.begin(), actualModifiesVariablesFromStatement7.end());
        std::unordered_set<String> expectedModifiesVariablesFromStatement7Set
            = {"po", "dist", "x", "depth", "p", "x", "done", "count"};
        REQUIRE(actualModifiesVariablesFromStatement7Set == expectedModifiesVariablesFromStatement7Set);

        // StmtNumber - 10 (Call Statement)
        std::vector<String> actualModifiesVariablesFromStatement = getModifiesVariablesFromStatement(10);
        std::unordered_set<String> actualModifiesVariablesFromStatementSet(actualModifiesVariablesFromStatement.begin(),
                                                                           actualModifiesVariablesFromStatement.end());
        std::unordered_set<String> expectedModifiesVariablesFromStatementSet = {"dist", "x", "depth", "p"};
        REQUIRE(actualModifiesVariablesFromStatementSet == expectedModifiesVariablesFromStatementSet);

        // StmtNumber - 24 (non-existent)
        std::vector<String> actualModifiesVariablesFromStatement24 = getModifiesVariablesFromStatement(24);
        std::unordered_set<String> actualModifiesVariablesFromStatement24Set(
            actualModifiesVariablesFromStatement24.begin(), actualModifiesVariablesFromStatement24.end());
        std::unordered_set<String> expectedModifiesVariablesFromStatement24Set = {};
        REQUIRE(actualModifiesVariablesFromStatement24Set == expectedModifiesVariablesFromStatement24Set);
    }

    SECTION("Modifies relationships stored correctly for test program - getModifiesVariablesFromProcedure")
    {
        // Procedure name - main
        std::vector<String> actualAllModifiesVariablesFromProcedureMain = getModifiesVariablesFromProcedure("main");
        std::unordered_set<String> actualAllModifiesVariablesFromProcedureMainSet(
            actualAllModifiesVariablesFromProcedureMain.begin(), actualAllModifiesVariablesFromProcedureMain.end());
        std::unordered_set<String> expectedAllModifiesVariablesFromProcedureMainSet
            = {"steps", "ro", "rd", "depth", "po", "done", "count", "dist", "x", "p"};
        REQUIRE(actualAllModifiesVariablesFromProcedureMainSet == expectedAllModifiesVariablesFromProcedureMainSet);

        // Procedure name - raymarch
        std::vector<String> actualAllModifiesVariablesFromProcedureRaymarch
            = getModifiesVariablesFromProcedure("raymarch");
        std::unordered_set<String> actualAllModifiesVariablesFromProcedureRaymarchSet(
            actualAllModifiesVariablesFromProcedureRaymarch.begin(),
            actualAllModifiesVariablesFromProcedureRaymarch.end());
        std::unordered_set<String> expectedAllModifiesVariablesFromProcedureRaymarchSet
            = {"ro", "rd", "depth", "po", "done", "count", "dist", "x", "p"};
        REQUIRE(actualAllModifiesVariablesFromProcedureRaymarchSet
                == expectedAllModifiesVariablesFromProcedureRaymarchSet);

        // Procedure name - spheresdf
        std::vector<String> actualAllModifiesVariablesFromProcedure = getModifiesVariablesFromProcedure("spheresdf");
        std::unordered_set<String> actualAllModifiesVariablesFromProcedureSet(
            actualAllModifiesVariablesFromProcedure.begin(), actualAllModifiesVariablesFromProcedure.end());
        std::unordered_set<String> expectedAllModifiesVariablesFromProcedureSet = {"dist", "x", "depth", "p"};
        REQUIRE(actualAllModifiesVariablesFromProcedureSet == expectedAllModifiesVariablesFromProcedureSet);

        // Procedure name - read (non-existent, keyword)
        std::vector<String> actualModifiesVariablesFromProcedureRead = getModifiesVariablesFromProcedure("read");
        std::unordered_set<String> actualModifiesVariablesFromProcedureReadSet(
            actualModifiesVariablesFromProcedureRead.begin(), actualModifiesVariablesFromProcedureRead.end());
        std::unordered_set<String> expectedModifiesVariablesFromProcedureReadSet = {};
        REQUIRE(actualModifiesVariablesFromProcedureReadSet == expectedModifiesVariablesFromProcedureReadSet);
    }

    SECTION("Modifies relationships stored correctly for test program - getAllModifiesStatements")
    {
        // StmtType - AnyStatement type
        std::vector<Integer> actualAllModifiesStatements = getAllModifiesStatements(AnyStatement);
        std::unordered_set<Integer> actualAllModifiesStatementsSet(actualAllModifiesStatements.begin(),
                                                                   actualAllModifiesStatements.end());
        std::unordered_set<Integer> expectedAllModifiesStatementsSet
            = {1, 2, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
        REQUIRE(actualAllModifiesStatementsSet == expectedAllModifiesStatementsSet);

        // StmtType - CallStatement type
        std::vector<Integer> actualAllModifiesCallStatements = getAllModifiesStatements(CallStatement);
        std::unordered_set<Integer> actualAllModifiesCallStatementsSet(actualAllModifiesCallStatements.begin(),
                                                                       actualAllModifiesCallStatements.end());
        std::unordered_set<Integer> expectedAllModifiesCallStatementsSet = {2, 10};
        REQUIRE(actualAllModifiesCallStatementsSet == expectedAllModifiesCallStatementsSet);

        // StmtType - AssignmentStatement type
        std::vector<Integer> actualAllModifiesAssignStatements = getAllModifiesStatements(AssignmentStatement);
        std::unordered_set<Integer> actualAllModifiesAssignStatementsSet(actualAllModifiesAssignStatements.begin(),
                                                                         actualAllModifiesAssignStatements.end());
        std::unordered_set<Integer> expectedAllModifiesAssignStatementsSet
            = {4, 5, 9, 12, 13, 14, 15, 16, 17, 20, 21, 22, 23};
        REQUIRE(actualAllModifiesAssignStatementsSet == expectedAllModifiesAssignStatementsSet);
    }

    SECTION("Modifies relationships stored correctly for test program - getAllModifiesVariables by statement type")
    {
        // StmtType - AnyStatement type
        std::vector<String> actualAllModifiesVariablesByStatementType
            = getAllUsesVariablesFromStatementType(AnyStatement);
        std::unordered_set<String> actualAllModifiesVariablesByStatementTypeSet(
            actualAllModifiesVariablesByStatementType.begin(), actualAllModifiesVariablesByStatementType.end());
        std::unordered_set<String> expectedAllModifiesVariablesByStatementTypeSet
            = {"steps", "ro", "rd", "depth", "po", "done", "count", "dist", "x", "p"};
        REQUIRE(actualAllModifiesVariablesByStatementTypeSet == expectedAllModifiesVariablesByStatementTypeSet);

        // StmtType - CallStatement type
        std::vector<String> actualAllModifiesVariablesByCallStatementType
            = getAllUsesVariablesFromStatementType(CallStatement);
        std::unordered_set<String> actualAllModifiesVariablesByCallStatementTypeSet(
            actualAllModifiesVariablesByCallStatementType.begin(), actualAllModifiesVariablesByCallStatementType.end());
        std::unordered_set<String> expectedAllModifiesVariablesByCallStatementTypeSet
            = {"ro", "rd", "depth", "po", "done", "count", "dist", "x", "p"};
        REQUIRE(actualAllModifiesVariablesByCallStatementTypeSet == expectedAllModifiesVariablesByCallStatementTypeSet);

        // StmtType - AssignmentStatement type
        std::vector<String> actualAllModifiesVariablesByAssignStatementType
            = getAllUsesVariablesFromStatementType(AssignmentStatement);
        std::unordered_set<String> actualAllModifiesVariablesByAssignStatementTypeSet(
            actualAllModifiesVariablesByAssignStatementType.begin(),
            actualAllModifiesVariablesByAssignStatementType.end());
        std::unordered_set<String> expectedAllModifiesVariablesByAssignStatementTypeSet
            = {"ro", "rd", "depth", "po", "done", "count", "dist", "x", "p"};
        REQUIRE(actualAllModifiesVariablesByAssignStatementTypeSet
                == expectedAllModifiesVariablesByAssignStatementTypeSet);

        // StmtType - WhileStatement type
        std::vector<String> actualAllModifiesVariablesByWhileStatementType
            = getAllUsesVariablesFromStatementType(WhileStatement);
        std::unordered_set<String> actualAllModifiesVariablesByWhileStatementTypeSet(
            actualAllModifiesVariablesByWhileStatementType.begin(),
            actualAllModifiesVariablesByWhileStatementType.end());
        std::unordered_set<String> expectedAllModifiesVariablesByWhileStatementTypeSet
            = {"po", "dist", "x", "depth", "p", "done", "count"};
        REQUIRE(actualAllModifiesVariablesByWhileStatementTypeSet
                == expectedAllModifiesVariablesByWhileStatementTypeSet);

        // StmtType - IfStatement type
        std::vector<String> actualAllModifiesVariablesByIfStatementType
            = getAllUsesVariablesFromStatementType(IfStatement);
        std::unordered_set<String> actualAllModifiesVariablesByIfStatementTypeSet(
            actualAllModifiesVariablesByIfStatementType.begin(), actualAllModifiesVariablesByIfStatementType.end());
        std::unordered_set<String> expectedAllModifiesVariablesByIfStatementTypeSet = {"done", "depth"};
        REQUIRE(actualAllModifiesVariablesByIfStatementTypeSet == expectedAllModifiesVariablesByIfStatementTypeSet);

        // StmtType - ReadStatement type
        std::vector<String> actualAllModifiesVariablesByReadStatementType
            = getAllUsesVariablesFromStatementType(ReadStatement);
        std::unordered_set<String> actualAllModifiesVariablesByReadStatementTypeSet(
            actualAllModifiesVariablesByReadStatementType.begin(), actualAllModifiesVariablesByReadStatementType.end());
        std::unordered_set<String> expectedAllModifiesVariablesByReadStatementTypeSet = {"steps", "depth", "p"};
        REQUIRE(actualAllModifiesVariablesByReadStatementTypeSet == expectedAllModifiesVariablesByReadStatementTypeSet);

        // StmtType - PrintStatement type
        std::vector<String> actualAllModifiesVariablesByPrintStatementType
            = getAllUsesVariablesFromStatementType(PrintStatement);
        std::unordered_set<String> actualAllModifiesVariablesByPrintStatementTypeSet(
            actualAllModifiesVariablesByPrintStatementType.begin(),
            actualAllModifiesVariablesByPrintStatementType.end());
        std::unordered_set<String> expectedAllModifiesVariablesByPrintStatementTypeSet = {};
        REQUIRE(actualAllModifiesVariablesByPrintStatementTypeSet
                == expectedAllModifiesVariablesByPrintStatementTypeSet);
    }
    SECTION("Modifies relationships stored correctly for test program - getAllModifiesVariablesFromProcedure")
    {
        std::vector<String> actualAllModifiesVariablesByProcedureMain = getUsesVariablesFromProcedure("main");
        std::unordered_set<String> actualAllModifiesVariablesByProcedureMainSet(
            actualAllModifiesVariablesByProcedureMain.begin(), actualAllModifiesVariablesByProcedureMain.end());
        std::unordered_set<String> expectedAllModifiesVariablesByProcedureMainSet
            = {"steps", "ro", "rd", "depth", "po", "done", "count", "dist", "x", "p"};
        REQUIRE(actualAllModifiesVariablesByProcedureMainSet == expectedAllModifiesVariablesByProcedureMainSet);
    }
}
