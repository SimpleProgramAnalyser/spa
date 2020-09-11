#include <string>

#include "ast_utils/AstUtils.h"
#include "catch.hpp"
#include "frontend/designExtractor/SemanticErrorsValidator.h"

TEST_CASE("Design Extractor identifies semantically valid program as valid - computeAverage")
{
    SemanticErrorsValidator seValidator(*getProgram3Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == true);
}

TEST_CASE("Design Extractor identifies semantically valid program with if as valid - printAscending")
{
    SemanticErrorsValidator seValidator(*getProgram4Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == true);
}

TEST_CASE("Design Extractor identifies semantically valid program with while as valid - sumDigits")
{
    SemanticErrorsValidator seValidator(*getProgram5Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == true);
}

TEST_CASE("Design Extractor identifies semantically invalid program with duplicate procedure name as invalid - "
          "sumDigitsDuplicate")
{
    SemanticErrorsValidator seValidator(*getProgram6Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == false);
}

TEST_CASE("Design Extractor identifies semantically valid program with while and if as valid - multiProcedureProgram")
{
    SemanticErrorsValidator seValidator(*getProgram7Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == true);
}

TEST_CASE("Design Extractor identifies semantically invalid program with cyclic calls as invalid - sumDigit and plus")
{
    SemanticErrorsValidator seValidator(*getProgram8Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == false);
}

TEST_CASE(
    "Design Extractor identifies semantically invalid program with cyclic calls as invalid - sumDigit, plus and minus")
{
    SemanticErrorsValidator seValidator(*getProgram9Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == false);
}

TEST_CASE("Design Extractor identifies semantically invalid program with non-existent procedure calls as invalid - "
          "sumDigit and plus")
{
    SemanticErrorsValidator seValidator(*getProgram10Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == false);
}

TEST_CASE("Design Extractor identifies semantically invalid program with nested while and if cyclic calls as invalid - "
          "printSumDigit, printSum and printNumber")
{
    SemanticErrorsValidator seValidator(*getProgram11Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == false);
}

TEST_CASE("Design Extractor identifies semantically invalid recursive program as invalid - recursivePrintAscending")
{

    SemanticErrorsValidator seValidator(*getProgram12Tree());
    Boolean isSemanticallyValid = seValidator.isProgramValid();
    REQUIRE(isSemanticallyValid == false);
}
