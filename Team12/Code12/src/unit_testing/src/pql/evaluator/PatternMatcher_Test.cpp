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
#include "pql/evaluator/PatternMatcher.h"

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
        doVectorsHaveSameElementsVoid(resTable.get("assign"), std::vector<std::string>({"13", "16", "21"}));
        doVectorsHaveSameElementsVoid(resTable.get("var"), std::vector<std::string>({"depth", "x"}));
        doVectorsHaveSameElementsVoid(
            resTable.getRelationships("var", "assign"),
            std::vector<std::pair<std::string, std::string>>({{"x", "16"}, {"x", "21"}, {"depth", "13"}}));
    }

    SECTION("Two wildcards")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(WildcardRefType, "_"),
                              ExpressionSpec(WildcardExpressionType));
        evaluatePattern(&clause1, &resTable);
        doVectorsHaveSameElementsVoid(
            resTable.get("assign"),
            std::vector<std::string>({"4", "5", "9", "12", "13", "14", "15", "16", "17", "20", "21", "22", "23"}));
    }

    SECTION("Assigned to variable is wildcard")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(WildcardRefType, "_"),
                              ExpressionSpec(createRefExpr("dist"), ExtendableLiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("assign"), std::vector<std::string>({"13", "16", "21"}));
    }

    SECTION("Assigned to variable is string literal")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(LiteralRefType, "x"),
                              ExpressionSpec(createRefExpr("dist"), ExtendableLiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("assign"), std::vector<std::string>({"16", "21"}));
    }

    SECTION("Numeral literal expression")
    {
        PatternClause clause1("assign", AssignPatternType, Reference(SynonymRefType, "var"),
                              ExpressionSpec(createRefExpr(19), ExtendableLiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("assign"), std::vector<std::string>({"5"}));
        doVectorsHaveSameElementsVoid(resTable.get("var"), std::vector<std::string>({"rd"}));
    }

    SECTION("Arithmetic expression")
    {
        PatternClause clause1(
            "assign", AssignPatternType, Reference(SynonymRefType, "var"),
            ExpressionSpec(
                createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth"))),
                LiteralExpressionType));
        evaluatePattern(&clause1, &resTable);
        doVectorsHaveSameElementsVoid(resTable.get("assign"), std::vector<std::string>({"9"}));
        doVectorsHaveSameElementsVoid(resTable.get("var"), std::vector<std::string>({"po"}));
        doVectorsHaveSameElementsVoid(resTable.getRelationships("var", "assign"),
                                      std::vector<std::pair<std::string, std::string>>({{"po", "9"}}));
    }
}
