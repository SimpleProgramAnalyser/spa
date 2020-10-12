#include <catch.hpp>
#include <pkb/tables/Tables.h>

#define p(x, y) std::make_pair((x), (y))
#define AS AssignmentStatement
#define RS ReadStatement
#define PS PrintStatement
#define WS WhileStatement
#define IS IfStatement
#define CS CallStatement

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

Vector<Pair<Integer, StatementType>> filterByType(Vector<Pair<Integer, StatementType>> vec, StatementType statementType)
{
    Vector<Pair<Integer, StatementType>> toReturn;
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(toReturn), [statementType](Pair<Integer, StatementType> p) {
        return statementType == AnyStatement || p.second == statementType;
    });
    return toReturn;
}

Vector<Integer> mapToIntegers(Vector<Pair<Integer, StatementType>> vec)
{
    Vector<Integer> toReturn;
    std::transform(vec.begin(), vec.end(), std::back_inserter(toReturn),
                   [](Pair<Integer, StatementType> p) { return p.first; });
    return toReturn;
}

SCENARIO("Iteration 1 Tables Test")
{
    ConstantTable constantTable;
    VariableTable variableTable;
    StatementTable statementTable;
    ProcedureTable procedureTable;
    GIVEN("Some entities")
    {
        Vector<Integer> constants = {1, 2, 13, 19};
        Vector<String> variables = {"steps", "depth", "ro", "rd", "count", "po", "done", "dist", "p", "x", "y", "z"};
        Vector<Pair<Integer, StatementType>> statements
            = {p(1, RS),  p(2, CS),  p(3, PS),  p(4, AS),  p(5, AS),  p(6, RS),  p(7, WS),  p(8, PS),
               p(9, AS),  p(10, CS), p(11, IS), p(12, AS), p(13, AS), p(14, AS), p(15, AS), p(16, AS),
               p(17, AS), p(18, AS), p(19, WS), p(20, AS), p(21, AS), p(22, AS), p(23, AS)};
        Vector<String> procedures = {"main", "raymarch", "spheresdf"};

        WHEN("constants are added")
        {
            for (auto constant : constants)
                constantTable.insertIntoConstantTable(constant);
            THEN("correct constants can be found")
            {
                REQUIRE_THAT(constantTable.getAllConstants(), Catch::UnorderedEquals(constants));
                for (auto constant : constants) {
                    REQUIRE(constantTable.isConstantInProgram(constant));
                }
            }
            THEN("incorrect constants cannot be found")
            {
                REQUIRE_FALSE(constantTable.isConstantInProgram(3));
                REQUIRE_FALSE(constantTable.isConstantInProgram(4));
                REQUIRE_FALSE(constantTable.isConstantInProgram(0));
                REQUIRE_FALSE(constantTable.isConstantInProgram(-1));
                REQUIRE_FALSE(constantTable.isConstantInProgram(-1000000));
                REQUIRE_FALSE(constantTable.isConstantInProgram(1000000));
                REQUIRE_FALSE(constantTable.isConstantInProgram(12));
                REQUIRE_FALSE(constantTable.isConstantInProgram(14));
                REQUIRE_FALSE(constantTable.isConstantInProgram(18));
                REQUIRE_FALSE(constantTable.isConstantInProgram(20));
            }
        }
        WHEN("variables are added")
        {
            for (const auto& variable : variables)
                variableTable.insertIntoVariableTable(variable);
            THEN("correct variables can be found")
            {
                REQUIRE_THAT(variableTable.getAllVariables(), Catch::UnorderedEquals(variables));
                for (const auto& variable : variables) {
                    REQUIRE(variableTable.isVariableInProgram(variable));
                }
            }
            THEN("incorrect variables cannot be found")
            {
                REQUIRE_FALSE(variableTable.isVariableInProgram(""));
                REQUIRE_FALSE(variableTable.isVariableInProgram("main"));
                REQUIRE_FALSE(variableTable.isVariableInProgram("spheresdf"));
                REQUIRE_FALSE(variableTable.isVariableInProgram("raymarch"));
                REQUIRE_FALSE(variableTable.isVariableInProgram("xx"));
                REQUIRE_FALSE(variableTable.isVariableInProgram("while"));
            }
        }
        WHEN("statements are added")
        {
            for (auto statement : statements)
                statementTable.insertIntoStatementTable(statement.first, statement.second);
            // create (un)realistic expectations, just like this mean world
            Array<Vector<Integer>, STATEMENT_TYPE_COUNT> statementLists;
            for (int i = 0; i < STATEMENT_TYPE_COUNT; i++) {
                statementLists[i] = mapToIntegers(filterByType(statements, StatementType(i)));
            }

            THEN("correct statements can be found")
            {
                // check for each statement type
                for (int i = 0; i < STATEMENT_TYPE_COUNT; i++) {
                    REQUIRE_THAT(statementTable.getAllStatements(StatementType(i)),
                                 Catch::UnorderedEquals(statementLists[i]));
                }
                // check for each statement
                for (const auto& statement : statements) {
                    REQUIRE(statementTable.isStatementInProgram(statement.first));
                }
            }
            THEN("incorrect statements cannot be found")
            {
                REQUIRE_FALSE(statementTable.isStatementInProgram(0));
                REQUIRE_FALSE(statementTable.isStatementInProgram(-1));
                REQUIRE_FALSE(statementTable.isStatementInProgram(-1000000));
                REQUIRE_FALSE(statementTable.isStatementInProgram(1000000));
                REQUIRE_FALSE(statementTable.isStatementInProgram(24));
            }
        }
        WHEN("procedures are added")
        {
            procedureTable.insertIntoProcedureTable("main", 1, 3);
            procedureTable.insertIntoProcedureTable("raymarch", 4, 14);
            procedureTable.insertIntoProcedureTable("spheresdf", 4, 23);

            THEN("correct procedures can be found")
            {
                REQUIRE_THAT(procedureTable.getAllProcedures(), Catch::UnorderedEquals(procedures));
                for (const auto& procedure : procedures) {
                    REQUIRE(procedureTable.isProcedureInProgram(procedure));
                }
            }
            THEN("incorrect procedures cannot be found")
            {
                REQUIRE_FALSE(procedureTable.isProcedureInProgram(""));
                REQUIRE_FALSE(procedureTable.isProcedureInProgram("main1"));
            }
        }
    }
}