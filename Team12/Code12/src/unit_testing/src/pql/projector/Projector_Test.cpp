/*
 * Unit tests for the public methods in Projector.cpp (under pql/projector).
 * Due to the OOP encapsulation, we cannot retrieve some private members of
 * the classes, and hence assume (give benefit of doubt) that the public
 * method; getResults(), is implemented correctly.
 */

#include "Ui.h"
#include "catch.hpp"
#include "pql/projector/FormattedQueryResult.h"
#include "pql/projector/Projector.h"

class UiStub: public Ui {
    Void postUiError(InputError err) override {}
};

TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> empty results list")
{
    // === Test set-up ===
    Vector<String> results;
    RawQueryResult rawQueryResult(results);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult, ui);

    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> non-empty results list, with single item only")
{
    // === Test set-up ===
    Vector<String> results;
    String result = "a";
    results.push_back(result);

    RawQueryResult rawQueryResult(results);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult, ui);

    // === Expected test results ===
    String expectedResultsStr = result;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> non-empty results list, with multiple items, "
          "should not sort")
{
    // === Test set-up ===
    Vector<String> results;

    String result1 = "b";
    String result2 = "a";

    results.push_back(result1);
    results.push_back(result2);

    RawQueryResult rawQueryResult(results);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult, ui);

    // === Expected test results ===
    String expectedResultsStr = result1 + ", " + result2;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("Projector::formatAutotester(RawQueryResult rawQueryResult) -> with syntax error")
{
    // === Test set-up ===
    String errorMessage = "SIGSEGV";
    RawQueryResult rawQueryResult = RawQueryResult::getSyntaxError(errorMessage);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatAutotester(rawQueryResult, ui);

    // === Expected test results ===
    FormattedQueryResult expectedFormattedQueryResults = FormattedQueryResult::emptyFormattedQueryResult();

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("Projector::formatUI(RawQueryResult rawQueryResult) -> empty results list")
{
    // === Test set-up ===
    Vector<String> results;
    RawQueryResult rawQueryResult(results);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatUI(rawQueryResult, ui);

    // === Expected test results ===
    String expectedResultsStr;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("Projector::formatUI(RawQueryResult rawQueryResult) -> non-empty results list, with single item only")
{
    // === Test set-up ===
    Vector<String> results;
    String result = "a";
    results.push_back(result);

    RawQueryResult rawQueryResult(results);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatUI(rawQueryResult, ui);

    // === Expected test results ===
    String expectedResultsStr = result;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE(
    "Projector::formatUI(RawQueryResult rawQueryResult) -> non-empty results list, with multiple items, should sort")
{
    // === Test set-up ===
    Vector<String> results;
    String result1 = "b";
    String result2 = "a";
    results.push_back(result1);
    results.push_back(result2);

    RawQueryResult rawQueryResult(results);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatUI(rawQueryResult, ui);

    // === Expected test results ===
    String expectedResultsStr = result2 + ", " + result1;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}

TEST_CASE("Projector::formatUI(RawQueryResult rawQueryResult) -> with syntax error")
{
    // === Test set-up ===
    String errorMessage = "";
    RawQueryResult rawQueryResult = RawQueryResult::getSyntaxError(errorMessage);
    Projector projector;
    UiStub ui;

    // === Execute test method ===
    FormattedQueryResult formattedQueryResult = projector.formatUI(rawQueryResult, ui);

    // === Expected test results ===
    String expectedResultsStr = errorMessage;
    FormattedQueryResult expectedFormattedQueryResults(expectedResultsStr);

    // === Check expected test results ===
    REQUIRE(formattedQueryResult == expectedFormattedQueryResults);
}
