/*
 * Unit tests for the public methods in RawQueryResult.cpp (under pql/projector).
 * Due to the OOP encapsulation, we cannot retrieve some private members of
 * the classes, and hence assume (give benefit of doubt) that the some public
 * methods, e.g,; count(), get(), isEmpty() is implemented correctly. Although, we
 * cannot test these methods in isolation, but would nonetheless test them
 * (with other methods).
 */

#include "Types.h"
#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/preprocessor/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

TEST_CASE("RawQueryResult::RawQueryResult(Vector<String> results) -> empty results vector")
{
    // === Test set-up ===
    Vector<String> results;


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    size_t expectedCount = 0;
    Boolean expectedIsEmpty = true;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);
}

TEST_CASE("RawQueryResult::RawQueryResult(Vector<String> results) -> single item in results vector")
{
    // === Test set-up ===
    Vector<String> results;

    String result = "proc";

    results.push_back(result);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    size_t expectedCount = 1;
    Boolean expectedIsEmpty = false;
    String expectedGetFirstItem = result;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);
    REQUIRE(rawQueryResult.get(0) == expectedGetFirstItem);
}

TEST_CASE("RawQueryResult::RawQueryResult(Vector<String> results) -> multiple items in results vector")
{
    // === Test set-up ===
    Vector<String> results;

    String result1 = "proc a";
    String result2 = "proc b";

    results.push_back(result1);
    results.push_back(result2);


    // === Execute test method ===
    RawQueryResult rawQueryResult(results);


    // === Expected test results ===
    size_t expectedCount = 2;
    Boolean expectedIsEmpty = false;
    String expectedGetFirstItem = result1;
    String expectedGetSecondItem = result2;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);
    REQUIRE(rawQueryResult.get(0) == expectedGetFirstItem);
    REQUIRE(rawQueryResult.get(1) == expectedGetSecondItem);
}
/*
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
}*/
