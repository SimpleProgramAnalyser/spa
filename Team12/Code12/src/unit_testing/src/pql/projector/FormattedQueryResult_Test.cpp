/*
 * Unit tests for the public methods in RawQueryResult.cpp (under pql/projector).
 * Due to the OOP encapsulation, we cannot retrieve some private members of
 * the classes, and hence assume (give benefit of doubt) that the public
 * method; getResults(), is implemented correctly.
 */

#include "catch.hpp"
#include "pql/parser/AqTypes.h"
#include "pql/projector/FormattedQueryResult.h"

TEST_CASE("FormattedQueryResult::FormattedQueryResult(String results) -> empty results string")
{
    // === Test set-up ===
    String results;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult(results);


    // === Expected test results ===
    String expectedResults = results;


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResults);
}

TEST_CASE("FormattedQueryResult::FormattedQueryResult(String results) -> non-empty results string")
{
    // === Test set-up ===
    String results = "abc";


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult(results);


    // === Expected test results ===
    String expectedResults = results;


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResults);
}

TEST_CASE("FormattedQueryResult::emptyFormattedQueryResult() ->")
{
    // === Test set-up ===
    String results;


    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = FormattedQueryResult::emptyFormattedQueryResult();


    // === Expected test results ===
    String expectedResults = results;


    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResults);
}

TEST_CASE("FormattedQueryResult::==(const FormattedQueryResult) const -> results string equals")
{
    // === Test set-up ===
    String results1 = "abc";

    FormattedQueryResult formattedQueryResult1(results1);

    String results2 = "abc";

    FormattedQueryResult formattedQueryResult2(results2);


    // === Execute test method ===
    Boolean equality = (formattedQueryResult1 == formattedQueryResult2);


    // === Expected test results ===
    Boolean expectedEquality = true;


    // === Check expected test results ===
    REQUIRE(equality == expectedEquality);
}

TEST_CASE("FormattedQueryResult::==(const FormattedQueryResult) const -> results string not equals")
{
    // === Test set-up ===
    String results1 = "abc";

    FormattedQueryResult formattedQueryResult1(results1);

    String results2 = "def";

    FormattedQueryResult formattedQueryResult2(results2);


    // === Execute test method ===
    Boolean equality = (formattedQueryResult1 == formattedQueryResult2);


    // === Expected test results ===
    Boolean expectedEquality = false;


    // === Check expected test results ===
    REQUIRE(equality == expectedEquality);
}
