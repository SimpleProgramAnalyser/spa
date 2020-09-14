/*
 * Unit tests for the public methods in Projector.cpp (under pql/projector).
 * Due to the OOP encapsulation, we cannot retrieve some private members of
 * the classes, and hence assume (give benefit of doubt) that the public
 * method; getResults(), is implemented correctly.
 */

#include "catch.hpp"
#include "pql/preprocessor/AqTypes.h"
#include "pql/projector/FormattedQueryResult.h"
#include "pql/projector/Projector.h"
#include "pql/projector/RawQueryResult.h"
/*
TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> empty rawQueryResult")
{
    // === Test set-up ===
    RawQueryResult rawQueryResult = RawQueryResult::emptyRawQueryResult();

    Projector projector;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult);

    // === Expected test results ===
    String expectedResults = "";

    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResults);
}

TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> non-empty rawQueryResult with 1 element in "
          "outer list")
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

    RawQueryResult rawQueryResult(results);

    Projector projector;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult);

    // === Expected test results ===
    String expectedResults = "a,b";

    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResults);
}

TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> non-empty rawQueryResult with multiple "
          "elements in outer list")
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

    RawQueryResult rawQueryResult(results);

    Projector projector;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult);

    // === Expected test results ===
    String expectedResults = "a,b|c,d";

    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResults);
}

TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> non-empty rawQueryResult with multiple "
          "elements in outer list, duplicate elements in at least one inner list")
{
    // === Test set-up ===
    Vector<Vector<Vector<String>>> results;
    Vector<String> resultInner1;

    String a1 = "a";
    String b1 = "b";
    String dupB1 = "b";

    resultInner1.push_back(a1);
    resultInner1.push_back(b1);
    resultInner1.push_back(dupB1);

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

    RawQueryResult rawQueryResult(results);

    Projector projector;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult);

    // === Expected test results ===
    String expectedResults = "a,b|c,d";

    // === Check expected test results ===
    REQUIRE(formattedQueryResult.getResults() == expectedResults);
}*/
