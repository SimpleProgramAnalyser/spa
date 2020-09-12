/*
 * Unit tests for the public methods in RawQueryResult.cpp (under pql/projector).
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
    Vector<Vector<Vector<String>>> results;


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Vector<Vector<Vector<String>>> expectedResults = results;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.getResults() == expectedResults);
}

TEST_CASE("RawQueryResult::RawQueryResult(Vector<Vector<String>> results) -> single item in results vector")
{
    // === Test set-up ===
    Vector<Vector<Vector<String>>> results;
    Vector<String> resultInner;

    String a = "a";
    String b = "b";

    resultInner.push_back(a);
    resultInner.push_back(b);

    Vector<Vector<String>> resultOuter;
    resultOuter.push_back(resultInner);

    results.push_back(resultOuter);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Vector<Vector<Vector<String>>> expectedResults = results;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.getResults() == expectedResults);
}

TEST_CASE("RawQueryResult::RawQueryResult(Vector<Vector<String>> results) -> multiple items in results vector")
{
    // === Test set-up ===
    Vector<Vector<Vector<String>>> results;
    Vector<String> resultInner1;

    String a1 = "a";
    String b1 = "b";

    resultInner1.push_back(a1);
    resultInner1.push_back(b1);

    Vector<Vector<String>> resultOuter1;
    resultOuter1.push_back(resultInner1);

    results.push_back(resultOuter1);

    Vector<String> resultInner2;

    String a2 = "c";
    String b2 = "d";

    resultInner2.push_back(a2);
    resultInner2.push_back(b2);

    Vector<Vector<String>> resultOuter2;
    resultOuter2.push_back(resultInner2);

    results.push_back(resultOuter2);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Vector<Vector<Vector<String>>> expectedResults = results;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.getResults() == expectedResults);
}

TEST_CASE("RawQueryResult::emptyRawQueryResult() -> ")
{
    // === Test set-up ===
    Vector<Vector<Vector<String>>> results;


    // === Execute test method ===
    RawQueryResult rawQueryResult = RawQueryResult::emptyRawQueryResult();


    // === Expected test results ===
    Vector<Vector<Vector<String>>> expectedResults = results;


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
    Vector<Vector<Vector<String>>> results;
    Vector<String> resultInner;

    String a = "a";
    String b = "b";

    resultInner.push_back(a);
    resultInner.push_back(b);

    Vector<Vector<String>> resultOuter;
    resultOuter.push_back(resultInner);

    results.push_back(resultOuter);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    Boolean expectedEmpty = false;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.isEmpty() == expectedEmpty);
}
