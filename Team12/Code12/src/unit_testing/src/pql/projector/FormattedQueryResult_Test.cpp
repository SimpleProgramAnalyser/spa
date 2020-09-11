/*
 * Tests for the public methods in RawQueryResult.cpp (under pql/projector).
 * Due to the OOP encapsulation, we cannot retrieve some private members of
 * the classes, and hence assume (give benefit of doubt) that the public
 * method; getResults(), is implemented correctly.
 */

#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
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
