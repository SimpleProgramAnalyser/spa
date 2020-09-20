/**
 * Integration tests between Frontend and PKB,
 * for the Uses relationship.
 */

#include "../../unit_testing/src/ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/FrontendManager.h"
#include "pkb/PKB.h"

TEST_CASE("Multiple procedures Spheresdf Uses")
{
    parseSimple(getProgram20String_multipleProceduresSpheresdf());

    SECTION("Uses relationships stored correctly for test program - getUsesesStatements")
    {
        // Variable name - depth, StmtType - AnyStatement type
        std::vector<Integer> actualUsesStatement = getUsesStatements("depth", AnyStatement);
        std::unordered_set<Integer> actualUsesStatementSet(actualUsesStatement.begin(), actualUsesStatement.end());
        std::unordered_set<Integer> expectedUsesStatementSet = {2, 3, 7, 8, 9, 10, 11, 12, 13, 17};
        REQUIRE(actualUsesStatementSet == expectedUsesStatementSet);

        // Variable name - print (non-existent, keyword), StmtType - AnyStatement type
        std::vector<Integer> actualUsesPrintAnyStatement = getUsesStatements("print", AnyStatement);
        std::unordered_set<Integer> actualUsesPrintAnyStatementSet(actualUsesPrintAnyStatement.begin(),
                                                                   actualUsesPrintAnyStatement.end());
        std::unordered_set<Integer> expectedUsesPrintAnyStatementSet = {};
        REQUIRE(actualUsesPrintAnyStatementSet == expectedUsesPrintAnyStatementSet);
    }

    SECTION("Uses relationships stored correctly for test program - getUsesProcedures")
    {
        // Variable name - depth
        std::vector<String> actualUsesProcedure = getUsesProcedures("depth");
        std::unordered_set<String> actualUsesProcedureSet(actualUsesProcedure.begin(), actualUsesProcedure.end());
        std::unordered_set<String> expectedUsesProcedureSet = {"main", "raymarch", "spheresdf"};
        REQUIRE(actualUsesProcedureSet == expectedUsesProcedureSet);

        // Variable name - raymarch (non-existent, procedure name)
        std::vector<String> actualUsesProcedureRaymarch = getUsesProcedures("raymarch");
        std::unordered_set<String> actualUsesProcedureRaymarchSet(actualUsesProcedureRaymarch.begin(),
                                                                  actualUsesProcedureRaymarch.end());
        std::unordered_set<String> expectedUsesProcedureRaymarchSet = {};
        REQUIRE(actualUsesProcedureRaymarchSet == expectedUsesProcedureRaymarchSet);
    }

    SECTION("Uses relationships stored correctly for test program - getUsesVariablesFromStatement")
    {
        // StmtNumber - 7 (While statement with nested If statement)
        std::vector<String> actualUsesVariablesFromStatement7 = getUsesVariablesFromStatement(7);
        std::unordered_set<String> actualUsesVariablesFromStatement7Set(actualUsesVariablesFromStatement7.begin(),
                                                                        actualUsesVariablesFromStatement7.end());
        std::unordered_set<String> expectedUsesVariablesFromStatement7Set
            = {"count", "steps", "depth", "ro", "rd", "x", "y", "z", "dist", "depth", "p", "epsilon", "count"};
        REQUIRE(actualUsesVariablesFromStatement7Set == expectedUsesVariablesFromStatement7Set);

        // StmtNumber - 10 (Call Statement)
        std::vector<String> actualUsesVariablesFromStatement = getUsesVariablesFromStatement(10);
        std::unordered_set<String> actualUsesVariablesFromStatementSet(actualUsesVariablesFromStatement.begin(),
                                                                       actualUsesVariablesFromStatement.end());
        std::unordered_set<String> expectedUsesVariablesFromStatementSet = {"x", "y", "z", "dist", "depth", "p"};
        REQUIRE(actualUsesVariablesFromStatementSet == expectedUsesVariablesFromStatementSet);

        // StmtNumber - 24 (non-existent)
        std::vector<String> actualUsesVariablesFromStatement24 = getUsesVariablesFromStatement(24);
        std::unordered_set<String> actualUsesVariablesFromStatement24Set(actualUsesVariablesFromStatement24.begin(),
                                                                         actualUsesVariablesFromStatement24.end());
        std::unordered_set<String> expectedUsesVariablesFromStatement24Set = {};
        REQUIRE(actualUsesVariablesFromStatement24Set == expectedUsesVariablesFromStatement24Set);
    }

    SECTION("Uses relationships stored correctly for test program - getUsesVariablesFromProcedure")
    {
        // Procedure name - main
        std::vector<String> actualAllUsesVariablesFromProcedureMain = getUsesVariablesFromProcedure("main");
        std::unordered_set<String> actualAllUsesVariablesFromProcedureMainSet(
            actualAllUsesVariablesFromProcedureMain.begin(), actualAllUsesVariablesFromProcedureMain.end());
        std::unordered_set<String> expectedAllUsesVariablesFromProcedureMainSet
            = {"depth", "count", "steps", "ro", "rd", "dist", "epsilon", "x", "y", "z", "p"};
        REQUIRE(actualAllUsesVariablesFromProcedureMainSet == expectedAllUsesVariablesFromProcedureMainSet);

        // Procedure name - raymarch
        std::vector<String> actualAllUsesVariablesFromProcedureRaymarch = getUsesVariablesFromProcedure("raymarch");
        std::unordered_set<String> actualAllUsesVariablesFromProcedureRaymarchSet(
            actualAllUsesVariablesFromProcedureRaymarch.begin(), actualAllUsesVariablesFromProcedureRaymarch.end());
        std::unordered_set<String> expectedAllUsesVariablesFromProcedureRaymarchSet
            = {"depth", "count", "steps", "ro", "rd", "dist", "epsilon", "x", "y", "z", "p"};
        REQUIRE(actualAllUsesVariablesFromProcedureRaymarchSet == expectedAllUsesVariablesFromProcedureRaymarchSet);

        // Procedure name - spheresdf
        std::vector<String> actualAllUsesVariablesFromProcedureSpheresdf = getUsesVariablesFromProcedure("spheresdf");
        std::unordered_set<String> actualAllUsesVariablesFromProcedureSpheresdfSet(
            actualAllUsesVariablesFromProcedureSpheresdf.begin(), actualAllUsesVariablesFromProcedureSpheresdf.end());
        std::unordered_set<String> expectedAllUsesVariablesFromProcedureSpheresdfSet
            = {"x", "y", "z", "dist", "depth", "p"};
        REQUIRE(actualAllUsesVariablesFromProcedureSpheresdfSet == expectedAllUsesVariablesFromProcedureSpheresdfSet);

        // Procedure name - read (non-existent, keyword)
        std::vector<String> actualUsesVariablesFromProcedureRead = getUsesVariablesFromProcedure("read");
        std::unordered_set<String> actualUsesVariablesFromProcedureReadSet(actualUsesVariablesFromProcedureRead.begin(),
                                                                           actualUsesVariablesFromProcedureRead.end());
        std::unordered_set<String> expectedUsesVariablesFromProcedureReadSet = {};
        REQUIRE(actualUsesVariablesFromProcedureReadSet == expectedUsesVariablesFromProcedureReadSet);
    }

    SECTION("Uses relationships stored correctly for test program - getAllUsesStatements")
    {
        // StmtType - AnyStatement type
        std::vector<Integer> actualAllUsesStatements = getAllUsesStatements(AnyStatement);
        std::unordered_set<Integer> actualAllUsesStatementsSet(actualAllUsesStatements.begin(),
                                                               actualAllUsesStatements.end());
        std::unordered_set<Integer> expectedAllUsesStatementsSet
            = {2, 3, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23};
        REQUIRE(actualAllUsesStatementsSet == expectedAllUsesStatementsSet);

        // StmtType - CallStatement type
        std::vector<Integer> actualAllUsesCallStatements = getAllUsesStatements(CallStatement);
        std::unordered_set<Integer> actualAllUsesCallStatementsSet(actualAllUsesCallStatements.begin(),
                                                                   actualAllUsesCallStatements.end());
        std::unordered_set<Integer> expectedAllUsesCallStatementsSet = {2, 10};
        REQUIRE(actualAllUsesCallStatementsSet == expectedAllUsesCallStatementsSet);

        // StmtType - AssignmentStatement type
        std::vector<Integer> actualAllUsesAssignStatements = getAllUsesStatements(AssignmentStatement);
        std::unordered_set<Integer> actualAllUsesAssignStatementsSet(actualAllUsesAssignStatements.begin(),
                                                                     actualAllUsesAssignStatements.end());
        std::unordered_set<Integer> expectedAllUsesAssignStatementsSet = {9, 12, 13, 14, 15, 16, 17, 20, 21, 22, 23};
        REQUIRE(actualAllUsesAssignStatementsSet == expectedAllUsesAssignStatementsSet);
    }

    SECTION("Uses relationships stored correctly for test program - getAllUsesVariables by statement type")
    {
        // StmtType - AnyStatement type
        std::vector<String> actualAllUsesVariablesByStatementType = getAllUsesVariablesFromStatementType(AnyStatement);
        std::unordered_set<String> actualAllUsesVariablesByStatementTypeSet(
            actualAllUsesVariablesByStatementType.begin(), actualAllUsesVariablesByStatementType.end());
        std::unordered_set<String> expectedAllUsesVariablesByStatementTypeSet
            = {"depth", "count", "steps", "ro", "rd", "dist", "epsilon", "x", "y", "z", "p"};
        REQUIRE(actualAllUsesVariablesByStatementTypeSet == expectedAllUsesVariablesByStatementTypeSet);

        // StmtType - CallStatement type
        std::vector<String> actualAllUsesVariablesByCallStatementType
            = getAllUsesVariablesFromStatementType(CallStatement);
        std::unordered_set<String> actualAllUsesVariablesByCallStatementTypeSet(
            actualAllUsesVariablesByCallStatementType.begin(), actualAllUsesVariablesByCallStatementType.end());
        std::unordered_set<String> expectedAllUsesVariablesByCallStatementTypeSet
            = {"depth", "count", "steps", "ro", "rd", "dist", "epsilon", "x", "y", "z", "p"};
        REQUIRE(actualAllUsesVariablesByCallStatementTypeSet == expectedAllUsesVariablesByCallStatementTypeSet);

        // StmtType - AssignmentStatement type
        std::vector<String> actualAllUsesVariablesByAssignStatementType
            = getAllUsesVariablesFromStatementType(AssignmentStatement);
        std::unordered_set<String> actualAllUsesVariablesByAssignStatementTypeSet(
            actualAllUsesVariablesByAssignStatementType.begin(), actualAllUsesVariablesByAssignStatementType.end());
        std::unordered_set<String> expectedAllUsesVariablesByAssignStatementTypeSet
            = {"ro", "rd", "depth", "dist", "x", "y", "z", "count"};
        REQUIRE(actualAllUsesVariablesByAssignStatementTypeSet == expectedAllUsesVariablesByAssignStatementTypeSet);

        // StmtType - WhileStatement type
        std::vector<String> actualAllUsesVariablesByWhileStatementType
            = getAllUsesVariablesFromStatementType(WhileStatement);
        std::unordered_set<String> actualAllUsesVariablesByWhileStatementTypeSet(
            actualAllUsesVariablesByWhileStatementType.begin(), actualAllUsesVariablesByWhileStatementType.end());
        std::unordered_set<String> expectedAllUsesVariablesByWhileStatementTypeSet
            = {"depth", "count", "steps", "ro", "rd", "dist", "epsilon", "x", "y", "z", "p"};
        REQUIRE(actualAllUsesVariablesByWhileStatementTypeSet == expectedAllUsesVariablesByWhileStatementTypeSet);

        // StmtType - IfStatement type
        std::vector<String> actualAllUsesVariablesByIfStatementType = getAllUsesVariablesFromStatementType(IfStatement);
        std::unordered_set<String> actualAllUsesVariablesByIfStatementTypeSet(
            actualAllUsesVariablesByIfStatementType.begin(), actualAllUsesVariablesByIfStatementType.end());
        std::unordered_set<String> expectedAllUsesVariablesByIfStatementTypeSet = {"dist", "epsilon", "depth"};
        REQUIRE(actualAllUsesVariablesByIfStatementTypeSet == expectedAllUsesVariablesByIfStatementTypeSet);

        // StmtType - ReadStatement type
        std::vector<String> actualAllUsesVariablesByReadStatementType
            = getAllUsesVariablesFromStatementType(ReadStatement);
        std::unordered_set<String> actualAllUsesVariablesByReadStatementTypeSet(
            actualAllUsesVariablesByReadStatementType.begin(), actualAllUsesVariablesByReadStatementType.end());
        std::unordered_set<String> expectedAllUsesVariablesByReadStatementTypeSet = {};
        REQUIRE(actualAllUsesVariablesByReadStatementTypeSet == expectedAllUsesVariablesByReadStatementTypeSet);

        // StmtType - PrintStatement type
        std::vector<String> actualAllUsesVariablesByPrintStatementType
            = getAllUsesVariablesFromStatementType(PrintStatement);
        std::unordered_set<String> actualAllUsesVariablesByPrintStatementTypeSet(
            actualAllUsesVariablesByPrintStatementType.begin(), actualAllUsesVariablesByPrintStatementType.end());
        std::unordered_set<String> expectedAllUsesVariablesByPrintStatementTypeSet = {"depth"};
        REQUIRE(actualAllUsesVariablesByPrintStatementTypeSet == expectedAllUsesVariablesByPrintStatementTypeSet);
    }

    SECTION("Uses relationships stored correctly for test program - getAllUsesVariablesFromProcedure")
    {
        std::vector<String> actualAllUsesVariablesByProcedureMain = getUsesVariablesFromProcedure("main");
        std::unordered_set<String> actualAllUsesVariablesByProcedureMainSet(
            actualAllUsesVariablesByProcedureMain.begin(), actualAllUsesVariablesByProcedureMain.end());
        std::unordered_set<String> expectedAllUsesVariablesByProcedureMainSet
            = {"depth", "count", "steps", "ro", "rd", "dist", "epsilon", "x", "y", "z", "p"};
        REQUIRE(actualAllUsesVariablesByProcedureMainSet == expectedAllUsesVariablesByProcedureMainSet);
    }
}