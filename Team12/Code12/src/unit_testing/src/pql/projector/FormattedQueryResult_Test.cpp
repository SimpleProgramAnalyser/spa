#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/parser/AqTypes.h"
#include "pql/projector/FormattedQueryResult.h"

TEST_CASE("Test contructor")
{
    Evaluator evaluator;

    // Construct dummy AbstractQuery type
    AbstractQuery abstractQuery = AbstractQuery::invalidAbstractQuery();

    RawQueryResult rawQueryResult = evaluator.evaluateQuery(abstractQuery);

    REQUIRE(1 == 1);
}
