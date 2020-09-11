#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/parser/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

TEST_CASE("RawQueryResult::RawQueryResult() -> empty results vector")
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

TEST_CASE("RawQueryResult::RawQueryResult() -> single item in results vector")
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
    REQUIRE(rawQueryResult.getResults() == results);
}

TEST_CASE("RawQueryResult::RawQueryResult() -> multiple items in results vector")
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
    REQUIRE(rawQueryResult.getResults() == results);
}
