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
    resTable.filterTable("abcd", std::vector<std::string>());

    REQUIRE(!resTable.hasResults());
    REQUIRE(resTable.get("abcd").empty());
    // check if other synonyms also return empty
    REQUIRE(resTable.get("efgh").empty());
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
    resTable.filterTable("abcd", expectedResultsAbcd);

    REQUIRE(resTable.hasResults());
    doVectorsHaveSameElementsVoid(resTable.get("efgh"), expectedResultsEfgh);
    doVectorsHaveSameElementsVoid(resTable.get("abcd"), expectedResultsAbcd);
}

TEST_CASE("ResultsTable filters results correctly")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    std::vector<std::string> newResults{"112", "18397", "3", "0", "143", "539"};
    std::vector<std::string> expectedResults{"3", "143", "539"};
    resTable.filterTable("abcd", originalResults);
    resTable.filterTable("abcd", newResults);

    doVectorsHaveSameElementsVoid(resTable.get("abcd"), expectedResults);
}

TEST_CASE("ResultsTable filters results correctly for synonym reference")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"piglin",      "dolphin",  "enderman", "creeper",
                                             "enderdragon", "villager", "mooshroom"};
    std::vector<std::string> newResults{"pig", "mooshroom", "sheep", "enderman", "dolphin", "salmon"};
    std::vector<std::string> expectedResults{"mooshroom", "enderman", "dolphin"};
    resTable.filterTable(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), originalResults);
    resTable.filterTable(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), newResults);

    doVectorsHaveSameElementsVoid(resTable.get("abcd"), expectedResults);
}

TEST_CASE("ResultsTable does not filter any results for non-synonym reference")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"piglin",      "dolphin",  "enderman", "creeper",
                                             "enderdragon", "villager", "mooshroom"};
    std::vector<std::string> newResults{"pig", "mooshroom", "sheep", "enderman", "dolphin", "salmon"};
    std::vector<std::string> moreResults{"enderdragon", "enderman", "shulker", "piglinbrute",
                                         "endermite",   "creeper",  "strider"};
    resTable.filterTable(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), originalResults);
    resTable.filterTable(Reference(WildcardRefType, "_"), newResults);
    resTable.filterTable(Reference(IntegerRefType, "12"), moreResults);

    ResultsTable resTable2(DeclarationTable{});
    resTable2.filterTable(Reference(SynonymRefType, "abcd", DesignEntity(VariableType)), originalResults);

    doVectorsHaveSameElementsVoid(resTable.get("abcd"), originalResults);
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
    std::vector<std::string> abcdResults = resTable.get("abcd");

    doVectorsHaveSameElementsVoid(abcdResults, std::vector<std::string>({"1", "2", "3", "4", "5"}));
}

TEST_CASE("checkIfSynonymHasConstraints returns false for synonym with no constraints, and returns true for synonym "
          "with constraints")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    std::vector<std::string> newResults{"112", "18397", "3", "0", "143", "539"};
    std::vector<std::string> expectedResults{"3", "143", "539"};

    REQUIRE_FALSE(resTable.checkIfSynonymHasConstraints("abcd"));

    resTable.filterTable("abcd", originalResults);
    resTable.filterTable("abcd", newResults);

    REQUIRE(resTable.checkIfSynonymHasConstraints("abcd"));
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
    resTable.filterTable("abcd", originalResults);
    resTable.eliminatePotentialValue("abcd", "134");

    ResultsTable resTableExpected(DeclarationTable{});
    std::vector<std::string> expectedResults{"1", "3", "7", "539", "23", "143", "9"};
    resTableExpected.filterTable("abcd", expectedResults);

    REQUIRE(resTable == resTableExpected);
}

TEST_CASE("eliminatePotentialValue does not do anything for synonym not found")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    resTable.filterTable("abcd", originalResults);
    resTable.eliminatePotentialValue("efgh", "134");

    ResultsTable resTableExpected(DeclarationTable{});
    resTableExpected.filterTable("abcd", originalResults);

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
    std::vector<std::pair<int, std::string>> astPqlProcessorRelationships({{6, "terracotta"}, {3, "concrete"}});
    std::vector<std::pair<std::string, int>> parserDesignExtractorRelationships(
        {{"treintaytres", 33}, {"ochentaynueve", 89}, {"doce", 12}});
    std::vector<std::pair<int, int>> astParserRelationships({{12, 34}});

    resTable.associateRelationships(mudkipSwampertRelationships,
                                    Reference(SynonymRefType, "mudkip", DesignEntity(ProcedureType)),
                                    Reference(SynonymRefType, "swampert", DesignEntity(VariableType)));
    resTable.associateRelationships(astPqlProcessorRelationships,
                                    Reference(SynonymRefType, "ast", DesignEntity(StmtType)),
                                    Reference(SynonymRefType, "pqlprocessor", DesignEntity(ProcedureType)));
    resTable.associateRelationships(parserDesignExtractorRelationships,
                                    Reference(SynonymRefType, "designextractor", DesignEntity(VariableType)),
                                    Reference(SynonymRefType, "parser", DesignEntity(StmtType)));
    resTable.associateRelationships(astParserRelationships, Reference(SynonymRefType, "ast", DesignEntity(StmtType)),
                                    Reference(SynonymRefType, "parser", DesignEntity(StmtType)));

    SECTION("checkIfHaveRelationships returns true for related synonyms, false for unrelated synonyms")
    {
        REQUIRE(resTable.checkIfHaveRelationships("ast", "parser"));
        REQUIRE(resTable.checkIfHaveRelationships("ast", "pqlprocessor"));
        REQUIRE(resTable.checkIfHaveRelationships("parser", "designextractor"));
        REQUIRE(resTable.checkIfHaveRelationships("swampert", "mudkip"));

        REQUIRE_FALSE(resTable.checkIfHaveRelationships("ast", "designextractor"));
        REQUIRE_FALSE(resTable.checkIfHaveRelationships("swampert", "ast"));
        REQUIRE_FALSE(resTable.checkIfHaveRelationships("mudkip", "parser"));
        REQUIRE_FALSE(resTable.checkIfHaveRelationships("parser", "pqlprocessor"));
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

        doVectorsHaveSameElementsVoid(resTable.getRelationships("ast", "parser"), astParserRelationshipsString);
        doVectorsHaveSameElementsVoid(resTable.getRelationships("ast", "pqlprocessor"),
                                      astPqlProcessorRelationshipsString);
        doVectorsHaveSameElementsVoid(resTable.getRelationships("parser", "designextractor"),
                                      designExtractorParserRelationshipsExpected);
        doVectorsHaveSameElementsVoid(resTable.getRelationships("swampert", "mudkip"),
                                      swampertMudkipRelationshipsExpected);
    }
}
