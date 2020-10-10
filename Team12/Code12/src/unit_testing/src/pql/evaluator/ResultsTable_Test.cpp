/**
 * Unit tests for the ResultsTable object in
 * Query Evaluator, which holds results of
 * evaluating PQL clauses.
 *
 * A combination of black-box tests and white-box tests.
 */
#include "EvaluatorTestingUtils.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "pql/evaluator/ResultsTable.h"

TEST_CASE("ResultsTable stores empty results correctly")
{
    // set up PKB
    resetPKB();
    insertIntoVariableTable("mercury");
    insertIntoVariableTable("venus");
    insertIntoVariableTable("earth");
    insertIntoVariableTable("mars");

    DeclarationTable declTable;
    DesignEntity anyVar(VariableType);
    declTable.addDeclaration("efgh", anyVar);
    ResultsTable resTable(declTable);
    resTable.storeResultsOne("abcd", std::vector<std::string>());

    REQUIRE(!resTable.hasResults());
    REQUIRE(resTable.getResultsOne("abcd").empty());
    // check if other synonyms also return empty
    REQUIRE(resTable.getResultsOne("efgh").empty());
}

TEST_CASE("ResultsTable stores results correctly")
{
    // set up PKB
    resetPKB();
    insertIntoVariableTable("mercury");
    insertIntoVariableTable("venus");
    insertIntoVariableTable("earth");
    insertIntoVariableTable("mars");

    DeclarationTable declTable;
    DesignEntity anyVar(VariableType);
    declTable.addDeclaration("efgh", anyVar);
    ResultsTable resTable(declTable);
    std::vector<std::string> expectedResultsAbcd{"7", "9", "3", "10", "143", "539"};
    std::vector<std::string> expectedResultsEfgh{"mercury", "venus", "earth", "mars"};
    resTable.storeResultsOne("abcd", expectedResultsAbcd);

    REQUIRE(resTable.hasResults());
    requireVectorsHaveSameElements(resTable.getResultsOne("efgh"), expectedResultsEfgh);
    requireVectorsHaveSameElements(resTable.getResultsOne("abcd"), expectedResultsAbcd);
}

TEST_CASE("ResultsTable filters results correctly")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    std::vector<std::string> newResults{"112", "18397", "3", "0", "143", "539"};
    std::vector<std::string> expectedResults{"3", "143", "539"};
    resTable.storeResultsOne("abcd", originalResults);
    resTable.storeResultsOne("abcd", newResults);

    requireVectorsHaveSameElements(resTable.getResultsOne("abcd"), expectedResults);
}

TEST_CASE("ResultsTable filters results correctly for synonym reference")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"piglin",      "dolphin",  "enderman", "creeper",
                                             "enderdragon", "villager", "mooshroom"};
    std::vector<std::string> newResults{"pig", "mooshroom", "sheep", "enderman", "dolphin", "salmon"};
    std::vector<std::string> expectedResults{"mooshroom", "enderman", "dolphin"};
    resTable.storeResultsOne(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), originalResults);
    resTable.storeResultsOne(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), newResults);

    requireVectorsHaveSameElements(resTable.getResultsOne("abcd"), expectedResults);
}

TEST_CASE("ResultsTable does not filter any results for non-synonym reference")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"piglin",      "dolphin",  "enderman", "creeper",
                                             "enderdragon", "villager", "mooshroom"};
    std::vector<std::string> newResults{"pig", "mooshroom", "sheep", "enderman", "dolphin", "salmon"};
    std::vector<std::string> moreResults{"enderdragon", "enderman", "shulker", "piglinbrute",
                                         "endermite",   "creeper",  "strider"};
    resTable.storeResultsOne(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), originalResults);
    resTable.storeResultsOne(Reference(WildcardRefType, "_"), newResults);
    resTable.storeResultsOne(Reference(IntegerRefType, "12"), moreResults);

    ResultsTable resTable2(DeclarationTable{});
    resTable2.storeResultsOne(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), originalResults);
    resTable2.getResultsZero();

    requireVectorsHaveSameElements(resTable.getResultsOne("abcd"), originalResults);
    REQUIRE(resTable == resTable2);
}

TEST_CASE("ResultsTable retrieves results from PKB correctly, if no elements found")
{
    // set up PKB
    resetPKB();
    insertIntoStatementTable(1, AssignmentStatement);
    insertIntoStatementTable(2, ReadStatement);
    insertIntoStatementTable(3, ReadStatement);
    insertIntoStatementTable(4, IfStatement);
    insertIntoStatementTable(5, PrintStatement);

    DeclarationTable declTable;
    DesignEntity anyStmt(StmtType);
    declTable.addDeclaration("abcd", anyStmt);
    ResultsTable resTable(declTable);

    requireVectorsHaveSameElements(resTable.getResultsOne("abcd"), std::vector<std::string>({"1", "2", "3", "4", "5"}));
}

TEST_CASE("doesSynonymHaveConstraints returns false for synonym with no constraints, and returns true for synonym "
          "with constraints")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    std::vector<std::string> newResults{"112", "18397", "3", "0", "143", "539"};
    std::vector<std::string> expectedResults{"3", "143", "539"};

    REQUIRE_FALSE(resTable.doesSynonymHaveConstraints("abcd"));

    resTable.storeResultsOne("abcd", originalResults);
    resTable.storeResultsOne("abcd", newResults);
    resTable.getResultsZero();

    REQUIRE(resTable.doesSynonymHaveConstraints("abcd"));
}

TEST_CASE("getTypeOfSynonym gets the type stored in declarations table successfully")
{
    DeclarationTable declTable{};
    DesignEntity mudkipDesignEntity(ProcedureType);
    DesignEntity pqlProcessorDesignEntity(ProcedureType);
    DesignEntity swampertDesignEntity(VariableType);
    DesignEntity designExtractorDesignEntity(VariableType);
    DesignEntity astDesignEntity(StmtType);
    DesignEntity parserDesignEntity(StmtType);
    declTable.addDeclaration("mudkip", mudkipDesignEntity);
    declTable.addDeclaration("swampert", swampertDesignEntity);
    declTable.addDeclaration("ast", astDesignEntity);
    declTable.addDeclaration("pqlprocessor", pqlProcessorDesignEntity);
    declTable.addDeclaration("designextractor", designExtractorDesignEntity);
    declTable.addDeclaration("parser", parserDesignEntity);
    ResultsTable resTable(declTable);

    REQUIRE(resTable.getTypeOfSynonym("mudkip") == ProcedureType);
    REQUIRE(resTable.getTypeOfSynonym("swampert") == VariableType);
    REQUIRE(resTable.getTypeOfSynonym("ast") == StmtType);
    REQUIRE(resTable.getTypeOfSynonym("pqlprocessor") == ProcedureType);
    REQUIRE(resTable.getTypeOfSynonym("designextractor") == VariableType);
    REQUIRE(resTable.getTypeOfSynonym("parser") == StmtType);
}

TEST_CASE("getTypeOfSynonym returns NonExistentType if synonym was not found in declarations table")
{
    DeclarationTable declTable{};
    DesignEntity mudkipDesignEntity(ProcedureType);
    DesignEntity pqlProcessorDesignEntity(ProcedureType);
    DesignEntity swampertDesignEntity(VariableType);
    DesignEntity designExtractorDesignEntity(VariableType);
    DesignEntity astDesignEntity(StmtType);
    DesignEntity parserDesignEntity(StmtType);
    declTable.addDeclaration("mudkip", mudkipDesignEntity);
    declTable.addDeclaration("swampert", swampertDesignEntity);
    declTable.addDeclaration("ast", astDesignEntity);
    declTable.addDeclaration("pqlprocessor", pqlProcessorDesignEntity);
    declTable.addDeclaration("designextractor", designExtractorDesignEntity);
    declTable.addDeclaration("parser", parserDesignEntity);
    ResultsTable resTable(declTable);

    REQUIRE(resTable.getTypeOfSynonym("mudkips") == NonExistentType);
    REQUIRE(resTable.getTypeOfSynonym("swamperts") == NonExistentType);
}

TEST_CASE("eliminatePotentialValue updates the ResultsTable successfully for synonym found in table")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    resTable.storeResultsOne("abcd", originalResults);
    resTable.eliminatePotentialValue("abcd", "134");

    ResultsTable resTableExpected(DeclarationTable{});
    std::vector<std::string> expectedResults{"1", "3", "7", "539", "23", "143", "9"};
    resTableExpected.storeResultsOne("abcd", expectedResults);

    REQUIRE(resTable == resTableExpected);
}

TEST_CASE("eliminatePotentialValue does not do anything for synonym not found")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    resTable.storeResultsOne("abcd", originalResults);
    resTable.eliminatePotentialValue("efgh", "134");

    ResultsTable resTableExpected(DeclarationTable{});
    resTableExpected.storeResultsOne("abcd", originalResults);

    REQUIRE(resTable == resTableExpected);
}

TEST_CASE("Relationships are stored properly in ResultsTable")
{
    resetPKB();
    insertIntoStatementTable(3, AssignmentStatement);
    insertIntoStatementTable(6, WhileStatement);
    insertIntoStatementTable(12, IfStatement);
    insertIntoStatementTable(33, ReadStatement);
    insertIntoStatementTable(34, PrintStatement);
    insertIntoStatementTable(89, CallStatement);
    insertIntoVariableTable("swift");
    insertIntoVariableTable("adkins");
    insertIntoVariableTable("treintaytres");
    insertIntoVariableTable("ochentaynueve");
    insertIntoVariableTable("doce");
    insertIntoProcedureTable("taylor");
    insertIntoProcedureTable("adele");
    insertIntoProcedureTable("terracotta");
    insertIntoProcedureTable("concrete");

    DeclarationTable declTable{};
    DesignEntity mudkipDesignEntity(ProcedureType);
    DesignEntity pqlProcessorDesignEntity(ProcedureType);
    DesignEntity swampertDesignEntity(VariableType);
    DesignEntity designExtractorDesignEntity(VariableType);
    DesignEntity astDesignEntity(StmtType);
    DesignEntity parserDesignEntity(StmtType);
    declTable.addDeclaration("mudkip", mudkipDesignEntity);
    declTable.addDeclaration("swampert", swampertDesignEntity);
    declTable.addDeclaration("ast", astDesignEntity);
    declTable.addDeclaration("pqlprocessor", pqlProcessorDesignEntity);
    declTable.addDeclaration("designextractor", designExtractorDesignEntity);
    declTable.addDeclaration("parser", parserDesignEntity);
    ResultsTable resTable(declTable);

    std::vector<std::pair<std::string, std::string>> mudkipSwampertRelationships(
        {{"taylor", "swift"}, {"adele", "adkins"}});
    std::vector<std::pair<std::string, std::string>> astPqlProcessorRelationships(
        {{"6", "terracotta"}, {"3", "concrete"}});
    std::vector<std::pair<std::string, std::string>> parserDesignExtractorRelationships(
        {{"treintaytres", "33"}, {"ochentaynueve", "89"}, {"doce", "12"}});
    std::vector<std::pair<std::string, std::string>> astParserRelationships({{"12", "34"}});

    resTable.storeResultsTwo("mudkip", {"taylor", "adele"}, "swampert", {"swift", "adkins"},
                             mudkipSwampertRelationships);
    resTable.storeResultsTwo("ast", {"6", "3"}, "pqlprocessor", {"terracotta", "concrete"},
                             astPqlProcessorRelationships);
    resTable.storeResultsTwo("parser", {"treintaytres", "ochentaynueve", "doce"}, "designextractor", {"33", "89", "12"},
                             parserDesignExtractorRelationships);
    resTable.storeResultsTwo("ast", {"12"}, "parser", {"34"}, astParserRelationships);

    SECTION("hasRelationships returns true for related synonyms, false for unrelated synonyms")
    {
        REQUIRE(resTable.hasRelationships("ast", "parser"));
        REQUIRE(resTable.hasRelationships("ast", "pqlprocessor"));
        REQUIRE(resTable.hasRelationships("parser", "designextractor"));
        REQUIRE(resTable.hasRelationships("swampert", "mudkip"));

        REQUIRE_FALSE(resTable.hasRelationships("ast", "designextractor"));
        REQUIRE_FALSE(resTable.hasRelationships("swampert", "ast"));
        REQUIRE_FALSE(resTable.hasRelationships("mudkip", "parser"));
        REQUIRE_FALSE(resTable.hasRelationships("parser", "pqlprocessor"));
    }

    SECTION("getRelationships retrieves all relationships in table")
    {
        std::vector<std::pair<std::string, std::string>> astPqlProcessorRelationshipsString(
            {{"6", "terracotta"}, {"3", "concrete"}});
        std::vector<std::pair<std::string, std::string>> astParserRelationshipsString({{"12", "34"}});
        // "parser" and "designextractor" are swapped
        std::vector<std::pair<std::string, std::string>> designExtractorParserRelationshipsExpected(
            {{"33", "treintaytres"}, {"89", "ochentaynueve"}, {"12", "doce"}});
        // "swampert" and "mudkip" are swapped
        std::vector<std::pair<std::string, std::string>> swampertMudkipRelationshipsExpected(
            {{"swift", "taylor"}, {"adkins", "adele"}});

        requireVectorsHaveSameElements(resTable.getResultsTwo("ast", "parser"), astParserRelationshipsString);
        requireVectorsHaveSameElements(resTable.getResultsTwo("ast", "pqlprocessor"),
                                       astPqlProcessorRelationshipsString);
        requireVectorsHaveSameElements(resTable.getResultsTwo("designextractor", "parser"),
                                       designExtractorParserRelationshipsExpected);
        requireVectorsHaveSameElements(resTable.getResultsTwo("swampert", "mudkip"),
                                       swampertMudkipRelationshipsExpected);
    }
}
