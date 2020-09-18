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

    Boolean expectedIsSyntaxError = false;
    String expectedErrorMessage;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);

    REQUIRE(rawQueryResult.isSyntaxError == expectedIsSyntaxError);
    REQUIRE(rawQueryResult.errorMessage == expectedErrorMessage);
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

    Boolean expectedIsSyntaxError = false;
    String expectedErrorMessage;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);
    REQUIRE(rawQueryResult.get(0) == expectedGetFirstItem);

    REQUIRE(rawQueryResult.isSyntaxError == expectedIsSyntaxError);
    REQUIRE(rawQueryResult.errorMessage == expectedErrorMessage);
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

    Boolean expectedIsSyntaxError = false;
    String expectedErrorMessage;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);
    REQUIRE(rawQueryResult.get(0) == expectedGetFirstItem);
    REQUIRE(rawQueryResult.get(1) == expectedGetSecondItem);

    REQUIRE(rawQueryResult.isSyntaxError == expectedIsSyntaxError);
    REQUIRE(rawQueryResult.errorMessage == expectedErrorMessage);
}

TEST_CASE("RawQueryResult::getSyntaxError() -> ")
{
    // === Test set-up ===
    String errorMessage = "SIGSEV";


    // === Execute test method ===
    RawQueryResult rawQueryResult = RawQueryResult::getSyntaxError(errorMessage);


    // === Expected test results ===
    size_t expectedCount = 0;
    Boolean expectedIsEmpty = true;

    Boolean expectedIsSyntaxError = true;
    String expectedErrorMessage = errorMessage;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);

    REQUIRE(rawQueryResult.isSyntaxError == expectedIsSyntaxError);
    REQUIRE(rawQueryResult.errorMessage == expectedErrorMessage);
}

TEST_CASE("RawQueryResult::sort() -> alphabets only")
{
    // === Test set-up ===
    Vector<String> results;

    String result1 = "proc b";
    String result2 = "proc a";
    String result3 = "proc c";

    results.push_back(result1);
    results.push_back(result2);
    results.push_back(result3);

    RawQueryResult rawQueryResult(results);


    // === Execute test method ===
    rawQueryResult.sort();


    // === Expected test results ===
    size_t expectedCount = 3;
    Boolean expectedIsEmpty = false;
    String expectedGetFirstItem = result2;
    String expectedGetSecondItem = result1;
    String expectedGetThirdItem = result3;

    Boolean expectedIsSyntaxError = false;
    String expectedErrorMessage;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);
    REQUIRE(rawQueryResult.get(0) == expectedGetFirstItem);
    REQUIRE(rawQueryResult.get(1) == expectedGetSecondItem);
    REQUIRE(rawQueryResult.get(2) == expectedGetThirdItem);

    REQUIRE(rawQueryResult.isSyntaxError == expectedIsSyntaxError);
    REQUIRE(rawQueryResult.errorMessage == expectedErrorMessage);
}

TEST_CASE("RawQueryResult::sort() -> digits only")
{
    // === Test set-up ===
    Vector<String> results;

    String result1 = "2";
    String result2 = "1";
    String result3 = "3";

    results.push_back(result1);
    results.push_back(result2);
    results.push_back(result3);

    RawQueryResult rawQueryResult(results);


    // === Execute test method ===
    rawQueryResult.sort();


    // === Expected test results ===
    size_t expectedCount = 3;
    Boolean expectedIsEmpty = false;
    String expectedGetFirstItem = result2;
    String expectedGetSecondItem = result1;
    String expectedGetThirdItem = result3;

    Boolean expectedIsSyntaxError = false;
    String expectedErrorMessage;


    // === Check expected test results ===
    REQUIRE(rawQueryResult.count() == expectedCount);
    REQUIRE(rawQueryResult.isEmpty() == expectedIsEmpty);
    REQUIRE(rawQueryResult.get(0) == expectedGetFirstItem);
    REQUIRE(rawQueryResult.get(1) == expectedGetSecondItem);
    REQUIRE(rawQueryResult.get(2) == expectedGetThirdItem);

    REQUIRE(rawQueryResult.isSyntaxError == expectedIsSyntaxError);
    REQUIRE(rawQueryResult.errorMessage == expectedErrorMessage);
}

TEST_CASE(
    "RawQueryResult::==(const RawQueryResult rawQueryResult) const -> multiple items in results vector, all equals, no syntax error")
{
    // === Test set-up ===
    Vector<String> results1;

    String result11 = "proc a";
    String result12 = "proc b";

    results1.push_back(result11);
    results1.push_back(result12);

    RawQueryResult rawQueryResult1(results1);

    Vector<String> results2;

    String result21 = "proc a";
    String result22 = "proc b";

    results2.push_back(result21);
    results2.push_back(result22);

    RawQueryResult rawQueryResult2(results2);


    // === Execute test method ===
    Boolean equals = rawQueryResult1 == rawQueryResult2;


    // === Expected test results ===
    size_t expectedCount1 = 2;
    Boolean expectedIsEmpty1 = false;
    String expectedGetFirstItem1 = result11;
    String expectedGetSecondItem1 = result12;

    size_t expectedCount2 = 2;
    Boolean expectedIsEmpty2 = false;
    String expectedGetFirstItem2 = result21;
    String expectedGetSecondItem2 = result22;

    Boolean expectedEquals = true;


    // === Check expected test results ===
    REQUIRE(rawQueryResult1.count() == expectedCount1);
    REQUIRE(rawQueryResult1.isEmpty() == expectedIsEmpty1);
    REQUIRE(rawQueryResult1.get(0) == expectedGetFirstItem1);
    REQUIRE(rawQueryResult1.get(1) == expectedGetSecondItem1);

    REQUIRE(rawQueryResult2.count() == expectedCount2);
    REQUIRE(rawQueryResult2.isEmpty() == expectedIsEmpty2);
    REQUIRE(rawQueryResult2.get(0) == expectedGetFirstItem2);
    REQUIRE(rawQueryResult2.get(1) == expectedGetSecondItem2);

    REQUIRE(equals == expectedEquals);
}

TEST_CASE("RawQueryResult::==(const RawQueryResult rawQueryResult) const -> multiple items in results vector, at least "
          "one not equals, no syntax error")
{
    // === Test set-up ===
    Vector<String> results1;

    String result11 = "proc a";
    String result12 = "proc b";

    results1.push_back(result11);
    results1.push_back(result12);

    RawQueryResult rawQueryResult1(results1);

    Vector<String> results2;

    String result21 = "proc a";
    String result22 = "proc c";

    results2.push_back(result21);
    results2.push_back(result22);

    RawQueryResult rawQueryResult2(results2);


    // === Execute test method ===
    Boolean equals = rawQueryResult1 == rawQueryResult2;


    // === Expected test results ===
    size_t expectedCount1 = 2;
    Boolean expectedIsEmpty1 = false;
    String expectedGetFirstItem1 = result11;
    String expectedGetSecondItem1 = result12;

    size_t expectedCount2 = 2;
    Boolean expectedIsEmpty2 = false;
    String expectedGetFirstItem2 = result21;
    String expectedGetSecondItem2 = result22;

    Boolean expectedEquals = false;


    // === Check expected test results ===
    REQUIRE(rawQueryResult1.count() == expectedCount1);
    REQUIRE(rawQueryResult1.isEmpty() == expectedIsEmpty1);
    REQUIRE(rawQueryResult1.get(0) == expectedGetFirstItem1);
    REQUIRE(rawQueryResult1.get(1) == expectedGetSecondItem1);

    REQUIRE(rawQueryResult2.count() == expectedCount2);
    REQUIRE(rawQueryResult2.isEmpty() == expectedIsEmpty2);
    REQUIRE(rawQueryResult2.get(0) == expectedGetFirstItem2);
    REQUIRE(rawQueryResult2.get(1) == expectedGetSecondItem2);

    REQUIRE(equals == expectedEquals);
}

TEST_CASE(
    "RawQueryResult::==(const RawQueryResult rawQueryResult) const -> same syntax error and error message")
{
    // === Test set-up ===
    String errorMessage1 = "SIGSEV";

    RawQueryResult rawQueryResult1 = RawQueryResult::getSyntaxError(errorMessage1);

    String errorMessage2 = "SIGSEV";

    RawQueryResult rawQueryResult2 = RawQueryResult::getSyntaxError(errorMessage2);


    // === Execute test method ===
    Boolean equals = rawQueryResult1 == rawQueryResult2;


    // === Expected test results ===
    size_t expectedCount1 = 0;
    Boolean expectedIsEmpty1 = true;

    Boolean expectedIsSyntaxError1 = true;
    String expectedErrorMessage1 = errorMessage1;

    size_t expectedCount2 = 0;
    Boolean expectedIsEmpty2 = true;

    Boolean expectedIsSyntaxError2 = true;
    String expectedErrorMessage2 = errorMessage2;

    Boolean expectedEquals = true;


    // === Check expected test results ===
    REQUIRE(rawQueryResult1.count() == expectedCount1);
    REQUIRE(rawQueryResult1.isEmpty() == expectedIsEmpty1);

    REQUIRE(rawQueryResult2.count() == expectedCount2);
    REQUIRE(rawQueryResult2.isEmpty() == expectedIsEmpty2);

    REQUIRE(equals == expectedEquals);
}

TEST_CASE(
    "RawQueryResult::==(const RawQueryResult rawQueryResult) const -> same syntax error, different error message")
{
    // === Test set-up ===
    String errorMessage1 = "SIGSEV";

    RawQueryResult rawQueryResult1 = RawQueryResult::getSyntaxError(errorMessage1);

    String errorMessage2 = "SEGFAULT";

    RawQueryResult rawQueryResult2 = RawQueryResult::getSyntaxError(errorMessage2);


    // === Execute test method ===
    Boolean equals = rawQueryResult1 == rawQueryResult2;


    // === Expected test results ===
    size_t expectedCount1 = 0;
    Boolean expectedIsEmpty1 = true;

    Boolean expectedIsSyntaxError1 = true;
    String expectedErrorMessage1 = errorMessage1;

    size_t expectedCount2 = 0;
    Boolean expectedIsEmpty2 = true;

    Boolean expectedIsSyntaxError2 = true;
    String expectedErrorMessage2 = errorMessage2;

    Boolean expectedEquals = false;


    // === Check expected test results ===
    REQUIRE(rawQueryResult1.count() == expectedCount1);
    REQUIRE(rawQueryResult1.isEmpty() == expectedIsEmpty1);

    REQUIRE(rawQueryResult2.count() == expectedCount2);
    REQUIRE(rawQueryResult2.isEmpty() == expectedIsEmpty2);

    REQUIRE(equals == expectedEquals);
}
