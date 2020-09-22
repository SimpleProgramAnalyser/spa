/**
 * Unit tests for the ResultsTable object in
 * Query Evaluator, which holds results of
 * evaluating PQL clauses.
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
    ResultsTable resTable(DeclarationTable{});
    resTable.filterTable("abcd", std::vector<std::string>());
    REQUIRE(!resTable.hasResults());
    REQUIRE(resTable.get("abcd").empty());
    REQUIRE(resTable.get("efgh").empty());
}

TEST_CASE("ResultsTable stores results correctly")
{
    ResultsTable resTable(DeclarationTable{});
    std::vector<std::string> expectedResults{"7", "9", "3", "10", "143", "539"};
    resTable.filterTable("abcd", expectedResults);
    compareVectorsNoOrder(resTable.get("abcd"), expectedResults);
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

TEST_CASE("ResultsTable retrieves results from PKB correctly, if no elements found")
{
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

TEST_CASE("checkIfSynonymHasConstraints returns true for synonym with no constraints, and returns false for synonym "
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
