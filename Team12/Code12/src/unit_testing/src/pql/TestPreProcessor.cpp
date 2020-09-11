#include "catch.hpp"
#include "lexer/Lexer.h"
#include "pql/parser/Preprocessor.h"

TEST_CASE("Test splitBySelect")
{
    //    StringList* stringList = splitProgram("while w,       w1; assign a; read read; Select w such that Follows (w,
    //    a)");

    Preprocessor pp;
    //    String queryString = "while w; Select w";
    //    String queryString = "while w; Select w;";
    //    String queryString = "stmt s; assign a ; Select s such that Follows(s, a)";
    //    String queryString = "while w,   w1; assign a; read read; Select w such that Follows (w, a)";
    //    String queryString = "assign a; Select a pattern a (_, \"x + y\")";
    String queryString = "assign a; Select a pattern a (_, _\"x + y\"_)";
    //    String queryString = "assign a; Select a pattern a (_, _)";
    AbstractQuery aq = pp.processQuery(queryString);

    REQUIRE(1 == 1);
}
