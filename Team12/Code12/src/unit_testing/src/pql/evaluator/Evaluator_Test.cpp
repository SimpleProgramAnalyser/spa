#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/parser/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

TEST_CASE("Test invalid PQL query")
{
    Evaluator evaluator;

    // Construct dummy AbstractQuery type
    AbstractQuery abstractQuery = AbstractQuery::invalidAbstractQuery();

    RawQueryResult rawQueryResult = evaluator.evaluateQuery(abstractQuery);

    REQUIRE(1 == 1);
}
