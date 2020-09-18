/**
 * Tests for the Modifies extractor component
 * of the Design Extractor in SPA Frontend
 */

#include "../../ast_utils/AstUtils.h"
#include "ast/AstLibrary.h"
#include "catch.hpp"
#include "frontend/designExtractor/ModifiesExtractor.cpp"

TEST_CASE("Modifies extractor works for single procedures, multiple statements- factorials")
{
    ProcedureModifiesMap actualMap = extractModifiesReturnMap(*getProgram2Tree_factorials(), std::vector<int>{0});

    ProcedureModifiesMap expectedMap;
    VariablesSet modifies{"limit", "current", "factorial", "answer"};
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("factorials", modifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Modifies extractor works for single procedures, multiple statements - printAscending")
{
    ProcedureModifiesMap actualMap = extractModifiesReturnMap(*getProgram4Tree_printAscending(), std::vector<int>{0});

    ProcedureModifiesMap expectedMap;
    VariablesSet modifies{"num1", "num2", "noSwap", "temp"};
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("printAscending", modifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Modifies extractor works for single procedures with while only - whileExample")
{
    ProcedureModifiesMap actualMap = extractModifiesReturnMap(*getProgram14Tree_whileExample(), std::vector<int>{0});

    ProcedureModifiesMap expectedMap;
    VariablesSet modifies{"x"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("whileExample", modifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE(
    "Modifies extractor works for single procedures with while and complicated conditionals - complicatedConditional")
{
    ProcedureModifiesMap actualMap
        = extractModifiesReturnMap(*getProgram15Tree_complicatedConditional(), std::vector<int>{0});

    ProcedureModifiesMap expectedMap;
    VariablesSet modifies{"x"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("complicatedConditional", modifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Modifies extractor works for single procedures, ending with while - spheresdf")
{
    ProcedureModifiesMap actualMap = extractModifiesReturnMap(*getProgram18Tree_endWithWhile(), std::vector<int>{0});

    ProcedureModifiesMap expectedMap;
    VariablesSet modifies{"dist", "x", "depth", "p"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("spheresdf", modifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Modifies extractor works for multiple procedures, multiple statements - computeCentroid")
{
    ProcedureModifiesMap actualMap
        = extractModifiesReturnMap(*getProgram7Tree_computeCentroid(), std::vector<int>{2, 1, 3, 0});

    ProcedureModifiesMap expectedMap;
    VariablesSet mainModifies{"count", "cenX", "cenY", "x", "y", "flag", "normSq"};
    VariablesSet readPointsModifies{"x", "y"};
    VariablesSet printModifies{};
    VariablesSet computeCentroidModifies{"count", "cenX", "cenY", "x", "y", "flag", "normSq"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("main", mainModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("readPoint", readPointsModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("printResults", printModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("computeCentroid", computeCentroidModifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Modifies extractor works for mutiple procedures, keyword as identifier")
{
    ProcedureModifiesMap actualMap
        = extractModifiesReturnMap(*getProgram16Tree_keywordsAsIdentifier(), std::vector<int>{1, 0});

    ProcedureModifiesMap expectedMap;
    VariablesSet procedureModifies{"print", "if", "call", "else", "procedure", "read"};
    VariablesSet ifModifies{"procedure", "else", "read"};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("procedure", procedureModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("if", ifModifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Modifies extractor works for mutiple procedures, variable with same name as procedure")
{
    ProcedureModifiesMap actualMap
        = extractModifiesReturnMap(*getProgram17Tree_sameVariableAndProcedureName(), std::vector<int>{2, 1, 0});

    ProcedureModifiesMap expectedMap;
    VariablesSet procedureModifies{"procedure", "string", "call"};
    VariablesSet callModifies{"procedure", "string"};
    VariablesSet stringModifies{};

    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("procedure", procedureModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("call", callModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("string", stringModifies));

    REQUIRE(actualMap == expectedMap);
}

TEST_CASE("Modifies extractor works for multiple procedures, multiple statements")
{

    ProcedureModifiesMap actualMap
        = extractModifiesReturnMap(*getProgram19Tree_multipleProcedures(), std::vector<int>{3, 2, 1, 0});

    ProcedureModifiesMap expectedMap;
    VariablesSet aModifies{"base", "depth", "factor", "num", "result"};
    VariablesSet bModifies{"factor", "result", "num"};
    VariablesSet cModifies{"base", "depth", "num", "result"};
    VariablesSet dModifies{"result"};
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("a", aModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("b", bModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("c", cModifies));
    expectedMap.insert(std::pair<ProcedureName, VariablesSet>("d", dModifies));

    REQUIRE(actualMap == expectedMap);
}
