/**
 * Integration tests between Frontend and PKB,
 * for the Procedure table, Variable Table, Statement Table, and Constant Table and the RootNode.
 */

#include "../../unit_testing/src/ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/FrontendManager.h"
#include "pkb/PKB.h"

TEST_CASE("Multiple procedures Spheresdf Tables")
{
    parseSimple(getProgram20String_multipleProceduresSpheresdf());
    SECTION("Procedure Table stored program correctly for test program")
    {
        std::vector<String> actualProcedures = getAllProcedures();
        std::unordered_set<String> actualProceduresSet(actualProcedures.begin(), actualProcedures.end());
        std::unordered_set<String> expectedProceduresSet = {"main", "raymarch", "spheresdf"};
        REQUIRE(actualProceduresSet == expectedProceduresSet);
    }

    SECTION("Variable Table stored variables in program correctly for test program")
    {
        std::vector<String> actualVariables = getAllVariables();
        std::unordered_set<String> actualVariablesSet(actualVariables.begin(), actualVariables.end());
        std::unordered_set<String> expectedVariablesSet
            = {"depth", "count", "steps", "ro", "rd", "po", "done", "dist", "epsilon", "x", "y", "z", "p"};
        REQUIRE(actualVariablesSet == expectedVariablesSet);
    }

    SECTION("Statement Table stored statements in program correctly for test program")
    {
        std::vector<Integer> actualStatements = getAllStatements(AnyStatement);
        std::unordered_set<Integer> actualStatementsSet(actualStatements.begin(), actualStatements.end());
        std::unordered_set<Integer> expectedStatementsSet
            = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
        REQUIRE(actualStatementsSet == expectedStatementsSet);

        std::vector<Integer> actualPrintStatements = getAllStatements(PrintStatement);
        std::unordered_set<Integer> actualPrintStatementsSet(actualPrintStatements.begin(),
                                                             actualPrintStatements.end());
        std::unordered_set<Integer> expectedPrintStatementsSet = {3, 8};
        REQUIRE(actualPrintStatementsSet == expectedPrintStatementsSet);

        std::vector<Integer> actualReadStatements = getAllStatements(ReadStatement);
        std::unordered_set<Integer> actualReadStatementsSet(actualReadStatements.begin(), actualReadStatements.end());
        std::unordered_set<Integer> expectedReadStatementsSet = {1, 6, 18};
        REQUIRE(actualReadStatementsSet == expectedReadStatementsSet);

        std::vector<Integer> actualCallStatements = getAllStatements(CallStatement);
        std::unordered_set<Integer> actualCallStatementsSet(actualCallStatements.begin(), actualCallStatements.end());
        std::unordered_set<Integer> expectedCallStatementsSet = {2, 10};
        REQUIRE(actualCallStatementsSet == expectedCallStatementsSet);

        std::vector<Integer> actualWhileStatements = getAllStatements(WhileStatement);
        std::unordered_set<Integer> actualWhileStatementsSet(actualWhileStatements.begin(),
                                                             actualWhileStatements.end());
        std::unordered_set<Integer> expectedWhileStatementsSet = {7, 19};
        REQUIRE(actualWhileStatementsSet == expectedWhileStatementsSet);

        std::vector<Integer> actualIfStatements = getAllStatements(IfStatement);
        std::unordered_set<Integer> actualIfStatementsSet(actualIfStatements.begin(), actualIfStatements.end());
        std::unordered_set<Integer> expectedIfStatementsSet = {11};
        REQUIRE(actualIfStatementsSet == expectedIfStatementsSet);

        std::vector<Integer> actualAssignStatements = getAllStatements(AssignmentStatement);
        std::unordered_set<Integer> actualAssignStatementsSet(actualAssignStatements.begin(),
                                                              actualAssignStatements.end());
        std::unordered_set<Integer> expectedAssignStatementsSet = {4, 5, 9, 12, 13, 14, 15, 16, 17, 20, 21, 22, 23};
        REQUIRE(actualAssignStatementsSet == expectedAssignStatementsSet);
    }

    /*

    SECTION("Constant Table stored constants in program correctly for test program")
    {
        // Check for constants
        std::vector<Integer> actualConstants = getAllConstants();
        std::unordered_set<Integer> actualConstantsSet(actualConstants.begin(), actualConstants.end());
        std::unordered_set<Integer> expectedConstantsSet
            = {1, 2, 13, 19};
        REQUIRE(actualConstantsSet == expectedConstantsSet);
    }
    */

    SECTION("RootNode of program is stored correctly for test program")
    {
        ProgramNode* actualRootNode = getRootNode();
        ProgramNode* expectedRootNode = getProgram20Tree_multipleProceduresSpheresdf();
        REQUIRE(*(actualRootNode) == *(expectedRootNode));
    }
}