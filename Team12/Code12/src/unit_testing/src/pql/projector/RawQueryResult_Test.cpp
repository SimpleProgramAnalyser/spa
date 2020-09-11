/*
 * Tests for the public methods in RawQueryResult.cpp (under pql/projector).
 * Due to the OOP encapsulation, we cannot retrieve some private members of
 * the classes, and hence assume (give benefit of doubt) that the public
 * method; getResults(), is implemented correctly.
 */

#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/parser/AqTypes.h"
#include "pql/projector/RawQueryResult.h"
#include "Types.h"

TEST_CASE("RawQueryResult::RawQueryResult(Vector<Vector<String>> results) -> empty results vector")
{
    // === Test set-up ===
    Vector<Vector<String>> results;


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Vector<Vector<String>> expectedResults = results;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.getResults() == expectedResults);
}

TEST_CASE("RawQueryResult::RawQueryResult(Vector<Vector<String>> results) -> single item in results vector")
{
    // === Test set-up ===
    Vector<Vector<String>> results;
    Vector<String> result;

    String a = "a";
    String b = "b";

    result.push_back(a);
    result.push_back(b);

    results.push_back(result);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Vector<Vector<String>> expectedResults = results;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.getResults() == expectedResults);
}

TEST_CASE("RawQueryResult::RawQueryResult(Vector<Vector<String>> results) -> multiple items in results vector")
{
    // === Test set-up ===
    Vector<Vector<String>> results;
    Vector<String> result;

    String a = "a";
    String b = "b";

    result.push_back(a);
    result.push_back(b);

    results.push_back(result);

    String c = "c";
    String d = "d";

    result.push_back(c);
    result.push_back(d);

    results.push_back(result);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Vector<Vector<String>> expectedResults = results;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.getResults() == expectedResults);
}

TEST_CASE("RawQueryResult::emptyRawQueryResult() -> ")
{
    // === Test set-up ===
    Vector<Vector<String>> results;


    // === Execute test method ===
    RawQueryResult rawQueryResult = RawQueryResult::emptyRawQueryResult();


    // === Expected test results ===
    Vector<Vector<String>> expectedResults = results;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.getResults() == expectedResults);
}

TEST_CASE("RawQueryResult::isEmpty() -> empty results vector")
{
    // === Test set-up ===
    Vector<Vector<String>> results;


    // === Execute test method ===
    RawQueryResult rawQueryResult = RawQueryResult::emptyRawQueryResult();


    // === Expected test results ===
    Boolean expectedEmpty = true;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.isEmpty() == expectedEmpty);
}

TEST_CASE("RawQueryResult::isEmpty() -> non-empty results vector")
{
    // === Test set-up ===
    Vector<Vector<String>> results;
    Vector<String> result;

    String a = "a";
    String b = "b";

    result.push_back(a);
    result.push_back(b);

    results.push_back(result);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Boolean expectedEmpty = false;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.isEmpty() == expectedEmpty);
}
