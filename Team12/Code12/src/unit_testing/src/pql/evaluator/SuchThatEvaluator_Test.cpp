/**
 * Black-box tests for the evaluation of such that
 * clauses in Query Evaluator.
 */
#include "EvaluatorTestingUtils.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "pql/evaluator/relationships/FollowsEvaluator.h"
#include "pql/evaluator/relationships/ModifiesEvaluator.h"
#include "pql/evaluator/relationships/NextEvaluator.h"
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
        requireVectorsHaveSameElements(resTable.getResultsOne("s"), std::vector<std::string>({"4"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"5"}));
        requireVectorsHaveSameElements(resTable.getResultsTwo("a", "s"),
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
        requireVectorsHaveSameElements(resTable.getResultsOne("s"),
                                       std::vector<std::string>({"1", "2", "3", "4", "5", "6"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, false, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"5"}));
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
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"5"}));
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
        requireVectorsHaveSameElements(resTable.getResultsOne("s"), std::vector<std::string>({"1", "2", "3", "4"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"5"}));
        requireVectorsHaveSameElements(
            resTable.getResultsTwo("a", "s"),
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
        requireVectorsHaveSameElements(resTable.getResultsOne("s"),
                                       std::vector<std::string>({"1", "2", "3", "4", "5", "6"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "a", DesignEntity(AssignType));
        evaluateFollowsClause(leftRef, rightRef, true, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"5"}));
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
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"1", "5"}));
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
        requireVectorsHaveSameElements(resTable.getResultsOne("s"), std::vector<std::string>({"4", "9"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("i"), std::vector<std::string>({"10", "7"}));
        requireVectorsHaveSameElements(resTable.getResultsTwo("i", "s"),
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
        requireVectorsHaveSameElements(resTable.getResultsOne("s"), std::vector<std::string>({"1", "4", "9", "10"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "i", DesignEntity(IfType));
        evaluateParentClause(leftRef, rightRef, false, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("i"), std::vector<std::string>({"10", "7"}));
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
        requireVectorsHaveSameElements(resTable.getResultsOne("i"), std::vector<std::string>({"4"}));
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
        requireVectorsHaveSameElements(resTable.getResultsOne("i"), std::vector<std::string>({"10", "16"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"14", "20", "22"}));
        requireVectorsHaveSameElements(resTable.getResultsTwo("i", "a"),
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
        requireVectorsHaveSameElements(resTable.getResultsOne("w"), std::vector<std::string>({"1", "15", "21", "6"}));
    }

    SECTION("Left wildcard, right synonym")
    {
        Reference leftRef(WildcardRefType, "_");
        Reference rightRef(SynonymRefType, "i", DesignEntity(IfType));
        evaluateParentClause(leftRef, rightRef, true, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("i"), std::vector<std::string>({"10", "16"}));
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
        requireVectorsHaveSameElements(resTable.getResultsOne("i"), std::vector<std::string>({"10"}));
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
    addModifiesRelationships(1, AssignmentStatement, std::vector<std::string>{"x"});
    addModifiesRelationships(2, ReadStatement, std::vector<std::string>{"y"});
    addModifiesRelationships(3, IfStatement, std::vector<std::string>{"z", "x"});
    addModifiesRelationships(4, AssignmentStatement, std::vector<std::string>{"z"});
    addModifiesRelationships(5, ReadStatement, std::vector<std::string>{"x"});
    addModifiesRelationships("proc1", std::vector<std::string>{"x", "y", "z"});
    addModifiesRelationships("proc2", std::vector<std::string>{"b", "c", "d"});
    addModifiesRelationships(6, CallStatement, std::vector<std::string>{"b", "c", "d"});
    addModifiesRelationships(7, ReadStatement, std::vector<std::string>{"c"});
    addModifiesRelationships(8, WhileStatement, std::vector<std::string>{"b", "d"});
    addModifiesRelationships(9, AssignmentStatement, std::vector<std::string>{"b"});
    addModifiesRelationships(10, ReadStatement, std::vector<std::string>{"d"});
    DeclarationTable declTable{};
    DesignEntity procDesignEntity(ProcedureType);
    DesignEntity ifDesignEntity(IfType);
    DesignEntity whileDesignEntity(WhileType);
    DesignEntity variableDesignEntity(VariableType);
    declTable.addDeclaration("p", procDesignEntity);
    declTable.addDeclaration("i", ifDesignEntity);
    declTable.addDeclaration("w", whileDesignEntity);
    declTable.addDeclaration("v", variableDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Both variables (procedure)")
    {
        Reference leftRef(SynonymRefType, "p", DesignEntity(ProcedureType));
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("p"), std::vector<std::string>({"proc1", "proc2"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("v"),
                                       std::vector<std::string>({"x", "y", "z", "b", "c", "d"}));
        requireVectorsHaveSameElements(
            resTable.getResultsTwo("v", "p"),
            std::vector<std::pair<std::string, std::string>>(
                {{"x", "proc1"}, {"y", "proc1"}, {"z", "proc1"}, {"d", "proc2"}, {"b", "proc2"}, {"c", "proc2"}}));
    }

    SECTION("Both variables (statement)")
    {
        Reference leftRef(SynonymRefType, "w", DesignEntity(WhileType));
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("w"), std::vector<std::string>({"8"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("v"), std::vector<std::string>({"b", "d"}));
        requireVectorsHaveSameElements(resTable.getResultsTwo("w", "v"),
                                       std::vector<std::pair<std::string, std::string>>({{"8", "b"}, {"8", "d"}}));
    }

    SECTION("Left procedure synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "p", DesignEntity(ProcedureType));
        Reference rightRef(WildcardRefType, "_");
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("p"), std::vector<std::string>({"proc1", "proc2"}));
    }

    SECTION("Left statement synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "i", DesignEntity(IfType));
        Reference rightRef(WildcardRefType, "_");
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("i"), std::vector<std::string>({"3"}));
    }

    SECTION("Left statement synonym, right known")
    {
        Reference leftRef(SynonymRefType, "i", DesignEntity(IfType));
        Reference rightRef(LiteralRefType, "b");
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }

    SECTION("Left procedure synonym, right known")
    {
        Reference leftRef(SynonymRefType, "p", DesignEntity(ProcedureType));
        Reference rightRef(LiteralRefType, "b");
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("p"), std::vector<std::string>({"proc2"}));
    }

    SECTION("Left statement known, right synonym")
    {
        Reference leftRef(IntegerRefType, "4");
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("v"), std::vector<std::string>({"z"}));
    }

    SECTION("Left procedure known, right synonym")
    {
        Reference leftRef(LiteralRefType, "proc1");
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("v"), std::vector<std::string>({"z", "x", "y"}));
    }

    SECTION("Both known (procedure)")
    {
        Reference leftRef(LiteralRefType, "proc1");
        Reference rightRef(LiteralRefType, "c");
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }

    SECTION("Both known (statement)")
    {
        Reference leftRef(IntegerRefType, "6");
        Reference rightRef(LiteralRefType, "c");
        evaluateModifiesClause(leftRef, rightRef, &resTable);
        REQUIRE(resTable.hasResults());
    }
}

TEST_CASE("Uses clauses are evaluated correctly")
{
    resetPKB();
    addUsesRelationships(1, AssignmentStatement, std::vector<std::string>{"x"});
    addUsesRelationships(2, PrintStatement, std::vector<std::string>{"y"});
    addUsesRelationships(3, IfStatement, std::vector<std::string>{"z", "x", "v", "u"});
    addUsesRelationships(4, AssignmentStatement, std::vector<std::string>{"z", "v", "u"});
    addUsesRelationships(5, PrintStatement, std::vector<std::string>{"x"});
    addUsesRelationships("proc1", std::vector<std::string>{"x", "y", "z", "v", "u"});
    addUsesRelationships("proc2", std::vector<std::string>{"b", "c", "d", "a", "f"});
    addUsesRelationships(6, CallStatement, std::vector<std::string>{"b", "c", "d", "a", "f"});
    addUsesRelationships(7, PrintStatement, std::vector<std::string>{"c"});
    addUsesRelationships(8, WhileStatement, std::vector<std::string>{"b", "d", "a", "f"});
    addUsesRelationships(9, AssignmentStatement, std::vector<std::string>{"b", "a", "f"});
    addUsesRelationships(10, PrintStatement, std::vector<std::string>{"d"});
    DeclarationTable declTable{};
    DesignEntity procDesignEntity(ProcedureType);
    DesignEntity assignDesignEntity(AssignType);
    DesignEntity variableDesignEntity(VariableType);
    declTable.addDeclaration("p", procDesignEntity);
    declTable.addDeclaration("a", assignDesignEntity);
    declTable.addDeclaration("v", variableDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Both variables (procedure)")
    {
        Reference leftRef(SynonymRefType, "p", DesignEntity(ProcedureType));
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("p"), std::vector<std::string>({"proc1", "proc2"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("v"),
                                       std::vector<std::string>({"x", "y", "z", "b", "c", "d", "v", "u", "a", "f"}));
        requireVectorsHaveSameElements(resTable.getResultsTwo("v", "p"),
                                       std::vector<std::pair<std::string, std::string>>({{"x", "proc1"},
                                                                                         {"y", "proc1"},
                                                                                         {"z", "proc1"},
                                                                                         {"d", "proc2"},
                                                                                         {"b", "proc2"},
                                                                                         {"c", "proc2"},
                                                                                         {"v", "proc1"},
                                                                                         {"u", "proc1"},
                                                                                         {"a", "proc2"},
                                                                                         {"f", "proc2"}}));
    }

    SECTION("Both variables (statement)")
    {
        Reference leftRef(SynonymRefType, "a", DesignEntity(AssignType));
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"1", "4", "9"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("v"),
                                       std::vector<std::string>({"b", "z", "v", "u", "a", "f", "x"}));
        requireVectorsHaveSameElements(
            resTable.getResultsTwo("a", "v"),
            std::vector<std::pair<std::string, std::string>>(
                {{"1", "x"}, {"4", "z"}, {"4", "v"}, {"4", "u"}, {"9", "b"}, {"9", "a"}, {"9", "f"}}));
    }

    SECTION("Left procedure synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "p", DesignEntity(ProcedureType));
        Reference rightRef(WildcardRefType, "_");
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("p"), std::vector<std::string>({"proc1", "proc2"}));
    }

    SECTION("Left statement synonym, right wildcard")
    {
        Reference leftRef(SynonymRefType, "a", DesignEntity(AssignType));
        Reference rightRef(WildcardRefType, "_");
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"1", "4", "9"}));
    }

    SECTION("Left statement synonym, right known")
    {
        Reference leftRef(SynonymRefType, "a", DesignEntity(AssignType));
        Reference rightRef(LiteralRefType, "b");
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("a"), std::vector<std::string>({"9"}));
    }

    SECTION("Left procedure synonym, right known")
    {
        Reference leftRef(SynonymRefType, "p", DesignEntity(ProcedureType));
        Reference rightRef(LiteralRefType, "u");
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("p"), std::vector<std::string>({"proc1"}));
    }

    SECTION("Left statement known, right synonym")
    {
        Reference leftRef(IntegerRefType, "4");
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("v"), std::vector<std::string>({"z", "v", "u"}));
    }

    SECTION("Left procedure known, right synonym")
    {
        Reference leftRef(LiteralRefType, "proc2");
        Reference rightRef(SynonymRefType, "v", DesignEntity(VariableType));
        evaluateUsesClause(leftRef, rightRef, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("v"),
                                       std::vector<std::string>({"b", "c", "d", "a", "f"}));
    }

    SECTION("Both known (procedure)")
    {
        Reference leftRef(LiteralRefType, "proc2");
        Reference rightRef(LiteralRefType, "x");
        evaluateUsesClause(leftRef, rightRef, &resTable);
        REQUIRE_FALSE(resTable.hasResults());
    }

    SECTION("Both known (statement)")
    {
        Reference leftRef(IntegerRefType, "6");
        Reference rightRef(LiteralRefType, "f");
        evaluateUsesClause(leftRef, rightRef, &resTable);
        REQUIRE(resTable.hasResults());
    }
}

TEST_CASE("Next* clauses are evaluated correctly")
{
    /*
            1. read x;
            2. print x;
            3. if (x > 2) {
            4.    x = 5;
                } else {
            5.    x = 6;}
            6. while (x < 0) {
            7.    read y;
            8.    x = x - y;}
            9. Dummy Node
     */

    resetPKB();

    insertIntoStatementTable(1, ReadStatement);
    insertIntoStatementTable(2, PrintStatement);
    insertIntoStatementTable(3, IfStatement);
    insertIntoStatementTable(4, AssignmentStatement);
    insertIntoStatementTable(5, AssignmentStatement);
    insertIntoStatementTable(6, WhileStatement);
    insertIntoStatementTable(7, ReadStatement);
    insertIntoStatementTable(8, AssignmentStatement);

    addNextRelationships(1, ReadStatement, 2, PrintStatement);
    addNextRelationships(2, PrintStatement, 3, IfStatement);
    addNextRelationships(3, IfStatement, 4, AssignmentStatement);
    addNextRelationships(3, IfStatement, 5, AssignmentStatement);
    addNextRelationships(4, AssignmentStatement, 6, WhileStatement);
    addNextRelationships(5, AssignmentStatement, 6, WhileStatement);
    addNextRelationships(6, WhileStatement, 7, ReadStatement);
    addNextRelationships(7, ReadStatement, 8, AssignmentStatement);
    addNextRelationships(8, AssignmentStatement, 6, WhileStatement);

    DeclarationTable declTable{};
    DesignEntity readDesignEntity(ReadType);
    DesignEntity printDesignEntity(PrintType);
    DesignEntity assignDesignEntity(AssignType);
    DesignEntity stmtDesignEntity(StmtType);
    DesignEntity whileDesignEntity(WhileType);
    DesignEntity ifDesignEntity(IfType);
    declTable.addDeclaration("re", readDesignEntity);
    declTable.addDeclaration("pn", printDesignEntity);
    declTable.addDeclaration("a", assignDesignEntity);
    declTable.addDeclaration("s", stmtDesignEntity);
    declTable.addDeclaration("w", whileDesignEntity);
    declTable.addDeclaration("if", ifDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Left integer, right synonym")
    {
        SECTION("Right synonym (statement)")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(SynonymRefType, "s", stmtDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("s"),
                                           Vector<String>({"2", "3", "4", "5", "6", "7", "8"}));
        }

        SECTION("Right synonym (read)")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(SynonymRefType, "re", readDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("re"), Vector<String>({"7"}));
        }

        SECTION("Right synonym (print)")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(SynonymRefType, "pn", printDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("pn"), Vector<String>({"2"}));
        }

        SECTION("Right synonym (assign)")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(SynonymRefType, "a", assignDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("a"), Vector<String>({"4", "5", "8"}));
        }

        SECTION("Right synonym (while)")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(SynonymRefType, "w", whileDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("w"), Vector<String>({"6"}));
        }

        SECTION("Right synonym (if)")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(SynonymRefType, "ifs", ifDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("ifs"), Vector<String>({"3"}));
        }
    }

    SECTION("Left synonym, right integer)")
    {
        SECTION("Left synonym (statement)")
        {
            Reference leftRef(SynonymRefType, "s", stmtDesignEntity);
            Reference rightRef(IntegerRefType, "8");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("s"),
                                           Vector<String>({"1", "2", "3", "4", "5", "6", "7", "8"}));
        }

        SECTION("Left synonym (read)")
        {
            Reference leftRef(SynonymRefType, "re", readDesignEntity);
            Reference rightRef(IntegerRefType, "8");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("re"), Vector<String>({"1", "7"}));
        }

        SECTION("Left synonym (print)")
        {
            Reference leftRef(SynonymRefType, "pn", printDesignEntity);
            Reference rightRef(IntegerRefType, "8");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("pn"), Vector<String>({"2"}));
        }

        SECTION("Left synonym (assign)")
        {
            Reference leftRef(SynonymRefType, "a", assignDesignEntity);
            Reference rightRef(IntegerRefType, "8");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("a"), Vector<String>({"4", "5", "8"}));
        }

        SECTION("Left synonym (while)")
        {
            Reference leftRef(SynonymRefType, "w", whileDesignEntity);
            Reference rightRef(IntegerRefType, "8");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("w"), Vector<String>({"6"}));
        }

        SECTION("Left synonym (if)")
        {
            Reference leftRef(SynonymRefType, "ifs", ifDesignEntity);
            Reference rightRef(IntegerRefType, "8");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("ifs"), Vector<String>({"3"}));
        }
    }

    SECTION("Left integer, right integer")
    {
        SECTION("Valid left integer, right integer")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(IntegerRefType, "6");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            REQUIRE(resTable.hasResults());
        }

        SECTION("Left integer greater than right integer")
        {
            Reference leftRef(IntegerRefType, "7");
            Reference rightRef(IntegerRefType, "3");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            REQUIRE(!resTable.hasResults());
        }

        SECTION("Left integer, right integer greater than max statement number")
        {
            Reference leftRef(IntegerRefType, "1");
            Reference rightRef(IntegerRefType, "9");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            REQUIRE(!resTable.hasResults());
        }
    }

    SECTION("Both any references")
    {
        // Test case for both wildcards
        SECTION("Both are wildcards")
        {
            Reference leftRef(WildcardRefType, "_");
            Reference rightRef(WildcardRefType, "_");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            REQUIRE(resTable.hasResults());
        }

        // Test case for left wildcard, right synonym
        SECTION("Left is wildcard, right is synonym (statement)")
        {
            Reference leftRef(WildcardRefType, "_");
            Reference rightRef(SynonymRefType, "s", stmtDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("s"),
                                           Vector<String>({"2", "3", "4", "5", "6", "7", "8"}));
        }

        SECTION("Left is wildcard, right is synonym (read)")
        {
            Reference leftRef(WildcardRefType, "_");
            Reference rightRef(SynonymRefType, "re", readDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("re"), Vector<String>({"7"}));
        }

        SECTION("Left is wildcard, right is synonym (print)")
        {
            Reference leftRef(WildcardRefType, "_");
            Reference rightRef(SynonymRefType, "pn", printDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("pn"), Vector<String>({"2"}));
        }

        SECTION("Left is wildcard, right is synonym (assign)")
        {
            Reference leftRef(WildcardRefType, "_");
            Reference rightRef(SynonymRefType, "a", assignDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("a"), Vector<String>({"4", "5", "8"}));
        }

        SECTION("Left is wildcard, right is synonym (while)")
        {
            Reference leftRef(WildcardRefType, "_");
            Reference rightRef(SynonymRefType, "w", whileDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("w"), Vector<String>({"6"}));
        }

        SECTION("Left is wildcard, right is synonym (if)")
        {
            Reference leftRef(WildcardRefType, "_");
            Reference rightRef(SynonymRefType, "ifs", ifDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("ifs"), Vector<String>({"3"}));
        }

        // Test case for left synonym, right wildcard
        SECTION("Left is synonym (statement), right is wildcard")
        {
            Reference leftRef(SynonymRefType, "s", stmtDesignEntity);
            Reference rightRef(WildcardRefType, "_");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("s"),
                                           Vector<String>({"1", "2", "3", "4", "5", "6", "7", "8"}));
        }

        SECTION("Left is synonym (read), right is wildcard")
        {
            Reference leftRef(SynonymRefType, "re", readDesignEntity);
            Reference rightRef(WildcardRefType, "_");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("re"), Vector<String>({"1", "7"}));
        }

        SECTION("Left is synonym (print), right is wildcard")
        {
            Reference leftRef(SynonymRefType, "pn", printDesignEntity);
            Reference rightRef(WildcardRefType, "_");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("pn"), Vector<String>({"2"}));
        }

        SECTION("Left is synonym (assign), right is wildcard")
        {
            Reference leftRef(SynonymRefType, "a", assignDesignEntity);
            Reference rightRef(WildcardRefType, "_");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("a"), Vector<String>({"4", "5", "8"}));
        }

        SECTION("Left is synonym (while), right is wildcard")
        {
            Reference leftRef(SynonymRefType, "w", whileDesignEntity);
            Reference rightRef(WildcardRefType, "_");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("w"), Vector<String>({"6"}));
        }

        SECTION("Left is synonym (if), right is wildcard")
        {
            Reference leftRef(SynonymRefType, "ifs", ifDesignEntity);
            Reference rightRef(WildcardRefType, "_");
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("ifs"), Vector<String>({"3"}));
        }

        // Test cases for both synonyms
        SECTION("Left is synonym (read), right is synonym (assign)")
        {
            Reference leftRef(SynonymRefType, "re", readDesignEntity);
            Reference rightRef(SynonymRefType, "a", assignDesignEntity);
            NextEvaluator(resTable).evaluateNextStarClause(leftRef, rightRef);
            requireVectorsHaveSameElements(resTable.getResultsOne("re"), Vector<String>({"1", "7"}));
            requireVectorsHaveSameElements(resTable.getResultsOne("a"), Vector<String>({"4", "5", "8"}));
            requireVectorsHaveSameElements(
                resTable.getResultsTwo("re", "a"),
                Vector<Pair<String, String>>({{"1", "4"}, {"1", "5"}, {"1", "8"}, {"7", "8"}}));
        }
    }
}
