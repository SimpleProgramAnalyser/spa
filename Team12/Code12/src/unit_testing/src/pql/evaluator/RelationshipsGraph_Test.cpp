/**
 * Unit tests for the RelationshipsGraph object in
 * Query Evaluator, which holds the relationships
 * constraints from evaluating PQL clauses.
 *
 * A combination of black-box tests and white-box tests.
 */
#include "EvaluatorTestingUtils.h"
#include "catch.hpp"

TEST_CASE("checkIfRelated verifies PotentialValue relationships correctly")
{
    RelationshipsGraph graph = setUpTestingGraph();
    REQUIRE(graph.checkIfRelated(PotentialValue("red", "ns1"), PotentialValue("green", "ew24")));
    REQUIRE(graph.checkIfRelated(PotentialValue("num", "3"), PotentialValue("purple", "outrampark")));
    REQUIRE(graph.checkIfRelated(PotentialValue("num", "16"), PotentialValue("purple", "sengkang")));
    REQUIRE(graph.checkIfRelated(PotentialValue("num", "16"), PotentialValue("purple", "outrampark")));
    REQUIRE(graph.checkIfRelated(PotentialValue("circle", "marymount"), PotentialValue("num", "16")));
    REQUIRE(graph.checkIfRelated(PotentialValue("circle", "marymount"), PotentialValue("purple", "outrampark")));
    REQUIRE(graph.checkIfRelated(PotentialValue("purple", "sengkang"), PotentialValue("circle", "marymount")));
    REQUIRE(graph.checkIfRelated(PotentialValue("num", "3"), PotentialValue("red", "ns25")));
    REQUIRE(graph.checkIfRelated(PotentialValue("green", "ew13"), PotentialValue("purple", "outrampark")));
    REQUIRE(graph.checkIfRelated(PotentialValue("green", "ew24"), PotentialValue("purple", "outrampark")));
    REQUIRE(graph.checkIfRelated(PotentialValue("CC", "4"), PotentialValue("DT", "15")));
    REQUIRE(graph.checkIfRelated(PotentialValue("DT", "9"), PotentialValue("CC", "19")));
    REQUIRE(graph.checkIfRelated(PotentialValue("DT", "26"), PotentialValue("CC", "10")));
    REQUIRE(graph.checkIfRelated(PotentialValue("CC", "E1"), PotentialValue("DT", "16")));

    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("red", "ns26"), PotentialValue("green", "ew14")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("red", "ns26"), PotentialValue("green", "ew13")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("red", "ns25"), PotentialValue("green", "ew4")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "6"), PotentialValue("purple", "outrampark")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "2"), PotentialValue("purple", "sengkang")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "12"), PotentialValue("purple", "serangoon")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("num", "6"), PotentialValue("circle", "dhobyghaut")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("circle", "onenorth"), PotentialValue("num", "23")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("CC", "4"), PotentialValue("CC", "19")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("CC", "4"), PotentialValue("DT", "16")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("DT", "15"), PotentialValue("DT", "9")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("NE", "6"), PotentialValue("CC", "1")));
    REQUIRE_FALSE(graph.checkIfRelated(PotentialValue("DT", "15"), PotentialValue("CE", "1")));
}

TEST_CASE("deleteOne updates ResultTable when a potential value no longer has any relationships")
{
    RelationshipsGraph graph = setUpTestingGraph();
    std::unique_ptr<ResultsTable> results = setUpResultsTable();
    // force early evaluation
    results->getResultsZero();
    // manual deletion of potential value
    graph.deleteOne(PotentialValue("red", "ns25"), results.get());
    requireVectorsHaveSameElements(
        results->getResultsOne("red"),
        std::vector<std::string>({"ns1",  "ns2",  "ns3",  "ns4",  "ns5",  "ns7",  "ns8",  "ns9",  "ns10",
                                  "ns11", "ns12", "ns13", "ns14", "ns15", "ns16", "ns17", "ns18", "ns19",
                                  "ns20", "ns21", "ns22", "ns23", "ns24", "ns26", "ns27", "ns28"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("green"),
        std::vector<std::string>({"ew1",  "ew2",  "ew3",  "ew4",  "ew5",  "ew6",  "ew7",  "ew8",
                                  "ew9",  "ew10", "ew11", "ew12", "ew14", "ew15", "ew16", "ew17",
                                  "ew18", "ew19", "ew20", "ew21", "ew22", "ew23", "ew24", "ew25",
                                  "ew26", "ew27", "ew28", "ew29", "ew30", "ew31", "ew32", "ew33"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("num"),
        std::vector<std::string>({"1",  "2",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12", "13",
                                  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25",
                                  "26", "27", "28", "29", "30", "31", "32", "33", "34", "35"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",      "promenade",   "nicollhighway",  "stadium",
                                  "mountbatten",  "dakota",         "payalebar",   "macpherson",     "taiseng",
                                  "bartley",      "serangoon",      "lorongchuan", "bishan",         "marymount",
                                  "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage", "buonavista",
                                  "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",   "labradorpark",
                                  "telokblangah", "harbourfront",   "bayfront",    "marinabay"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("purple"),
        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia",
                                  "farrerpark", "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang",
                                  "buangkok", "sengkang", "punggol"}));
    // CC unmodified as it is unrelated
    requireVectorsHaveSameElements(
        results->getResultsOne("CC"),
        std::vector<std::string>({"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
                                  "11", "12", "13", "14", "15", "16", "17", "19", "20", "21",
                                  "22", "23", "24", "25", "26", "27", "28", "29", "E1", "E2"}));
    // same for DT
    requireVectorsHaveSameElements(
        results->getResultsOne("DT"),
        std::vector<std::string>({"1",  "2",  "3",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12", "13",
                                  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25",
                                  "26", "27", "28", "29", "30", "31", "32", "33", "34", "35"}));
    // delete non-existent value
    graph.deleteOne(PotentialValue("circle", "harbourfront"), results.get());
    requireVectorsHaveSameElements(
        results->getResultsOne("purple"),
        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia",
                                  "farrerpark", "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang",
                                  "buangkok", "sengkang", "punggol"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",      "promenade",   "nicollhighway",  "stadium",
                                  "mountbatten",  "dakota",         "payalebar",   "macpherson",     "taiseng",
                                  "bartley",      "serangoon",      "lorongchuan", "bishan",         "marymount",
                                  "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage", "buonavista",
                                  "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",   "labradorpark",
                                  "telokblangah", "harbourfront",   "bayfront",    "marinabay"}));
}

TEST_CASE("deleteOne leaves other relationships untouched")
{
    RelationshipsGraph graph = setUpTestingGraph();
    std::unique_ptr<ResultsTable> results = setUpResultsTable();
    graph.deleteOne(PotentialValue("purple", "sengkang"), results.get());

    // purple is modified
    requireVectorsHaveSameElements(
        results->getResultsOne("purple"),
        std::vector<std::string>({"harbourfront", "outrampark", "chinatown", "clarkequay", "dhobyghaut", "littleindia",
                                  "farrerpark", "boonkeng", "potongpasir", "woodleigh", "serangoon", "kovan", "hougang",
                                  "buangkok", "punggol"}));
    // num, circle, green, red unmodified as there is still a relationship
    // between 16 <--> marymount <--> ew24 <--> ns1
    requireVectorsHaveSameElements(results->getResultsOne("num"),
                                   std::vector<std::string>({"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
                                                             "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                                                             "21", "22", "23", "24", "25", "26", "27", "28", "29"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("circle"),
        std::vector<std::string>({"dhobyghaut",   "brasbasah",    "esplanade",      "promenade",   "nicollhighway",
                                  "stadium",      "mountbatten",  "dakota",         "payalebar",   "macpherson",
                                  "taiseng",      "bartley",      "serangoon",      "lorongchuan", "bishan",
                                  "marymount",    "caldecott",    "botanicgardens", "farrerroad",  "hollandvillage",
                                  "buonavista",   "onenorth",     "kentridge",      "hawparvilla", "pasirpanjang",
                                  "labradorpark", "telokblangah", "harbourfront"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("green"),
        std::vector<std::string>({"ew1",  "ew2",  "ew3",  "ew4",  "ew5",  "ew6",  "ew7",  "ew8",  "ew9",
                                  "ew10", "ew11", "ew12", "ew13", "ew14", "ew15", "ew16", "ew17", "ew18",
                                  "ew19", "ew20", "ew21", "ew22", "ew23", "ew24", "ew25", "ew26", "ew27",
                                  "ew28", "ew29", "ew30", "ew31", "ew32", "ew33"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("red"),
        std::vector<std::string>({"ns1",  "ns2",  "ns3",  "ns4",  "ns5",  "ns7",  "ns8",  "ns9",  "ns10",
                                  "ns11", "ns12", "ns13", "ns14", "ns15", "ns16", "ns17", "ns18", "ns19",
                                  "ns20", "ns21", "ns22", "ns23", "ns24", "ns25", "ns26", "ns27", "ns28"}));
    // CC, DT unmodified as it is unrelated
    requireVectorsHaveSameElements(
        results->getResultsOne("CC"),
        std::vector<std::string>({"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
                                  "11", "12", "13", "14", "15", "16", "17", "19", "20", "21",
                                  "22", "23", "24", "25", "26", "27", "28", "29", "E1", "E2"}));
    requireVectorsHaveSameElements(
        results->getResultsOne("DT"),
        std::vector<std::string>({"1",  "2",  "3",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12", "13",
                                  "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25",
                                  "26", "27", "28", "29", "30", "31", "32", "33", "34", "35"}));
}

TEST_CASE("deleteOne does nothing to ResultsTable if synonym does not exist")
{
    RelationshipsGraph graph = setUpTestingGraph();
    std::unique_ptr<ResultsTable> resultsActual = setUpResultsTable();
    graph.deleteOne(PotentialValue("blue", "ns26"), resultsActual.get());
    std::unique_ptr<ResultsTable> resultsExpected = setUpResultsTable();
    REQUIRE(*resultsActual == *resultsExpected);
}
