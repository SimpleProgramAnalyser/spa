/**
 * Tests for the parsing of queries with "with" clauses.
 */

#include "AbstractQueryBuilder.h"
#include "PreprocessorTestingUtils.h"
#include "catch.hpp"

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

TEST_CASE("Spaces between synonym and attribute in Select is parsed correctly")
{
    AbstractQuery abstractQuery1 = processQuery(
        "assign s; constant c; Select s       .      stmt# such that Follows(8, 9) with c   . value      = s . stmt#");
    AbstractQuery abstractQuery2 = processQuery(
        "assign s; constant c; Select s.      stmt# such that Follows(8, 9) with c   . value      = s . stmt#");
    AbstractQuery abstractQuery3 = processQuery(
        "assign s; constant c; Select s       .stmt# such that Follows(8, 9) with c   . value      = s . stmt#");
    AbstractQuery abstractQuery4 = processQuery(
        "assign s; constant c; Select s.stmt# such that Follows(8, 9) with c   . value      = s . stmt#");

    AbstractQuery expectedAbstractQuery = AbstractQueryBuilder::create()
                                              .addSelectSynonym("s", "stmt#", AssignType)
                                              .addDeclaration("s", "assign")
                                              .addDeclaration("c", "constant")
                                              .addSuchThatClause(FollowsType, IntegerRefType, "8", NonExistentType,
                                                                 IntegerRefType, "9", NonExistentType)
                                              .addWithClause(AttributeRefType, "c", ConstantType, ValueType,
                                                             AttributeRefType, "s", AssignType, StmtNumberType)
                                              .build();

    REQUIRE(abstractQuery1 == expectedAbstractQuery);
    REQUIRE(abstractQuery2 == expectedAbstractQuery);
    REQUIRE(abstractQuery3 == expectedAbstractQuery);
    REQUIRE(abstractQuery4 == expectedAbstractQuery);
}

TEST_CASE("Syntactically invalid with clause returns syntax error")
{
    AbstractQuery abstractQuery = processQuery("read re; Select BOOLEAN such that Parent (re, _) with 5");
    REQUIRE(abstractQuery.isSyntacticallyInvalid());
}
