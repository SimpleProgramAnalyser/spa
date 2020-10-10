/*
 * Black-box tests for the public methods in Evaluator.cpp
 * (under pql/evaluator), also making use of PKB methods.
 * Because Evaluator relies on the Program Knowledge Base
 * heavily, independent unit testing is difficult to perform.
 *
 * Hence, most testing of the Evaluator is done by the
 * integration tests between Evaluator and PKB.
 */

#include "../../ast_utils/AstUtils.h"
#include "../parser/AbstractQueryBuilder.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "pql/evaluator/Evaluator.h"

std::string getErrorMessage()
{
    return "ERROR CODE 3735929054: PQL was not parsed. SIGSYNTAX obtained. This incident will be reported.";
}

TEST_CASE("Evaluator::evaluateQuery(AbstractQuery query) -> invalid (syntatically) PQL query returns empty")
{
    // === Test set-up ===
    AbstractQuery abstractQuery = AbstractQuery::invalidAbstractQuery();

    // === Execute test method ===
    RawQueryResult rawQueryResult = evaluateQuery(abstractQuery);

    // === Expected test results ===
    RawQueryResult expectedRawQueryResult = RawQueryResult::getSyntaxError(getErrorMessage());

    REQUIRE(rawQueryResult == expectedRawQueryResult);
}

TEST_CASE("Evaluator::evaluateQuery(AbstractQuery query) -> vacuously true PQL query returns all matches")
{
    // === Test set-up ===
    resetPKB();
    insertIntoStatementTable(23, ReadStatement);
    insertIntoStatementTable(24, PrintStatement);
    insertIntoStatementTable(29, WhileStatement);
    addModifiesRelationships(12, AssignmentStatement, std::vector<std::string>{"creeper"});
    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addDeclaration("awman", "stmt")
              .addSuchThatClause(ModifiesType, SynonymRefType, "awman", StmtType, WildcardRefType, "_", NonExistentType)
              .build();
    std::vector<std::string> expectedResults = {"23", "24", "29"};

    // === Execute test method ===
    RawQueryResult rawQueryResult = evaluateQuery(expectedAbstractQuery);

    // === Expected test results ===
    RawQueryResult expectedRawQueryResult{expectedResults};

    REQUIRE(rawQueryResult == expectedRawQueryResult);
}

TEST_CASE(
    "Evaluator::evaluateQuery(AbstractQuery query) -> valid query examples (multiple clauses) returns valid results")
{
    // === Test set-up ===
    resetPKB();
    assignRootNode(getProgram20Tree_multipleProceduresSpheresdf());
    for (int i = 110; i < 141; i++) {
        insertIntoStatementTable(i, WhileStatement);
    }
    for (int i = 10; i < 41; i++) {
        insertIntoStatementTable(i, AssignmentStatement);
    }
    addParentRelationships(123, WhileStatement, 13, AssignmentStatement);
    addParentRelationships(129, WhileStatement, 21, AssignmentStatement);
    addParentRelationships(124, WhileStatement, 16, AssignmentStatement);
    addParentRelationships(125, WhileStatement, 4, AssignmentStatement);
    addParentRelationships(122, WhileStatement, 5, AssignmentStatement);
    addParentRelationships(126, WhileStatement, 9, AssignmentStatement);
    addParentRelationships(127, WhileStatement, 14, AssignmentStatement);
    addParentRelationships(128, WhileStatement, 15, AssignmentStatement);
    addParentRelationships(121, WhileStatement, 17, AssignmentStatement);
    addParentRelationships(120, WhileStatement, 20, AssignmentStatement);
    addParentRelationships(130, WhileStatement, 22, AssignmentStatement);
    addParentRelationships(119, WhileStatement, 23, AssignmentStatement);
    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("diamond")
              .addDeclaration("diamond", "stmt")
              .addDeclaration("netherite", "assign")
              .addDeclaration("diamond", "stmt")
              .addSuchThatClause(ParentType, SynonymRefType, "diamond", StmtType, SynonymRefType, "netherite",
                                 AssignType)
              .addAssignPatternClause("netherite", AssignPatternType, WildcardRefType, "_", NonExistentType, "dist",
                                      ExtendableLiteralExpressionType)
              .build();
    // reverse the order of the clauses
    AbstractQuery reverseAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("diamond")
              .addDeclaration("diamond", "stmt")
              .addDeclaration("netherite", "assign")
              .addDeclaration("diamond", "stmt")
              .addAssignPatternClause("netherite", AssignPatternType, WildcardRefType, "_", NonExistentType, "dist",
                                      ExtendableLiteralExpressionType)
              .addSuchThatClause(ParentType, SynonymRefType, "diamond", StmtType, SynonymRefType, "netherite", AssignType)
              .build();
    std::vector<std::string> expectedResults = {"123", "124", "129"};

    // === Execute test method ===
    RawQueryResult rawQueryResult = evaluateQuery(expectedAbstractQuery);
    RawQueryResult rawQueryResult2 = evaluateQuery(reverseAbstractQuery);

    // === Expected test results ===
    RawQueryResult expectedRawQueryResult{expectedResults};

    REQUIRE(rawQueryResult == expectedRawQueryResult);
    REQUIRE(rawQueryResult2 == expectedRawQueryResult);
}
