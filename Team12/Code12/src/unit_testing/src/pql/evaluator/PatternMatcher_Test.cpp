/**
 * Black-box tests for the evaluation of pattern
 * clauses in Query Evaluator.
 */
#include "../../ast_utils/AstUtils.h"
#include "../parser/AbstractQueryBuilder.h"
#include "EvaluatorTestingUtils.h"
#include "ast/AstLibrary.h"
#include "catch.hpp"
#include "pkb/PKB.h"
#include "pql/evaluator/pattern/PatternMatcher.h"

TEST_CASE("Assignment statement patterns are evaluated correctly")
{
    resetPKB();
    assignRootNode(getProgram20Tree_multipleProceduresSpheresdf());
    DeclarationTable declTable{};
    DesignEntity assignDesignEntity(AssignType);
    DesignEntity variableDesignEntity(VariableType);
    declTable.addDeclaration("assign", assignDesignEntity);
    declTable.addDeclaration("var", variableDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Two synonyms")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(SynonymRefType, "var"),
                              ExpressionSpec(createRefExpr("dist"), ExtendableLiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("assign"), std::vector<std::string>({"13", "16", "21"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("var"), std::vector<std::string>({"depth", "x"}));
        requireVectorsHaveSameElements(
            resTable.getResultsTwo("var", "assign"),
            std::vector<std::pair<std::string, std::string>>({{"x", "16"}, {"x", "21"}, {"depth", "13"}}));
    }

    SECTION("Two wildcards")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(WildcardRefType, "_"),
                              ExpressionSpec(WildcardExpressionType));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(
            resTable.getResultsOne("assign"),
            std::vector<std::string>({"4", "5", "9", "12", "13", "14", "15", "16", "17", "20", "21", "22", "23"}));
    }

    SECTION("Assigned to variable is wildcard")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(WildcardRefType, "_"),
                              ExpressionSpec(createRefExpr("dist"), ExtendableLiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("assign"), std::vector<std::string>({"13", "16", "21"}));
    }

    SECTION("Assigned to variable is string literal")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(LiteralRefType, "x"),
                              ExpressionSpec(createRefExpr("dist"), ExtendableLiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("assign"), std::vector<std::string>({"16", "21"}));
    }

    SECTION("Numeral literal expression")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(SynonymRefType, "var"),
                              ExpressionSpec(createRefExpr(19), ExtendableLiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("assign"), std::vector<std::string>({"5"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("var"), std::vector<std::string>({"rd"}));
    }

    SECTION("Arithmetic expression")
    {
        PatternClause clause1(
            "assign", AssignPatternType, Reference(SynonymRefType, "var"),
            ExpressionSpec(
                createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth"))),
                LiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("assign"), std::vector<std::string>({"9"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("var"), std::vector<std::string>({"po"}));
        requireVectorsHaveSameElements(resTable.getResultsTwo("var", "assign"),
                                       std::vector<std::pair<std::string, std::string>>({{"po", "9"}}));
    }
}

TEST_CASE("While statement patterns are evaluated correctly")
{
    resetPKB();
    assignRootNode(getProgram20Tree_multipleProceduresSpheresdf());
    DeclarationTable declTable{};
    DesignEntity whileDesignEntity(WhileType);
    DesignEntity variableDesignEntity(VariableType);
    declTable.addDeclaration("while", whileDesignEntity);
    declTable.addDeclaration("var", variableDesignEntity);
    ResultsTable resTable(declTable);

    SECTION("Wildcard")
    {
        PatternClause clause1("while", WhilePatternType, Reference(WildcardRefType, "_"));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("while"), std::vector<String>({"7", "19"}));
    }

    SECTION("Literal Variable")
    {
        PatternClause clause1("while", WhilePatternType, Reference(LiteralRefType, "count"));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("while"), std::vector<String>({"7"}));
    }

    SECTION("Synonym Variable")
    {
        PatternClause clause1("while", WhilePatternType, Reference(SynonymRefType, "var"));
        evaluatePattern(&clause1, &resTable);
        requireVectorsHaveSameElements(resTable.getResultsOne("while"), std::vector<String>({"7", "19"}));
        requireVectorsHaveSameElements(resTable.getResultsOne("var"),
                                       std::vector<String>({"count", "steps", "x", "p"}));
        requireVectorsHaveSameElements(resTable.getResultsTwo("while", "var"),
                                      std::vector<std::pair<std::string, std::string>>(
                                          {{"7", "count"}, {"7", "steps"}, {"19", "x"}, {"19", "p"}}));
    }
}

//TEST_CASE("If statement patterns are evaluated correctly")
//{
//    resetPKB();
//    assignRootNode(getProgram19Tree_multipleProcedures());
//    DeclarationTable declTable{};
//    DesignEntity ifDesignEntity(IfType);
//    DesignEntity variableDesignEntity(VariableType);
//    declTable.addDeclaration("if", ifDesignEntity);
//    declTable.addDeclaration("var", variableDesignEntity);
//    ResultsTable resTable(declTable);
//
//    SECTION("Wildcard")
//    {
//        PatternClause clause1("if", IfPatternType, Reference(WildcardRefType, "_"));
//        evaluatePattern(&clause1, &resTable);
//        requireVectorsHaveSameElements(resTable.getResultsOne("if"), std::vector<String>({"12", "16"}));
//    }
//
//    SECTION("Literal Variable")
//    {
//        PatternClause clause1("if", IfPatternType, Reference(LiteralRefType, "depth"));
//        evaluatePattern(&clause1, &resTable);
//        requireVectorsHaveSameElements(resTable.getResultsOne("if"), std::vector<String>({"16"}));
//    }
//
//    SECTION("Synonym Variable")
//    {
//        PatternClause clause1("if", IfPatternType, Reference(SynonymRefType, "var"));
//        evaluatePattern(&clause1, &resTable);
//        requireVectorsHaveSameElements(resTable.getResultsOne("if"), std::vector<String>({"12", "16"}));
//        requireVectorsHaveSameElements(resTable.getResultsOne("var"), std::vector<String>({"base", "depth"}));
//        requireVectorsHaveSameElements(
//            resTable.getResultsTwo("var", "if"),
//            std::vector<std::pair<std::string, std::string>>({{"12", "base"}, {"16", "depth"}}));
//    }
//}