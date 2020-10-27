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
    std::unique_ptr<ResultsTable> table = setUpResultsTableWithSameTestingGraph();
    table->getResultsZero();
    REQUIRE(table->getRelationshipsGraph().compareStructure(setUpTestingGraph()));
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
    std::unique_ptr<ResultsTable> table = setUpResultsTableWithSameTestingGraph();

    SECTION("getResultsTwo returns correct relationships for related synonyms")
    {
        requireVectorsHaveSameElements(table->getResultsTwo("red", "green"), {{"ns1", "ew24"}, {"ns25", "ew13"}});
        requireVectorsHaveSameElements(table->getResultsTwo("num", "circle"),
                                       {{"16", "marymount"}, {"3", "esplanade"}});
        requireVectorsHaveSameElements(table->getResultsTwo("purple", "green"),
                                       {{"sengkang", "ew24"}, {"outrampark", "ew24"}, {"outrampark", "ew13"}});
        requireVectorsHaveSameElements(table->getResultsTwo("DT", "CC"),
                                       {{"15", "4"}, {"9", "19"}, {"26", "10"}, {"16", "E1"}});
    }

    SECTION("getResultsTwo returns cross-product for unrelated synonyms")
    {
        requireVectorsHaveSameElements(table->getResultsTwo("red", "DT"), {{"ns1", "15"},
                                                                           {"ns25", "15"},
                                                                           {"ns1", "9"},
                                                                           {"ns25", "9"},
                                                                           {"ns1", "26"},
                                                                           {"ns25", "26"},
                                                                           {"ns1", "16"},
                                                                           {"ns25", "16"}});
        requireVectorsHaveSameElements(
            table->getResultsTwo("num", "CC"),
            {{"16", "4"}, {"3", "4"}, {"16", "19"}, {"3", "19"}, {"16", "10"}, {"3", "10"}, {"16", "E1"}, {"3", "E1"}});
        requireVectorsHaveSameElements(table->getResultsTwo("CC", "purple"), {{"4", "sengkang"},
                                                                              {"4", "outrampark"},
                                                                              {"19", "sengkang"},
                                                                              {"19", "outrampark"},
                                                                              {"10", "sengkang"},
                                                                              {"10", "outrampark"},
                                                                              {"E1", "sengkang"},
                                                                              {"E1", "outrampark"}});
        requireVectorsHaveSameElements(table->getResultsTwo("DT", "circle"), {{"15", "marymount"},
                                                                              {"15", "esplanade"},
                                                                              {"9", "marymount"},
                                                                              {"9", "esplanade"},
                                                                              {"26", "marymount"},
                                                                              {"26", "esplanade"},
                                                                              {"16", "marymount"},
                                                                              {"16", "esplanade"}});
        requireVectorsHaveSameElements(table->getResultsTwo("green", "CC"), {{"ew24", "4"},
                                                                             {"ew13", "4"},
                                                                             {"ew24", "19"},
                                                                             {"ew13", "19"},
                                                                             {"ew24", "10"},
                                                                             {"ew13", "10"},
                                                                             {"ew24", "E1"},
                                                                             {"ew13", "E1"}});
    }
}

TEST_CASE("getResultsN returns expected relationships")
{
    std::unique_ptr<ResultsTable> table = setUpResultsTableWithSameTestingGraph();
    NtupledResult results = table->getResultsN({"red", "green", "num", "purple", "circle", "CC", "DT"});
    requireVectorsHaveSameElements(results, {{"ns1", "ew24", "16", "sengkang", "marymount", "4", "15"},
                                             {"ns1", "ew24", "16", "outrampark", "marymount", "4", "15"},
                                             {"ns25", "ew13", "3", "outrampark", "esplanade", "4", "15"},
                                             {"ns1", "ew24", "16", "sengkang", "marymount", "19", "9"},
                                             {"ns1", "ew24", "16", "outrampark", "marymount", "19", "9"},
                                             {"ns25", "ew13", "3", "outrampark", "esplanade", "19", "9"},
                                             {"ns1", "ew24", "16", "sengkang", "marymount", "10", "26"},
                                             {"ns1", "ew24", "16", "outrampark", "marymount", "10", "26"},
                                             {"ns25", "ew13", "3", "outrampark", "esplanade", "10", "26"},
                                             {"ns1", "ew24", "16", "sengkang", "marymount", "E1", "16"},
                                             {"ns1", "ew24", "16", "outrampark", "marymount", "E1", "16"},
                                             {"ns25", "ew13", "3", "outrampark", "esplanade", "E1", "16"}});
}

TEST_CASE("getResultsN performs Cartesian product for repeated synonyms")
{
    ResultsTable table(DeclarationTable{});
    table.storeResultsOne("o", {"1", "2", "3"});
    table.storeResultsOne("p", {"4", "5", "6"});
    table.storeResultsOne("q", {"Cpp", "Java", "Python", "Rust"});
    table.storeResultsOne("r", {"a", "b", "c", "d", "e"});

    SECTION("Select <p, p, p>")
    {
        NtupledResult results = table.getResultsN({"p", "p", "p"});
        requireVectorsHaveSameElements(results, {{"4", "4", "4"}, {"4", "4", "5"}, {"4", "4", "6"}, {"4", "5", "4"},
                                                 {"4", "5", "5"}, {"4", "5", "6"}, {"4", "6", "4"}, {"4", "6", "5"},
                                                 {"4", "6", "6"}, {"5", "4", "4"}, {"5", "4", "5"}, {"5", "4", "6"},
                                                 {"5", "5", "4"}, {"5", "5", "5"}, {"5", "5", "6"}, {"5", "6", "4"},
                                                 {"5", "6", "5"}, {"5", "6", "6"}, {"6", "4", "4"}, {"6", "4", "5"},
                                                 {"6", "4", "6"}, {"6", "5", "4"}, {"6", "5", "5"}, {"6", "5", "6"},
                                                 {"6", "6", "4"}, {"6", "6", "5"}, {"6", "6", "6"}});
    }

    SECTION("Select <p, o, p>")
    {
        NtupledResult results = table.getResultsN({"p", "o", "p"});
        requireVectorsHaveSameElements(results, {{"4", "1", "4"}, {"4", "1", "5"}, {"4", "1", "6"}, {"4", "2", "4"},
                                                 {"4", "2", "5"}, {"4", "2", "6"}, {"4", "3", "4"}, {"4", "3", "5"},
                                                 {"4", "3", "6"}, {"5", "1", "4"}, {"5", "1", "5"}, {"5", "1", "6"},
                                                 {"5", "2", "4"}, {"5", "2", "5"}, {"5", "2", "6"}, {"5", "3", "4"},
                                                 {"5", "3", "5"}, {"5", "3", "6"}, {"6", "1", "4"}, {"6", "1", "5"},
                                                 {"6", "1", "6"}, {"6", "2", "4"}, {"6", "2", "5"}, {"6", "2", "6"},
                                                 {"6", "3", "4"}, {"6", "3", "5"}, {"6", "3", "6"}});
    }

    SECTION("Select <p, o, p, o>")
    {
        NtupledResult results = table.getResultsN({"p", "o", "p", "o"});
        requireVectorsHaveSameElements(
            results, {{"4", "1", "4", "1"}, {"4", "1", "5", "1"}, {"4", "1", "6", "1"}, {"4", "2", "4", "1"},
                      {"4", "2", "5", "1"}, {"4", "2", "6", "1"}, {"4", "3", "4", "1"}, {"4", "3", "5", "1"},
                      {"4", "3", "6", "1"}, {"5", "1", "4", "1"}, {"5", "1", "5", "1"}, {"5", "1", "6", "1"},
                      {"5", "2", "4", "1"}, {"5", "2", "5", "1"}, {"5", "2", "6", "1"}, {"5", "3", "4", "1"},
                      {"5", "3", "5", "1"}, {"5", "3", "6", "1"}, {"6", "1", "4", "1"}, {"6", "1", "5", "1"},
                      {"6", "1", "6", "1"}, {"6", "2", "4", "1"}, {"6", "2", "5", "1"}, {"6", "2", "6", "1"},
                      {"6", "3", "4", "1"}, {"6", "3", "5", "1"}, {"6", "3", "6", "1"}, {"4", "1", "4", "2"},
                      {"4", "1", "5", "2"}, {"4", "1", "6", "2"}, {"4", "2", "4", "2"}, {"4", "2", "5", "2"},
                      {"4", "2", "6", "2"}, {"4", "3", "4", "2"}, {"4", "3", "5", "2"}, {"4", "3", "6", "2"},
                      {"5", "1", "4", "2"}, {"5", "1", "5", "2"}, {"5", "1", "6", "2"}, {"5", "2", "4", "2"},
                      {"5", "2", "5", "2"}, {"5", "2", "6", "2"}, {"5", "3", "4", "2"}, {"5", "3", "5", "2"},
                      {"5", "3", "6", "2"}, {"6", "1", "4", "2"}, {"6", "1", "5", "2"}, {"6", "1", "6", "2"},
                      {"6", "2", "4", "2"}, {"6", "2", "5", "2"}, {"6", "2", "6", "2"}, {"6", "3", "4", "2"},
                      {"6", "3", "5", "2"}, {"6", "3", "6", "2"}, {"4", "1", "4", "3"}, {"4", "1", "5", "3"},
                      {"4", "1", "6", "3"}, {"4", "2", "4", "3"}, {"4", "2", "5", "3"}, {"4", "2", "6", "3"},
                      {"4", "3", "4", "3"}, {"4", "3", "5", "3"}, {"4", "3", "6", "3"}, {"5", "1", "4", "3"},
                      {"5", "1", "5", "3"}, {"5", "1", "6", "3"}, {"5", "2", "4", "3"}, {"5", "2", "5", "3"},
                      {"5", "2", "6", "3"}, {"5", "3", "4", "3"}, {"5", "3", "5", "3"}, {"5", "3", "6", "3"},
                      {"6", "1", "4", "3"}, {"6", "1", "5", "3"}, {"6", "1", "6", "3"}, {"6", "2", "4", "3"},
                      {"6", "2", "5", "3"}, {"6", "2", "6", "3"}, {"6", "3", "4", "3"}, {"6", "3", "5", "3"},
                      {"6", "3", "6", "3"}});
    }

    SECTION("Select <o, p, q, p>")
    {
        NtupledResult results = table.getResultsN({"o", "p", "q", "p"});
        requireVectorsHaveSameElements(
            results,
            {{"1", "4", "Cpp", "4"},    {"1", "4", "Cpp", "5"},    {"1", "4", "Cpp", "6"},  {"1", "4", "Python", "4"},
             {"1", "4", "Python", "5"}, {"1", "4", "Python", "6"}, {"1", "4", "Java", "4"}, {"1", "4", "Java", "5"},
             {"1", "4", "Java", "6"},   {"1", "4", "Rust", "4"},   {"1", "4", "Rust", "5"}, {"1", "4", "Rust", "6"},
             {"1", "5", "Cpp", "4"},    {"1", "5", "Cpp", "5"},    {"1", "5", "Cpp", "6"},  {"1", "5", "Python", "4"},
             {"1", "5", "Python", "5"}, {"1", "5", "Python", "6"}, {"1", "5", "Java", "4"}, {"1", "5", "Java", "5"},
             {"1", "5", "Java", "6"},   {"1", "5", "Rust", "4"},   {"1", "5", "Rust", "5"}, {"1", "5", "Rust", "6"},
             {"1", "6", "Cpp", "4"},    {"1", "6", "Cpp", "5"},    {"1", "6", "Cpp", "6"},  {"1", "6", "Python", "4"},
             {"1", "6", "Python", "5"}, {"1", "6", "Python", "6"}, {"1", "6", "Java", "4"}, {"1", "6", "Java", "5"},
             {"1", "6", "Java", "6"},   {"1", "6", "Rust", "4"},   {"1", "6", "Rust", "5"}, {"1", "6", "Rust", "6"},
             {"2", "4", "Cpp", "4"},    {"2", "4", "Cpp", "5"},    {"2", "4", "Cpp", "6"},  {"2", "4", "Python", "4"},
             {"2", "4", "Python", "5"}, {"2", "4", "Python", "6"}, {"2", "4", "Java", "4"}, {"2", "4", "Java", "5"},
             {"2", "4", "Java", "6"},   {"2", "4", "Rust", "4"},   {"2", "4", "Rust", "5"}, {"2", "4", "Rust", "6"},
             {"2", "5", "Cpp", "4"},    {"2", "5", "Cpp", "5"},    {"2", "5", "Cpp", "6"},  {"2", "5", "Python", "4"},
             {"2", "5", "Python", "5"}, {"2", "5", "Python", "6"}, {"2", "5", "Java", "4"}, {"2", "5", "Java", "5"},
             {"2", "5", "Java", "6"},   {"2", "5", "Rust", "4"},   {"2", "5", "Rust", "5"}, {"2", "5", "Rust", "6"},
             {"2", "6", "Cpp", "4"},    {"2", "6", "Cpp", "5"},    {"2", "6", "Cpp", "6"},  {"2", "6", "Python", "4"},
             {"2", "6", "Python", "5"}, {"2", "6", "Python", "6"}, {"2", "6", "Java", "4"}, {"2", "6", "Java", "5"},
             {"2", "6", "Java", "6"},   {"2", "6", "Rust", "4"},   {"2", "6", "Rust", "5"}, {"2", "6", "Rust", "6"},
             {"3", "4", "Cpp", "4"},    {"3", "4", "Cpp", "5"},    {"3", "4", "Cpp", "6"},  {"3", "4", "Python", "4"},
             {"3", "4", "Python", "5"}, {"3", "4", "Python", "6"}, {"3", "4", "Java", "4"}, {"3", "4", "Java", "5"},
             {"3", "4", "Java", "6"},   {"3", "4", "Rust", "4"},   {"3", "4", "Rust", "5"}, {"3", "4", "Rust", "6"},
             {"3", "5", "Cpp", "4"},    {"3", "5", "Cpp", "5"},    {"3", "5", "Cpp", "6"},  {"3", "5", "Python", "4"},
             {"3", "5", "Python", "5"}, {"3", "5", "Python", "6"}, {"3", "5", "Java", "4"}, {"3", "5", "Java", "5"},
             {"3", "5", "Java", "6"},   {"3", "5", "Rust", "4"},   {"3", "5", "Rust", "5"}, {"3", "5", "Rust", "6"},
             {"3", "6", "Cpp", "4"},    {"3", "6", "Cpp", "5"},    {"3", "6", "Cpp", "6"},  {"3", "6", "Python", "4"},
             {"3", "6", "Python", "5"}, {"3", "6", "Python", "6"}, {"3", "6", "Java", "4"}, {"3", "6", "Java", "5"},
             {"3", "6", "Java", "6"},   {"3", "6", "Rust", "4"},   {"3", "6", "Rust", "5"}, {"3", "6", "Rust", "6"}});
    }

    SECTION("Select <p, q, r, p>")
    {
        NtupledResult results = table.getResultsN({"p", "q", "r", "p"});
        requireVectorsHaveSameElements(
            results,
            {{"4", "Cpp", "a", "4"},    {"4", "Cpp", "a", "5"},    {"4", "Cpp", "a", "6"},    {"4", "Cpp", "b", "4"},
             {"4", "Cpp", "b", "5"},    {"4", "Cpp", "b", "6"},    {"4", "Cpp", "c", "4"},    {"4", "Cpp", "c", "5"},
             {"4", "Cpp", "c", "6"},    {"4", "Cpp", "d", "4"},    {"4", "Cpp", "d", "5"},    {"4", "Cpp", "d", "6"},
             {"4", "Cpp", "e", "4"},    {"4", "Cpp", "e", "5"},    {"4", "Cpp", "e", "6"},    {"4", "Java", "a", "4"},
             {"4", "Java", "a", "5"},   {"4", "Java", "a", "6"},   {"4", "Java", "b", "4"},   {"4", "Java", "b", "5"},
             {"4", "Java", "b", "6"},   {"4", "Java", "c", "4"},   {"4", "Java", "c", "5"},   {"4", "Java", "c", "6"},
             {"4", "Java", "d", "4"},   {"4", "Java", "d", "5"},   {"4", "Java", "d", "6"},   {"4", "Java", "e", "4"},
             {"4", "Java", "e", "5"},   {"4", "Java", "e", "6"},   {"4", "Python", "a", "4"}, {"4", "Python", "a", "5"},
             {"4", "Python", "a", "6"}, {"4", "Python", "b", "4"}, {"4", "Python", "b", "5"}, {"4", "Python", "b", "6"},
             {"4", "Python", "c", "4"}, {"4", "Python", "c", "5"}, {"4", "Python", "c", "6"}, {"4", "Python", "d", "4"},
             {"4", "Python", "d", "5"}, {"4", "Python", "d", "6"}, {"4", "Python", "e", "4"}, {"4", "Python", "e", "5"},
             {"4", "Python", "e", "6"}, {"4", "Rust", "a", "4"},   {"4", "Rust", "a", "5"},   {"4", "Rust", "a", "6"},
             {"4", "Rust", "b", "4"},   {"4", "Rust", "b", "5"},   {"4", "Rust", "b", "6"},   {"4", "Rust", "c", "4"},
             {"4", "Rust", "c", "5"},   {"4", "Rust", "c", "6"},   {"4", "Rust", "d", "4"},   {"4", "Rust", "d", "5"},
             {"4", "Rust", "d", "6"},   {"4", "Rust", "e", "4"},   {"4", "Rust", "e", "5"},   {"4", "Rust", "e", "6"},
             {"5", "Cpp", "a", "4"},    {"5", "Cpp", "a", "5"},    {"5", "Cpp", "a", "6"},    {"5", "Cpp", "b", "4"},
             {"5", "Cpp", "b", "5"},    {"5", "Cpp", "b", "6"},    {"5", "Cpp", "c", "4"},    {"5", "Cpp", "c", "5"},
             {"5", "Cpp", "c", "6"},    {"5", "Cpp", "d", "4"},    {"5", "Cpp", "d", "5"},    {"5", "Cpp", "d", "6"},
             {"5", "Cpp", "e", "4"},    {"5", "Cpp", "e", "5"},    {"5", "Cpp", "e", "6"},    {"5", "Java", "a", "4"},
             {"5", "Java", "a", "5"},   {"5", "Java", "a", "6"},   {"5", "Java", "b", "4"},   {"5", "Java", "b", "5"},
             {"5", "Java", "b", "6"},   {"5", "Java", "c", "4"},   {"5", "Java", "c", "5"},   {"5", "Java", "c", "6"},
             {"5", "Java", "d", "4"},   {"5", "Java", "d", "5"},   {"5", "Java", "d", "6"},   {"5", "Java", "e", "4"},
             {"5", "Java", "e", "5"},   {"5", "Java", "e", "6"},   {"5", "Python", "a", "4"}, {"5", "Python", "a", "5"},
             {"5", "Python", "a", "6"}, {"5", "Python", "b", "4"}, {"5", "Python", "b", "5"}, {"5", "Python", "b", "6"},
             {"5", "Python", "c", "4"}, {"5", "Python", "c", "5"}, {"5", "Python", "c", "6"}, {"5", "Python", "d", "4"},
             {"5", "Python", "d", "5"}, {"5", "Python", "d", "6"}, {"5", "Python", "e", "4"}, {"5", "Python", "e", "5"},
             {"5", "Python", "e", "6"}, {"5", "Rust", "a", "4"},   {"5", "Rust", "a", "5"},   {"5", "Rust", "a", "6"},
             {"5", "Rust", "b", "4"},   {"5", "Rust", "b", "5"},   {"5", "Rust", "b", "6"},   {"5", "Rust", "c", "4"},
             {"5", "Rust", "c", "5"},   {"5", "Rust", "c", "6"},   {"5", "Rust", "d", "4"},   {"5", "Rust", "d", "5"},
             {"5", "Rust", "d", "6"},   {"5", "Rust", "e", "4"},   {"5", "Rust", "e", "5"},   {"5", "Rust", "e", "6"},
             {"6", "Cpp", "a", "4"},    {"6", "Cpp", "a", "5"},    {"6", "Cpp", "a", "6"},    {"6", "Cpp", "b", "4"},
             {"6", "Cpp", "b", "5"},    {"6", "Cpp", "b", "6"},    {"6", "Cpp", "c", "4"},    {"6", "Cpp", "c", "5"},
             {"6", "Cpp", "c", "6"},    {"6", "Cpp", "d", "4"},    {"6", "Cpp", "d", "5"},    {"6", "Cpp", "d", "6"},
             {"6", "Cpp", "e", "4"},    {"6", "Cpp", "e", "5"},    {"6", "Cpp", "e", "6"},    {"6", "Java", "a", "4"},
             {"6", "Java", "a", "5"},   {"6", "Java", "a", "6"},   {"6", "Java", "b", "4"},   {"6", "Java", "b", "5"},
             {"6", "Java", "b", "6"},   {"6", "Java", "c", "4"},   {"6", "Java", "c", "5"},   {"6", "Java", "c", "6"},
             {"6", "Java", "d", "4"},   {"6", "Java", "d", "5"},   {"6", "Java", "d", "6"},   {"6", "Java", "e", "4"},
             {"6", "Java", "e", "5"},   {"6", "Java", "e", "6"},   {"6", "Python", "a", "4"}, {"6", "Python", "a", "5"},
             {"6", "Python", "a", "6"}, {"6", "Python", "b", "4"}, {"6", "Python", "b", "5"}, {"6", "Python", "b", "6"},
             {"6", "Python", "c", "4"}, {"6", "Python", "c", "5"}, {"6", "Python", "c", "6"}, {"6", "Python", "d", "4"},
             {"6", "Python", "d", "5"}, {"6", "Python", "d", "6"}, {"6", "Python", "e", "4"}, {"6", "Python", "e", "5"},
             {"6", "Python", "e", "6"}, {"6", "Rust", "a", "4"},   {"6", "Rust", "a", "5"},   {"6", "Rust", "a", "6"},
             {"6", "Rust", "b", "4"},   {"6", "Rust", "b", "5"},   {"6", "Rust", "b", "6"},   {"6", "Rust", "c", "4"},
             {"6", "Rust", "c", "5"},   {"6", "Rust", "c", "6"},   {"6", "Rust", "d", "4"},   {"6", "Rust", "d", "5"},
             {"6", "Rust", "d", "6"},   {"6", "Rust", "e", "4"},   {"6", "Rust", "e", "5"},   {"6", "Rust", "e", "6"}});
    }
}

TEST_CASE("mergeTwoSynonyms merges first synonym first when required to")
{
    ResultsTable table(DeclarationTable{});
    table.storeResultsOne("ciento", {"101", "102", "103", "104", "105", "106"});
    table.storeResultsTwo("doscientos", "ciento", {{"203", "101"}, {"202", "104"}, {"201", "107"}, {"204", "108"}});
    PairedResult resultsTwo = table.getResultsTwo("doscientos", "ciento");
    requireVectorsHaveSameElements(resultsTwo, {{"202", "104"}, {"203", "101"}});
    ClauseResult resultsOne = table.getResultsOne("doscientos");
    // if second synonym "ciento" is merged first, this gives {"201", "202", "203", "204"}
    //
    // the "filtering" of second synonym "doscientos" is not
    // done, since second synonym did not exist in graph yet
    requireVectorsHaveSameElements(resultsOne, {"202", "203"});
}

TEST_CASE("mergeTwoSynonyms merges second synonym first when required to")
{
    ResultsTable table(DeclarationTable{});
    table.storeResultsOne("ciento", {"101", "102", "103", "104", "105", "106"});
    table.storeResultsTwo("ciento", "doscientos", {{"101", "203"}, {"104", "202"}, {"107", "201"}, {"108", "204"}});
    PairedResult resultsTwo = table.getResultsTwo("doscientos", "ciento");
    requireVectorsHaveSameElements(resultsTwo, {{"202", "104"}, {"203", "101"}});
    ClauseResult resultsOne = table.getResultsOne("doscientos");
    // if first synonym "ciento" is merged first, this gives {"201", "202", "203", "204"}
    //
    // the "filtering" of second synonym "doscientos" is not
    // done, since second synonym did not exist in graph yet
    requireVectorsHaveSameElements(resultsOne, {"202", "203"});
}
