#include "catch.hpp"
#include "pql/preprocessor/AqTypes.h"
#include "pql/preprocessor/Preprocessor.h"

TEST_CASE("Test splitBySelect")
{
    //    StringList* stringList = splitProgram("while w,       w1; assign a; read read; Select w such that Follows (w,
    //    a)");

    Preprocessor pp;
    //    String queryString = "while w; Select w";
    //    String queryString = "while w; Select w;";
    //    String queryString = "stmt s; assign a ; Select s such that Follows(s, a)";
        String queryString = "while w,   w1; assign a; read read; Select w such that Follows (w, a)";
    //    String queryString = "assign a; Select a pattern a (_, \"x + y\")";
//    String queryString = "assign a; Select a pattern a (_, _\"x + y\"_)";
    //    String queryString = "assign a; Select a pattern a (_, _)";
    AbstractQuery aq1 = pp.processQuery(queryString);
    AbstractQuery aq2 = pp.processQuery(queryString);

    bool equals = aq1 == aq2;


//    ClauseVector cV1;
//    ClauseVector cV2;
//    Clause* clause1 = new Clause(SuchThatClauseType);
//    Clause* clause2 = new Clause(SuchThatClauseType);
//    cV1.add(clause1);
//    cV2.add(clause2);
//    bool equals1 = *clause1 == *clause2;
//    bool equals = cV1 == cV2;


    REQUIRE(1 == 1);
}
