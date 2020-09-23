/**
 * Unit tests for the RelationshipsGraph object in
 * Query Evaluator, which holds the relationships
 * constraints from evaluating PQL clauses.
 *
 * A combination of black-box tests and white-box tests.
 */
#include "EvaluatorTestingUtils.h"
#include "catch.hpp"
#include "pql/evaluator/ResultsTable.h"

RelationshipsGraph setUpTestingGraph()
{
    std::vector<std::pair<std::string, std::string>> redGreenRelationships(
        {{"ns25", "ew13"}, {"ns26", "ew14"}, {"ns1", "ew24"}});
    std::vector<std::pair<int, std::string>> numPurpleRelationships(
        {{6, "dhobyghaut"}, {3, "outrampark"}, {16, "sengkang"}});
    std::vector<std::pair<std::string, int>> circleNumRelationships({{"onenorth", 23},
                                                                     {"harbourfront", 29},
                                                                     {"bartley", 12},
                                                                     {"hollandvillage", 21},
                                                                     {"marymount", 16},
                                                                     {"dhobyghaut", 1}});
    std::vector<std::pair<int, int>> ccDtRelationships({{4, 15}, {19, 9}, {10, 26}});
    std::vector<std::pair<std::string, std::string>> circlePurpleRelationships(
        {{"harbourfront", "harbourfront"}, {"dhobyghaut", "dhobyghaut"}, {"serangoon", "serangoon"}});
    std::vector<std::pair<int, int>> interSynonymRelationships({{271, 271}, {559, 52}, {52, 271}});

    RelationshipsGraph graph;
    graph.insertRelationships(redGreenRelationships, "red", "green");
    graph.insertRelationships(numPurpleRelationships, "num", "purple");
    graph.insertRelationships(circleNumRelationships, "circle", "num");
    graph.insertRelationships(ccDtRelationships, "CC", "DT");
    graph.insertRelationships(circlePurpleRelationships, "circle", "purple");
    graph.insertRelationships(interSynonymRelationships, "interSynonym", "interSynonym");
    return graph;
}

ResultsTable setUpResultsTable()
{
    ResultsTable results(DeclarationTable{});
    results.filterTable("green",
                        std::vector<std::string>(
                            {"ew1",  "ew2",  "ew3",  "ew4",  "ew5",  "ew6",  "ew7",  "ew8",  "ew9",  "ew10", "ew11",
                             "ew12", "ew13", "ew14", "ew15", "ew16", "ew17", "ew18", "ew19", "ew20", "ew21", "ew22",
                             "ew23", "ew24", "ew25", "ew26", "ew27", "ew28", "ew29", "ew30", "ew31", "ew32", "ew33"}));
    results.filterTable("purple",
                        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut",
                                                  "littleindia", "farrerpark", "boonkeng", "potongpasir", "woodleigh",
                                                  "serangoon", "kovan", "hougang", "buangkok", "sengkang", "punggol"}));
    results.filterTable(
        "circle",
        std::vector<std::string>({"dhobyghaut",   "brasbasah",    "esplanade",      "promenade",   "nicollhighway",
                                  "stadium",      "mountbatten",  "dakota",         "payalebar",   "macpherson",
                                  "taiseng",      "bartley",      "serangoon",      "lorongchuan", "bishan",
                                  "marymount",    "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage",
                                  "buonavista",   "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",
                                  "labradorpark", "telokblangah", "harbourfront"}));
    return results;
}

TEST_CASE("RelationshipsGraph stores PotentialValue relationships correctly")
{
    RelationshipsGraph graph = setUpTestingGraph();
    REQUIRE(graph.checkIfRelated(PotentialValue("red", "ns26"), PotentialValue("green", "ew14")));
    REQUIRE(graph.checkIfRelated(PotentialValue("red", "ns1"), PotentialValue("green", "ew24")));
    REQUIRE(graph.checkIfRelated(PotentialValue("num", "3"), PotentialValue("purple", "outrampark")));
    REQUIRE(graph.checkIfRelated(PotentialValue("num", "16"), PotentialValue("purple", "sengkang")));
    REQUIRE(graph.checkIfRelated(PotentialValue("circle", "onenorth"), PotentialValue("num", "23")));
    REQUIRE(graph.checkIfRelated(PotentialValue("circle", "marymount"), PotentialValue("num", "16")));
    REQUIRE(graph.checkIfRelated(PotentialValue("CC", "4"), PotentialValue("DT", "15")));
    REQUIRE(graph.checkIfRelated(PotentialValue("DT", "9"), PotentialValue("CC", "19")));
    REQUIRE(graph.checkIfRelated(PotentialValue("DT", "26"), PotentialValue("CC", "10")));
    REQUIRE(graph.checkIfRelated(PotentialValue("interSynonym", "271"), PotentialValue("interSynonym", "271")));
    REQUIRE(graph.checkIfRelated(PotentialValue("interSynonym", "52"), PotentialValue("interSynonym", "559")));
    REQUIRE(graph.checkIfRelated(PotentialValue("interSynonym", "271"), PotentialValue("interSynonym", "52")));

    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("red", "ns26"), PotentialValue("green", "ew13")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("red", "ns25"), PotentialValue("green", "ew4")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "6"), PotentialValue("purple", "outrampark")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "16"), PotentialValue("purple", "outrampark")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "2"), PotentialValue("purple", "sengkang")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "12"), PotentialValue("purple", "serangoon")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "6"), PotentialValue("circle", "dhobyghaut")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("purple", "sengkang"), PotentialValue("circle", "marymount")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("CC", "4"), PotentialValue("CC", "19")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("CC", "4"), PotentialValue("DT", "16")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("DT", "15"), PotentialValue("DT", "9")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("NE", "6"), PotentialValue("CC", "1")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("DT", "15"), PotentialValue("CE", "1")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("interSynonym", "52"), PotentialValue("interSynonym", "52")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("interSynonym", "271"), PotentialValue("interSynonym", "559")));
}

TEST_CASE("RelationshipsGraph updates ResultTable when a potential value no longer has any relationships")
{
    RelationshipsGraph graph = setUpTestingGraph();
    ResultsTable results = setUpResultsTable();
    graph.deleteFromGraph(PotentialValue("red", "ns26"), &results);
    doVectorsHaveSameElementsVoid(
        results.get("green"),
        std::vector<std::string>({"ew1",  "ew2",  "ew3",  "ew4",  "ew5",  "ew6",  "ew7",  "ew8",
                                  "ew9",  "ew10", "ew11", "ew12", "ew13", "ew15", "ew16", "ew17",
                                  "ew18", "ew19", "ew20", "ew21", "ew22", "ew23", "ew24", "ew25",
                                  "ew26", "ew27", "ew28", "ew29", "ew30", "ew31", "ew32", "ew33"}));

    graph.deleteFromGraph(PotentialValue("circle", "harbourfront"), &results);
    doVectorsHaveSameElementsVoid(
        results.get("purple"),
        std::vector<std::string>({"outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia", "farrerpark",
                                  "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang", "buangkok",
                                  "sengkang", "punggol"}));
    // deleting from relationships graph does not check if
    // potential value is already deleted from results graph
    doVectorsHaveSameElementsVoid(
        results.get("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",    "esplanade",      "promenade",   "nicollhighway",
                                  "stadium",      "mountbatten",  "dakota",         "payalebar",   "macpherson",
                                  "taiseng",      "bartley",      "serangoon",      "lorongchuan", "bishan",
                                  "marymount",    "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage",
                                  "buonavista",   "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",
                                  "labradorpark", "telokblangah", "harbourfront"}));
}

TEST_CASE("RelationshipsGraph removes multiple potential values from ResultTable")
{
    RelationshipsGraph graph = setUpTestingGraph();
    ResultsTable results = setUpResultsTable();
    graph.deleteFromGraph(PotentialValue("num", "16"), &results);
    // sengkang is gone from purple
    doVectorsHaveSameElementsVoid(
        results.get("purple"),
        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia",
                                  "farrerpark", "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang",
                                  "buangkok", "punggol"}));
    // marymount is gone from circle
    doVectorsHaveSameElementsVoid(
        results.get("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",      "esplanade",   "promenade",      "nicollhighway",
                                  "stadium",      "mountbatten",    "dakota",      "payalebar",      "macpherson",
                                  "taiseng",      "bartley",        "serangoon",   "lorongchuan",    "bishan",
                                  "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage", "buonavista",
                                  "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",   "labradorpark",
                                  "telokblangah", "harbourfront"}));
}

TEST_CASE("deleteFromGraph leaves other relationships untouched")
{
    RelationshipsGraph graph = setUpTestingGraph();
    ResultsTable results = setUpResultsTable();
    graph.deleteFromGraph(PotentialValue("purple", "sengkang"), &results);

    doVectorsHaveSameElementsVoid(
        results.get("purple"),
        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia",
                                  "farrerpark", "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang",
                                  "buangkok", "punggol"}));
    doVectorsHaveSameElementsVoid(
        results.get("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",      "esplanade",   "promenade",      "nicollhighway",
                                  "stadium",      "mountbatten",    "dakota",      "payalebar",      "macpherson",
                                  "taiseng",      "bartley",        "serangoon",   "lorongchuan",    "bishan",
                                  "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage", "buonavista",
                                  "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",   "labradorpark",
                                  "telokblangah", "harbourfront"}));
}

TEST_CASE("deleteFromGraph does nothing to ResultsTable if no relationships exist for that potential value")
{
    RelationshipsGraph graph = setUpTestingGraph();
    ResultsTable resultsActual = setUpResultsTable();
    graph.deleteFromGraph(PotentialValue("blue", "ns26"), &resultsActual);
    ResultsTable resultsExpected = setUpResultsTable();

    REQUIRE(resultsActual == resultsExpected);
}

TEST_CASE("RelationshipsGraph cache stores Synonym relationships correctly")
{
    RelationshipsGraph graph = setUpTestingGraph();
    REQUIRE(graph.checkCachedRelationships("red", "green"));
    REQUIRE(graph.checkCachedRelationships("num", "purple"));
    REQUIRE(graph.checkCachedRelationships("circle", "num"));
    REQUIRE(graph.checkCachedRelationships("DT", "CC"));
    REQUIRE(graph.checkCachedRelationships("purple", "circle"));
    REQUIRE(graph.checkCachedRelationships("interSynonym", "interSynonym"));

    REQUIRE_FALSE(graph.checkCachedRelationships("interSynonym", "circle"));
    REQUIRE_FALSE(graph.checkCachedRelationships("num", "red"));
    REQUIRE_FALSE(graph.checkCachedRelationships("green", "yellow"));
}

TEST_CASE("RelationshipsGraph cache is not affected by potential value not in graph")
{
    RelationshipsGraph graphActual = setUpTestingGraph();
    ResultsTable results = setUpResultsTable();
    graphActual.deleteFromGraph(PotentialValue("blue", "ns26"), &results);
    RelationshipsGraph graphExpected = setUpTestingGraph();

    REQUIRE(graphActual.checkEqualIncludingCache(graphExpected));
}

TEST_CASE("RelationshipsGraph cache is cleared after relationships are modified")
{
    RelationshipsGraph graph = setUpTestingGraph();
    ResultsTable results = setUpResultsTable();
    REQUIRE(graph.checkCachedRelationships("red", "green"));
    REQUIRE(graph.checkCachedRelationships("num", "purple"));
    REQUIRE(graph.checkCachedRelationships("circle", "num"));
    REQUIRE(graph.checkCachedRelationships("DT", "CC"));
    REQUIRE(graph.checkCachedRelationships("purple", "circle"));
    REQUIRE(graph.checkCachedRelationships("interSynonym", "interSynonym"));

    graph.deleteFromGraph(PotentialValue("purple", "dhobyghaut"), &results);
    REQUIRE(graph.checkCachedRelationships("red", "green"));
    REQUIRE(graph.checkCachedRelationships("circle", "num"));
    REQUIRE(graph.checkCachedRelationships("DT", "CC"));
    REQUIRE(graph.checkCachedRelationships("interSynonym", "interSynonym"));
    REQUIRE_FALSE(graph.checkCachedRelationships("num", "purple"));
    REQUIRE_FALSE(graph.checkCachedRelationships("purple", "circle"));
}
