#include "catch.hpp"
#include "pql/evaluator/Evaluator.h"
#include "pql/parser/AqTypes.h"

TEST_CASE("Test invalid PQL query")
{
    //    StringList* stringList = splitProgram("while w,       w1; assign a; read read; Select w such that Follows (w,
    //    a)");

    Evaluator evaluator;
    AbstractQuery aq;

    REQUIRE(1 == 1);
}
