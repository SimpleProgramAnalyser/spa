/**
 * Integration tests between Frontend and PKB,
 * for Follows and Follows* relationships.
 */

#include "catch.hpp"
#include "frontend/FrontendManager.h"
#include "pkb/PKB.h"

String getTestProgram()
{
    String spheresdf = "\
procedure main { \t\n\
     read steps;\n\
     call raymarch;\n\
     print depth; }\n\
\n\
procedure raymarch {\n\
     ro = 13;\n\
     rd = 19;\n\
     read depth; \n\
    while (count < steps) {\n\
            print depth;\n\
            po = ro + rd * depth;\n\
          call spheresdf;\n\
          if (dist < epsilon) then {\n\
                  done = depth; }\n\
                else {\n\
                  depth = depth + dist;} \n\
          count = count + 1; }}\n\
\n\
procedure spheresdf {\n\
     dist = x * x + y * y + z * z;\n\
     x = dist;\n\
     depth = depth;\n\
     read p;\n\
     while (x != p) {\n\
            p = x;\n\
            x = (dist / x + x) / 2; } \n\
      dist = x - 1;\n\
      x = x * x + y * y / 2; }\n\
";
    /*
Annotated with statement numbers:
procedure main {
1.     read steps;
2.     call raymarch;
3.     print depth; }

procedure raymarch {
4.     ro = 13;
5.     rd = 19;
6.     read depth;
7.     while (count < steps) {
8.            print depth;
9.            po = ro + rd * depth;
10.          call spheresdf;
11.          if (dist < epsilon) then {
12.                  done = depth; }
                else {
13.                  depth = depth + dist;}
14.          count = count + 1; }}

procedure spheresdf {
15.     dist = x * x + y * y + z * z;
16.     x = dist;
17.     depth = depth;
18.     read p;
19.     while (x != p) {
20.            p = x;
21.            x = (dist / x + x) / 2; }
22.      dist = x - 1;
23.      x = x * x + y * y / 2; }
    */
    return spheresdf;
}

TEST_CASE("Follows relationships stored correctly for test program")
{
    parseSimple(getTestProgram());
    std::vector<Integer> actualBefore = getAllBeforeStatementsTyped(AnyStatement, AnyStatement);
    std::unordered_set<Integer> actualBeforeSet(actualBefore.begin(), actualBefore.end());
    std::unordered_set<Integer> expectedBeforeSet = {1, 2, 4, 5, 6, 8, 9, 10, 11, 15, 16, 17, 18, 19, 20, 22};
    REQUIRE(actualBeforeSet == expectedBeforeSet);

    std::vector<Integer> actualAfter = getAllAfterStatementsTyped(AnyStatement, AnyStatement);
    std::unordered_set<Integer> actualAfterSet(actualAfter.begin(), actualAfter.end());
    std::unordered_set<Integer> expectedAfterSet = {2, 3, 5, 6, 7, 9, 10, 11, 14, 16, 17, 18, 19, 21, 22, 23};
    REQUIRE(actualAfterSet == expectedAfterSet);
}

TEST_CASE("Follows* relationships stored correctly for test program")
{
    parseSimple(getTestProgram());
}
