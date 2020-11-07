/**
 * Black-box tests for Affects Evaluator methods
 * and Affects BIP methods.
 *
 * Stub classes are used to emulate the PKB.
 */

#include <utility>

#include "../../cfg_utils/CfgUtils.h"
#include "EvaluatorTestingUtils.h"
#include "catch.hpp"
#include "pql/evaluator/relationships/affects/AffectsBipEvaluator.h"

#define PROGRAM_20_MAPS                                                                                      \
    {{1, {"steps"}},                                                                                         \
     {2, {"ro", "rd", "depth", "po", "dist", "x", "p", "done", "count"}},                                    \
     {4, {"ro"}},                                                                                            \
     {5, {"rd"}},                                                                                            \
     {6, {"depth"}},                                                                                         \
     {7, {"po", "dist", "x", "depth", "p", "done", "count"}},                                                \
     {9, {"po"}},                                                                                            \
     {10, {"dist", "x", "depth", "p"}},                                                                      \
     {11, {"done", "depth"}},                                                                                \
     {12, {"done"}},                                                                                         \
     {13, {"depth"}},                                                                                        \
     {14, {"count"}},                                                                                        \
     {15, {"dist"}},                                                                                         \
     {16, {"x"}},                                                                                            \
     {17, {"depth"}},                                                                                        \
     {18, {"p"}},                                                                                            \
     {19, {"p", "x"}},                                                                                       \
     {20, {"p"}},                                                                                            \
     {21, {"x"}},                                                                                            \
     {22, {"dist"}},                                                                                         \
     {23, {"x"}}},                                                                                           \
        {{2, {"count", "steps", "depth", "ro", "rd", "x", "y", "z", "dist", "p", "epsilon"}},                \
         {3, {"depth"}},                                                                                     \
         {7, {"count", "steps", "depth", "ro", "rd", "x", "y", "z", "dist", "p", "epsilon"}},                \
         {8, {"depth"}},                                                                                     \
         {9, {"ro", "rd", "depth"}},                                                                         \
         {10, {"x", "y", "z", "dist", "depth", "p"}},                                                        \
         {11, {"dist", "epsilon", "depth"}},                                                                 \
         {12, {"depth"}},                                                                                    \
         {13, {"depth", "dist"}},                                                                            \
         {14, {"count"}},                                                                                    \
         {15, {"x", "y", "z"}},                                                                              \
         {16, {"dist"}},                                                                                     \
         {17, {"depth"}},                                                                                    \
         {19, {"x", "p", "dist"}},                                                                           \
         {20, {"x"}},                                                                                        \
         {21, {"dist", "x"}},                                                                                \
         {22, {"x"}},                                                                                        \
         {23, {"x", "y"}}},                                                                                  \
    {                                                                                                        \
        {1, ReadStatement}, {2, CallStatement}, {3, PrintStatement}, {4, AssignmentStatement},               \
            {5, AssignmentStatement}, {6, ReadStatement}, {7, WhileStatement}, {8, PrintStatement},          \
            {9, AssignmentStatement}, {10, CallStatement}, {11, IfStatement}, {12, AssignmentStatement},     \
            {13, AssignmentStatement}, {14, AssignmentStatement}, {15, AssignmentStatement},                 \
            {16, AssignmentStatement}, {17, AssignmentStatement}, {18, ReadStatement}, {19, WhileStatement}, \
            {20, AssignmentStatement}, {21, AssignmentStatement}, {22, AssignmentStatement},                 \
        {                                                                                                    \
            23, AssignmentStatement                                                                          \
        }                                                                                                    \
    }

#define PROGRAM_20_MAPS_NO_CALL                                                                              \
    {{1, {"steps"}},                                                                                         \
     {4, {"ro"}},                                                                                            \
     {5, {"rd"}},                                                                                            \
     {6, {"depth"}},                                                                                         \
     {7, {"po", "dist", "x", "depth", "p", "done", "count"}},                                                \
     {9, {"po"}},                                                                                            \
     {11, {"done", "depth"}},                                                                                \
     {12, {"done"}},                                                                                         \
     {13, {"depth"}},                                                                                        \
     {14, {"count"}},                                                                                        \
     {15, {"dist"}},                                                                                         \
     {16, {"x"}},                                                                                            \
     {17, {"depth"}},                                                                                        \
     {18, {"p"}},                                                                                            \
     {19, {"p", "x"}},                                                                                       \
     {20, {"p"}},                                                                                            \
     {21, {"x"}},                                                                                            \
     {22, {"dist"}},                                                                                         \
     {23, {"x"}}},                                                                                           \
        {{3, {"depth"}},                                                                                     \
         {7, {"count", "steps", "depth", "ro", "rd", "x", "y", "z", "dist", "p", "epsilon"}},                \
         {8, {"depth"}},                                                                                     \
         {9, {"ro", "rd", "depth"}},                                                                         \
         {11, {"dist", "epsilon", "depth"}},                                                                 \
         {12, {"depth"}},                                                                                    \
         {13, {"depth", "dist"}},                                                                            \
         {14, {"count"}},                                                                                    \
         {15, {"x", "y", "z"}},                                                                              \
         {16, {"dist"}},                                                                                     \
         {17, {"depth"}},                                                                                    \
         {19, {"x", "p", "dist"}},                                                                           \
         {20, {"x"}},                                                                                        \
         {21, {"dist", "x"}},                                                                                \
         {22, {"x"}},                                                                                        \
         {23, {"x", "y"}}},                                                                                  \
    {                                                                                                        \
        {1, ReadStatement}, {2, CallStatement}, {3, PrintStatement}, {4, AssignmentStatement},               \
            {5, AssignmentStatement}, {6, ReadStatement}, {7, WhileStatement}, {8, PrintStatement},          \
            {9, AssignmentStatement}, {10, CallStatement}, {11, IfStatement}, {12, AssignmentStatement},     \
            {13, AssignmentStatement}, {14, AssignmentStatement}, {15, AssignmentStatement},                 \
            {16, AssignmentStatement}, {17, AssignmentStatement}, {18, ReadStatement}, {19, WhileStatement}, \
            {20, AssignmentStatement}, {21, AssignmentStatement}, {22, AssignmentStatement},                 \
        {                                                                                                    \
            23, AssignmentStatement                                                                          \
        }                                                                                                    \
    }

/**
 * A stub class to be used in the unit tests, so there
 * is no need for AffectsEvaluator to access the PKB.
 */
class AffectsFacadeStub: public AffectsEvaluatorFacade {
private:
    std::unordered_map<Integer, std::unordered_set<String>> modifiesMap;
    std::unordered_map<Integer, std::unordered_set<String>> usesMap;
    std::unordered_map<Integer, StatementType> typeMap;

public:
    /**
     * Constructor for an AffectsFacadeStub, a stub
     * class for AffectsEvaluatorFacade.
     *
     * @param modifiedVariablesMap Hash table of statements
     *                             to variables modified.
     * @param usedVariablesMap Hash table of statements to
     *                         variables used.
     * @param statementTypes Hash table of statements to
     *                       the type of the statement
     */
    AffectsFacadeStub(std::unordered_map<Integer, std::unordered_set<String>> modifiedVariablesMap,
                      std::unordered_map<Integer, std::unordered_set<String>> usedVariablesMap,
                      std::unordered_map<Integer, StatementType> statementTypes):
        modifiesMap(std::move(modifiedVariablesMap)),
        usesMap(std::move(usedVariablesMap)), typeMap(std::move(statementTypes))
    {}

    Vector<String> getUsed(Integer stmtNum) override
    {
        auto position = usesMap.find(stmtNum);
        if (position == usesMap.end()) {
            return Vector<String>();
        } else {
            return Vector<String>(position->second.begin(), position->second.end());
        }
    }

    Vector<String> getModified(Integer stmtNum) override
    {
        auto position = modifiesMap.find(stmtNum);
        if (position == modifiesMap.end()) {
            return Vector<String>();
        } else {
            return Vector<String>(position->second.begin(), position->second.end());
        }
    }

    StatementType getType(Integer stmtNum) override
    {
        auto position = typeMap.find(stmtNum);
        if (position == typeMap.end()) {
            return NonExistentStatement;
        } else {
            return position->second;
        }
    }
};

class AffectsBipFacadeStub: public AffectsBipFacade, public AffectsFacadeStub {
private:
    CfgNode* cfgNode;

public:
    /**
     * Constructor for an AffectsBipFacadeStub, a stub
     * class for AffectsBipFacade.
     *
     * @param modifiedVariablesMap Hash table of statements
     *                             to variables modified.
     * @param usedVariablesMap Hash table of statements to
     *                         variables used.
     * @param statementTypes Hash table of statements to
     *                       the type of the statement
     * @param cfgBip The CFG BIP used by this facade.
     */
    AffectsBipFacadeStub(std::unordered_map<Integer, std::unordered_set<String>> modifiedVariablesMap,
                         std::unordered_map<Integer, std::unordered_set<String>> usedVariablesMap,
                         std::unordered_map<Integer, StatementType> statementTypes, CfgNode* cfgBip):
        AffectsFacadeStub(std::move(modifiedVariablesMap), std::move(usedVariablesMap), std::move(statementTypes)),
        cfgNode(cfgBip)
    {}

    Vector<String> getUsed(Integer stmtNum) override
    {
        return AffectsFacadeStub::getUsed(stmtNum);
    }

    Vector<String> getModified(Integer stmtNum) override
    {
        return AffectsFacadeStub::getModified(stmtNum);
    }

    StatementType getType(Integer stmtNum) override
    {
        return AffectsFacadeStub::getType(stmtNum);
    }

    CfgNode* getCfg(const String&) override
    {
        return cfgNode;
    }

    Vector<String> getProcedure(Integer stmtNum) override
    {
        return {"dummy"};
    }

    Vector<String> getRelevantProcedures() override
    {
        return {"dummy"};
    }
};

AffectsFacadeStub* getFacadeProgram20()
{
    return new AffectsFacadeStub(PROGRAM_20_MAPS);
}

AffectsFacadeStub* getFacadeProgram20NoCall()
{
    return new AffectsFacadeStub(PROGRAM_20_MAPS_NO_CALL);
}

AffectsBipFacadeStub* getBipFacadeProgram20()
{
    return new AffectsBipFacadeStub(PROGRAM_20_MAPS_NO_CALL, getProgram20CfgBip_multipleProceduresSpheresdf().first);
}

TEST_CASE("Affects Search works over CFG of program 20")
{
    ResultsTable results{DeclarationTable()};
    AffectsEvaluator evaluator(results, getFacadeProgram20());
    const CfgNode* cfgMain = getProgram20Cfg_main().first;
    const CfgNode* cfgRaymarch = getProgram20Cfg_raymarch().first;
    const CfgNode* cfgSpheresdf = getProgram20Cfg_spheresdf().first;

    // main
    std::unordered_map<String, std::unordered_set<Integer>> affectsMapMain;
    AffectsTuple mainResults;
    evaluator.affectsSearchForUnitTesting(cfgMain, affectsMapMain, mainResults);
    REQUIRE(mainResults == AffectsTuple({}, {}, {}));
    REQUIRE(affectsMapMain == std::unordered_map<String, std::unordered_set<Integer>>({}));

    // raymarch
    std::unordered_map<String, std::unordered_set<Integer>> affectsMapRaymarch;
    AffectsTuple raymarchResults;
    evaluator.affectsSearchForUnitTesting(cfgRaymarch, affectsMapRaymarch, raymarchResults);
    REQUIRE(raymarchResults == AffectsTuple({4, 5, 13, 14}, {9, 14}, {{4, 9}, {5, 9}, {13, 9}, {14, 14}}));
    REQUIRE(affectsMapRaymarch
            == std::unordered_map<String, std::unordered_set<Integer>>(
                {{"count", {14}}, {"depth", {13}}, {"ro", {4}}, {"rd", {5}}, {"done", {12}}, {"po", {9}}}));

    // spheresdf
    std::unordered_map<String, std::unordered_set<Integer>> affectsMapSpheresdf;
    AffectsTuple spheresdfResults;
    evaluator.affectsSearchForUnitTesting(cfgSpheresdf, affectsMapSpheresdf, spheresdfResults);
    REQUIRE(spheresdfResults
            == AffectsTuple(
                {15, 16, 21}, {16, 20, 21, 22, 23},
                {{15, 16}, {15, 21}, {16, 20}, {16, 21}, {16, 22}, {16, 23}, {21, 20}, {21, 21}, {21, 22}, {21, 23}}));
    REQUIRE(affectsMapSpheresdf
            == std::unordered_map<String, std::unordered_set<Integer>>(
                {{"depth", {17}}, {"x", {23}}, {"p", {20}}, {"dist", {22}}}));
}

TEST_CASE("Affects Search works over CFG BIP of program 20")
{
    ResultsTable results{DeclarationTable()};
    AffectsEvaluator evaluator(results, getFacadeProgram20NoCall());
    CfgNode* cfgBipSpheresdf = getProgram20CfgBip_multipleProceduresSpheresdf().first;

    std::unordered_map<String, std::unordered_set<Integer>> affectsMapSpheresdf;
    AffectsTuple spheresdfResults;
    evaluator.affectsSearchForUnitTesting(cfgBipSpheresdf, affectsMapSpheresdf, spheresdfResults);
    REQUIRE(spheresdfResults
            == AffectsTuple({4, 5, 13, 14, 15, 16, 17, 21, 22, 23}, {9, 12, 13, 14, 15, 16, 17, 20, 21, 22, 23},
                            {{4, 9},   {5, 9},   {13, 9},  {13, 17}, {14, 14}, {15, 16}, {15, 21},
                             {16, 20}, {16, 21}, {16, 22}, {16, 23}, {17, 9},  {17, 12}, {17, 13},
                             {17, 17}, {21, 20}, {21, 21}, {21, 22}, {21, 23}, {22, 13}, {23, 15}}));
    REQUIRE(affectsMapSpheresdf
            == std::unordered_map<String, std::unordered_set<Integer>>({{"count", {14}},
                                                                        {"ro", {4}},
                                                                        {"rd", {5}},
                                                                        {"done", {12}},
                                                                        {"po", {9}},
                                                                        {"depth", {13, 17}},
                                                                        {"x", {23}},
                                                                        {"p", {20}},
                                                                        {"dist", {22}}}));
}

TEST_CASE("Affects BIP * Search works over CFG BIP of program 20")
{
    ResultsTable resTable{DeclarationTable()};
    AffectsBipEvaluator evaluator(resTable, getBipFacadeProgram20());

    SECTION("AffectsBip*(4, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(4);
        requireVectorsHaveSameElements(results, {9});
    }

    SECTION("AffectsBip*(5, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(5);
        requireVectorsHaveSameElements(results, {9});
    }

    SECTION("AffectsBip*(13, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(13);
        requireVectorsHaveSameElements(results, {9, 12, 13, 17});
    }

    SECTION("AffectsBip*(14, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(14);
        requireVectorsHaveSameElements(results, {14});
    }

    SECTION("AffectsBip*(15, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(15);
        requireVectorsHaveSameElements(results, {9, 12, 13, 15, 16, 17, 20, 21, 22, 23});
    }

    SECTION("AffectsBip*(16, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(16);
        requireVectorsHaveSameElements(results, {9, 12, 13, 15, 16, 17, 20, 21, 22, 23});
    }

    SECTION("AffectsBip*(17, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(17);
        requireVectorsHaveSameElements(results, {9, 12, 13, 17});
    }

    SECTION("AffectsBip*(21, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(21);
        requireVectorsHaveSameElements(results, {9, 12, 13, 15, 16, 17, 20, 21, 22, 23});
    }

    SECTION("AffectsBip*(22, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(22);
        requireVectorsHaveSameElements(results, {9, 12, 13, 17});
    }

    SECTION("AffectsBip*(23, _)")
    {
        Vector<Integer> results = evaluator.affectsBipStarSearchForUnitTesting(23);
        requireVectorsHaveSameElements(results, {9, 12, 13, 15, 16, 17, 20, 21, 22, 23});
    }
}
