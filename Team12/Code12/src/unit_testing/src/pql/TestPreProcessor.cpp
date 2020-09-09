//
// Created by Chester Sim on 6/9/20.
//

#include "pql/PreProcessor.h"

#include "catch.hpp"

TEST_CASE("Test splitBySelect") {

    PreProcessor pp;
    String queryString = "while w,   w1; assign a; read read; Select w such that Parent(w, a)";
    pp.processQuery(queryString);
    REQUIRE(1 == 1);
}
