/*
 * Integration tests between the PQL and PKB units of
 * our SPA system.
 *
 * (In this series of tests, we mainly focus on the
 * Autotester format (instead of the UI format).)
 */

#include "../../unit_testing/src/ast_utils/AstUtils.h"
#include "catch.hpp"
#include "pql/PqlManager.h"
#include "pkb/PKB.h"


TEST_CASE("syntatically invalid query")
{
    // === Test set-up ===
    String query = "stmt s; __ Select s such that Follows (s, _)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("semantically invalid query")
{
    // === Test set-up ===
    String query = "stmt s;";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("vacuously true query no clauses")
{
    // === Test set-up ===
    String query = "stmt s; Select s";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(2,s)";

    QueryResultFormatType format = AutotesterFormat;

    SECTION("AssignmentStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("CallStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addFollowsRelationships(2, AssignmentStatement, 3, CallStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("IfStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addFollowsRelationships(2, AssignmentStatement, 3, IfStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("PrintStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addFollowsRelationships(2, AssignmentStatement, 3, PrintStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("ReadStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addFollowsRelationships(2, AssignmentStatement, 3, ReadStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("WhileStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addFollowsRelationships(2, AssignmentStatement, 3, WhileStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }
}

TEST_CASE("(vacuously true) query with such that Follows clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Follows(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows clause, left operand line number, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(2,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand line number, right operand synonym, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Follows(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * From this test onward, we will not be (intentionally) testing for vacuously
 * true queries, and queries in which at least one clause is not related to
 * the synonym, as there are too many combinations (we give the benefit of
 * doubt that our autotester system tests will cover
 * all of these cases).
 */

/*
 * Additionally, for this kind of test case, we would not be explicitly testing
 * for all StatementType, but only test AssignmentStatement type.
 */

TEST_CASE("query with such that Follows clause, left operand synonym, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(s,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows clause, left operand wildcard, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(_,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows clause, left operand line number, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(2,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand synonym, right operand synonym, but both synonyms are the same")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(s,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);
    addFollowsRelationships(3, AssignmentStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand wildcard, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(_,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);
    addFollowsRelationships(3, AssignmentStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "4, 3";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows clause, left operand wildcard, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * TODO: Get this test case to work after PKB bug fixed, where
 * a Parent can have multiple children (unlike follows.)
 */
/*
TEST_CASE("query with such that Parent clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(2,s)";

    QueryResultFormatType format = AutotesterFormat;

     // Obviously, there are many more possible combinations, however
     // we won't be exhaustively testing for all of them.

    SECTION("AssignmentStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addParentRelationships(2, WhileStatement, 3, AssignmentStatement);
        addParentRelationships(2, WhileStatement, 4, AssignmentStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("CallStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addParentRelationships(2, WhileStatement, 3, CallStatement);
        addParentRelationships(2, WhileStatement, 4, CallStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("IfStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addParentRelationships(2, WhileStatement, 3, IfStatement);
        addParentRelationships(2, WhileStatement, 4, IfStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("PrintStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addParentRelationships(2, WhileStatement, 3, PrintStatement);
        addParentRelationships(2, WhileStatement, 4, PrintStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("ReadStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addParentRelationships(2, WhileStatement, 3, ReadStatement);
        addParentRelationships(2, WhileStatement, 4, ReadStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("WhileStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addParentRelationships(2, WhileStatement, 3, WhileStatement);
        addParentRelationships(2, WhileStatement, 4, WhileStatement);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }
}
*/

TEST_CASE("(vacuously true) query with such that Parent clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Parent(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent clause, left operand line number, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(2,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent clause, left operand line number, right operand synonym, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Parent(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * From this test onward, we will not be (intentionally) testing for vacuously
 * true queries, and queries in which at least one clause is not related to
 * the synonym, as there are too many combinations (we give the benefit of
 * doubt that our autotester system tests will cover
 * all of these cases).
 */

/*
 * Additionally, for this kind of test case, we would not be explicitly testing
 * for all StatementType, but only test AssignmentStatement type.
 */

TEST_CASE("query with such that Parent clause, left operand synonym, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(s,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    addParentRelationships(2, WhileStatement, 3, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent clause, left operand wildcard, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(_,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent clause, left operand line number, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(2,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent clause, left operand synonym, right operand synonym, but both synonyms are the same")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(s,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);
    addParentRelationships(3, WhileStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent clause, left operand wildcard, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(_,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);
    addParentRelationships(3, WhileStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "4, 3";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent clause, left operand wildcard, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Uses clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "variable v; Select v such that Uses(2,v)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();

    Vector<String> varNames;

    String varName1 = "a";
    String varName2 = "b";

    varNames.push_back(varName1);
    varNames.push_back(varName2);

    addUsesRelationships(2, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = varName2 + ", " + varName1;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Uses clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "variable v, v1; Select v such that Uses(2,v1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    Vector<String> varNames;

    String varName3 = "x";
    String varName4 = "y";

    varNames.push_back(varName3);
    varNames.push_back(varName4);

    addUsesRelationships(2, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = varName1 + ", " + varName2;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Uses clause, left operand line number, right operand wildcard")
{
    // === Test set-up ===
    String query = "variable v; Select v such that Uses(2,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    Vector<String> varNames;

    String varName3 = "x";
    String varName4 = "y";

    varNames.push_back(varName3);
    varNames.push_back(varName4);

    addUsesRelationships(2, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = varName1 + ", " + varName2;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Uses clause, left operand line number, right operand synonym, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "variable v, v1; Select v such that Uses(2,v1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * From this test onward, we will not be (intentionally) testing for vacuously
 * true queries, and queries in which at least one clause is not related to
 * the synonym, as there are too many combinations (we give the benefit of
 * doubt that our autotester system tests will cover
 * all of these cases).
 */

/*
 * Additionally, for this kind of test case, we would not be explicitly testing
 * for all StatementType, but only test AssignmentStatement type.
 */

TEST_CASE("query with such that Uses clause, left operand synonym, right operand variable name")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Uses(s,\"a\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    Vector<String> varNames;

    varNames.push_back(varName1);
    varNames.push_back(varName2);

    addUsesRelationships(2, AssignmentStatement, varNames);
    addUsesRelationships(3, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Uses clause, left operand procedure name, right operand variable name")
{
    // === Test set-up ===
    String query = "procedure p; Select p such that Uses(\"proc3\",\"x\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    String procName1 = "proc1";
    String procName2 = "proc2";

    insertIntoProcedureTable(procName1);
    insertIntoProcedureTable(procName2);

    Vector<String> varNames;

    String varName3 = "x";
    String varName4 = "y";

    varNames.push_back(varName3);
    varNames.push_back(varName4);

    String procName3 = "proc3";

    addUsesRelationships(procName3, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = procName1 + ", " + procName2;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Uses clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Uses(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<String> varNames;

    String varName1 = "a";
    String varName2 = "b";

    varNames.push_back(varName1);
    varNames.push_back(varName2);

    addUsesRelationships(2, AssignmentStatement, varNames);
    addUsesRelationships(3, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Modifies clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "variable v; Select v such that Modifies(2,v)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();

    Vector<String> varNames;

    String varName1 = "a";
    String varName2 = "b";

    varNames.push_back(varName1);
    varNames.push_back(varName2);

    addModifiesRelationships(2, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = varName2 + ", " + varName1;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Modifies clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "variable v, v1; Select v such that Modifies(2,v1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    Vector<String> varNames;

    String varName3 = "x";
    String varName4 = "y";

    varNames.push_back(varName3);
    varNames.push_back(varName4);

    addModifiesRelationships(2, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = varName1 + ", " + varName2;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Modifies clause, left operand line number, right operand wildcard")
{
    // === Test set-up ===
    String query = "variable v; Select v such that Modifies(2,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    Vector<String> varNames;

    String varName3 = "x";
    String varName4 = "y";

    varNames.push_back(varName3);
    varNames.push_back(varName4);

    addModifiesRelationships(2, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = varName1 + ", " + varName2;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Modifies clause, left operand line number, right operand synonym, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "variable v, v1; Select v such that Modifies(2,v1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * From this test onward, we will not be (intentionally) testing for vacuously
 * true queries, and queries in which at least one clause is not related to
 * the synonym, as there are too many combinations (we give the benefit of
 * doubt that our autotester system tests will cover
 * all of these cases).
 */

/*
 * Additionally, for this kind of test case, we would not be explicitly testing
 * for all StatementType, but only test AssignmentStatement type.
 */

TEST_CASE("query with such that Modifies clause, left operand synonym, right operand variable name")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Modifies(s,\"a\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    Vector<String> varNames;

    varNames.push_back(varName1);
    varNames.push_back(varName2);

    addModifiesRelationships(2, AssignmentStatement, varNames);
    addModifiesRelationships(3, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Modifies clause, left operand procedure name, right operand variable name")
{
    // === Test set-up ===
    String query = "procedure p; Select p such that Modifies(\"proc3\",\"x\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    String varName1 = "a";
    String varName2 = "b";

    insertIntoVariableTable(varName1);
    insertIntoVariableTable(varName2);

    String procName1 = "proc1";
    String procName2 = "proc2";

    insertIntoProcedureTable(procName1);
    insertIntoProcedureTable(procName2);

    Vector<String> varNames;

    String varName3 = "x";
    String varName4 = "y";

    varNames.push_back(varName3);
    varNames.push_back(varName4);

    String procName3 = "proc3";

    addModifiesRelationships(procName3, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = procName1 + ", " + procName2;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Modifies clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Modifies(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<String> varNames;

    String varName1 = "a";
    String varName2 = "b";

    varNames.push_back(varName1);
    varNames.push_back(varName2);

    addModifiesRelationships(2, AssignmentStatement, varNames);
    addModifiesRelationships(3, AssignmentStatement, varNames);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows* clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(2,s)";

    QueryResultFormatType format = AutotesterFormat;

    SECTION("AssignmentStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> afterStmttypePairs;

        Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, AssignmentStatement);
        Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, AssignmentStatement);

        afterStmttypePairs.push_back(afterStmttypePair1);
        afterStmttypePairs.push_back(afterStmttypePair2);

        addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("CallStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> afterStmttypePairs;

        Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, CallStatement);
        Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, CallStatement);

        afterStmttypePairs.push_back(afterStmttypePair1);
        afterStmttypePairs.push_back(afterStmttypePair2);

        addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("IfStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> afterStmttypePairs;

        Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, IfStatement);
        Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, IfStatement);

        afterStmttypePairs.push_back(afterStmttypePair1);
        afterStmttypePairs.push_back(afterStmttypePair2);

        addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("PrintStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> afterStmttypePairs;

        Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, PrintStatement);
        Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, PrintStatement);

        afterStmttypePairs.push_back(afterStmttypePair1);
        afterStmttypePairs.push_back(afterStmttypePair2);

        addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("ReadStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> afterStmttypePairs;

        Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, ReadStatement);
        Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, ReadStatement);

        afterStmttypePairs.push_back(afterStmttypePair1);
        afterStmttypePairs.push_back(afterStmttypePair2);

        addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("WhileStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> afterStmttypePairs;

        Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, WhileStatement);
        Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, WhileStatement);

        afterStmttypePairs.push_back(afterStmttypePair1);
        afterStmttypePairs.push_back(afterStmttypePair2);

        addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }
}

TEST_CASE("(vacuously true) query with such that Follows* clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Follows*(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs;

    Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, CallStatement);

    afterStmttypePairs.push_back(afterStmttypePair1);
    afterStmttypePairs.push_back(afterStmttypePair2);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows* clause, left operand line number, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(2,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs;

    Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, CallStatement);

    afterStmttypePairs.push_back(afterStmttypePair1);
    afterStmttypePairs.push_back(afterStmttypePair2);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows* clause, left operand line number, right operand synonym, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Follows*(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * From this test onward, we will not be (intentionally) testing for vacuously
 * true queries, and queries in which at least one clause is not related to
 * the synonym, as there are too many combinations (we give the benefit of
 * doubt that our autotester system tests will cover
 * all of these cases).
 */

/*
 * Additionally, for this kind of test case, we would not be explicitly testing
 * for all StatementType, but only test AssignmentStatement type.
 */

TEST_CASE("query with such that Follows* clause, left operand synonym, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(s,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<Pair<Integer, StatementType>> afterStmttypePairs1;

    Pair<Integer, StatementType> afterStmttypePair11 = std::make_pair(3, CallStatement);

    afterStmttypePairs1.push_back(afterStmttypePair11);

    addFollowsRelationshipsStar(1, AssignmentStatement, afterStmttypePairs1);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs2;

    Pair<Integer, StatementType> afterStmttypePair21 = std::make_pair(3, CallStatement);

    afterStmttypePairs2.push_back(afterStmttypePair21);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "2, 1";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows* clause, left operand wildcard, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(_,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs;

    Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, CallStatement);

    afterStmttypePairs.push_back(afterStmttypePair1);
    afterStmttypePairs.push_back(afterStmttypePair2);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows* clause, left operand line number, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(2,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs;

    Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, CallStatement);

    afterStmttypePairs.push_back(afterStmttypePair1);
    afterStmttypePairs.push_back(afterStmttypePair2);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows* clause, left operand synonym, right operand synonym, but both synonyms are the same")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(s,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs1;

    Pair<Integer, StatementType> afterStmttypePair11 = std::make_pair(3, CallStatement);

    afterStmttypePairs1.push_back(afterStmttypePair11);

    addFollowsRelationshipsStar(1, AssignmentStatement, afterStmttypePairs1);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs2;

    Pair<Integer, StatementType> afterStmttypePair21 = std::make_pair(3, CallStatement);

    afterStmttypePairs2.push_back(afterStmttypePair21);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows* clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<Pair<Integer, StatementType>> afterStmttypePairs1;

    Pair<Integer, StatementType> afterStmttypePair11 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair12 = std::make_pair(4, CallStatement);

    afterStmttypePairs1.push_back(afterStmttypePair11);
    afterStmttypePairs1.push_back(afterStmttypePair12);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs1);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs2;

    Pair<Integer, StatementType> afterStmttypePair21 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair22 = std::make_pair(4, CallStatement);

    afterStmttypePairs2.push_back(afterStmttypePair21);
    afterStmttypePairs2.push_back(afterStmttypePair22);

    addFollowsRelationshipsStar(3, AssignmentStatement, afterStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows* clause, left operand wildcard, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(_,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<Pair<Integer, StatementType>> afterStmttypePairs1;

    Pair<Integer, StatementType> afterStmttypePair11 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair12 = std::make_pair(4, CallStatement);

    afterStmttypePairs1.push_back(afterStmttypePair11);
    afterStmttypePairs1.push_back(afterStmttypePair12);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs1);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs2;

    Pair<Integer, StatementType> afterStmttypePair21 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair22 = std::make_pair(5, CallStatement);

    afterStmttypePairs2.push_back(afterStmttypePair21);
    afterStmttypePairs2.push_back(afterStmttypePair22);

    addFollowsRelationshipsStar(3, AssignmentStatement, afterStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "5, 4, 3";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows* clause, left operand wildcard, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows*(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> afterStmttypePairs;

    Pair<Integer, StatementType> afterStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> afterStmttypePair2 = std::make_pair(4, CallStatement);

    afterStmttypePairs.push_back(afterStmttypePair1);
    afterStmttypePairs.push_back(afterStmttypePair2);

    addFollowsRelationshipsStar(2, AssignmentStatement, afterStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent* clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(2,s)";

    QueryResultFormatType format = AutotesterFormat;

    SECTION("AssignmentStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

        Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, AssignmentStatement);
        Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, AssignmentStatement);

        childrenStmttypePairs.push_back(childrenStmttypePair1);
        childrenStmttypePairs.push_back(childrenStmttypePair2);

        addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("CallStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

        Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, CallStatement);
        Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, CallStatement);

        childrenStmttypePairs.push_back(childrenStmttypePair1);
        childrenStmttypePairs.push_back(childrenStmttypePair2);

        addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("IfStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

        Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, IfStatement);
        Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, IfStatement);

        childrenStmttypePairs.push_back(childrenStmttypePair1);
        childrenStmttypePairs.push_back(childrenStmttypePair2);

        addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("PrintStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

        Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, PrintStatement);
        Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, PrintStatement);

        childrenStmttypePairs.push_back(childrenStmttypePair1);
        childrenStmttypePairs.push_back(childrenStmttypePair2);

        addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("ReadStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

        Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, ReadStatement);
        Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, ReadStatement);

        childrenStmttypePairs.push_back(childrenStmttypePair1);
        childrenStmttypePairs.push_back(childrenStmttypePair2);

        addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }

    SECTION("WhileStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();

        Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

        Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, WhileStatement);
        Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, WhileStatement);

        childrenStmttypePairs.push_back(childrenStmttypePair1);
        childrenStmttypePairs.push_back(childrenStmttypePair2);

        addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

        PqlManager pqlManager;


        // === Execute test method ===
        FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


        // === Expected test results ===
        String expectedResultsStr = "4, 3";
        FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


        // === Check expected test results ===
        REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
    }
}

TEST_CASE("(vacuously true) query with such that Parent* clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Parent*(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

    Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, CallStatement);

    childrenStmttypePairs.push_back(childrenStmttypePair1);
    childrenStmttypePairs.push_back(childrenStmttypePair2);

    addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent* clause, left operand line number, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(2,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

    Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, CallStatement);

    childrenStmttypePairs.push_back(childrenStmttypePair1);
    childrenStmttypePairs.push_back(childrenStmttypePair2);

    addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent* clause, left operand line number, right operand synonym, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Parent*(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * From this test onward, we will not be (intentionally) testing for vacuously
 * true queries, and queries in which at least one clause is not related to
 * the synonym, as there are too many combinations (we give the benefit of
 * doubt that our autotester system tests will cover
 * all of these cases).
 */

/*
 * Additionally, for this kind of test case, we would not be explicitly testing
 * for all StatementType, but only test AssignmentStatement type.
 */

TEST_CASE("query with such that Parent* clause, left operand synonym, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(s,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<Pair<Integer, StatementType>> childrenStmttypePairs1;

    Pair<Integer, StatementType> childrenStmttypePair11 = std::make_pair(3, CallStatement);

    childrenStmttypePairs1.push_back(childrenStmttypePair11);

    addParentRelationshipsStar(1, AssignmentStatement, childrenStmttypePairs1);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs2;

    Pair<Integer, StatementType> childrenStmttypePair21 = std::make_pair(3, CallStatement);

    childrenStmttypePairs2.push_back(childrenStmttypePair21);

    addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "2, 1";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent* clause, left operand wildcard, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(_,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

    Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, CallStatement);

    childrenStmttypePairs.push_back(childrenStmttypePair1);
    childrenStmttypePairs.push_back(childrenStmttypePair2);

    addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent* clause, left operand line number, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(2,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

    Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, CallStatement);

    childrenStmttypePairs.push_back(childrenStmttypePair1);
    childrenStmttypePairs.push_back(childrenStmttypePair2);

    addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent* clause, left operand synonym, right operand synonym, but both synonyms are the same")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(s,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs1;

    Pair<Integer, StatementType> childrenStmttypePair11 = std::make_pair(3, CallStatement);

    childrenStmttypePairs1.push_back(childrenStmttypePair11);

    addParentRelationshipsStar(1, AssignmentStatement, childrenStmttypePairs1);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs2;

    Pair<Integer, StatementType> childrenStmttypePair21 = std::make_pair(3, CallStatement);

    childrenStmttypePairs2.push_back(childrenStmttypePair21);

    addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent* clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<Pair<Integer, StatementType>> childrenStmttypePairs1;

    Pair<Integer, StatementType> childrenStmttypePair11 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair12 = std::make_pair(4, CallStatement);

    childrenStmttypePairs1.push_back(childrenStmttypePair11);
    childrenStmttypePairs1.push_back(childrenStmttypePair12);

    addParentRelationshipsStar(2, AssignmentStatement, childrenStmttypePairs1);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs2;

    Pair<Integer, StatementType> childrenStmttypePair21 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair22 = std::make_pair(4, CallStatement);

    childrenStmttypePairs2.push_back(childrenStmttypePair21);
    childrenStmttypePairs2.push_back(childrenStmttypePair22);

    addParentRelationshipsStar(3, AssignmentStatement, childrenStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent* clause, left operand wildcard, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(_,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<Pair<Integer, StatementType>> childrenStmttypePairs1;

    Pair<Integer, StatementType> childrenStmttypePair11 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair12 = std::make_pair(4, CallStatement);

    childrenStmttypePairs1.push_back(childrenStmttypePair11);
    childrenStmttypePairs1.push_back(childrenStmttypePair12);

    addParentRelationshipsStar(2, AssignmentStatement, childrenStmttypePairs1);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs2;

    Pair<Integer, StatementType> childrenStmttypePair21 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair22 = std::make_pair(5, CallStatement);

    childrenStmttypePairs2.push_back(childrenStmttypePair21);
    childrenStmttypePairs2.push_back(childrenStmttypePair22);

    addParentRelationshipsStar(3, AssignmentStatement, childrenStmttypePairs2);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "5, 4, 3";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent* clause, left operand wildcard, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent*(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    Vector<Pair<Integer, StatementType>> childrenStmttypePairs;

    Pair<Integer, StatementType> childrenStmttypePair1 = std::make_pair(3, AssignmentStatement);
    Pair<Integer, StatementType> childrenStmttypePair2 = std::make_pair(4, CallStatement);

    childrenStmttypePairs.push_back(childrenStmttypePair1);
    childrenStmttypePairs.push_back(childrenStmttypePair2);

    addParentRelationshipsStar(2, WhileStatement, childrenStmttypePairs);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * Up to this point, we have tested all the such that clauses, now we turn our
 * attention to pattern clause.
 */

TEST_CASE("(vacuously true) query with Pattern clause, left operand variable name, right operand full expression")
{
    // === Test set-up ===
    String query = "assign a; stmt s; Select s pattern a(\"sum\",\"(num1 + num2) * num3\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with Pattern clause, left operand variable name, right operand wildcard partial expression")
{
    // === Test set-up ===
    String query = "assign a; stmt s; Select s pattern a(\"sum\",_\"(num1 + num 2)\"_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with Pattern clause, left operand variable name, right operand wildcard expression")
{
    // === Test set-up ===
    String query = "assign a; stmt s; Select s pattern a(\"sum\",_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with Pattern clause, left operand variable name, right operand full expression, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "assign a; stmt s; Select s pattern a(\"sum\",\"(num1 + num2) + num3\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * From this test onward, we will not be (intentionally) testing for vacuously
 * true queries, and queries in which at least one clause is not related to
 * the synonym, as there are too many combinations (we give the benefit of
 * doubt that our autotester system tests will cover
 * all of these cases).
 */


TEST_CASE("query with Pattern clause, left operand synonym, right operand full expression")
{
    // === Test set-up ===
    String query = "assign a; variable v; Select v pattern a(v,\"(num1 + num2) * num3\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "sum";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with Pattern clause, left operand synonym, right operand wildcard partial expression")
{
    // === Test set-up ===
    String query = "assign a; variable v; Select v pattern a(v,_\"(num1 + num 2)\"_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "sum";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with Pattern clause, left operand synonym, right operand wildcard expression")
{
    // === Test set-up ===
    String query = "assign a; variable v; Select v pattern a(v,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "ave, sum";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with Pattern clause, left operand variable synonym, right operand full expression, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "assign a; stmt s; variable v; Select s pattern a(v,\"(num1 + num2) + num3\")";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with Pattern clause, left operand wildcard, right operand wildcard")
{
    // === Test set-up ===
    String query = "assign a; stmt s; Select s pattern a(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * Up to this point, we have been testing with a single clause, now we turn our
 * attention to possible combinations of 2 clauses (a pattern clause, and some
 * other clause that is not pattern).
 */
TEST_CASE("(vacuously true) query with such that Follows clause, left operand synonym, right operand wildcard, Pattern clause, left operand wildcard, right operand wildcard, but clauses unrelated to synonym, all operands in clauses unrelated")
{
    // === Test set-up ===
    String query = "stmt s, s1; assign a; Select s such that Follows(s1,_) pattern a(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows clause, left operand synonym, right operand wildcard, Pattern clause, left operand wildcard, right operand wildcard, but clauses unrelated to synonym, at least one operand in clauses is related")
{
    // === Test set-up ===
    String query = "stmt s; assign a; Select s such that Follows(a,_) pattern a(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    addFollowsRelationships(4, AssignmentStatement, 5, AssignmentStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 4, 5, 6, 7, 8";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand synonym, right operand wildcard, Pattern clause, left operand wildcard, right operand wildcard, but clauses unrelated to synonym, all clauses in operands unrelated, and at least one clause is false")
{
    // === Test set-up ===
    String query = "stmt s, s1; assign a; Select s such that Follows(s1,_) pattern a(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(4, CallStatement);
    insertIntoStatementTable(5, IfStatement);
    insertIntoStatementTable(6, PrintStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, WhileStatement);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * TODO: Get this test to work after merging from new master
 */
/*
TEST_CASE("query with such that Uses clause, left operand synonym, right operand synonym, Pattern clause, left operand synonym, right operand wildcard, at least one operand in clauses is related")
{
    // === Test set-up ===
    String query = "stmt s; variable v; assign a; Select s such that Uses(s,v) pattern a(v,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<String> varNames1;

    String varName1 = "num1";
    String varName2 = "num2";
    String varName3 = "num3";

    varNames1.push_back(varName1);
    varNames1.push_back(varName2);
    varNames1.push_back(varName3);

    addUsesRelationships(4, AssignmentStatement, varNames1);

    Vector<String> varNames2;

    String varName4 = "sum";

    varNames2.push_back(varName4);

    addUsesRelationships(5, AssignmentStatement, varNames2);

    Vector<String> varNames3;

    String varName5 = "ave";

    varNames3.push_back(varName5);

    addUsesRelationships(6, AssignmentStatement, varNames3);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "6, 5";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}
*/

TEST_CASE("query with such that Uses clause, left operand synonym, right operand synonym, Pattern clause, left operand synonym, right operand wildcard, all operands in clauses unrelated")
{
    // === Test set-up ===
    String query = "stmt s; variable v, v1; assign a; Select s such that Uses(s,v1) pattern a(v,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    resetPKB();
    Vector<String> varNames1;

    String varName1 = "num1";
    String varName2 = "num2";
    String varName3 = "num3";

    varNames1.push_back(varName1);
    varNames1.push_back(varName2);
    varNames1.push_back(varName3);

    addUsesRelationships(4, AssignmentStatement, varNames1);

    Vector<String> varNames2;

    String varName4 = "sum";

    varNames2.push_back(varName4);

    addUsesRelationships(5, AssignmentStatement, varNames2);

    Vector<String> varNames3;

    String varName5 = "ave";

    varNames3.push_back(varName5);

    addUsesRelationships(6, AssignmentStatement, varNames3);

    ProgramNode* rootNodeToAssign = getProgram1Tree_compute();
    assignRootNode(rootNodeToAssign);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "6, 5, 4";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    // REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}
