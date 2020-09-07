//
// Created by Chester Sim on 6/9/20.
//

#include "pql/PreProcessor.h"

#include "catch.hpp"
#include "iostream"

TEST_CASE("Test splitBySelect") {

    PreProcessor pp;
    String queryString = "While w1, w2, w3; Assign a; Select w such that Parent(w, a)";
    pp.processQuery(queryString);
    REQUIRE(1 == 1);
}
