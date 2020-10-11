/**
 * Unit tests for the TableUpdate object used in
 * RelationshipsGraph, which represents a change
 * to the values and edges in the graph.
 *
 * Black-box tests are used for the classes.
 */

#include "catch.hpp"
#include "pql/evaluator/RelationshipsGraph.cpp"

typedef PotentialValue Pv;

RelationshipsGraph setUpTestingGraphForTableUpdates()
{
    return RelationshipsGraph({{1, {Pv("var1", "x"), Pv("var2", "y")}},
                               {2, {Pv("var1", "x"), Pv("var2", "d")}},
                               {3, {Pv("d", "c"), Pv("b", "d"), Pv("a", "b"), Pv("ba", "a")}},
                               {4, {Pv("a", "b"), Pv("var2", "d")}}},
                              5);
}

TEST_CASE("ValuesTableDelete, EdgesTableDelete removes values and edges correctly")
{
    RelationshipsGraph graph = setUpTestingGraphForTableUpdates();
    TableUpdate* instruction1 = new EdgesTableDelete(3, PotentialValue("a", "b"));
    TableUpdate* instruction2 = new ValuesTableDelete(PotentialValue("a", "b"), 3);
    (*instruction1)(graph);
    (*instruction2)(graph);
    REQUIRE(graph
            == RelationshipsGraph({{1, {Pv("var1", "x"), Pv("var2", "y")}},
                                   {2, {Pv("var1", "x"), Pv("var2", "d")}},
                                   {3, {Pv("d", "c"), Pv("b", "d"), Pv("ba", "a")}},
                                   {4, {Pv("a", "b"), Pv("var2", "d")}}},
                                  5));
    delete instruction1;
    delete instruction2;
}

TEST_CASE("ValuesTableDelete, EdgesTableDeleteEdge deletes entire edges correctly")
{
    setUpTestingGraphForTableUpdates();
    RelationshipsGraph graph = setUpTestingGraphForTableUpdates();
    TableUpdate* instruction1 = new EdgesTableDeleteEdge(4);
    TableUpdate* instruction2 = new ValuesTableDelete(PotentialValue("a", "b"), 4);
    TableUpdate* instruction3 = new ValuesTableDelete(PotentialValue("var2", "d"), 4);
    (*instruction1)(graph);
    (*instruction2)(graph);
    (*instruction3)(graph);
    REQUIRE(graph
            == RelationshipsGraph({{1, {Pv("var1", "x"), Pv("var2", "y")}},
                                   {2, {Pv("var1", "x"), Pv("var2", "d")}},
                                   {3, {Pv("d", "c"), Pv("b", "d"), Pv("ba", "a"), Pv("a", "b")}}},
                                  5));
    delete instruction1;
    delete instruction2;
    delete instruction3;
}

TEST_CASE("ValuesTableInsert, EdgesTableInsert inserts edges and values correctly")
{
    RelationshipsGraph graph = setUpTestingGraphForTableUpdates();
    TableUpdate* instruction1 = new EdgesTableInsert(4, PotentialValue("var1", "x"));
    TableUpdate* instruction2 = new EdgesTableInsert(4, PotentialValue("b", "d"));
    TableUpdate* instruction3 = new ValuesTableInsert(PotentialValue("b", "d"), 4);
    TableUpdate* instruction4 = new ValuesTableInsert(PotentialValue("var1", "x"), 4);
    (*instruction1)(graph);
    (*instruction2)(graph);
    (*instruction3)(graph);
    (*instruction4)(graph);
    REQUIRE(graph
            == RelationshipsGraph({{1, {Pv("var1", "x"), Pv("var2", "y")}},
                                   {2, {Pv("var1", "x"), Pv("var2", "d")}},
                                   {3, {Pv("d", "c"), Pv("b", "d"), Pv("a", "b"), Pv("ba", "a")}},
                                   {4, {Pv("a", "b"), Pv("var2", "d"), Pv("b", "d"), Pv("var1", "x")}}},
                                  5));
    delete instruction1;
    delete instruction2;
    delete instruction3;
    delete instruction4;
}

TEST_CASE("ValuesTableNewSet, ValuesTableInsert, EdgesTableInsert puts a new value correctly")
{
    RelationshipsGraph graph = setUpTestingGraphForTableUpdates();
    TableUpdate* instruction1 = new ValuesTableNewSet(PotentialValue("myNewVal", "12345"));
    TableUpdate* instruction2 = new ValuesTableInsert(PotentialValue("myNewVal", "12345"), 1);
    TableUpdate* instruction3 = new EdgesTableInsert(1, PotentialValue("myNewVal", "12345"));
    (*instruction1)(graph);
    (*instruction2)(graph);
    (*instruction3)(graph);
    REQUIRE(graph
            == RelationshipsGraph({{1, {Pv("var1", "x"), Pv("var2", "y"), Pv("myNewVal", "12345")}},
                                   {2, {Pv("var1", "x"), Pv("var2", "d")}},
                                   {3, {Pv("d", "c"), Pv("b", "d"), Pv("a", "b"), Pv("ba", "a")}},
                                   {4, {Pv("a", "b"), Pv("var2", "d")}}},
                                  5));
    delete instruction1;
    delete instruction2;
    delete instruction3;
}

TEST_CASE("ValuesTableForceInsertNewest, EdgesTableInsertToNewest puts a new value correctly")
{
    RelationshipsGraph graph = setUpTestingGraphForTableUpdates();
    TableUpdate* instruction1 = new EdgesTableNewSet();
    TableUpdate* instruction2 = new ValuesTableForceInsertNewest(PotentialValue("myNewVal", "12345"));
    TableUpdate* instruction3 = new EdgesTableInsertToNewest(PotentialValue("myNewVal", "12345"));
    TableUpdate* instruction4 = new ValuesTableForceInsertNewest(PotentialValue("myNewVal2", "123456"));
    TableUpdate* instruction5 = new EdgesTableInsertToNewest(PotentialValue("myNewVal2", "123456"));
    (*instruction1)(graph);
    (*instruction2)(graph);
    (*instruction3)(graph);
    (*instruction4)(graph);
    (*instruction5)(graph);
    REQUIRE(graph
            == RelationshipsGraph({{1, {Pv("var1", "x"), Pv("var2", "y")}},
                                   {2, {Pv("var1", "x"), Pv("var2", "d")}},
                                   {3, {Pv("d", "c"), Pv("b", "d"), Pv("a", "b"), Pv("ba", "a")}},
                                   {4, {Pv("a", "b"), Pv("var2", "d")}},
                                   {5, {Pv("myNewVal", "12345"), Pv("myNewVal2", "123456")}}},
                                  6));
    delete instruction1;
    delete instruction2;
    delete instruction3;
    delete instruction4;
    delete instruction5;
}

TEST_CASE("EdgesTableNewSet, EdgesTableInsertToNewest, ValuesTableInsertNewest puts a new edge correctly")
{
    RelationshipsGraph graph = setUpTestingGraphForTableUpdates();
    TableUpdate* instruction1 = new EdgesTableNewSet();
    TableUpdate* instruction2 = new EdgesTableInsertToNewest(PotentialValue("a", "b"));
    TableUpdate* instruction3 = new EdgesTableInsertToNewest(PotentialValue("var1", "x"));
    TableUpdate* instruction4 = new ValuesTableInsertNewest(PotentialValue("a", "b"));
    TableUpdate* instruction5 = new ValuesTableInsertNewest(PotentialValue("var1", "x"));
    (*instruction1)(graph);
    (*instruction2)(graph);
    (*instruction3)(graph);
    (*instruction4)(graph);
    (*instruction5)(graph);
    REQUIRE(graph
            == RelationshipsGraph({{1, {Pv("var1", "x"), Pv("var2", "y")}},
                                   {2, {Pv("var1", "x"), Pv("var2", "d")}},
                                   {3, {Pv("d", "c"), Pv("b", "d"), Pv("a", "b"), Pv("ba", "a")}},
                                   {4, {Pv("a", "b"), Pv("var2", "d")}},
                                   {5, {Pv("a", "b"), Pv("var1", "x")}}},
                                  6));
    delete instruction1;
    delete instruction2;
    delete instruction3;
    delete instruction4;
    delete instruction5;
}

TEST_CASE("ValuesTableForceInsertNewest acts the same as ValuesTableInsertNewest if values exist")
{
    RelationshipsGraph graph1 = setUpTestingGraphForTableUpdates();
    RelationshipsGraph graph2 = setUpTestingGraphForTableUpdates();
    TableUpdate* instruction1 = new EdgesTableNewSet();
    TableUpdate* instruction2 = new EdgesTableInsertToNewest(PotentialValue("a", "b"));
    TableUpdate* instruction3 = new EdgesTableInsertToNewest(PotentialValue("var1", "x"));
    TableUpdate* instruction4 = new ValuesTableInsertNewest(PotentialValue("a", "b"));
    TableUpdate* instruction5 = new ValuesTableInsertNewest(PotentialValue("var1", "x"));
    TableUpdate* instruction4f = new ValuesTableForceInsertNewest(PotentialValue("a", "b"));
    TableUpdate* instruction5f = new ValuesTableForceInsertNewest(PotentialValue("var1", "x"));

    (*instruction1)(graph1);
    (*instruction2)(graph1);
    (*instruction3)(graph1);
    (*instruction4)(graph1);
    (*instruction5)(graph1);

    (*instruction1)(graph2);
    (*instruction2)(graph2);
    (*instruction3)(graph2);
    (*instruction4f)(graph2);
    (*instruction5f)(graph2);

    REQUIRE(graph1 == graph2);

    delete instruction1;
    delete instruction2;
    delete instruction3;
    delete instruction4;
    delete instruction5;
    delete instruction4f;
    delete instruction5f;
}
