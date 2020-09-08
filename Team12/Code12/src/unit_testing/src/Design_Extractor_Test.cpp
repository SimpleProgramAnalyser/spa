#include "catch.hpp"
/*
#include "frontend/designExtractor/DesignExtractor.cpp"
*/
#include "ast/AstLibrary.cpp"
#include "ast/AstLibrary.h"
#include "ast/AstTypes.h"
#include "ast/AstTypes.cpp"

#include<string>  

/*
void require(bool b)
{
    REQUIRE(b);
}

TEST_CASE("1st Test")
{


    require(1 == 1);
} */


/*
TEST_CASE("Design Extractor can spot semantic errors correctly") 
{
    
    List<StatementNode> statements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(3, Variable("num3"))));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3"))
    )));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("ave"), createDivExpr(createRefExpr("sum"), createRefExpr(3)))
        ));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(6, Variable("ave"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* computeProcedure = createProcedureNode("compute", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* programNode = createProgramNode("SimpleProgram", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.checkProgramValidity();

    REQUIRE(isSemanticallyValid == true);
    
}
*/