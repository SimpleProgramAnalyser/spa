/**
 * Tests for the Uses extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
#include "ast/AstLibrary.h"
#include "catch.hpp"
#include "frontend/designExtractor/UsesExtractor.cpp"

TEST_CASE("Uses extractor works for single procedures, multiple statements")
{
    ProcedureUsesMap actualMap = extractUsesReturnMap(*getProgram2Tree_factorials(), std::vector<int>{0});

    ProcedureUsesMap expectedMap;
    VariablesSet uses{"current", "limit", "factorial"};
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("factorials", uses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for single procedures, multiple statements - printAscending")
{
    ProcedureUsesMap actualMap = extractUsesReturnMap(*getProgram4Tree_printAscending(), std::vector<int>{0});

    ProcedureUsesMap expectedMap;
    VariablesSet uses{"num1", "num2", "temp", "noSwap"};
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("printAscending", uses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for single procedures with while only - whileExample")
{
    ProcedureUsesMap actualMap = extractUsesReturnMap(*getProgram14Tree_whileExample(), std::vector<int>{0});

    ProcedureUsesMap expectedMap;
    VariablesSet uses{"x"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("whileExample", uses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for single procedures with while and complicated conditionals - complicatedConditional")
{
    ProcedureUsesMap actualMap = extractUsesReturnMap(*getProgram15Tree_complicatedConditional(), std::vector<int>{0});

    ProcedureUsesMap expectedMap;
    VariablesSet uses{"x", "a", "y", "z", "b", "c"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("complicatedConditional", uses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for single procedures, ending with while - spheresdf")
{
    ProcedureUsesMap actualMap = extractUsesReturnMap(*getProgram18Tree_endWithWhile(), std::vector<int>{0});

    ProcedureUsesMap expectedMap;
    VariablesSet uses{"x", "y", "z", "dist", "depth", "p"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("spheresdf", uses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for multiple procedures, multiple statements - computeCentroid")
{
    ProcedureUsesMap actualMap = extractUsesReturnMap(*getProgram7Tree_computeCentroid(), std::vector<int>{2, 1, 3, 0});

    ProcedureUsesMap expectedMap;
    VariablesSet mainUses{"x", "y", "count", "cenX", "cenY", "flag", "normSq"};
    VariablesSet readPointsUses{};
    VariablesSet printUses{"flag", "cenX", "cenY", "normSq"};
    VariablesSet computeCentroidUses{"x", "y", "count", "cenX", "cenY"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("main", mainUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("readPoint", readPointsUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("printResults", printUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("computeCentroid", computeCentroidUses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for mutiple procedures, keyword as identifier")
{
    ProcedureUsesMap actualMap = extractUsesReturnMap(*getProgram16Tree_keywordsAsIdentifier(), std::vector<int>{1, 0});

    ProcedureUsesMap expectedMap;
    VariablesSet procedureUses{"print", "if", "then", "while", "procedure", "else", "call"};
    VariablesSet ifUses{"print", "if", "then", "while", "procedure"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("procedure", procedureUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("if", ifUses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for mutiple procedures, variable with same name as procedure")
{
    ProcedureUsesMap actualMap
        = extractUsesReturnMap(*getProgram17Tree_sameVariableAndProcedureName(), std::vector<int>{2, 1, 0});

    ProcedureUsesMap expectedMap;
    VariablesSet procedureUses{"procedure", "string"};
    VariablesSet callUses{"procedure", "string"};
    VariablesSet stringUses{"string"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("procedure", procedureUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("call", callUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("string", stringUses));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Uses extractor works for multiple procedures, multiple statements")
{
    ProcedureUsesMap actualMap
        = extractUsesReturnMap(*getProgram19Tree_multipleProcedures(), std::vector<int>{3, 2, 1, 0});

    ProcedureUsesMap expectedMap;
    VariablesSet aUses{"base", "depth", "factor", "num", "result"};
    VariablesSet bUses{"depth", "factor", "result"};
    VariablesSet cUses{"base", "depth", "num", "result"};
    VariablesSet dUses{"depth", "result"};
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("a", aUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("b", bUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("c", cUses));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("d", dUses));
    REQUIRE(actualMap == expectedMap);
}
