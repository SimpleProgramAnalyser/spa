/**
 * Black-box tests for the evaluation of such that
 * clauses in Query Evaluator.
 */
#include "EvaluatorTestingUtils.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "pql/evaluator/relationships/FollowsEvaluator.h"
#include "pql/evaluator/relationships/ModifiesEvaluator.h"
#include "pql/evaluator/relationships/ParentEvaluator.h"
#include "pql/evaluator/relationships/UsesEvaluator.h"

TEST_CASE("Follows clauses are evaluated correctly")
{
    resetPKB();
    addFollowsRelationships(1, AssignmentStatement, 2, ReadStatement);
    addFollowsRelationships(2, ReadStatement, 3, PrintStatement);
    addFollowsRelationships(3, PrintStatement, 4, CallStatement);
    addFollowsRelationships(4, CallStatement, 5, AssignmentStatement);
    addFollowsRelationships(5, AssignmentStatement, 6, WhileStatement);
    addFollowsRelationships(6, WhileStatement, 7, IfStatement);
    DeclarationTable declTable{};
    DesignEntity assignDesignEntity(AssignType);
    DesignEntity statementDesignEntity(StmtType);
    declTable.addDeclaration("a", assignDesignEntity);
    declTable.addDeclaration("s", statementDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Both variables")
    {
        Reference leftRef(SynonymRefType, "s", DesignEntity(StmtType));
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("s"), std::vector<std::string>({"4"}));
        doVectorsHaveSameElementsVoid(resTable.get("a"), std::vector<std::string>({"5"}));
        doVectorsHaveSameElementsVoid(resTable.getRelationships("a", "s"),
                                      std::vector<std::pair<std::string, std::string>>({{"5", "4"}}));
    }

    SECTION("Both wildcards")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(WildcardRefType, "_");
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        REQUIRE(resTable.hasResults());
    }

    SECTION("Left synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "s", DesignEntity(StmtType));
        Reference rightRef(WildcardRefType, "_");
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("s"), std::vector<std::string>({"1", "2", "3", "4", "5", "6"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("a"), std::vector<std::string>({"5"}));
    }

    SECTION("Left known, right variable")
    {
        Reference leftRef(IntegerRefType, "5");
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }

    SECTION("Left variable, right known")
    {
        Reference leftRef(SynonymRefType, "a", DesignEntity(AssignType));
        Reference rightRef(IntegerRefType, "6");
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("a"), std::vector<std::string>({"5"}));
    }

    SECTION("Both known")
    {
        Reference leftRef(IntegerRefType, "5");
        Reference rightRef(IntegerRefType, "7");
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }
}

TEST_CASE("Follows star clauses are evaluated correctly")
{
    resetPKB();
    addFollowsRelationshipsStar(1, AssignmentStatement,
                                std::vector<std::pair<int, StatementType>>({{2, ReadStatement},
                                                                            {3, PrintStatement},
                                                                            {4, CallStatement},
                                                                            {5, AssignmentStatement},
                                                                            {6, WhileStatement},
                                                                            {7, IfStatement}}));
    addFollowsRelationshipsStar(2, ReadStatement,
                                std::vector<std::pair<int, StatementType>>({{3, PrintStatement},
                                                                            {4, CallStatement},
                                                                            {5, AssignmentStatement},
                                                                            {6, WhileStatement},
                                                                            {7, IfStatement}}));
    addFollowsRelationshipsStar(
        3, PrintStatement,
        std::vector<std::pair<int, StatementType>>(
            {{4, CallStatement}, {5, AssignmentStatement}, {6, WhileStatement}, {7, IfStatement}}));
    addFollowsRelationshipsStar(
        4, CallStatement,
        std::vector<std::pair<int, StatementType>>({{5, AssignmentStatement}, {6, WhileStatement}, {7, IfStatement}}));
    addFollowsRelationshipsStar(5, AssignmentStatement,
                                std::vector<std::pair<int, StatementType>>({{6, WhileStatement}, {7, IfStatement}}));
    addFollowsRelationshipsStar(6, WhileStatement, std::vector<std::pair<int, StatementType>>({{7, IfStatement}}));
    DeclarationTable declTable{};
    DesignEntity assignDesignEntity(AssignType);
    DesignEntity statementDesignEntity(StmtType);
    declTable.addDeclaration("a", assignDesignEntity);
    declTable.addDeclaration("s", statementDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Both variables")
    {
        Reference leftRef(SynonymRefType, "s", DesignEntity(StmtType));
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("s"), std::vector<std::string>({"1", "2", "3", "4"}));
        doVectorsHaveSameElementsVoid(resTable.get("a"), std::vector<std::string>({"5"}));
        doVectorsHaveSameElementsVoid(
            resTable.getRelationships("a", "s"),
            std::vector<std::pair<std::string, std::string>>({{"5", "4"}, {"5", "3"}, {"5", "2"}, {"5", "1"}}));
    }

    SECTION("Both wildcards")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(WildcardRefType, "_");
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        REQUIRE(resTable.hasResults());
    }

    SECTION("Left synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "s", DesignEntity(StmtType));
        Reference rightRef(WildcardRefType, "_");
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("s"), std::vector<std::string>({"1", "2", "3", "4", "5", "6"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("a"), std::vector<std::string>({"5"}));
    }

    SECTION("Left known, right variable")
    {
        Reference leftRef(IntegerRefType, "5");
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }

    SECTION("Left variable, right known")
    {
        Reference leftRef(SynonymRefType, "a", DesignEntity(AssignType));
        Reference rightRef(IntegerRefType, "6");
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("a"), std::vector<std::string>({"1", "5"}));
    }

    SECTION("Both known")
    {
        Reference leftRef(IntegerRefType, "5");
        Reference rightRef(IntegerRefType, "7");
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        REQUIRE(resTable.hasResults());
    }
}

TEST_CASE("Parent clauses are evaluated correctly")
{
    resetPKB();
    addParentRelationships(1, WhileStatement, 2, ReadStatement);
    addParentRelationships(1, WhileStatement, 3, PrintStatement);
    addParentRelationships(4, IfStatement, 5, AssignmentStatement);
    addParentRelationships(4, IfStatement, 6, WhileStatement);
    addParentRelationships(4, IfStatement, 7, IfStatement);
    addParentRelationships(4, IfStatement, 8, CallStatement);
    addParentRelationships(9, WhileStatement, 10, IfStatement);
    addParentRelationships(10, IfStatement, 11, WhileStatement);
    DeclarationTable declTable{};
    DesignEntity ifDesignEntity(IfType);
    DesignEntity statementDesignEntity(StmtType);
    declTable.addDeclaration("i", ifDesignEntity);
    declTable.addDeclaration("s", statementDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Both variables")
    {
        Reference leftRef(SynonymRefType, "s", DesignEntity(StmtType));
        Reference rightRef(SynonymRefType, "i", DesignEntity(AssignType));
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("s"), std::vector<std::string>({"4", "9"}));
        doVectorsHaveSameElementsVoid(resTable.get("i"), std::vector<std::string>({"10", "7"}));
        doVectorsHaveSameElementsVoid(resTable.getRelationships("i", "s"),
                                      std::vector<std::pair<std::string, std::string>>({{"10", "9"}, {"7", "4"}}));
    }

    SECTION("Both wildcards")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(WildcardRefType, "_");
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        REQUIRE(resTable.hasResults());
    }

    SECTION("Left synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "s", DesignEntity(StmtType));
        Reference rightRef(WildcardRefType, "_");
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("s"), std::vector<std::string>({"1", "4", "9", "10"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "i", DesignEntity(IfType));
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("i"), std::vector<std::string>({"10", "7"}));
    }

    SECTION("Left known, right variable")
    {
        Reference leftRef(IntegerRefType, "5");
        Reference rightRef(SynonymRefType, "i", DesignEntity(IfType));
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }

    SECTION("Left variable, right known")
    {
        Reference leftRef(SynonymRefType, "i", DesignEntity(IfType));
        Reference rightRef(IntegerRefType, "6");
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("i"), std::vector<std::string>({"4"}));
    }

    SECTION("Both known")
    {
        Reference leftRef(IntegerRefType, "4");
        Reference rightRef(IntegerRefType, "6");
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        REQUIRE(resTable.hasResults());
    }
}

TEST_CASE("Parent star clauses are evaluated correctly")
{
    resetPKB();
    addParentRelationshipsStar(
        1, WhileStatement,
        std::vector<std::pair<int, StatementType>>(
            {{2, ReadStatement},        {3, PrintStatement},  {4, CallStatement},        {5, AssignmentStatement},
             {6, WhileStatement},       {7, ReadStatement},   {8, PrintStatement},       {9, CallStatement},
             {10, IfStatement},         {11, ReadStatement},  {12, PrintStatement},      {13, CallStatement},
             {14, AssignmentStatement}, {15, WhileStatement}, {16, IfStatement},         {17, ReadStatement},
             {18, PrintStatement},      {19, CallStatement},  {20, AssignmentStatement}, {21, WhileStatement},
             {22, AssignmentStatement}}));
    addParentRelationshipsStar(
        6, WhileStatement,
        std::vector<std::pair<int, StatementType>>({{7, ReadStatement}, {8, PrintStatement}, {9, CallStatement}}));
    addParentRelationshipsStar(10, IfStatement,
                               std::vector<std::pair<int, StatementType>>({{11, ReadStatement},
                                                                           {12, PrintStatement},
                                                                           {13, CallStatement},
                                                                           {14, AssignmentStatement},
                                                                           {15, WhileStatement},
                                                                           {16, IfStatement},
                                                                           {17, ReadStatement},
                                                                           {18, PrintStatement},
                                                                           {19, CallStatement},
                                                                           {20, AssignmentStatement},
                                                                           {21, WhileStatement},
                                                                           {22, AssignmentStatement}}));
    addParentRelationshipsStar(15, WhileStatement,
                               std::vector<std::pair<int, StatementType>>({{16, IfStatement},
                                                                           {17, ReadStatement},
                                                                           {18, PrintStatement},
                                                                           {19, CallStatement},
                                                                           {20, AssignmentStatement},
                                                                           {21, WhileStatement},
                                                                           {22, AssignmentStatement}}));
    addParentRelationshipsStar(16, IfStatement,
                               std::vector<std::pair<int, StatementType>>({{17, ReadStatement},
                                                                           {18, PrintStatement},
                                                                           {19, CallStatement},
                                                                           {20, AssignmentStatement},
                                                                           {21, WhileStatement},
                                                                           {22, AssignmentStatement}}));
    addParentRelationshipsStar(21, WhileStatement,
                               std::vector<std::pair<int, StatementType>>({{22, AssignmentStatement}}));
    DeclarationTable declTable{};
    DesignEntity ifDesignEntity(IfType);
    DesignEntity assignDesignEntity(AssignType);
    DesignEntity whileDesignEntity(WhileType);
    declTable.addDeclaration("i", ifDesignEntity);
    declTable.addDeclaration("a", assignDesignEntity);
    declTable.addDeclaration("w", whileDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Both variables")
    {
        Reference leftRef(SynonymRefType, "i", DesignEntity(IfType));
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("i"), std::vector<std::string>({"10", "16"}));
        doVectorsHaveSameElementsVoid(resTable.get("a"), std::vector<std::string>({"14", "20", "22"}));
        doVectorsHaveSameElementsVoid(resTable.getRelationships("i", "a"),
                                      std::vector<std::pair<std::string, std::string>>(
                                          {{"10", "14"}, {"10", "20"}, {"10", "22"}, {"16", "20"}, {"16", "22"}}));
    }

    SECTION("Both wildcards")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(WildcardRefType, "_");
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        REQUIRE(resTable.hasResults());
    }

    SECTION("Left synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "w", DesignEntity(WhileType));
        Reference rightRef(WildcardRefType, "_");
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("w"), std::vector<std::string>({"1", "15", "21", "6"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "i", DesignEntity(IfType));
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("i"), std::vector<std::string>({"10", "16"}));
    }

    SECTION("Left known, right variable")
    {
        Reference leftRef(IntegerRefType, "5");
        Reference rightRef(SynonymRefType, "i", DesignEntity(IfType));
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }

    SECTION("Left variable, right known")
    {
        Reference leftRef(SynonymRefType, "i", DesignEntity(IfType));
        Reference rightRef(IntegerRefType, "16");
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("i"), std::vector<std::string>({"10"}));
    }

    SECTION("Both known")
    {
        Reference leftRef(IntegerRefType, "1");
        Reference rightRef(IntegerRefType, "22");
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        REQUIRE(resTable.hasResults());
    }
}

TEST_CASE("Modifies clauses are evaluated correctly")
{
    resetPKB();

    SECTION("Both variables") {}

    SECTION("Both wildcards") {}

    SECTION("Left known, right wildcard") {}

    SECTION("Left known, right variable") {}

    SECTION("Left wildcard, right known") {}

    SECTION("Left variable, right known") {}

    SECTION("Both known") {}
}

TEST_CASE("Uses clauses are evaluated correctly")
{
    resetPKB();

    SECTION("Both variables") {}

    SECTION("Both wildcards") {}

    SECTION("Left known, right wildcard") {}

    SECTION("Left known, right variable") {}

    SECTION("Left wildcard, right known") {}

    SECTION("Left variable, right known") {}

    SECTION("Both known") {}
}
