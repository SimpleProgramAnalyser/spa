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
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

/*
 * TODO: Get this test case to work, with PKB side
 */
/*
TEST_CASE("vacuously true query no clauses")
{
    // === Test set-up ===
    String query = "stmt s; Select s";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

    PqlManager pqlManager;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = pqlManager.executeQuery(query, format);


    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);


    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}
*/

/*
 * TODO: Get this test case to work, with PKB side
 */
/*
TEST_CASE("query with such that follows clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(2,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
*/

/*
 * TODO: Get this test case to work, with PKB side
 */
/*
TEST_CASE("(vacuously true) query with such that follows clause, left operand line number, right operand synonym")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Follows(2,s1)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
*/

/*
 * TODO: Get this test case to work, with PKB side
 */
/*
TEST_CASE("(vacuously true) query with such that follows clause, left operand line number, right operand wildcard")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(2,_)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
*/

TEST_CASE("query with such that follows clause, left operand line number, right operand synonym, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(1000,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
 * TODO: Get this test case to work, with PKB side
 */
/*
TEST_CASE("query with such that follows clause, left operand synonym, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(s,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
*/

/*
 * TODO: Get this test case to work, with PKB side
 */
/*
TEST_CASE("(vacuously true) query with such that follows clause, left operand synonym , right operand line number")
{
    // === Test set-up ===
    String query = "stmt s, s1; Select s such that Follows(s1, 3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
*/

/*
 * TODO: Get this test case to work, with PKB side
 */
/*
TEST_CASE("(vacuously true) query with such that follows clause, left operand wildcard, right operand line number")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(_,3)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
*/

TEST_CASE("query with such that follows clause, left operand synonym, right operand line number, but clause unrelated to synonym and is false")
{
    // === Test set-up ===
    String query = "stmt s; Select s such that Follows(1000,s)";

    QueryResultFormatType format = AutotesterFormat;

    // Call PKB API to add some dummy relationships
    addFollowsRelationships(2, AnyStatement, 3, AnyStatement);

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
