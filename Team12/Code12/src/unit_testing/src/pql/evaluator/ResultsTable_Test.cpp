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
    std::vector<std::string> procedures = {"taylor", "adele", "terracotta", "concrete"};
    std::vector<std::string> statements = {"3", "6", "12", "33", "34", "89"};
    std::vector<std::string> variables = {"swift", "adkins", "treintaytres", "ochentaynueve", "doce"};

    ResultsTable resTable(DeclarationTable{});
    resTable.storeResultsOne("mudkip", procedures);
    resTable.storeResultsOne("swampert", variables);
    resTable.storeResultsOne("ast", statements);
    resTable.storeResultsOne("pqlprocessor", procedures);
    resTable.storeResultsOne("designextractor", statements);
    resTable.storeResultsOne("parser", variables);

    std::vector<std::pair<std::string, std::string>> mudkipSwampertRelationships(
        {{"taylor", "swift"}, {"adele", "adkins"}});
    std::vector<std::pair<std::string, std::string>> astPqlProcessorRelationships(
        {{"6", "terracotta"}, {"3", "concrete"}});
    std::vector<std::pair<std::string, std::string>> parserDesignExtractorRelationships(
        {{"treintaytres", "33"}, {"ochentaynueve", "89"}, {"doce", "12"}});
    std::vector<std::pair<std::string, std::string>> astDesignExtractorRelationships({{"6", "33"}});

    resTable.storeResultsTwo("mudkip", "swampert", mudkipSwampertRelationships);
    resTable.storeResultsTwo("ast", "pqlprocessor", astPqlProcessorRelationships);
    resTable.storeResultsTwo("parser", "designextractor", parserDesignExtractorRelationships);
    resTable.storeResultsTwo("ast", "designextractor", astDesignExtractorRelationships);
    resTable.getResultsZero();

    SECTION("hasRelationships returns true for related synonyms, false for unrelated synonyms")
    {
        REQUIRE(resTable.hasRelationships("ast", "parser"));
        REQUIRE(resTable.hasRelationships("ast", "pqlprocessor"));
        REQUIRE(resTable.hasRelationships("parser", "designextractor"));
        REQUIRE(resTable.hasRelationships("swampert", "mudkip"));
        REQUIRE(resTable.hasRelationships("ast", "designextractor"));
        REQUIRE(resTable.hasRelationships("parser", "pqlprocessor"));

        REQUIRE_FALSE(resTable.hasRelationships("swampert", "ast"));
        REQUIRE_FALSE(resTable.hasRelationships("mudkip", "parser"));
    }

    SECTION("getResults retrieves all relationships in table")
    {
        std::vector<std::pair<std::string, std::string>> astPqlProcessorRelationshipsString({{"6", "terracotta"}});
        std::vector<std::pair<std::string, std::string>> astParserRelationshipsString({{"6", "treintaytres"}});
        // "parser" and "designextractor" are swapped
        std::vector<std::pair<std::string, std::string>> designExtractorParserRelationshipsExpected(
            {{"33", "treintaytres"}});
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

TEST_CASE("Results of two synonyms merges to give expected relationships")
{
    std::vector<std::pair<std::string, std::string>> redGreenRelationships(
        {{"ns25", "ew13"}, {"ns26", "ew14"}, {"ns1", "ew24"}});
    std::vector<std::pair<std::string, std::string>> numPurpleRelationships(
        {{"6", "dhobyghaut"}, {"3", "outrampark"}, {"16", "sengkang"}, {"stc", "sengkang"}, {"16", "outrampark"}});
    std::vector<std::pair<std::string, std::string>> circleNumRelationships({{"onenorth", "23"},
                                                                             {"harbourfront", "29"},
                                                                             {"bartley", "12"},
                                                                             {"hollandvillage", "21"},
                                                                             {"marymount", "16"},
                                                                             {"dhobyghaut", "1"},
                                                                             {"esplanade", "3"}});
    std::vector<std::pair<std::string, std::string>> ccDtRelationships(
        {{"4", "15"}, {"19", "9"}, {"10", "26"}, {"E1", "16"}});
    std::vector<std::pair<std::string, std::string>> circleRedRelationships(
        {{"esplanade", "ns25"}, {"marymount", "ns1"}, {"marinabay", "ns27"}});

    ResultsTable table(DeclarationTable{});
    table.storeResultsTwo("red", "green", redGreenRelationships);
    table.storeResultsTwo("num", "purple", numPurpleRelationships);
    table.storeResultsTwo("circle", "num", circleNumRelationships);
    table.storeResultsTwo("CC", "DT", ccDtRelationships);
    table.storeResultsTwo("circle", "red", circleRedRelationships);
    table.getResultsZero();
    REQUIRE(table.getRelationshipsGraph().compareStructure(setUpTestingGraph()));
}

TEST_CASE("Results of two synonyms merges to give empty results")
{
    std::vector<std::pair<std::string, std::string>> redGreenRelationships(
        {{"ns25", "ew13"}, {"ns26", "ew14"}, {"ns1", "ew24"}});
    std::vector<std::pair<std::string, std::string>> numPurpleRelationships(
        {{"6", "dhobyghaut"}, {"3", "outrampark"}, {"16", "sengkang"}, {"stc", "sengkang"}, {"16", "outrampark"}});
    std::vector<std::pair<std::string, std::string>> circleNumRelationships({{"onenorth", "23"},
                                                                             {"harbourfront", "29"},
                                                                             {"bartley", "12"},
                                                                             {"hollandvillage", "21"},
                                                                             {"marymount", "16"},
                                                                             {"dhobyghaut", "1"},
                                                                             {"esplanade", "3"}});
    std::vector<std::pair<std::string, std::string>> ccDtRelationships(
        {{"4", "15"}, {"19", "9"}, {"10", "26"}, {"E1", "16"}});
    std::vector<std::pair<std::string, std::string>> circlePurpleRelationships(
        {{"esplanade", "sengkang"}, {"marymount", "potongpasir"}, {"marinabay", "outrampark"}});

    ResultsTable table(DeclarationTable{});
    table.storeResultsTwo("red", "green", redGreenRelationships);
    table.storeResultsTwo("num", "purple", numPurpleRelationships);
    table.storeResultsTwo("circle", "num", circleNumRelationships);
    table.storeResultsTwo("CC", "DT", ccDtRelationships);
    table.storeResultsTwo("circle", "purple", circlePurpleRelationships);
    table.getResultsZero();
    REQUIRE_FALSE(table.hasResults());
}

TEST_CASE("getResultsTwo returns expected relationships")
{
    std::vector<std::pair<std::string, std::string>> redGreenRelationships(
        {{"ns25", "ew13"}, {"ns26", "ew14"}, {"ns1", "ew24"}});
    std::vector<std::pair<std::string, std::string>> numPurpleRelationships(
        {{"6", "dhobyghaut"}, {"3", "outrampark"}, {"16", "sengkang"}, {"stc", "sengkang"}, {"16", "outrampark"}});
    std::vector<std::pair<std::string, std::string>> circleNumRelationships({{"onenorth", "23"},
                                                                             {"harbourfront", "29"},
                                                                             {"bartley", "12"},
                                                                             {"hollandvillage", "21"},
                                                                             {"marymount", "16"},
                                                                             {"dhobyghaut", "1"},
                                                                             {"esplanade", "3"}});
    std::vector<std::pair<std::string, std::string>> ccDtRelationships(
        {{"4", "15"}, {"19", "9"}, {"10", "26"}, {"E1", "16"}});
    std::vector<std::pair<std::string, std::string>> circleRedRelationships(
        {{"esplanade", "ns25"}, {"marymount", "ns1"}, {"marinabay", "ns27"}});

    ResultsTable table(DeclarationTable{});
    table.storeResultsTwo("red", "green", redGreenRelationships);
    table.storeResultsTwo("num", "purple", numPurpleRelationships);
    table.storeResultsTwo("circle", "num", circleNumRelationships);
    table.storeResultsTwo("CC", "DT", ccDtRelationships);
    table.storeResultsTwo("circle", "red", circleRedRelationships);

    SECTION("getResultsTwo returns correct relationships for related synonyms")
    {
        requireVectorsHaveSameElements(table.getResultsTwo("red", "green"), {{"ns1", "ew24"}, {"ns25", "ew13"}});
        requireVectorsHaveSameElements(table.getResultsTwo("num", "circle"), {{"16", "marymount"}, {"3", "esplanade"}});
        requireVectorsHaveSameElements(table.getResultsTwo("purple", "green"),
                                       {{"sengkang", "ew24"}, {"outrampark", "ew24"}, {"outrampark", "ew13"}});
        requireVectorsHaveSameElements(table.getResultsTwo("DT", "CC"),
                                       {{"15", "4"}, {"9", "19"}, {"26", "10"}, {"16", "E1"}});
    }

    SECTION("getResultsTwo returns cross-product for unrelated synonyms")
    {
        requireVectorsHaveSameElements(table.getResultsTwo("red", "DT"), {{"ns1", "15"},
                                                                          {"ns25", "15"},
                                                                          {"ns1", "9"},
                                                                          {"ns25", "9"},
                                                                          {"ns1", "26"},
                                                                          {"ns25", "26"},
                                                                          {"ns1", "16"},
                                                                          {"ns25", "16"}});
        requireVectorsHaveSameElements(
            table.getResultsTwo("num", "CC"),
            {{"16", "4"}, {"3", "4"}, {"16", "19"}, {"3", "19"}, {"16", "10"}, {"3", "10"}, {"16", "E1"}, {"3", "E1"}});
        requireVectorsHaveSameElements(table.getResultsTwo("CC", "purple"), {{"4", "sengkang"},
                                                                             {"4", "outrampark"},
                                                                             {"19", "sengkang"},
                                                                             {"19", "outrampark"},
                                                                             {"10", "sengkang"},
                                                                             {"10", "outrampark"},
                                                                             {"E1", "sengkang"},
                                                                             {"E1", "outrampark"}});
        requireVectorsHaveSameElements(table.getResultsTwo("DT", "circle"), {{"15", "marymount"},
                                                                             {"15", "esplanade"},
                                                                             {"9", "marymount"},
                                                                             {"9", "esplanade"},
                                                                             {"26", "marymount"},
                                                                             {"26", "esplanade"},
                                                                             {"16", "marymount"},
                                                                             {"16", "esplanade"}});
        requireVectorsHaveSameElements(table.getResultsTwo("green", "CC"), {{"ew24", "4"},
                                                                            {"ew13", "4"},
                                                                            {"ew24", "19"},
                                                                            {"ew13", "19"},
                                                                            {"ew24", "10"},
                                                                            {"ew13", "10"},
                                                                            {"ew24", "E1"},
                                                                            {"ew13", "E1"}});
    }
}
