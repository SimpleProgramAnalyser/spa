/**
 * Unit tests for utility methods used by Query Evaluator.
 */
#include <functional>
#include <stdexcept>

#include "catch.hpp"
#include "pql/evaluator/EvaluatorUtils.h"
#include "pql/evaluator/relationships/RelationshipsUtil.h"

/**
 * Helper function to check if a procedure throws a std::runtime_error
 */
void requireThrowRuntimeError(const std::function<void(void)>& procedure)
{
    try {
        procedure();
        REQUIRE(false);
    } catch (const std::runtime_error& e) {
        REQUIRE(true);
    }
}

TEST_CASE("convertToClauseResult converts list of integers to list of strings")
{
    std::vector<int> original({7, 3, 7, 2, 340, 29, 48, 0, 3});
    std::vector<std::string> expected({"7", "3", "7", "2", "340", "29", "48", "0", "3"});

    REQUIRE(convertToClauseResult(original) == expected);
    REQUIRE(convertToClauseResult(std::vector<int>{}).empty());
}

TEST_CASE("mapToStatementType maps DesignEntityType to StatementType correctly")
{
    REQUIRE(mapToStatementType(StmtType) == AnyStatement);
    REQUIRE(mapToStatementType(ReadType) == ReadStatement);
    REQUIRE(mapToStatementType(PrintType) == PrintStatement);
    REQUIRE(mapToStatementType(CallType) == CallStatement);
    REQUIRE(mapToStatementType(WhileType) == WhileStatement);
    REQUIRE(mapToStatementType(IfType) == IfStatement);
    REQUIRE(mapToStatementType(AssignType) == AssignmentStatement);

    requireThrowRuntimeError([]() { mapToStatementType(VariableType); });
    requireThrowRuntimeError([]() { mapToStatementType(ConstantType); });
    requireThrowRuntimeError([]() { mapToStatementType(ProcedureType); });
    requireThrowRuntimeError([]() { mapToStatementType(NonExistentType); });
}

TEST_CASE("removeDuplicates removes duplicate elements in vector")
{
    std::vector<std::string> original({"7", "3", "7", "2", "340", "29", "48", "0", "3"});
    std::vector<std::string> expected({"0", "2", "29", "3", "340", "48", "7"});
    std::vector<std::string> originalSorted = removeDuplicates(original);
    std::sort(originalSorted.begin(), originalSorted.end());
    REQUIRE(originalSorted == expected);
}

TEST_CASE("verifyStatementType works for all possible statement types")
{
    std::vector<std::pair<int, StatementType>> original({{24, ReadStatement},
                                                         {45, PrintStatement},
                                                         {78, CallStatement},
                                                         {394, WhileStatement},
                                                         {395, IfStatement},
                                                         {16057, AssignmentStatement},
                                                         {304, AssignmentStatement},
                                                         {192, IfStatement},
                                                         {2048, ReadStatement},
                                                         {311, AssignmentStatement},
                                                         {123, ReadStatement},
                                                         {708, AssignmentStatement},
                                                         {7, CallStatement},
                                                         {34, WhileStatement},
                                                         {6, IfStatement},
                                                         {3, WhileStatement},
                                                         {5, AssignmentStatement}});
    std::vector<int> anyExpected({3, 5, 6, 7, 24, 34, 45, 78, 123, 192, 304, 311, 394, 395, 708, 2048, 16057});
    std::vector<int> readExpected({24, 123, 2048});
    std::vector<int> printExpected({45});
    std::vector<int> ifExpected({6, 192, 395});
    std::vector<int> whileExpected({3, 34, 394});
    std::vector<int> assignExpected({5, 304, 311, 708, 16057});
    std::vector<int> callExpected({7, 78});

    std::vector<int> anyActual = verifyStatementType(AnyStatement, original);
    std::vector<int> readActual = verifyStatementType(ReadStatement, original);
    std::vector<int> printActual = verifyStatementType(PrintStatement, original);
    std::vector<int> ifActual = verifyStatementType(IfStatement, original);
    std::vector<int> whileActual = verifyStatementType(WhileStatement, original);
    std::vector<int> assignActual = verifyStatementType(AssignmentStatement, original);
    std::vector<int> callActual = verifyStatementType(CallStatement, original);

    std::sort(anyActual.begin(), anyActual.end());
    std::sort(readActual.begin(), readActual.end());
    std::sort(printActual.begin(), printActual.end());
    std::sort(ifActual.begin(), ifActual.end());
    std::sort(whileActual.begin(), whileActual.end());
    std::sort(assignActual.begin(), assignActual.end());
    std::sort(callActual.begin(), callActual.end());

    REQUIRE(anyActual == anyExpected);
    REQUIRE(readActual == readExpected);
    REQUIRE(printActual == printExpected);
    REQUIRE(ifActual == ifExpected);
    REQUIRE(whileActual == whileExpected);
    REQUIRE(assignActual == assignExpected);
    REQUIRE(callActual == callExpected);
}

TEST_CASE("canMatchOnlyOne works for all possible reference types")
{
    REQUIRE(canMatchOnlyOne(LiteralRefType));
    REQUIRE(canMatchOnlyOne(IntegerRefType));
    REQUIRE_FALSE(canMatchOnlyOne(SynonymRefType));
    REQUIRE_FALSE(canMatchOnlyOne(WildcardRefType));
}

TEST_CASE("canMatchMultiple works for all possible reference types")
{
    REQUIRE(canMatchMultiple(SynonymRefType));
    REQUIRE(canMatchMultiple(WildcardRefType));
    REQUIRE_FALSE(canMatchMultiple(LiteralRefType));
    REQUIRE_FALSE(canMatchMultiple(IntegerRefType));
}

TEST_CASE("refHasConstraints works for all possible reference types")
{
    ResultsTable results(DeclarationTable{});
    Reference refSyn1(SynonymRefType, "syn", DesignEntity(CallType));
    Reference refSyn2(SynonymRefType, "pn", DesignEntity(PrintType));
    Reference refSyn3(SynonymRefType, "myVar", DesignEntity(VariableType));
    Reference refSyn4(SynonymRefType, "calcFactorials", DesignEntity(ProcedureType));
    Reference ref1(LiteralRefType, "greetings");
    Reference ref2(IntegerRefType, "30");
    Reference ref3(WildcardRefType, "_");

    results.filterTable(refSyn1, std::vector<std::string>({"1", "16", "3"}));
    results.filterTable(refSyn3, std::vector<std::string>({"pillager", "ravager", "vindicator", "witch", "evoker"}));
    results.filterTable(ref1, std::vector<std::string>({"a", "b", "c"}));
    results.filterTable(ref2, std::vector<std::string>({"1", "2", "3"}));
    results.filterTable(ref3, std::vector<std::string>({"d", "e", "f"}));

    REQUIRE(refHasConstraints(refSyn1, &results));
    REQUIRE(refHasConstraints(refSyn3, &results));
    REQUIRE_FALSE(refHasConstraints(refSyn2, &results));
    REQUIRE_FALSE(refHasConstraints(refSyn4, &results));
    // non-synonym references returns false
    REQUIRE_FALSE(refHasConstraints(ref1, &results));
    REQUIRE_FALSE(refHasConstraints(ref2, &results));
    REQUIRE_FALSE(refHasConstraints(ref3, &results));
}

TEST_CASE("refsHaveRelationship works for all possible reference types")
{
    ResultsTable results(DeclarationTable{});
    Reference refSyn1(SynonymRefType, "syn", DesignEntity(CallType));
    Reference refSyn2(SynonymRefType, "pn", DesignEntity(PrintType));
    Reference refSyn3(SynonymRefType, "myVar", DesignEntity(VariableType));
    Reference refSyn4(SynonymRefType, "calcFactorials", DesignEntity(ProcedureType));
    Reference ref1(LiteralRefType, "greetings");
    Reference ref2(IntegerRefType, "30");
    Reference ref3(WildcardRefType, "_");

    results.filterTable(refSyn1, std::vector<std::string>({"1", "2", "3", "4", "5", "7", "8", "13"}));
    results.filterTable(refSyn2, std::vector<std::string>({"9", "10", "13", "14"}));
    results.filterTable(refSyn3, std::vector<std::string>(
                                     {"sin", "cos", "average", "sum", "fib", "fact", "istrue", "maybe", "isfalse"}));
    results.filterTable(refSyn4, std::vector<std::string>({"if", "calculate", "throw", "assert", "terminate"}));

    results.associateRelationships(std::vector<std::pair<std::string, std::string>>({{"7", "9"}}), refSyn1, refSyn2);
    results.associateRelationships(
        std::vector<std::pair<std::string, std::string>>({{"5", "average"}, {"8", "sum"}, {"13", "fact"}}), refSyn1,
        refSyn3);
    results.associateRelationships(
        std::vector<std::pair<std::string, std::string>>({{"istrue", "calculate"}, {"isfalse", "assert"}}), refSyn3,
        refSyn4);

    REQUIRE(refsHaveRelationship(refSyn1, refSyn2, &results));
    REQUIRE(refsHaveRelationship(refSyn3, refSyn1, &results));
    REQUIRE(refsHaveRelationship(refSyn4, refSyn3, &results));
    REQUIRE_FALSE(refsHaveRelationship(refSyn2, refSyn3, &results));
    REQUIRE_FALSE(refsHaveRelationship(refSyn4, refSyn2, &results));
    REQUIRE_FALSE(refsHaveRelationship(refSyn1, refSyn4, &results));
    // non-synonym references returns false
    REQUIRE_FALSE(refsHaveRelationship(refSyn1, ref1, &results));
    REQUIRE_FALSE(refsHaveRelationship(ref1, refSyn1, &results));
    REQUIRE_FALSE(refsHaveRelationship(refSyn2, ref2, &results));
    REQUIRE_FALSE(refsHaveRelationship(ref2, refSyn2, &results));
    REQUIRE_FALSE(refsHaveRelationship(refSyn3, ref3, &results));
    REQUIRE_FALSE(refsHaveRelationship(ref3, refSyn3, &results));
}
