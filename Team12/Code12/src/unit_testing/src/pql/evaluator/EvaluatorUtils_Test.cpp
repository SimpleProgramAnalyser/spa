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

TEST_CASE("PotentialValue and SynonymWithValue")
{
    PotentialValue pv1("a", "2");
    SynonymWithValue swv2("a", "3");
    PotentialValue pv3("a", "7");
    SynonymWithValue swv4("a", "1029871");

    PotentialValue pv5("b", "1");
    SynonymWithValue swv6("b", "2");
    PotentialValue pv7("b", "results");

    PotentialValue pv8("c", "helloworld");
    PotentialValue pv9("c", "byeworld");
    PotentialValue pv10("c", "theworld");

    PotentialValue pv11("efgh", "231454");
    SynonymWithValue swv12("efgh", "231454");
    PotentialValue pv13("efgh", "232454");

    SynonymWithValue swv14("colour", "rojo");
    SynonymWithValue swv15("colour", "verde");
    SynonymWithValue swv16("colour", "azul");

    SECTION("PotentialValue hashes to different values")
    {
        REQUIRE(PotentialValueHasher()(pv11) == PotentialValueHasher()(swv12.asPv()));

        REQUIRE_FALSE(PotentialValueHasher()(pv1) == PotentialValueHasher()(swv2.asPv()));
        REQUIRE_FALSE(PotentialValueHasher()(swv2.asPv()) == PotentialValueHasher()(pv3));
        REQUIRE_FALSE(PotentialValueHasher()(pv3) == PotentialValueHasher()(swv4.asPv()));

        REQUIRE_FALSE(PotentialValueHasher()(pv5) == PotentialValueHasher()(swv6.asPv()));
        REQUIRE_FALSE(PotentialValueHasher()(swv6.asPv()) == PotentialValueHasher()(pv7));

        REQUIRE_FALSE(PotentialValueHasher()(pv8) == PotentialValueHasher()(pv9));
        REQUIRE_FALSE(PotentialValueHasher()(pv9) == PotentialValueHasher()(pv10));

        REQUIRE_FALSE(PotentialValueHasher()(pv11) == PotentialValueHasher()(pv13));

        REQUIRE_FALSE(PotentialValueHasher()(swv14.asPv()) == PotentialValueHasher()(swv15.asPv()));
        REQUIRE_FALSE(PotentialValueHasher()(swv15.asPv()) == PotentialValueHasher()(swv16.asPv()));
    }

    SECTION("SynonymWithValue hashes to same value for same synonym")
    {
        REQUIRE(SynonymWithValueHasher()(pv11.asSwv()) == SynonymWithValueHasher()(swv12));

        REQUIRE(SynonymWithValueHasher()(pv1.asSwv()) == SynonymWithValueHasher()(swv2));
        REQUIRE(SynonymWithValueHasher()(swv2) == SynonymWithValueHasher()(pv3.asSwv()));
        REQUIRE(SynonymWithValueHasher()(pv3.asSwv()) == SynonymWithValueHasher()(swv4));

        REQUIRE(SynonymWithValueHasher()(pv5.asSwv()) == SynonymWithValueHasher()(swv6));
        REQUIRE(SynonymWithValueHasher()(swv6) == SynonymWithValueHasher()(pv7.asSwv()));

        REQUIRE(SynonymWithValueHasher()(pv8.asSwv()) == SynonymWithValueHasher()(pv9.asSwv()));
        REQUIRE(SynonymWithValueHasher()(pv9.asSwv()) == SynonymWithValueHasher()(pv10.asSwv()));

        REQUIRE(SynonymWithValueHasher()(pv11.asSwv()) == SynonymWithValueHasher()(pv13.asSwv()));

        REQUIRE(SynonymWithValueHasher()(swv14) == SynonymWithValueHasher()(swv15));
        REQUIRE(SynonymWithValueHasher()(swv15) == SynonymWithValueHasher()(swv16));
    }

    SECTION("PotentialValue are not equal")
    {
        REQUIRE(pv11 == swv12.asPv());

        REQUIRE_FALSE(pv1 == swv2.asPv());
        REQUIRE_FALSE(swv2.asPv() == pv3);
        REQUIRE_FALSE(pv3 == swv4.asPv());

        REQUIRE_FALSE(pv5 == swv6.asPv());
        REQUIRE_FALSE(swv6.asPv() == pv7);

        REQUIRE_FALSE(pv8 == pv9);
        REQUIRE_FALSE(pv9 == pv10);

        REQUIRE_FALSE(pv11 == pv13);

        REQUIRE_FALSE(swv14.asPv() == swv15.asPv());
        REQUIRE_FALSE(swv15.asPv() == swv16.asPv());
    }

    SECTION("SynonymWithValue are equal for same synonym")
    {
        REQUIRE(pv11.asSwv() == swv12);

        REQUIRE(pv1.asSwv() == swv2);
        REQUIRE(swv2 == pv3.asSwv());
        REQUIRE(pv3.asSwv() == swv4);

        REQUIRE(pv5.asSwv() == swv6);
        REQUIRE(swv6 == pv7.asSwv());

        REQUIRE(pv8.asSwv() == pv9.asSwv());
        REQUIRE(pv9.asSwv() == pv10.asSwv());

        REQUIRE(pv11.asSwv() == pv13.asSwv());

        REQUIRE(swv14 == swv15);
        REQUIRE(swv15 == swv16);
    }

    SECTION("std::unordered_set of PotentialValue allows one synonym with multiple values") {
        std::unordered_set<PotentialValue, PotentialValueHasher> set;
        set.insert(pv1);
        set.insert(static_cast<PotentialValue>(swv2));
        set.insert(pv3);
        set.insert(static_cast<PotentialValue>(swv4));
        set.insert(pv5);
        set.insert(static_cast<PotentialValue>(swv6));
        set.insert(pv7);
        set.insert(pv8);
        set.insert(pv9);
        set.insert(pv10);
        set.insert(pv11);
        set.insert(static_cast<PotentialValue>(swv12));
        set.insert(pv13);
        set.insert(static_cast<PotentialValue>(swv14));
        set.insert(static_cast<PotentialValue>(swv15));
        set.insert(static_cast<PotentialValue>(swv16));
        REQUIRE(set.size() == 15);
    }

    SECTION("std::unordered_set of SynonymWithValue only allows one synonym") {
        std::unordered_set<SynonymWithValue, SynonymWithValueHasher> set;
        set.insert(static_cast<SynonymWithValue>(pv1));
        set.insert(swv2);
        set.insert(static_cast<SynonymWithValue>(pv3));
        set.insert(swv4);
        set.insert(static_cast<SynonymWithValue>(pv5));
        set.insert(swv6);
        set.insert(static_cast<SynonymWithValue>(pv7));
        set.insert(static_cast<SynonymWithValue>(pv8));
        set.insert(static_cast<SynonymWithValue>(pv9));
        set.insert(static_cast<SynonymWithValue>(pv10));
        set.insert(static_cast<SynonymWithValue>(pv11));
        set.insert(swv12);
        set.insert(static_cast<SynonymWithValue>(pv13));
        set.insert(swv14);
        set.insert(swv15);
        set.insert(swv16);
        REQUIRE(set.size() == 5);
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
