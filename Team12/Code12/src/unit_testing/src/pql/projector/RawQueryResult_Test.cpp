/*
 * Unit tests for the public methods in RawQueryResult.cpp (under pql/projector).
 * Due to the OOP encapsulation, we cannot retrieve some private members of
 * the classes, and hence assume (give benefit of doubt) that the public
 * method; getResults(), is implemented correctly.
 */

#include "Types.h"
#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/preprocessor/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

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

TEST_CASE(
    "RawQueryResult::==(const RawQueryResult rawQueryResult) const -> multiple items in results vector, all equals")
{
    // === Test set-up ===
    Vector<Vector<Vector<String>>> results1;
    Vector<String> resultInner11;

    String a11 = "a";
    String b11 = "b";

    resultInner11.push_back(a11);
    resultInner11.push_back(b11);

    Vector<Vector<String>> resultOuter11;
    resultOuter11.push_back(resultInner11);

    results1.push_back(resultOuter11);

    Vector<String> resultInner12;

    String a12 = "c";
    String b12 = "d";

    resultInner12.push_back(a12);
    resultInner12.push_back(b12);

    Vector<Vector<String>> resultOuter12;
    resultOuter12.push_back(resultInner12);

    results1.push_back(resultOuter12);

    RawQueryResult rawQueryResult1(results1);

    Vector<Vector<Vector<String>>> results2;
    Vector<String> resultInner21;

    String a21 = "a";
    String b21 = "b";

    resultInner21.push_back(a21);
    resultInner21.push_back(b21);

    Vector<Vector<String>> resultOuter21;
    resultOuter21.push_back(resultInner21);

    results2.push_back(resultOuter21);

    Vector<String> resultInner22;

    String a22 = "c";
    String b22 = "d";

    resultInner22.push_back(a22);
    resultInner22.push_back(b22);

    Vector<Vector<String>> resultOuter22;
    resultOuter22.push_back(resultInner22);

    results2.push_back(resultOuter22);

    RawQueryResult rawQueryResult2(results2);

    // === Execute test method ===
    Boolean equality = (rawQueryResult1 == rawQueryResult2);

    // === Expected test results ===
    Boolean expectedEquality = true;

    // === Check expected test results ===
    REQUIRE(equality == expectedEquality);
}

TEST_CASE("RawQueryResult::==(const RawQueryResult rawQueryResult) const -> multiple items in results vector, at least "
          "one not equals")
{
    // === Test set-up ===
    Vector<Vector<Vector<String>>> results1;
    Vector<String> resultInner11;

    String a11 = "a";
    String b11 = "c";

    resultInner11.push_back(a11);
    resultInner11.push_back(b11);

    Vector<Vector<String>> resultOuter11;
    resultOuter11.push_back(resultInner11);

    results1.push_back(resultOuter11);

    Vector<String> resultInner12;

    String a12 = "c";
    String b12 = "d";

    resultInner12.push_back(a12);
    resultInner12.push_back(b12);

    Vector<Vector<String>> resultOuter12;
    resultOuter12.push_back(resultInner12);

    results1.push_back(resultOuter12);

    RawQueryResult rawQueryResult1(results1);

    Vector<Vector<Vector<String>>> results2;
    Vector<String> resultInner21;

    String a21 = "a";
    String b21 = "b";

    resultInner21.push_back(a21);
    resultInner21.push_back(b21);

    Vector<Vector<String>> resultOuter21;
    resultOuter21.push_back(resultInner21);

    results2.push_back(resultOuter21);

    Vector<String> resultInner22;

    String a22 = "c";
    String b22 = "d";

    resultInner22.push_back(a22);
    resultInner22.push_back(b22);

    Vector<Vector<String>> resultOuter22;
    resultOuter22.push_back(resultInner22);

    results2.push_back(resultOuter22);

    RawQueryResult rawQueryResult2(results2);

    // === Execute test method ===
    Boolean equality = (rawQueryResult1 == rawQueryResult2);

    // === Expected test results ===
    Boolean expectedEquality = false;

    // === Check expected test results ===
    REQUIRE(equality == expectedEquality);
}
