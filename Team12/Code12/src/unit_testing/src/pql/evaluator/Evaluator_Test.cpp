/*
 * Integration tests for the public methods in Evaluator.cpp
 * (under pql/evaluator). Due to the OOP encapsulation, we
 * cannot test/retrieve some members of the class, and will
 * only focus on testing the evaluateQuery(..) public method
 * (which is the only public method of the class).
 *
 * Additionally, as our evaluateQuery(..) method (indirectly)
 * calls PKB, and mocking stubs/dependencies is generally
 * hard in C++, we would be inserting actual records
 * (e.g, fake Follows relationships) in PKB and then
 * testing our evaluatQuery(..) method from there.
 *
 * Thus, the tests here in this class, are actually 
 * PQL-PKB integration tests.
 */

#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/parser/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

TEST_CASE("Evaluator::evaluateQuery(AbstractQuery query) -> invalid (syntatically) PQL query")
{
    // === Test set-up ===
    Evaluator evaluator;

    AbstractQuery abstractQuery = AbstractQuery::invalidAbstractQuery();


    // === Execute test method ===
    RawQueryResult rawQueryResult = evaluator.evaluateQuery(abstractQuery);


    // === Expected test results ===
    RawQueryResult expectedRawQueryResult = RawQueryResult::emptyRawQueryResult();

    REQUIRE(rawQueryResult == expectedRawQueryResult);
}

TEST_CASE("Evaluator::evaluateQuery(AbstractQuery query) -> vacuously true PQL query")
{
    // === Test set-up ===
    Evaluator evaluator;

    AbstractQuery abstractQuery = AbstractQuery::invalidAbstractQuery();


    // === Execute test method ===
    RawQueryResult rawQueryResult = evaluator.evaluateQuery(abstractQuery);


    // === Expected test results ===
    RawQueryResult expectedRawQueryResult = RawQueryResult::emptyRawQueryResult();

    REQUIRE(rawQueryResult == expectedRawQueryResult);
}
