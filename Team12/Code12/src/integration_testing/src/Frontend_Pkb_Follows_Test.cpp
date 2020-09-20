/**
 * Integration tests between Frontend and PKB,
 * for Follows and Follows* relationships.
 */

#include "../../unit_testing/src/ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/FrontendManager.h"
#include "pkb/PKB.h"

TEST_CASE("Multiple procedures Spheresdf Follows")
{
    parseSimple(getProgram20String_multipleProceduresSpheresdf());
    SECTION("Follows relationships stored correctly for test program - getAllBeforeStatementsTyped")
    {

        // Before - AnyStatement type, After - AnyStatement type
        std::vector<Integer> actualBefore = getAllBeforeStatementsTyped(AnyStatement, AnyStatement);
        std::unordered_set<Integer> actualBeforeSet(actualBefore.begin(), actualBefore.end());
        std::unordered_set<Integer> expectedBeforeSet = {1, 2, 4, 5, 6, 8, 9, 10, 11, 15, 16, 17, 18, 19, 20, 22};
        REQUIRE(actualBeforeSet == expectedBeforeSet);

        // Before - WhileStatement type, After - CallStatement type
        std::vector<Integer> actualBeforeWhileCall = getAllBeforeStatementsTyped(WhileStatement, CallStatement);
        std::unordered_set<Integer> actualBeforeWhileCallSet(actualBeforeWhileCall.begin(),
                                                             actualBeforeWhileCall.end());
        std::unordered_set<Integer> expectedBeforeWhileCallSet = {};
        REQUIRE(actualBeforeWhileCallSet == expectedBeforeWhileCallSet);
    }

    SECTION("Follows relationships stored correctly for test program - getAllAfterStatementsTyped")
    {
        parseSimple(getProgram20String_multipleProceduresSpheresdf());

        // Before - AnyStatement type, After - AnyStatement type
        std::vector<Integer> actualAfterTyped = getAllAfterStatementsTyped(AnyStatement, AnyStatement);
        std::unordered_set<Integer> actualAfterTypedSet(actualAfterTyped.begin(), actualAfterTyped.end());
        std::unordered_set<Integer> expectedAfterTypedSet = {2, 3, 5, 6, 7, 9, 10, 11, 14, 16, 17, 18, 19, 21, 22, 23};
        REQUIRE(actualAfterTypedSet == expectedAfterTypedSet);

        // Before - WhileStatement type, After - AssignmentStatement type
        std::vector<Integer> actualAfterWhileAssign = getAllAfterStatementsTyped(WhileStatement, AssignmentStatement);
        std::unordered_set<Integer> actualAfterWhileAssignSet(actualAfterWhileAssign.begin(),
                                                              actualAfterWhileAssign.end());
        std::unordered_set<Integer> expectedAfterWhileAssignSet = {22};
        REQUIRE(actualAfterWhileAssignSet == expectedAfterWhileAssignSet);
    }

    SECTION("Follows* relationships stored correctly for test program - getAllBeforeStatementsTypedStar")
    {
        // Before - AnyStatement type, After - CallStatement type
        std::vector<Integer> actualBeforeCall = getAllBeforeStatementsTypedStar(AnyStatement, CallStatement);
        std::unordered_set<Integer> actualBeforeCallSet(actualBeforeCall.begin(), actualBeforeCall.end());
        std::unordered_set<Integer> expectedBeforeCallSet = {1, 8, 9};
        REQUIRE(actualBeforeCallSet == expectedBeforeCallSet);

        // Before - AnyStatement type, After - PrintStatement type
        std::vector<Integer> actualBeforePrint = getAllBeforeStatementsTypedStar(AnyStatement, PrintStatement);
        std::unordered_set<Integer> actualBeforePrintSet(actualBeforePrint.begin(), actualBeforePrint.end());
        std::unordered_set<Integer> expectedBeforePrintSet = {1, 2};
        REQUIRE(actualBeforePrintSet == expectedBeforePrintSet);

        // Before - AnyStatement type, After - WhileStatement type
        std::vector<Integer> actualBeforeWhile = getAllBeforeStatementsTypedStar(AnyStatement, WhileStatement);
        std::unordered_set<Integer> actualBeforeWhileSet(actualBeforeWhile.begin(), actualBeforeWhile.end());
        std::unordered_set<Integer> expectedBeforeWhileSet = {4, 5, 6, 15, 16, 17, 18};
        REQUIRE(actualBeforeWhileSet == expectedBeforeWhileSet);

        // Before - AnyStatement type, After - IfStatement type
        std::vector<Integer> actualBeforeIf = getAllBeforeStatementsTypedStar(AnyStatement, IfStatement);
        std::unordered_set<Integer> actualBeforeIfSet(actualBeforeIf.begin(), actualBeforeIf.end());
        std::unordered_set<Integer> expectedBeforeIfSet = {8, 9, 10};
        REQUIRE(actualBeforeIfSet == expectedBeforeIfSet);

        // Before - WhileStatement type, After - AnyStatement type
        std::vector<Integer> actualBeforeWhileAny = getAllBeforeStatementsTypedStar(WhileStatement, AnyStatement);
        std::unordered_set<Integer> actualBeforeWhileAnySet(actualBeforeWhileAny.begin(), actualBeforeWhileAny.end());
        std::unordered_set<Integer> expectedBeforeWhileAnySet = {19};
        REQUIRE(actualBeforeWhileAnySet == expectedBeforeWhileAnySet);

        // Before - WhileStatement type, After - CallStatement type
        std::vector<Integer> actualBeforeWhileCall = getAllBeforeStatementsTypedStar(WhileStatement, CallStatement);
        std::unordered_set<Integer> actualBeforeWhileCallSet(actualBeforeWhileCall.begin(),
                                                             actualBeforeWhileCall.end());
        std::unordered_set<Integer> expectedBeforeWhileCallSet = {};
        REQUIRE(actualBeforeWhileCallSet == expectedBeforeWhileCallSet);

        // Before - WhileStatement type, After - IfStatement type
        std::vector<Integer> actualBeforeWhileIf = getAllBeforeStatementsTypedStar(WhileStatement, IfStatement);
        std::unordered_set<Integer> actualBeforeWhileIfSet(actualBeforeWhileIf.begin(), actualBeforeWhileIf.end());
        std::unordered_set<Integer> expectedBeforeWhileIfSet = {};
        REQUIRE(actualBeforeWhileIfSet == expectedBeforeWhileIfSet);

        // Before - IfStatement type, After - AnyStatement type
        std::vector<Integer> actualBeforeIfAny = getAllBeforeStatementsTypedStar(IfStatement, AnyStatement);
        std::unordered_set<Integer> actualBeforeIfAnySet(actualBeforeIfAny.begin(), actualBeforeIfAny.end());
        std::unordered_set<Integer> expectedBeforeIfAnySet = {11};
        REQUIRE(actualBeforeIfAnySet == expectedBeforeIfAnySet);
    }

    SECTION("Follows* relationships stored correctly for test program - getAllAfterStatementsTypedStar")
    {
        // Before - AnyStatement type, After - PrintStatement type
        std::vector<Integer> actualAfterPrint = getAllAfterStatementsTypedStar(AnyStatement, PrintStatement);
        std::unordered_set<Integer> actualAfterPrintSet(actualAfterPrint.begin(), actualAfterPrint.end());
        std::unordered_set<Integer> expectedAfterPrintSet = {3};
        REQUIRE(actualAfterPrintSet == expectedAfterPrintSet);

        // Before - AnyStatement type, After - CallStatement type
        std::vector<Integer> actualAfterCall = getAllAfterStatementsTypedStar(AnyStatement, CallStatement);
        std::unordered_set<Integer> actualAfterCallSet(actualAfterCall.begin(), actualAfterCall.end());
        std::unordered_set<Integer> expectedAfterCallSet = {2, 10};
        REQUIRE(actualAfterCallSet == expectedAfterCallSet);

        // Before - AnyStatement type, After - IfStatement type
        std::vector<Integer> actualAfterIf = getAllAfterStatementsTypedStar(AnyStatement, IfStatement);
        std::unordered_set<Integer> actualAfterIfSet(actualAfterIf.begin(), actualAfterIf.end());
        std::unordered_set<Integer> expectedAfterIfSet = {11};
        REQUIRE(actualAfterIfSet == expectedAfterIfSet);

        // Before - AnyStatement type, After - AssignmentStatement type
        std::vector<Integer> actualAfterAssign = getAllAfterStatementsTypedStar(AnyStatement, AssignmentStatement);
        std::unordered_set<Integer> actualAfterAssignSet(actualAfterAssign.begin(), actualAfterAssign.end());
        std::unordered_set<Integer> expectedAfterAssignSet = {5, 9, 14, 16, 17, 21, 22, 23};
        REQUIRE(actualAfterAssignSet == expectedAfterAssignSet);

        // Before - WhileStatement type, After - AssignmentStatement type
        std::vector<Integer> actualAfterWhileAssign
            = getAllAfterStatementsTypedStar(WhileStatement, AssignmentStatement);
        std::unordered_set<Integer> actualAfterWhileAssignSet(actualAfterWhileAssign.begin(),
                                                              actualAfterWhileAssign.end());
        std::unordered_set<Integer> expectedAfterWhileAssignSet = {22, 23};
        REQUIRE(actualAfterWhileAssignSet == expectedAfterWhileAssignSet);

        // Before - IfStatement type, After - AssignmentStatement type
        std::vector<Integer> actualAfterIfAssign = getAllAfterStatementsTypedStar(IfStatement, AssignmentStatement);
        std::unordered_set<Integer> actualAfterIfAssignSet(actualAfterIfAssign.begin(), actualAfterIfAssign.end());
        std::unordered_set<Integer> expectedAfterIfAssignSet = {14};
        REQUIRE(actualAfterIfAssignSet == expectedAfterIfAssignSet);
    }

    SECTION("Follows* relationships stored correctly for test program - getAllAfterStatementsStar")
    {
        // Before Integer - 1, After - AnyStatement type
        std::vector<Integer> actualAfterStatement1Any = getAllAfterStatementsStar(1, AnyStatement);
        std::unordered_set<Integer> actualAfterStatement1AnySet(actualAfterStatement1Any.begin(),
                                                                actualAfterStatement1Any.end());
        std::unordered_set<Integer> expectedAfterStatement1AnySet = {2, 3};
        REQUIRE(actualAfterStatement1AnySet == expectedAfterStatement1AnySet);

        // Before Integer - 2, After - PrintStatement type
        std::vector<Integer> actualAfterStatementPrint = getAllAfterStatementsStar(1, PrintStatement);
        std::unordered_set<Integer> actualAfterStatementPrintSet(actualAfterStatementPrint.begin(),
                                                                 actualAfterStatementPrint.end());
        std::unordered_set<Integer> expectedAfterStatementPrintSet = {3};
        REQUIRE(actualAfterStatementPrintSet == expectedAfterStatementPrintSet);

        // Before Integer - 23, After - AnyStatement type
        std::vector<Integer> actualAfterStatement23Any = getAllAfterStatementsStar(23, AnyStatement);
        std::unordered_set<Integer> actualAfterStatement23AnySet(actualAfterStatement23Any.begin(),
                                                                 actualAfterStatement23Any.end());
        std::unordered_set<Integer> expectedAfterStatement23AnySet = {};
        REQUIRE(actualAfterStatement23AnySet == expectedAfterStatement23AnySet);
    }

    SECTION("Follows* relationships stored correctly for test program - getAllBeforeStatementsStar")
    {
        // After Integer - 1, Before - AnyStatement type
        std::vector<Integer> actualBeforeStatementAny = getAllBeforeStatementsStar(1, AnyStatement);
        std::unordered_set<Integer> actualBeforeStatementAnySet(actualBeforeStatementAny.begin(),
                                                                actualBeforeStatementAny.end());
        std::unordered_set<Integer> expectedBeforeStatementAnySet = {};
        REQUIRE(actualBeforeStatementAnySet == expectedBeforeStatementAnySet);

        // After Integer - 23, Before - AssignmentStatement type
        std::vector<Integer> actualBeforeStatementAssign = getAllBeforeStatementsStar(23, AssignmentStatement);
        std::unordered_set<Integer> actualBeforeStatementAssignSet(actualBeforeStatementAssign.begin(),
                                                                   actualBeforeStatementAssign.end());
        std::unordered_set<Integer> expectedBeforeStatementAssignSet = {15, 16, 17, 22};
        REQUIRE(actualBeforeStatementAssignSet == expectedBeforeStatementAssignSet);
    }
}