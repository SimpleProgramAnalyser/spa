//
// Created by Chester Sim on 6/9/20.
//

#include "pql/PreProcessor.h"

#include "catch.hpp"

TEST_CASE("Test splitBySelect") {

    PreProcessor pp;
//    String queryString = "while w; Select w";
    String queryString = "while w; Select w;";
//    String queryString = "stmt s; assign a; Select s such that Follows(s, a)";
//    String queryString = "while w,   w1; assign a; read read; Select w such that Follows (w, a)";
    AbstractQuery aq = pp.processQuery(queryString);
    REQUIRE(1 == 1);
}
