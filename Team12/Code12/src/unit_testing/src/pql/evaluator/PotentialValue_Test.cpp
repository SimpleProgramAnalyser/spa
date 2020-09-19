/**
 * Unit tests for PotentialValue struct, used in
 * Query Evaluator and Relationship Table.
 */
#include "catch.hpp"
#include "pql/evaluator/EvaluatorUtils.h"

TEST_CASE("PotentialValues with same values are equal, and hash to same value")
{
    PotentialValue value1("asahskjals", "2292821");
    PotentialValue value2("asahskjals", "2292821");
    REQUIRE(PotentialValueHasher()(value1) == PotentialValueHasher()(value2));
    REQUIRE(value1 == value2);
}

TEST_CASE("PotentialValues with different values are not equal, hash to different value")
{
    PotentialValue value1("asahskjals", "2292821");
    PotentialValue value2("asahskjals", "2292820");
    REQUIRE_FALSE(PotentialValueHasher()(value1) == PotentialValueHasher()(value2));
    REQUIRE_FALSE(value1 == value2);
}

TEST_CASE("Position matters in PotentialValue equality")
{
    PotentialValue value1("asahskjals", "2292821");
    PotentialValue value2("2292821", "asahskjals");
    REQUIRE_FALSE(PotentialValueHasher()(value1) == PotentialValueHasher()(value2));
    REQUIRE_FALSE(value1 == value2);
}

TEST_CASE("PotentialValues can be found in unordered_map")
{
    PotentialValue value1("a", "21");
    PotentialValue value2("a", "21");
    std::unordered_map<PotentialValue, int, PotentialValueHasher> map;
    map.insert(std::pair<PotentialValue, int>(value1, 12345));
    REQUIRE(map.find(value2) != map.end());
}
