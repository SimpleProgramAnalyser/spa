/*
 * Integration tests for the public methods in Evaluator.cpp
 * (under pql/evaluator). Because Evaluator relies on the
 * Program Knowledge Base heavily, unit testing is difficult
 * to perform. Most testing of the Evaluator is done by
 * the integration tests between Evaluator and PKB.
 */

#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"

std::string getErrorMessage()
{
    return "ERROR CODE 3735929054: PQL was not parsed. SIGSYNTAX obtained. This incident will be reported.";
}

TEST_CASE("Evaluator::evaluateQuery(AbstractQuery query) -> invalid (syntatically) PQL query")
{
    // === Test set-up ===
    AbstractQuery abstractQuery = AbstractQuery::invalidAbstractQuery();

    // === Execute test method ===
    RawQueryResult rawQueryResult = evaluateQuery(abstractQuery);

    // === Expected test results ===
    RawQueryResult expectedRawQueryResult = RawQueryResult::getSyntaxError(getErrorMessage());

    REQUIRE(rawQueryResult == expectedRawQueryResult);
}

//TEST_CASE("Evaluator::evaluateQuery(AbstractQuery query) -> vacuously true PQL query")
//{
//    // === Test set-up ===
//    AbstractQuery abstractQuery = AbstractQuery::invalidAbstractQuery();
//
//    // === Execute test method ===
//    RawQueryResult rawQueryResult = evaluateQuery(abstractQuery);
//
//    // === Expected test results ===
//    RawQueryResult expectedRawQueryResult{std::vector<String>()};
//
//    REQUIRE(rawQueryResult == expectedRawQueryResult);
//}
