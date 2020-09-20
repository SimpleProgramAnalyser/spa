/*
 * Integration tests between the PQL and PKB units of
 * our SPA system.
 *
 * (In this series of tests, we mainly focus on the
 * Autotester format (instead of the UI format).)
 */

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
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
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
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);
    addFollowsRelationships(3, AssignmentStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Follows clause, left operand wildcard, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(_,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AssignmentStatement, 3, AssignmentStatement);
    addFollowsRelationships(3, AssignmentStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "4, 3";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Follows clause, left operand wildcard, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
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
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(2,s)";

    QueryResultFormatType format = AutotesterFormat;

    /*
     * Obviously, there are many more possible combinations, however
     * we won't be exhaustively testing for all of them.
     */

    SECTION("AssignmentStatement type")
    {
        // Call PKB API to add some dummy relationships
        resetPKB();
        addParentRelationships(2, WhileStatement, 3, AssignmentStatement);

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
        addParentRelationships(2, WhileStatement, 3, CallStatement);

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
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
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
    addParentRelationships(2, WhileStatement, 3, WhileStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent clause, left operand wildcard, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(_,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
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
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent clause, left operand synonym, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(s,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);
    addParentRelationships(3, WhileStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "3, 2";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("query with such that Parent clause, left operand wildcard, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(_,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addParentRelationships(2, WhileStatement, 3, AssignmentStatement);
    addParentRelationships(3, WhileStatement, 4, AssignmentStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr = "4, 3";
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("(vacuously true) query with such that Parent clause, left operand wildcard, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Parent(_,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
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
    REQUIRE(formattedQueryResult.getResults() == expectedResultsStr);
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}
