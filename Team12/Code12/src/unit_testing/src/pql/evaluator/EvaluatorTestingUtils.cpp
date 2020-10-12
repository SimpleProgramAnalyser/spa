/**
 * Implementation of helper methods for testing of evaluator.
 */
#include "EvaluatorTestingUtils.h"

typedef PotentialValue Pv;

RelationshipsGraph setUpTestingGraph()
{
    // edge numbers are set up to be the same as
    // the result table result
    return RelationshipsGraph({{1,
                                {Pv("red", "ns1"), Pv("green", "ew24"), Pv("num", "16"), Pv("purple", "sengkang"),
                                 Pv("circle", "marymount")}},
                               {2,
                                {Pv("red", "ns1"), Pv("green", "ew24"), Pv("num", "16"), Pv("purple", "outrampark"),
                                 Pv("circle", "marymount")}},
                               {3,
                                {Pv("red", "ns25"), Pv("green", "ew13"), Pv("num", "3"), Pv("purple", "outrampark"),
                                 Pv("circle", "esplanade")}},
                               {4, {Pv("CC", "4"), Pv("DT", "15")}},
                               {5, {Pv("CC", "19"), Pv("DT", "9")}},
                               {6, {Pv("CC", "10"), Pv("DT", "26")}},
                               {7, {Pv("CC", "E1"), Pv("DT", "16")}}},
                              8);
}

std::unique_ptr<ResultsTable> setUpResultsTable()
{
    std::unique_ptr<ResultsTable> results = std::unique_ptr<ResultsTable>(new ResultsTable{DeclarationTable()});
    results->storeResultsOne(
        "red", std::vector<std::string>({"ns1",  "ns2",  "ns3",  "ns4",  "ns5",  "ns7",  "ns8",  "ns9",  "ns10",
                                         "ns11", "ns12", "ns13", "ns14", "ns15", "ns16", "ns17", "ns18", "ns19",
                                         "ns20", "ns21", "ns22", "ns23", "ns24", "ns25", "ns26", "ns27", "ns28"}));
    results->storeResultsOne(
        "green", std::vector<std::string>({"ew1",  "ew2",  "ew3",  "ew4",  "ew5",  "ew6",  "ew7",  "ew8",  "ew9",
                                           "ew10", "ew11", "ew12", "ew13", "ew14", "ew15", "ew16", "ew17", "ew18",
                                           "ew19", "ew20", "ew21", "ew22", "ew23", "ew24", "ew25", "ew26", "ew27",
                                           "ew28", "ew29", "ew30", "ew31", "ew32", "ew33"}));
    results->storeResultsOne(
        "num", std::vector<std::string>({"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12",
                                         "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24",
                                         "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35"}));
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
                                  "labradorpark", "telokblangah", "harbourfront",   "bayfront",    "marinabay"}));
    results->storeResultsOne("CC",
                             std::vector<std::string>({"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10",
                                                       "11", "12", "13", "14", "15", "16", "17", "19", "20", "21",
                                                       "22", "23", "24", "25", "26", "27", "28", "29", "E1", "E2"}));
    results->storeResultsOne(
        "DT", std::vector<std::string>({"1",  "2",  "3",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12", "13",
                                        "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25",
                                        "26", "27", "28", "29", "30", "31", "32", "33", "34", "35"}));
    results->getResultsZero();
    return std::move(results);
}