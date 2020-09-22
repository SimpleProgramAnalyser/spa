/**
 * Unit tests for the RelationshipsGraph object in
 * Query Evaluator, which holds the relationships
 * constraints from evaluating PQL clauses.
 *
 * A combination of black-box tests and white-box tests.
 */
#include "catch.hpp"
#include "pql/evaluator/ResultsTable.h"

TEST_CASE("RelationshipsGraph stores PotentialValue relationships correctly")
{

}

TEST_CASE("RelationshipsGraph updates ResultTable when a potential value no longer has any relationships")
{

}

TEST_CASE("RelationshipsGraph removes multiple potential values from ResultTable")
{

}

TEST_CASE("deleteFromGraph leaves other relationships untouched")
{

}

TEST_CASE("deleteFromGraph does nothing if no relationships exist for that synonym")
{

}

TEST_CASE("RelationshipsGraph cache stores Synonym relationships correctly") {

}

TEST_CASE("RelationshipsGraph cache is cleared after relationships are modified") {

}

TEST_CASE("checkIfRelated returns true if two potential values are related")
{

}

TEST_CASE("checkIfRelated returns false if two potential values are not in the graph")
{

}

TEST_CASE("checkIfRelated returns false if two potential values are unrelated")
{

}
