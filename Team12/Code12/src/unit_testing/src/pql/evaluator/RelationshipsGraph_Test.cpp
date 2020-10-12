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
    std::vector<std::pair<std::string, std::string>> numPurpleRelationships(
        {{"6", "dhobyghaut"}, {"3", "outrampark"}, {"16", "sengkang"}});
    std::vector<std::pair<std::string, std::string>> circleNumRelationships({{"onenorth", "23"},
                                                                             {"harbourfront", "29"},
                                                                             {"bartley", "12"},
                                                                             {"hollandvillage", "21"},
                                                                             {"marymount", "16"},
                                                                             {"dhobyghaut", "1"}});
    std::vector<std::pair<std::string, std::string>> ccDtRelationships({{"4", "15"}, {"19", "9"}, {"10", "26"}});
    std::vector<std::pair<std::string, std::string>> circlePurpleRelationships(
        {{"harbourfront", "harbourfront"}, {"dhobyghaut", "dhobyghaut"}, {"serangoon", "serangoon"}});
    std::vector<std::pair<std::string, std::string>> interSynonymRelationships(
        {{"271", "271"}, {"559", "52"}, {"52", "271"}});

    RelationshipsGraph graph;
    graph.insertRelationships(redGreenRelationships, "red", true, "green", true);
    graph.insertRelationships(numPurpleRelationships, "num", true, "purple", true);
    graph.insertRelationships(circleNumRelationships, "circle", true, "num", true);
    graph.insertRelationships(ccDtRelationships, "CC", true, "DT", true);
    graph.insertRelationships(circlePurpleRelationships, "circle", true, "purple", true);
    graph.insertRelationships(interSynonymRelationships, "interSynonym", true, "interSynonym", true);
    return graph;
}

std::unique_ptr<ResultsTable> setUpResultsTable()
{
    std::unique_ptr<ResultsTable> results = std::unique_ptr<ResultsTable>(new ResultsTable{DeclarationTable()});
    results->storeResultsOne(
        "green", std::vector<std::string>({"ew1",  "ew2",  "ew3",  "ew4",  "ew5",  "ew6",  "ew7",  "ew8",  "ew9",
                                           "ew10", "ew11", "ew12", "ew13", "ew14", "ew15", "ew16", "ew17", "ew18",
                                           "ew19", "ew20", "ew21", "ew22", "ew23", "ew24", "ew25", "ew26", "ew27",
                                           "ew28", "ew29", "ew30", "ew31", "ew32", "ew33"}));
    results->storeResultsOne(
        "purple", std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut",
                                            "littleindia", "farrerpark", "boonkeng", "potongpasir", "woodleigh",
                                            "serangoon", "kovan", "hougang", "buangkok", "sengkang", "punggol"}));
    results->storeResultsOne(
        "circle",
        std::vector<std::string>({"dhobyghaut",   "brasbasah",    "esplanade",      "promenade",   "nicollhighway",
                                  "stadium",      "mountbatten",  "dakota",         "payalebar",   "macpherson",
                                  "taiseng",      "bartley",      "serangoon",      "lorongchuan", "bishan",
                                  "marymount",    "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage",
                                  "buonavista",   "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",
                                  "labradorpark", "telokblangah", "harbourfront"}));
    results->storeResultsOne("num",
                             std::vector<std::string>({"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
                                                       "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                                                       "21", "22", "23", "24", "25", "26", "27", "28", "29"}));
    return std::move(results);
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
    REQUIRE(graph.checkIfRelated(PotentialValue("interSynonym", "52"), PotentialValue("interSynonym", "52")));

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
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("interSynonym", "271"), PotentialValue("interSynonym", "559")));
}

TEST_CASE("RelationshipsGraph updates ResultTable when a potential value no longer has any relationships")
{
    RelationshipsGraph graph = setUpTestingGraph();
    std::unique_ptr<ResultsTable> results = setUpResultsTable();
    // force early evaluation
    results->getResultsZero();
    // manual deletion of potential value
    graph.deleteOne(PotentialValue("red", "ns26"), results.get());
    requireVectorsHaveSameElements(
        results->getResultsOne("green"),
        std::vector<std::string>({"ew1",  "ew2",  "ew3",  "ew4",  "ew5",  "ew6",  "ew7",  "ew8",
                                  "ew9",  "ew10", "ew11", "ew12", "ew13", "ew15", "ew16", "ew17",
                                  "ew18", "ew19", "ew20", "ew21", "ew22", "ew23", "ew24", "ew25",
                                  "ew26", "ew27", "ew28", "ew29", "ew30", "ew31", "ew32", "ew33"}));

    graph.deleteOne(PotentialValue("circle", "harbourfront"), results.get());
    requireVectorsHaveSameElements(
        results->getResultsOne("purple"),
        std::vector<std::string>({"outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia", "farrerpark",
                                  "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang", "buangkok",
                                  "sengkang", "punggol"}));
    // deleting from relationships graph does not check if
    // potential value is already deleted from results graph
    requireVectorsHaveSameElements(
        results->getResultsOne("circle"),
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
    std::unique_ptr<ResultsTable> results = setUpResultsTable();
    // force early evaluation
    results->getResultsZero();
    // manual deletion of potential value
    graph.deleteOne(PotentialValue("num", "16"), results.get());
    // sengkang is gone from purple
    requireVectorsHaveSameElements(
        results->getResultsOne("purple"),
        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia",
                                  "farrerpark", "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang",
                                  "buangkok", "punggol"}));
    // marymount is gone from circle
    requireVectorsHaveSameElements(
        results->getResultsOne("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",      "esplanade",   "promenade",      "nicollhighway",
                                  "stadium",      "mountbatten",    "dakota",      "payalebar",      "macpherson",
                                  "taiseng",      "bartley",        "serangoon",   "lorongchuan",    "bishan",
                                  "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage", "buonavista",
                                  "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",   "labradorpark",
                                  "telokblangah", "harbourfront"}));
}

TEST_CASE("deleteOne leaves other relationships untouched")
{
    RelationshipsGraph graph = setUpTestingGraph();
    std::unique_ptr<ResultsTable> results = setUpResultsTable();
    graph.deleteOne(PotentialValue("purple", "sengkang"), results.get());

    // original is unmodified as the method assumes it will only be
    // called after original potential value is removed from results
    requireVectorsHaveSameElements(
        results->getResultsOne("purple"),
        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia",
                                  "farrerpark", "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang",
                                  "sengkang", "buangkok", "punggol"}));
    // num is unmodified as there is still a relationship
    // between 16 <--> marymount
    requireVectorsHaveSameElements(results->getResultsOne("num"),
                                   std::vector<std::string>({"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
                                                             "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                                                             "21", "22", "23", "24", "25", "26", "27", "28", "29"}));
    // same for circle
    requireVectorsHaveSameElements(
        results->getResultsOne("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",    "esplanade",      "promenade",   "nicollhighway",
                                  "stadium",      "mountbatten",  "dakota",         "payalebar",   "macpherson",
                                  "taiseng",      "bartley",      "serangoon",      "lorongchuan", "bishan",
                                  "marymount",    "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage",
                                  "buonavista",   "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",
                                  "labradorpark", "telokblangah", "harbourfront"}));
}

TEST_CASE("deleteOne does nothing to ResultsTable if no relationships exist for that potential value")
{
    RelationshipsGraph graph = setUpTestingGraph();
    std::unique_ptr<ResultsTable> resultsActual = setUpResultsTable();
    graph.deleteOne(PotentialValue("blue", "ns26"), resultsActual.get());
    std::unique_ptr<ResultsTable> resultsExpected = setUpResultsTable();

    REQUIRE(*resultsActual == *resultsExpected);
}
