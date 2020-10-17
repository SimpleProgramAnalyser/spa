/**
 * Integration tests between Frontend and PKB,
 * for Parent and Parent* relationships.
 */
#include "../../unit_testing/src/ast_utils/AstUtils.cpp"
#include "Utils.h"
#include "catch.hpp"
#include "frontend/FrontendManager.h"
#include "pkb/PKB.h"

TEST_CASE("Multiple procedures Spheresdf Parent")
{
    UiStub ui;
    parseSimple(getProgram20String_multipleProceduresSpheresdf(), ui);
    SECTION("Parent relationships stored correctly for test program - getAllParentStatementsTyped")
    {
        // Parent - AnyStatement type, Child - AnyStatement type
        std::vector<Integer> actualParent = getAllParentStatementsTyped(AnyStatement, AnyStatement);
        std::unordered_set<Integer> actualParentSet(actualParent.begin(), actualParent.end());
        std::unordered_set<Integer> expectedParentSet = {7, 11, 19};
        REQUIRE(actualParentSet == expectedParentSet);

        // Parent - WhileStatement type, Child - CallStatement type
        std::vector<Integer> actualParentWhileCall = getAllParentStatementsTyped(WhileStatement, CallStatement);
        std::unordered_set<Integer> actualParentWhileCallSet(actualParentWhileCall.begin(),
                                                             actualParentWhileCall.end());
        std::unordered_set<Integer> expectedParentWhileCallSet = {7};
        REQUIRE(actualParentWhileCallSet == expectedParentWhileCallSet);
    }

    SECTION("Parent relationships stored correctly for test program - getAllChildStatementsTyped")
    {
        // Parent - AnyStatement type, Child - AnyStatement type
        std::vector<Integer> actualChild = getAllChildStatementsTyped(AnyStatement, AnyStatement);
        std::unordered_set<Integer> actualChildSet(actualChild.begin(), actualChild.end());
        std::unordered_set<Integer> expectedChildSet = {8, 9, 10, 11, 12, 13, 14, 20, 21};
        REQUIRE(actualChildSet == expectedChildSet);

        // Parent - WhileStatement type, Child - AssignmentStatement type
        std::vector<Integer> actualChildWhileAssign = getAllChildStatementsTyped(WhileStatement, AssignmentStatement);
        std::unordered_set<Integer> actualChildWhileAssignSet(actualChildWhileAssign.begin(),
                                                              actualChildWhileAssign.end());
        std::unordered_set<Integer> expectedChildWhileAssignSet = {9, 14, 20, 21};
        REQUIRE(actualChildWhileAssignSet == expectedChildWhileAssignSet);
    }

    SECTION("Parent* relationships stored correctly for test program - getAllParentStatementsTypedStar")
    {
        // Parent - AnyStatement type, Child - CallStatement type
        std::vector<Integer> actualParentCall = getAllParentStatementsTypedStar(AnyStatement, CallStatement);
        std::unordered_set<Integer> actualParentCallSet(actualParentCall.begin(), actualParentCall.end());
        std::unordered_set<Integer> expectedParentCallSet = {7};
        REQUIRE(actualParentCallSet == expectedParentCallSet);

        // Parent - AnyStatement type, Child - PrintStatement type
        std::vector<Integer> actualParentPrint = getAllParentStatementsTypedStar(AnyStatement, PrintStatement);
        std::unordered_set<Integer> actualParentPrintSet(actualParentPrint.begin(), actualParentPrint.end());
        std::unordered_set<Integer> expectedParentPrintSet = {7};
        REQUIRE(actualParentPrintSet == expectedParentPrintSet);

        // Parent - AnyStatement type, Child - WhileStatement type
        std::vector<Integer> actualParentWhile = getAllParentStatementsTypedStar(AnyStatement, WhileStatement);
        std::unordered_set<Integer> actualParentWhileSet(actualParentWhile.begin(), actualParentWhile.end());
        std::unordered_set<Integer> expectedParentWhileSet = {};
        REQUIRE(actualParentWhileSet == expectedParentWhileSet);

        // Parent - AnyStatement type, Child - IfStatement type
        std::vector<Integer> actualParentIf = getAllParentStatementsTypedStar(AnyStatement, IfStatement);
        std::unordered_set<Integer> actualParentIfSet(actualParentIf.begin(), actualParentIf.end());
        std::unordered_set<Integer> expectedParentIfSet = {7};
        REQUIRE(actualParentIfSet == expectedParentIfSet);

        // Parent - WhileStatement type, Child - AnyStatement type
        std::vector<Integer> actualParentWhileAny = getAllParentStatementsTypedStar(WhileStatement, AnyStatement);
        std::unordered_set<Integer> actualParentWhileAnySet(actualParentWhileAny.begin(), actualParentWhileAny.end());
        std::unordered_set<Integer> expectedParentWhileAnySet = {7, 19};
        REQUIRE(actualParentWhileAnySet == expectedParentWhileAnySet);

        // Parent - WhileStatement type, Child - CallStatement type
        std::vector<Integer> actualParentWhileCall = getAllParentStatementsTypedStar(WhileStatement, CallStatement);
        std::unordered_set<Integer> actualParentWhileCallSet(actualParentWhileCall.begin(),
                                                             actualParentWhileCall.end());
        std::unordered_set<Integer> expectedParentWhileCallSet = {7};
        REQUIRE(actualParentWhileCallSet == expectedParentWhileCallSet);

        // Parent - WhileStatement type, Child - IfStatement type
        std::vector<Integer> actualParentWhileIf = getAllParentStatementsTypedStar(WhileStatement, IfStatement);
        std::unordered_set<Integer> actualParentWhileIfSet(actualParentWhileIf.begin(), actualParentWhileIf.end());
        std::unordered_set<Integer> expectedParentWhileIfSet = {7};
        REQUIRE(actualParentWhileIfSet == expectedParentWhileIfSet);

        // Parent - IfStatement type, Child - AnyStatement type
        std::vector<Integer> actualParentIfAny = getAllParentStatementsTypedStar(IfStatement, AnyStatement);
        std::unordered_set<Integer> actualParentIfAnySet(actualParentIfAny.begin(), actualParentIfAny.end());
        std::unordered_set<Integer> expectedParentIfAnySet = {11};
        REQUIRE(actualParentIfAnySet == expectedParentIfAnySet);
    }

    SECTION("Parent* relationships stored correctly for test program - getAllChildStatementsTypedStar")
    {
        // Parent - AnyStatement type, Child - PrintStatement type
        std::vector<Integer> actualChildPrint = getAllChildStatementsTypedStar(AnyStatement, PrintStatement);
        std::unordered_set<Integer> actualChildPrintSet(actualChildPrint.begin(), actualChildPrint.end());
        std::unordered_set<Integer> expectedChildPrintSet = {8};
        REQUIRE(actualChildPrintSet == expectedChildPrintSet);

        // Parent - AnyStatement type, Child - CallStatement type
        std::vector<Integer> actualChildCall = getAllChildStatementsTypedStar(AnyStatement, CallStatement);
        std::unordered_set<Integer> actualChildCallSet(actualChildCall.begin(), actualChildCall.end());
        std::unordered_set<Integer> expectedChildCallSet = {10};
        REQUIRE(actualChildCallSet == expectedChildCallSet);

        // Parent - AnyStatement type, Child - IfStatement type
        std::vector<Integer> actualChildIf = getAllChildStatementsTypedStar(AnyStatement, IfStatement);
        std::unordered_set<Integer> actualChildIfSet(actualChildIf.begin(), actualChildIf.end());
        std::unordered_set<Integer> expectedChildIfSet = {11};
        REQUIRE(actualChildIfSet == expectedChildIfSet);

        // Parent - AnyStatement type, Child - AssignmentStatement type
        std::vector<Integer> actualChildAssign = getAllChildStatementsTypedStar(AnyStatement, AssignmentStatement);
        std::unordered_set<Integer> actualChildAssignSet(actualChildAssign.begin(), actualChildAssign.end());
        std::unordered_set<Integer> expectedChildAssignSet = {9, 12, 13, 14, 20, 21};
        REQUIRE(actualChildAssignSet == expectedChildAssignSet);

        // Parent - WhileStatement type, Child - AssignmentStatement type
        std::vector<Integer> actualChildWhileAssign
            = getAllChildStatementsTypedStar(WhileStatement, AssignmentStatement);
        std::unordered_set<Integer> actualChildWhileAssignSet(actualChildWhileAssign.begin(),
                                                              actualChildWhileAssign.end());
        std::unordered_set<Integer> expectedChildWhileAssignSet = {9, 12, 13, 14, 20, 21};
        REQUIRE(actualChildWhileAssignSet == expectedChildWhileAssignSet);

        // Parent - IfStatement type, Child - AssignmentStatement type
        std::vector<Integer> actualChildIfAssign = getAllChildStatementsTypedStar(IfStatement, AssignmentStatement);
        std::unordered_set<Integer> actualChildIfAssignSet(actualChildIfAssign.begin(), actualChildIfAssign.end());
        std::unordered_set<Integer> expectedChildIfAssignSet = {12, 13};
        REQUIRE(actualChildIfAssignSet == expectedChildIfAssignSet);
    }

    SECTION("Parent* relationships stored correctly for test program - getAllChildStatementsStar")
    {
        // Parent Integer - 1 (ReadStatement), Child - AnyStatement type
        std::vector<Integer> actualChildStatementAny = getAllChildStatementsStar(1, AnyStatement);
        std::unordered_set<Integer> actualChildStatementAnySet(actualChildStatementAny.begin(),
                                                               actualChildStatementAny.end());
        std::unordered_set<Integer> expectedChildStatementAnySet = {};
        REQUIRE(actualChildStatementAnySet == expectedChildStatementAnySet);

        // Parent Integer - 2 (CallStatament), Child - PrintStatement type
        std::vector<Integer> actualChildStatementPrint = getAllChildStatementsStar(2, PrintStatement);
        std::unordered_set<Integer> actualChildStatementPrintSet(actualChildStatementPrint.begin(),
                                                                 actualChildStatementPrint.end());
        std::unordered_set<Integer> expectedChildStatementPrintSet = {};
        REQUIRE(actualChildStatementPrintSet == expectedChildStatementPrintSet);

        // Parent Integer - 7 (WhileStatement), Child - AssignmentStatement type
        std::vector<Integer> actualChildStatementAssign = getAllChildStatementsStar(7, AssignmentStatement);
        std::unordered_set<Integer> actualChildStatementAssignSet(actualChildStatementAssign.begin(),
                                                                  actualChildStatementAssign.end());
        std::unordered_set<Integer> expectedChildStatementAssignSet = {9, 12, 13, 14};
        REQUIRE(actualChildStatementAssignSet == expectedChildStatementAssignSet);

        // Parent Integer - 23 (AssignmentStatement), Child - AssignmentStatement type
        std::vector<Integer> actualChildStatement23Assign = getAllChildStatementsStar(23, AssignmentStatement);
        std::unordered_set<Integer> actualChildStatement23AssignSet(actualChildStatement23Assign.begin(),
                                                                    actualChildStatement23Assign.end());
        std::unordered_set<Integer> expectedChildStatement23AssignSet = {};
        REQUIRE(actualChildStatement23AssignSet == expectedChildStatement23AssignSet);
    }

    SECTION("Parent* relationships stored correctly for test program - getAllParentStatementsStar")
    {
        // Child Integer - 1, Parent - AnyStatement type
        std::vector<Integer> actualParentStatementAny = getAllParentStatementsStar(1, AnyStatement);
        std::unordered_set<Integer> actualParentStatementAnySet(actualParentStatementAny.begin(),
                                                                actualParentStatementAny.end());
        std::unordered_set<Integer> expectedParentStatementAnySet = {};
        REQUIRE(actualParentStatementAnySet == expectedParentStatementAnySet);

        // Child Integer - 13, Parent - WhileStatement type
        std::vector<Integer> actualParentStatementWhile = getAllParentStatementsStar(13, WhileStatement);
        std::unordered_set<Integer> actualParentStatementWhileSet(actualParentStatementWhile.begin(),
                                                                  actualParentStatementWhile.end());
        std::unordered_set<Integer> expectedParentStatementWhileSet = {7};
        REQUIRE(actualParentStatementWhileSet == expectedParentStatementWhileSet);

        // Child Integer - 23, Parent - AssignmentStatement type
        std::vector<Integer> actualParentStatementAssign = getAllParentStatementsStar(23, AssignmentStatement);
        std::unordered_set<Integer> actualParentStatementAssignSet(actualParentStatementAssign.begin(),
                                                                   actualParentStatementAssign.end());
        std::unordered_set<Integer> expectedParentStatementAssignSet = {};
        REQUIRE(actualParentStatementAssignSet == expectedParentStatementAssignSet);
    }
}