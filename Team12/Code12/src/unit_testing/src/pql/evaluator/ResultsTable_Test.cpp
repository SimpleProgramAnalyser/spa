/**
 * Unit tests for the ResultsTable object in
 * Query Evaluator, which holds results of
 * evaluating PQL clauses.
 *
 * A combination of black-box tests and white-box tests.
 */
#include "catch.hpp"
#include "pkb/PKB.h"
#include "pql/evaluator/ResultsTable.h"

/**
 * A method to compare two vectors, to see whether
 * they have the same elements regardless of order.
 *
 * @tparam T Type of elements in the vector. Elements
 *           must implement the method
 *           bool cmp(const T&, const T&).
 */
template <typename T>
void compareVectorsNoOrder(std::vector<T> vector1, std::vector<T> vector2)
{
    std::sort(vector1.begin(), vector1.end());
    std::sort(vector2.begin(), vector2.end());

    REQUIRE(vector1 == vector2);
}

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
    compareVectorsNoOrder(resTable.get("efgh"), expectedResultsEfgh);
    compareVectorsNoOrder(resTable.get("abcd"), expectedResultsAbcd);
}

TEST_CASE("ResultsTable filters results correctly")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> originalResults{"1", "3", "7", "539", "23", "134", "143", "9"};
    std::vector<std::string> newResults{"112", "18397", "3", "0", "143", "539"};
    std::vector<std::string> expectedResults{"3", "143", "539"};
    resTable.filterTable("abcd", originalResults);
    resTable.filterTable("abcd", newResults);

    compareVectorsNoOrder(resTable.get("abcd"), expectedResults);
}

TEST_CASE("ResultsTable filters results correctly for synonym reference")
{

}

TEST_CASE("ResultsTable does not filter any results correctly for non-synonym reference")
{

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

    compareVectorsNoOrder(abcdResults, std::vector<std::string>({"1", "2", "3", "4", "5"}));
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

TEST_CASE("checkIfRelated returns true for related synonyms") {

}

TEST_CASE("checkIfRelated returns false for unrelated synonyms") {

}

TEST_CASE("getTypeOfSynonym gets the type stored in declarations table successfully") {

}

TEST_CASE("getTypeOfSynonym returns NonExistentType if synonym was not found in declarations table") {

}


TEST_CASE("eliminatePotentialValue updates the ResultsTable successfully for synonym found in table") {

}

TEST_CASE("eliminatePotentialValue does not do anything for synonym not found") {

}

TEST_CASE("Relationships are stored properly in ResultsTable") {

}

TEST_CASE("checkIfHaveRelationships returns true when relationships are present") {

}

TEST_CASE("checkIfHaveRelationships returns false when relationships are absent") {

}

TEST_CASE("getRelationships retrieves all relationships in table") {

}
