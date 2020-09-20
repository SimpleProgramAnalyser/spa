#include "AbstractQueryBuilder.h"
#include "catch.hpp"
#include "pql/preprocessor/AqTypes.h"
#include "pql/preprocessor/Preprocessor.h"

AbstractQuery processQuery(String queryString)
{
    Preprocessor preprocessor;
    return preprocessor.processQuery(queryString);
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
              .addSuchThatClause("Follows", SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
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
                                              .addPatternClause("a", AssignPatternType, WildcardRefType, "_",
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
              .addSuchThatClause("Follows*", SynonymRefType, "a", StmtType, WildcardRefType, "_", NonExistentType)
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
              .addSuchThatClause("Parent", SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
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
              .addSuchThatClause("Parent*", SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
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
              .addSuchThatClause("Uses", SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
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
              .addSuchThatClause("Modifies", SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
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
              .addSuchThatClause("Follows*", SynonymRefType, "re", ReadType, IntegerRefType, "5", NonExistentType)
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
              .addSuchThatClause("Follows*", SynonymRefType, "pn", PrintType, IntegerRefType, "5", NonExistentType)
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
              .addSuchThatClause("Follows*", SynonymRefType, "cl", CallType, IntegerRefType, "5", NonExistentType)
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
              .addSuchThatClause("Follows*", SynonymRefType, "w", WhileType, IntegerRefType, "5", NonExistentType)
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
              .addSuchThatClause("Follows*", SynonymRefType, "ifs", IfType, IntegerRefType, "5", NonExistentType)
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
              .addSuchThatClause("Follows*", SynonymRefType, "a", AssignType, IntegerRefType, "5", NonExistentType)
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

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("a")
              .addDeclaration("a", "assign")
              .addDeclaration("re", "read")
              .addSuchThatClause("Parent", SynonymRefType, "a", AssignType, SynonymRefType, "re", ReadType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
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
              .addSuchThatClause("Parent*", SynonymRefType, "w", WhileType, SynonymRefType, "re", ReadType)
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
              .addSuchThatClause("Follows*", SynonymRefType, "s", StmtType, SynonymRefType, "pn", PrintType)
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
              .addSuchThatClause("Parent", SynonymRefType, "ifs", IfType, SynonymRefType, "cl", CallType)
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
              .addSuchThatClause("Follows", SynonymRefType, "w", WhileType, SynonymRefType, "ifs", IfType)
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
              .addSuchThatClause("Modifies", SynonymRefType, "w", WhileType, SynonymRefType, "v", VariableType)
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
              .addSuchThatClause("Uses", SynonymRefType, "ifs", IfType, LiteralRefType, "x", NonExistentType)
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
              .addPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y * z + a % d",
                                LiteralExpressionType)
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
                                              .addPatternClause("asdf", AssignPatternType, WildcardRefType, "_",
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
              .addPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y * z + a % d",
                                ExtendableLiteralExpressionType)
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
              .addPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y * (z + a)",
                                ExtendableLiteralExpressionType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
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
              .addPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y",
                                LiteralExpressionType)
              .addSuchThatClause("Uses", SynonymRefType, "a", AssignType, LiteralRefType, "z", NonExistentType)
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
              .addSuchThatClause("Uses", SynonymRefType, "a", AssignType, LiteralRefType, "z", NonExistentType)
              .addPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "x + y",
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
              .addSuchThatClause("Follows*", SynonymRefType, "s", StmtType, SynonymRefType, "a", AssignType)
              .addPatternClause("a", AssignPatternType, WildcardRefType, "_", NonExistentType, "_",
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
              .addSuchThatClause("Follows*", SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
              .addSuchThatClause("Uses", SynonymRefType, "s", StmtType, WildcardRefType, "_", NonExistentType)
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
                                              .addPatternClause("a1", AssignPatternType, WildcardRefType, "_",
                                                                NonExistentType, "x + y", LiteralExpressionType)
                                              .addPatternClause("a2", AssignPatternType, WildcardRefType, "_",
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
              .addSuchThatClause("Follows*", SynonymRefType, "s1", StmtType, SynonymRefType, "s2", StmtType)
              .addSuchThatClause("Uses", SynonymRefType, "s1", StmtType, SynonymRefType, "v", VariableType)
              .addSuchThatClause("Modifies", SynonymRefType, "s2", StmtType, SynonymRefType, "v", VariableType)
              .addSuchThatClause("Parent", SynonymRefType, "s1", StmtType, WildcardRefType, "_", NonExistentType)
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

// TODO: Uncomment it when with clause is implemented
// TEST_CASE("Such That Follows* and With Statement")
//{
//    AbstractQuery abstractQuery
//        = processQuery("assign a; Select a and such that Follows* (a, _) and with a.stmt# = 12");
//
//    REQUIRE(abstractQuery.isInvalid());
//}

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
              .addSuchThatClause("Follows*", SynonymRefType, "s", StmtType, IntegerRefType, "5", NonExistentType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Keyword as variable names")
{
    AbstractQuery abstractQuery
        = processQuery("assign assign; stmt while; Select assign such that Parent* (while, assign)");

    AbstractQuery expectedAbstractQuery
        = AbstractQueryBuilder::create()
              .addSelectSynonym("assign")
              .addDeclaration("assign", "assign")
              .addDeclaration("while", "stmt")
              .addSuchThatClause("Parent*", SynonymRefType, "while", StmtType, SynonymRefType, "assign", AssignType)
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
              .addSuchThatClause("Follows", SynonymRefType, "Select", StmtType, SynonymRefType, "stmt", StmtType)
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
              .addSuchThatClause("Follows", SynonymRefType, "Select", StmtType, SynonymRefType, "stmt", StmtType)
              .build();

    bool equals = abstractQuery == expectedAbstractQuery;

    REQUIRE(equals);
}

TEST_CASE("Follows* with a space between between Follows and *")
{
    AbstractQuery abstractQuery = processQuery("stmt sa; Select sa such that Follows * (sa, _)");

    REQUIRE(abstractQuery.isInvalid());
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