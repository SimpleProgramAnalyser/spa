#include "AbstractQueryBuilder.h"
#include "catch.hpp"
#include "pql/preprocessor/AqTypes.h"
#include "pql/preprocessor/Preprocessor.h"

AbstractQuery processQuery(String queryString)
{
    return Preprocessor::processQuery(queryString);
}

TEST_CASE("Only Declarations without Select Clause returns Invalid Abstract Query")
{
    AbstractQuery abstractQuery = processQuery("stmt s;");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Only Select Clause without Declarations return Invalid Abstract Query")
{
    AbstractQuery abstractQuery = processQuery("Select s");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Select Synonym without Clauses")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s");

    //    AbstractQueryBuilder abstractQueryBuilder;
    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create().addSelectSynonym("s").addDeclaration("s", "stmt").build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Follows (s, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(FollowsType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Pattern a (_, \"x + y\")")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \"x + y\")");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("a")
                                              .addDeclaration("a", "assign")
                                              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_",
                                                                      NonExistentType, "x + y", LiteralExpressionType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

/************************************************************************************/
/*  One Clause | Such That Clause | Relationship Variation                          */
/************************************************************************************/

TEST_CASE("Such That Follows* Statement")
{
    AbstractQuery abstractQuery = processQuery("stmt a; Select a such that Follows* (a, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "stmt")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "a", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Parent")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Parent (s, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(ParentType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Parent*")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Parent* (s, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(ParentStarType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Uses")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Uses (s, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(UsesType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Modifies")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Modifies(s, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(ModifiesType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

/************************************************************************************/
/*  One Clause | Such That Clause | 1 Declaration Variation                         */
/************************************************************************************/

TEST_CASE("Such That Follows* Read")
{
    AbstractQuery abstractQuery = processQuery("read re; Select re such that Follows* (re, 5)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("re")
              .addDeclaration("re", "read")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "re", ReadType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* Print")
{
    AbstractQuery abstractQuery = processQuery("print pn; Select pn such that Follows* (pn, 5)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("pn")
              .addDeclaration("pn", "print")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "pn", PrintType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* Call")
{
    AbstractQuery abstractQuery = processQuery("call cl; Select cl such that Follows* (cl, 5)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("cl")
              .addDeclaration("cl", "call")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "cl", CallType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* While")
{
    AbstractQuery abstractQuery = processQuery("while w; Select w such that Follows* (w, 5)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("w")
              .addDeclaration("w", "while")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "w", WhileType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* If")
{
    AbstractQuery abstractQuery = processQuery("if ifs; Select ifs such that Follows* (ifs, 5)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("ifs")
              .addDeclaration("ifs", "if")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "ifs", IfType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* Assign")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a such that Follows* (a, 5)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "a", AssignType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Modifies Procedure")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p such that Modifies (p, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("p")
                                              .addDeclaration("p", "procedure")
                                              .addSuchThatClause(ModifiesProcedureType, SynonymRefType, "p",
                                                                 ProcedureType, WildcardRefType, "_", NonExistentType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Uses Procedure")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p such that Uses (p, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("p")
                                              .addDeclaration("p", "procedure")
                                              .addSuchThatClause(UsesProcedureType, SynonymRefType, "p", ProcedureType,
                                                                 WildcardRefType, "_", NonExistentType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* Constant Returns Error")
{
    AbstractQuery abstractQuery = processQuery("constant c; Select c such that Follows* (c, 5)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Follows* Variable Returns Error")
{
    AbstractQuery abstractQuery = processQuery("variable v; Select v such that Follows* (v, 5)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Follows* Procedure Returns Error")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p such that Follows* (p, 5)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Variable cannot be rightRef of Follows")
{
    AbstractQuery abstractQuery = processQuery("assign a; variable v; Select a such that Follows (a, v)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Variable cannot be rightRef of Parent")
{
    AbstractQuery abstractQuery = processQuery("while w; variable v; Select w such that Parent (w, v)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Assign as Parent Does Not Returns Error") // Accept as semantically correct, but return no results
{
    AbstractQuery abstractQuery = processQuery("assign a; read re; Select a such that Parent (a, re)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Modifies Wildcard Returns Error")
{
    AbstractQuery abstractQuery = processQuery("variable v; Select v such that Modifies (_, v)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Uses Wildcard Returns Error")
{
    AbstractQuery abstractQuery = processQuery("variable v; Select v such that Uses (_, v)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Uses left reference is a constant")
{
    AbstractQuery abstractQuery = processQuery("stmt s; constant c; Select s such that Uses(c, \"x\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Uses left reference is a variable")
{
    AbstractQuery abstractQuery = processQuery("stmt s; variable v; Select s such that Uses(v, \"x\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Modifies left reference is a constant")
{
    AbstractQuery abstractQuery = processQuery("stmt s; constant c; Select s such that Modifies (c, \"x\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Modifies left reference is a variable")
{
    AbstractQuery abstractQuery = processQuery("stmt s; variable v; Select s such that Modifies (v, \"x\")");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  One Clause | Such That Clause | 2 Declaration Variation                         */
/************************************************************************************/

TEST_CASE("Such That Parent* While and Read")
{
    AbstractQuery abstractQuery = processQuery("while w; read re; Select w such that Parent* (w, re)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("w")
              .addDeclaration("w", "while")
              .addDeclaration("re", "read")
              .addSuchThatClause(ParentStarType, SynonymRefType, "w", WhileType, SynonymRefType, "re", ReadType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* Stmt and Print")
{
    AbstractQuery abstractQuery = processQuery("stmt s; print pn; Select s such that Follows* (s, pn)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addDeclaration("pn", "print")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "s", StmtType, SynonymRefType, "pn", PrintType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Parent If and Call")
{
    AbstractQuery abstractQuery = processQuery("if ifs; call cl; Select cl such that Parent (ifs, cl)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("cl")
              .addDeclaration("cl", "call")
              .addDeclaration("ifs", "if")
              .addSuchThatClause(ParentType, SynonymRefType, "ifs", IfType, SynonymRefType, "cl", CallType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows While and If")
{
    AbstractQuery abstractQuery = processQuery("while w; if ifs; Select w such that Follows (w, ifs)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("w")
              .addDeclaration("w", "while")
              .addDeclaration("ifs", "if")
              .addSuchThatClause(FollowsType, SynonymRefType, "w", WhileType, SynonymRefType, "ifs", IfType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Modifies While and Variable")
{
    AbstractQuery abstractQuery = processQuery("while w; variable v; Select v such that Modifies (w, v)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("v")
              .addDeclaration("w", "while")
              .addDeclaration("v", "variable")
              .addSuchThatClause(ModifiesType, SynonymRefType, "w", WhileType, SynonymRefType, "v", VariableType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Uses If and Literal Ident")
{
    AbstractQuery abstractQuery = processQuery("if ifs; Select ifs such that Uses (ifs, \"x\")");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("ifs")
              .addDeclaration("ifs", "if")
              .addSuchThatClause(UsesType, SynonymRefType, "ifs", IfType, LiteralRefType, "x", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Uses Literal Ident and Literal Ident")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Uses (\"main\", \"x\")");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s")
                                              .addDeclaration("s", "stmt")
                                              .addSuchThatClause(UsesType, LiteralRefType, "main", NonExistentType,
                                                                 LiteralRefType, "x", NonExistentType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Modifies NonVariable Returns Error")
{
    AbstractQuery abstractQuery = processQuery("assign a; stmt s; Select a such that Modifies (a, s)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Uses NonVariable Returns Error")
{
    AbstractQuery abstractQuery = processQuery("stmt s; read re; Select s such that Uses (s, re)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Uses Constant Returns Error")
{
    AbstractQuery abstractQuery = processQuery("stmt s; constant c; Select s such that Uses (s, c)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Incomplete Relationship ")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Uses (s,");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  One Clause | Pattern Clause                                                     */
/************************************************************************************/

TEST_CASE("Pattern Clause Wildcard leftRef, Expression rightRef")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \"x + y * z + a % d\")");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType,
                                      "x + y * z + a % d", LiteralExpressionType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Pattern Clause Wildcard leftRef, Wildcard rightRef")
{
    AbstractQuery abstractQuery = processQuery("assign asdf; Select asdf pattern asdf (_, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("asdf")
                                              .addDeclaration("asdf", "assign")
                                              .addAssignPatternClause("asdf", AssignPatternType, WildcardRefType, "_",
                                                                      NonExistentType, "", WildcardExpressionType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Pattern Clause Wildcard leftRef, Extendable Expression rightRef")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, _\"x + y * z + a % d\"_)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType,
                                      "x + y * z + a % d", ExtendableLiteralExpressionType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Pattern Clause with Parentheses in Expression")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, _\"x + y * (z + a)\"_)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y * (z + a)",
                                      ExtendableLiteralExpressionType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Pattern Left Reference is a Non Variable Synonym Type")
{
    AbstractQuery abstractQuery = processQuery("stmt s; assign a; Select s pattern a (s, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Pattern Left Reference is a ExtendableLiteralExpressionType")
{
    AbstractQuery abstractQuery = processQuery("stmt s; assign a; Select s pattern a (_\"x\"_, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern Left Reference is a variable")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, _, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("v")
              .addDeclaration("ifs", "if")
              .addDeclaration("v", "variable")
              .addIfWhilePatternClause("ifs", IfPatternType, SynonymRefType, "v", VariableType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("If Pattern Left Reference is a wildcard")
{
    AbstractQuery abstractQuery = processQuery("if ifs; Select ifs pattern ifs (_, _, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("ifs")
              .addDeclaration("ifs", "if")
              .addIfWhilePatternClause("ifs", IfPatternType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("If Pattern Left Reference is a literal")
{
    AbstractQuery abstractQuery = processQuery("if ifs; Select ifs pattern ifs (\"x\", _, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("ifs")
              .addDeclaration("ifs", "if")
              .addIfWhilePatternClause("ifs", IfPatternType, LiteralRefType, "x", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("While Pattern Left Reference is a variable")
{
    AbstractQuery abstractQuery = processQuery("while w; variable v; Select v pattern w (v, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("v")
              .addDeclaration("w", "while")
              .addDeclaration("v", "variable")
              .addIfWhilePatternClause("w", WhilePatternType, SynonymRefType, "v", VariableType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("While Pattern Left Reference is a wildcard")
{
    AbstractQuery abstractQuery = processQuery("while w; Select w pattern w (_, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("w")
              .addDeclaration("w", "while")
              .addIfWhilePatternClause("w", WhilePatternType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("While Pattern Left Reference is a literal")
{
    AbstractQuery abstractQuery = processQuery("while w; Select w pattern w (\"x\", _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("w")
              .addDeclaration("w", "while")
              .addIfWhilePatternClause("w", WhilePatternType, LiteralRefType, "x", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("If Pattern left Reference is a variable.varName")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v.varName, _, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern second Reference is literal")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, \"x\", _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern second Reference is integer")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, 5, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern second Reference is synonym")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, v, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern third Reference is literal")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, _, \"x\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern third Reference is integer")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, _, 5)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern third Reference is synonym")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, _, v)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("If Pattern all Reference are synonyms")
{
    AbstractQuery abstractQuery = processQuery("if ifs; variable v; Select v pattern ifs (v, v, v)");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  Two Clause | Such That + Pattern                                                */
/************************************************************************************/

TEST_CASE("Pattern and Such That Clauses")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \"x + y\") such that Uses (a, \"z\")");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y",
                                      LiteralExpressionType)
              .addSuchThatClause(UsesType, SynonymRefType, "a", AssignType, LiteralRefType, "z", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That and Pattern Clauses 1")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a such that Uses (a, \"z\") pattern a (_, \"x + y\")");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addSuchThatClause(UsesType, SynonymRefType, "a", AssignType, LiteralRefType, "z", NonExistentType)
              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y",
                                      LiteralExpressionType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That and Pattern Clauses 2")
{
    AbstractQuery abstractQuery = processQuery("assign a; stmt s; Select a such that Follows* (s, a) pattern a (_, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "s", StmtType, SynonymRefType, "a", AssignType)
              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "_",
                                      WildcardExpressionType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

/************************************************************************************/
/*  'and' connector within Clauses                                                  */
/************************************************************************************/

TEST_CASE("Such That Follows* and Uses Statement")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Follows* (s, _) and Uses (s, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .addSuchThatClause(UsesType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Pattern and Pattern Statement")
{
    AbstractQuery abstractQuery
        = processQuery("assign a1, a2; Select a1 pattern a1 (_, \"x + y\") and a2 (_, \"z * z\")");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("a1")
                                              .addDeclaration("a1", "assign")
                                              .addDeclaration("a2", "assign")
                                              .addAssignPatternClause("a1", AssignPatternType, WildcardRefType, "_",
                                                                      NonExistentType, "x + y", LiteralExpressionType)
                                              .addAssignPatternClause("a2", AssignPatternType, WildcardRefType, "_",
                                                                      NonExistentType, "z * z", LiteralExpressionType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* and Uses and Modifies and Parent Statement")
{
    AbstractQuery abstractQuery = processQuery("stmt s1, s2; variable v; Select s1 such that Follows* (s1, s2) and "
                                               "Uses (s1, v) and Modifies (s2, v) and Parent (s1, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("s1")
              .addDeclaration("s1", "stmt")
              .addDeclaration("s2", "stmt")
              .addDeclaration("v", "variable")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "s1", StmtType, SynonymRefType, "s2", StmtType)
              .addSuchThatClause(UsesType, SynonymRefType, "s1", StmtType, SynonymRefType, "v", VariableType)
              .addSuchThatClause(ModifiesType, SynonymRefType, "s2", StmtType, SynonymRefType, "v", VariableType)
              .addSuchThatClause(ParentType, SynonymRefType, "s1", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Such That Follows* and Pattern Statement")
{
    AbstractQuery abstractQuery
        = processQuery("assign a; Select a and such that Follows* (a, _) and pattern a (_, \"x + y\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Such That Follows* and With Statement")
{
    AbstractQuery abstractQuery
        = processQuery("assign a; Select a and such that Follows* (a, _) and with a.stmt# = 12");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("'and' occurs before clauses")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s and such that Follows* (s, _) and Uses (s, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("'and' occurs between such that identifier")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such and that Follows* (s, _) and Uses (s, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("'and' occurs after clause identifier")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that and Follows* (s, _) and Uses (s, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Two consecutive 'and'")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that and Follows* (s, _) and and Uses (s, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Clause ends with 'and'")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Follows* (s, _) and");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  Miscellaneous                                                                   */
/************************************************************************************/

TEST_CASE("Many Asterisks")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Follows** (s, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("1 Extra Closed Parentheses")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \"(x + y))\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("1 Extra Closed Parentheses 2")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \"(x + (y + z)))\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Inverted Parentheses")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \")x(\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Correct Parentheses")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \"(x)\")");

    REQUIRE(!abstractQuery.isInvalid());
}

TEST_CASE("Vacuously True")
{
    AbstractQuery abstractQuery = processQuery("assign a; stmt s; Select a such that Follows* (s, 5)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addDeclaration("s", "stmt")
              .addSuchThatClause(FollowsStarType, SynonymRefType, "s", StmtType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Keyword as variable names")
{
    AbstractQuery abstractQuery
        = processQuery("assign assign; stmt while; Select assign such that Parent* (while, assign)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("assign")
                                              .addDeclaration("assign", "assign")
                                              .addDeclaration("while", "stmt")
                                              .addSuchThatClause(ParentStarType, SynonymRefType, "while", StmtType,
                                                                 SynonymRefType, "assign", AssignType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Invalid Variable Name")
{
    AbstractQuery abstractQuery
        = processQuery("assign assign; stmt whil&e; Select assign such that Parent* (whil&e, assign)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Select as variable name")
{
    AbstractQuery abstractQuery = processQuery("stmt stmt, Select; Select Select such that Follows(Select, stmt)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("Select")
              .addDeclaration("stmt", "stmt")
              .addDeclaration("Select", "stmt")
              .addSuchThatClause(FollowsType, SynonymRefType, "Select", StmtType, SynonymRefType, "stmt", StmtType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Case sensitivity of keywords")
{
    AbstractQuery abstractQuery = processQuery("Stmt s; Select s");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Case sensitivity of variable names")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select S");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Variable whitespace between tokens")
{
    AbstractQuery abstractQuery = processQuery(
        "stmt     stmt   , Select\n; \n\nSelect  Select\t such\n that    Follows \n (    Select  ,\n stmt  )\n");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("Select")
              .addDeclaration("stmt", "stmt")
              .addDeclaration("Select", "stmt")
              .addSuchThatClause(FollowsType, SynonymRefType, "Select", StmtType, SynonymRefType, "stmt", StmtType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Follows* with a space between between Follows and *")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a such that Follows * (a, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("a")
                                              .addDeclaration("a", "assign")
                                              .addSuchThatClause(FollowsStarType, SynonymRefType, "a", AssignType,
                                                                 WildcardRefType, "_", NonExistentType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("ReadType as left reference of Uses")
{
    AbstractQuery abstractQuery = processQuery("read re; Select re such that Uses(re, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("PrintType as left reference of Modifies")
{
    AbstractQuery abstractQuery = processQuery("print pn; Select pn such that Modifies(pn, _)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Incomplete Such That")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Incomplete Clause")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("No space between Clauses")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a pattern a (_, \"x + y\")such that Uses (a, \"z\")");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addAssignPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y",
                                      LiteralExpressionType)
              .addSuchThatClause(UsesType, SynonymRefType, "a", AssignType, LiteralRefType, "z", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Follows**")
{
    AbstractQuery abstractQuery = processQuery("stmt a; Select a such that Follows** (a, _)");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  Select BOOLEAN                                                                  */
/************************************************************************************/

TEST_CASE("Select BOOLEAN only")
{
    AbstractQuery abstractQuery = processQuery("Select BOOLEAN");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create().build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select BOOLEAN with no Clauses")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select BOOLEAN");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create().addDeclaration("s", "stmt").build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select BOOLEAN with Follows")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select BOOLEAN such that Follows (s, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s", "stmt")
              .addSuchThatClause(FollowsType, SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select BOOLEAN semantically incorrect query")
{
    AbstractQuery abstractQuery = processQuery("read re; Select BOOLEAN such that Parent (re, _)");

    REQUIRE(abstractQuery.toReturnFalseResult());
}

//
// TEST_CASE("Select BOOLEAN semantically then syntactically incorrect query")
//{
//    AbstractQuery abstractQuery = processQuery("read re; Select BOOLEAN such that Parent (re, _) with 5");
//
//    REQUIRE(abstractQuery.isInvalid());
//    REQUIRE(!abstractQuery.toReturnFalseResult());
//}

/************************************************************************************/
/*  Select Tuple                                                                    */
/************************************************************************************/

TEST_CASE("Select Tuple <s1, s2> such that Follows")
{
    AbstractQuery abstractQuery = processQuery("stmt s1, s2; Select <s1, s2> such that Follows (s1, s2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addDeclaration("s2", "stmt")
              .addSelectSynonym("s1")
              .addSelectSynonym("s2")
              .addSuchThatClause(FollowsType, SynonymRefType, "s1", StmtType, SynonymRefType, "s2", StmtType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select Tuple <s1, s2, v1, v2, w, ifs> such that Follows")
{
    AbstractQuery abstractQuery = processQuery(
        "stmt s1, s2; variable v1, v2; while w; if ifs; Select <s1, s2, v1, v2, w, ifs> such that Follows (s1, s2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addDeclaration("s2", "stmt")
              .addDeclaration("v1", "variable")
              .addDeclaration("v2", "variable")
              .addDeclaration("w", "while")
              .addDeclaration("ifs", "if")
              .addSelectSynonym("s1")
              .addSelectSynonym("s2")
              .addSelectSynonym("v1")
              .addSelectSynonym("v2")
              .addSelectSynonym("w")
              .addSelectSynonym("ifs")
              .addSuchThatClause(FollowsType, SynonymRefType, "s1", StmtType, SynonymRefType, "s2", StmtType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select empty tuple")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select <  \n\t \v > such that Follows (s, _)");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  Select Attribute                                                                */
/************************************************************************************/

TEST_CASE("Select stmt#")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s.stmt# such that Uses (s, \"x\")");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s", "stmt")
              .addSelectSynonym("s", "stmt#", StmtType)
              .addSuchThatClause(UsesType, SynonymRefType, "s", StmtType, LiteralRefType, "x", NonExistentType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select procName")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p.procName such that Uses (p, \"x\")");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("p", "procedure")
              .addSelectSynonym("p", "procName", ProcedureType)
              .addSuchThatClause(UsesType, SynonymRefType, "p", ProcedureType, LiteralRefType, "x", NonExistentType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select varName")
{
    AbstractQuery abstractQuery = processQuery("stmt s; variable v; Select v.varName such that Uses (s, v)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s", "stmt")
              .addDeclaration("v", "variable")
              .addSelectSynonym("v", "varName", VariableType)
              .addSuchThatClause(UsesType, SynonymRefType, "s", StmtType, SynonymRefType, "v", VariableType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Select value")
{
    AbstractQuery abstractQuery = processQuery("constant c; Select c.value");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("c", "constant")
                                              .addSelectSynonym("c", "value", ConstantType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

/************************************************************************************/
/*  Advance Relationships                                                           */
/************************************************************************************/

TEST_CASE("Such that Affect between synonym stmts")
{
    AbstractQuery abstractQuery = processQuery("assign a1, a2; Select a1 such that Affects (a1, a2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("a1", "assign")
              .addDeclaration("a2", "assign")
              .addSelectSynonym("a1")
              .addSuchThatClause(AffectsType, SynonymRefType, "a1", AssignType, SynonymRefType, "a2", AssignType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Affect* between synonym stmts")
{
    AbstractQuery abstractQuery = processQuery("assign a1, a2; Select a1 such that Affects* (a1, a2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("a1", "assign")
              .addDeclaration("a2", "assign")
              .addSelectSynonym("a1")
              .addSuchThatClause(AffectsStarType, SynonymRefType, "a1", AssignType, SynonymRefType, "a2", AssignType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Next between synonym stmts")
{
    AbstractQuery abstractQuery = processQuery("stmt s1, s2; Select s1 such that Next (s1, s2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addDeclaration("s2", "stmt")
              .addSelectSynonym("s1")
              .addSuchThatClause(NextType, SynonymRefType, "s1", StmtType, SynonymRefType, "s2", StmtType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Next* between synonym stmts")
{
    AbstractQuery abstractQuery = processQuery("stmt s1, s2; Select s1 such that Next* (s1, s2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addDeclaration("s2", "stmt")
              .addSelectSynonym("s1")
              .addSuchThatClause(NextStarType, SynonymRefType, "s1", StmtType, SynonymRefType, "s2", StmtType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Calls between synonym stmts")
{
    AbstractQuery abstractQuery = processQuery("procedure p1, p2; Select p1 such that Calls (p1, p2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("p1", "procedure")
              .addDeclaration("p2", "procedure")
              .addSelectSynonym("p1")
              .addSuchThatClause(CallsType, SynonymRefType, "p1", ProcedureType, SynonymRefType, "p2", ProcedureType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Calls* between synonym procedures")
{
    AbstractQuery abstractQuery = processQuery("procedure p1, p2; Select p1 such that Calls* (p1, p2)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("p1", "procedure")
                                              .addDeclaration("p2", "procedure")
                                              .addSelectSynonym("p1")
                                              .addSuchThatClause(CallsStarType, SynonymRefType, "p1", ProcedureType,
                                                                 SynonymRefType, "p2", ProcedureType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Affect between Wildcards")
{
    AbstractQuery abstractQuery = processQuery("assign a1; Select a1 such that Affects (_, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("a1", "assign")
                                              .addSelectSynonym("a1")
                                              .addSuchThatClause(AffectsType, WildcardRefType, "_", NonExistentType,
                                                                 WildcardRefType, "_", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Affect between Integers")
{
    AbstractQuery abstractQuery = processQuery("assign a1; Select a1 such that Affects (1, 2)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("a1", "assign")
                                              .addSelectSynonym("a1")
                                              .addSuchThatClause(AffectsType, IntegerRefType, "1", NonExistentType,
                                                                 IntegerRefType, "2", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Affect* between synonym stmt and Wildcard")
{
    AbstractQuery abstractQuery = processQuery("assign a1; Select a1 such that Affects* (a1, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("a1", "assign")
                                              .addSelectSynonym("a1")
                                              .addSuchThatClause(AffectsStarType, SynonymRefType, "a1", AssignType,
                                                                 WildcardRefType, "_", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Affect* between synonym stmt and Integer")
{
    AbstractQuery abstractQuery = processQuery("assign a1; Select a1 such that Affects* (a1, 4)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("a1", "assign")
                                              .addSelectSynonym("a1")
                                              .addSuchThatClause(AffectsStarType, SynonymRefType, "a1", AssignType,
                                                                 IntegerRefType, "4", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Affect* between Wildcard and Integer")
{
    AbstractQuery abstractQuery = processQuery("assign a1; Select a1 such that Affects* (_, 4)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("a1", "assign")
                                              .addSelectSynonym("a1")
                                              .addSuchThatClause(AffectsStarType, WildcardRefType, "_", NonExistentType,
                                                                 IntegerRefType, "4", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Next between Wildcards")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Next (_, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addSelectSynonym("s1")
              .addSuchThatClause(NextType, WildcardRefType, "_", NonExistentType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Next between Integers")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Next (1, 2)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addSelectSynonym("s1")
              .addSuchThatClause(NextType, IntegerRefType, "1", NonExistentType, IntegerRefType, "2", NonExistentType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Next* between synonym stmt and Wildcard")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Next* (s1, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addSelectSynonym("s1")
              .addSuchThatClause(NextStarType, SynonymRefType, "s1", StmtType, WildcardRefType, "_", NonExistentType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Next* between synonym stmt and Integer")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Next* (s1, 4)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addDeclaration("s1", "stmt")
              .addSelectSynonym("s1")
              .addSuchThatClause(NextStarType, SynonymRefType, "s1", StmtType, IntegerRefType, "4", NonExistentType)
              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Next* between Wildcard and Integer")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Next* (_, 4)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("s1", "stmt")
                                              .addSelectSynonym("s1")
                                              .addSuchThatClause(NextStarType, WildcardRefType, "_", NonExistentType,
                                                                 IntegerRefType, "4", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Calls between Wildcards")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Calls (_, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("s1", "stmt")
                                              .addSelectSynonym("s1")
                                              .addSuchThatClause(CallsType, WildcardRefType, "_", NonExistentType,
                                                                 WildcardRefType, "_", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Calls between Literal")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Calls (\"proc1\", \"proc2\")");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("s1", "stmt")
                                              .addSelectSynonym("s1")
                                              .addSuchThatClause(CallsType, LiteralRefType, "proc1", NonExistentType,
                                                                 LiteralRefType, "proc2", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Calls* between synonym procedure and Wildcard")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p such that Calls* (p, _)");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("p", "procedure")
                                              .addSelectSynonym("p")
                                              .addSuchThatClause(CallsStarType, SynonymRefType, "p", ProcedureType,
                                                                 WildcardRefType, "_", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Calls* between synonym procedure and Literal")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p such that Calls* (p, \"proc1\")");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("p", "procedure")
                                              .addSelectSynonym("p")
                                              .addSuchThatClause(CallsStarType, SynonymRefType, "p", ProcedureType,
                                                                 LiteralRefType, "proc1", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Such that Calls* between Wildcard and Literal")
{
    AbstractQuery abstractQuery = processQuery("stmt s1; Select s1 such that Calls* (_, \"proc1\")");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addDeclaration("s1", "stmt")
                                              .addSelectSynonym("s1")
                                              .addSuchThatClause(CallsStarType, WildcardRefType, "_", NonExistentType,
                                                                 LiteralRefType, "proc1", NonExistentType)
                                              .build();

    bool equal = abstractQuery == expectedAbstractQuery;

    REQUIRE(equal);
}

TEST_CASE("Invalid such that Affects between read stmt")
{
    AbstractQuery abstractQuery = processQuery("read re1, re2; Select re1 such that Affects (re1, re2)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Affects between print stmt")
{
    AbstractQuery abstractQuery = processQuery("print pn1, pn2; Select pn1 such that Affects (pn1, pn2)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Affects between while stmt")
{
    AbstractQuery abstractQuery = processQuery("while w1, w2; Select w1 such that Affects (w1, w2)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Affects between if stmt")
{
    AbstractQuery abstractQuery = processQuery("if ifs1, ifs2; Select ifs1 such that Affects (ifs1, ifs2)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Affects between call stmt")
{
    AbstractQuery abstractQuery = processQuery("call cl1, cl2; Select cl1 such that Affects (cl1, cl2)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Affects between procedures")
{
    AbstractQuery abstractQuery = processQuery("procedure p1, p2; Select p1 such that Affects (p1, p2)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Next between procedures")
{
    AbstractQuery abstractQuery = processQuery("procedure p1, p2; Select p1 such that Next (p1, p2)");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Affect with Literal")
{
    AbstractQuery abstractQuery = processQuery("assign a; Select a such that Affect (a, \"proc\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Next with Literal")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s such that Next (s, \"proc\")");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("Invalid such that Calls with Integer")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p such that Calls (p, 2)");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  With Clause                                                                     */
/************************************************************************************/

TEST_CASE("With stmt# and value")
{
    AbstractQuery abstractQuery = processQuery("stmt s; constant c; Select s with s.stmt# = c.value");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s")
                                              .addDeclaration("s", "stmt")
                                              .addDeclaration("c", "constant")
                                              .addWithClause(AttributeRefType, "s", StmtType, StmtNumberType,
                                                             AttributeRefType, "c", ConstantType, ValueType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With assign stmt# and value")
{
    AbstractQuery abstractQuery = processQuery("assign a; constant c; Select a with a.stmt# = c.value");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("a")
                                              .addDeclaration("a", "assign")
                                              .addDeclaration("c", "constant")
                                              .addWithClause(AttributeRefType, "a", AssignType, StmtNumberType,
                                                             AttributeRefType, "c", ConstantType, ValueType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With value and stmt#")
{
    AbstractQuery abstractQuery = processQuery("stmt s; constant c; Select s with c.value = s.stmt#");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s")
                                              .addDeclaration("s", "stmt")
                                              .addDeclaration("c", "constant")
                                              .addWithClause(AttributeRefType, "c", ConstantType, ValueType,
                                                             AttributeRefType, "s", StmtType, StmtNumberType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With stmt# and prog_line")
{
    AbstractQuery abstractQuery = processQuery("stmt s; prog_line pl; Select s with s.stmt# = pl");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s")
                                              .addDeclaration("s", "stmt")
                                              .addDeclaration("pl", "prog_line")
                                              .addWithClause(AttributeRefType, "s", StmtType, StmtNumberType,
                                                             SynonymRefType, "pl", Prog_LineType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With value and prog_line")
{
    AbstractQuery abstractQuery = processQuery("constant c; prog_line pl; Select c.value with c.value = pl");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("c", "value", ConstantType)
                                              .addDeclaration("c", "constant")
                                              .addDeclaration("pl", "prog_line")
                                              .addWithClause(AttributeRefType, "c", ConstantType, ValueType,
                                                             SynonymRefType, "pl", Prog_LineType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With stmt# and integer")
{
    AbstractQuery abstractQuery = processQuery("stmt s; constant c; Select s with s.stmt# = 5");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s")
                                              .addDeclaration("s", "stmt")
                                              .addDeclaration("c", "constant")
                                              .addWithClause(AttributeRefType, "s", StmtType, StmtNumberType,
                                                             IntegerRefType, "5", NonExistentType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With value and integer")
{
    AbstractQuery abstractQuery = processQuery("constant c; Select c with c.value = 5");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("c")
                                              .addDeclaration("c", "constant")
                                              .addWithClause(AttributeRefType, "c", ConstantType, ValueType,
                                                             IntegerRefType, "5", NonExistentType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With prog_line and integer")
{
    AbstractQuery abstractQuery = processQuery("prog_line pl; Select pl with pl = 5");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("pl")
                                              .addDeclaration("pl", "prog_line")
                                              .addWithClause(SynonymRefType, "pl", Prog_LineType, NoAttributeType,
                                                             IntegerRefType, "5", NonExistentType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With stmt# and stmt#")
{
    AbstractQuery abstractQuery = processQuery("stmt s1, s2; Select s1 with s1.stmt# = s2.stmt#");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s1")
                                              .addDeclaration("s1", "stmt")
                                              .addDeclaration("s2", "stmt")
                                              .addWithClause(AttributeRefType, "s1", StmtType, StmtNumberType,
                                                             AttributeRefType, "s2", StmtType, StmtNumberType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With value and value")
{
    AbstractQuery abstractQuery = processQuery("constant c1, c2; Select c1 with c1.value = c2.value");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("c1")
                                              .addDeclaration("c1", "constant")
                                              .addDeclaration("c2", "constant")
                                              .addWithClause(AttributeRefType, "c1", ConstantType, ValueType,
                                                             AttributeRefType, "c2", ConstantType, ValueType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With prog_line and prog_line")
{
    AbstractQuery abstractQuery = processQuery("prog_line pl1, pl2; Select pl1 with pl1 = pl2");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("pl1")
                                              .addDeclaration("pl1", "prog_line")
                                              .addDeclaration("pl2", "prog_line")
                                              .addWithClause(SynonymRefType, "pl1", Prog_LineType, NoAttributeType,
                                                             SynonymRefType, "pl2", Prog_LineType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With integer and integer")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s with 5 = 5");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s")
                                              .addDeclaration("s", "stmt")
                                              .addWithClause(IntegerRefType, "5", NonExistentType, NoAttributeType,
                                                             IntegerRefType, "5", NonExistentType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With stmt# and literal")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s with s.stmt# = \"five\"");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("With value and literal")
{
    AbstractQuery abstractQuery = processQuery("constant c; Select c with c.value = \"five\"");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("With prog_line and literal")
{
    AbstractQuery abstractQuery = processQuery("prog_line pl; Select pl with pl = \"five\"");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("With integer and literal")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s with 5 = \"five\"");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("With procName and varName")
{
    AbstractQuery abstractQuery = processQuery("procedure p; variable v; Select p with p.procName = v.varName");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("p")
                                              .addDeclaration("p", "procedure")
                                              .addDeclaration("v", "variable")
                                              .addWithClause(AttributeRefType, "p", ProcedureType, ProcNameType,
                                                             AttributeRefType, "v", VariableType, VarNameType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With procName and literal")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p with p.procName = \"main\"");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("p")
                                              .addDeclaration("p", "procedure")
                                              .addWithClause(AttributeRefType, "p", ProcedureType, ProcNameType,
                                                             LiteralRefType, "main", NonExistentType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With varName and literal")
{
    AbstractQuery abstractQuery = processQuery("variable v; Select v with v.varName = \"x\"");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("v")
                                              .addDeclaration("v", "variable")
                                              .addWithClause(AttributeRefType, "v", VariableType, VarNameType,
                                                             LiteralRefType, "x", NonExistentType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With procName and procName")
{
    AbstractQuery abstractQuery = processQuery("procedure p1, p2; Select p1 with p1.procName = p2.procName");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("p1")
                                              .addDeclaration("p1", "procedure")
                                              .addDeclaration("p2", "procedure")
                                              .addWithClause(AttributeRefType, "p1", ProcedureType, ProcNameType,
                                                             AttributeRefType, "p2", ProcedureType, ProcNameType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With varName and varName")
{
    AbstractQuery abstractQuery = processQuery("variable v1, v2; Select v1 with v1.varName = v2.varName");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("v1")
                                              .addDeclaration("v1", "variable")
                                              .addDeclaration("v2", "variable")
                                              .addWithClause(AttributeRefType, "v1", VariableType, VarNameType,
                                                             AttributeRefType, "v2", VariableType, VarNameType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With literal and literal")
{
    AbstractQuery abstractQuery = processQuery("stmt s; Select s with \"main\" = \"main\"");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s")
                                              .addDeclaration("s", "stmt")
                                              .addWithClause(LiteralRefType, "main", NonExistentType, NoAttributeType,
                                                             LiteralRefType, "main", NonExistentType, NoAttributeType)
                                              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("With procName and integer")
{
    AbstractQuery abstractQuery = processQuery("procedure p; Select p with p.procName = 5");

    REQUIRE(abstractQuery.isInvalid());
}

TEST_CASE("With procName and prog_line")
{
    AbstractQuery abstractQuery = processQuery("procedure p; prog_line pl; Select p with p.procName = pl");

    REQUIRE(abstractQuery.isInvalid());
}

/************************************************************************************/
/*  Complex Queries                                                                 */
/************************************************************************************/

TEST_CASE("Complex Queries 1")
{
    AbstractQuery abstractQuery = processQuery(
        R"(assign a1, a2; while w1, w2; Select a2 pattern a1 ("x", _) and a2 ("x",_"x"_) such that Affects (a1, a2) and Parent* (w2, a2) and Parent* (w1, w2))");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a2")
              .addDeclaration("a1", "assign")
              .addDeclaration("a2", "assign")
              .addDeclaration("w1", "while")
              .addDeclaration("w2", "while")
              .addAssignPatternClause("a1", AssignPatternType, LiteralRefType, "x", NonExistentType, "_",
                                      WildcardExpressionType)
              .addAssignPatternClause("a2", AssignPatternType, LiteralRefType, "x", NonExistentType, "x",
                                      ExtendableLiteralExpressionType)
              .addSuchThatClause(AffectsType, SynonymRefType, "a1", AssignType, SynonymRefType, "a2", AssignType)
              .addSuchThatClause(ParentStarType, SynonymRefType, "w2", WhileType, SynonymRefType, "a2", AssignType)
              .addSuchThatClause(ParentStarType, SynonymRefType, "w1", WhileType, SynonymRefType, "w2", WhileType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Complex Queries 2")
{
    AbstractQuery abstractQuery
        = processQuery(R"(while w1, w2, w3; Select <w1, w2, w3> such that Parent* (w1, w2) and Parent* (w2, w3))");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("w1")
              .addSelectSynonym("w2")
              .addSelectSynonym("w3")
              .addDeclaration("w1", "while")
              .addDeclaration("w2", "while")
              .addDeclaration("w3", "while")
              .addSuchThatClause(ParentStarType, SynonymRefType, "w1", WhileType, SynonymRefType, "w2", WhileType)
              .addSuchThatClause(ParentStarType, SynonymRefType, "w2", WhileType, SynonymRefType, "w3", WhileType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Complex Queries 3")
{
    AbstractQuery abstractQuery = processQuery("assign a; while w; prog_line n; Select a such that Parent* (w, a) and "
                                               "Next* (60, n) pattern a(\"x\", _) with a.stmt# = n");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addDeclaration("w", "while")
              .addDeclaration("n", "prog_line")
              .addSuchThatClause(ParentStarType, SynonymRefType, "w", WhileType, SynonymRefType, "a", AssignType)
              .addSuchThatClause(NextStarType, IntegerRefType, "60", NonExistentType, SynonymRefType, "n",
                                 Prog_LineType)
              .addAssignPatternClause("a", AssignPatternType, LiteralRefType, "x", NonExistentType, "_",
                                      WildcardExpressionType)
              .addWithClause(AttributeRefType, "a", AssignType, StmtNumberType, SynonymRefType, "n", Prog_LineType,
                             NoAttributeType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Complex Queries 4")
{
    AbstractQuery abstractQuery = processQuery(
        "while w; assign a; print p; variable v; Select a such that Uses(a, v) and Uses(p, v) pattern a(v, _)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addDeclaration("w", "while")
              .addDeclaration("p", "print")
              .addDeclaration("v", "variable")
              .addSuchThatClause(UsesType, SynonymRefType, "a", AssignType, SynonymRefType, "v", VariableType)
              .addSuchThatClause(UsesType, SynonymRefType, "p", PrintType, SynonymRefType, "v", VariableType)
              .addAssignPatternClause("a", AssignPatternType, SynonymRefType, "v", VariableType, "_",
                                      WildcardExpressionType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}