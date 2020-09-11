#include "catch.hpp"
/*
#include "frontend/designExtractor/DesignExtractor.cpp"
*/
#include "ast/AstLibrary.cpp"
#include "ast/AstLibrary.h"
#include "ast/AstTypes.h"

#include "frontend/designExtractor/SemanticErrorsValidator.h"
#include<string>  


String computeAverage = "\
procedure compute {\n\
\n\
    read num1;\n\
    read num2;\n\
    read num3;\n\
\n\
    sum = (num1 + num2) * num3;\n\
    ave = sum / 3;\n\
\n\
    print ave;\n\
}";

TEST_CASE("Design Extractor identifies semantically valid program as valid - computeAverage")
{
    List<StatementNode> statements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(3, Variable("num3"))));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        4, Variable("sum"),
        createTimesExpr(createPlusExpr(createRefExpr("num1"), createRefExpr("num2")), createRefExpr("num3")))));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("ave"), createDivExpr(createRefExpr("sum"), createRefExpr(3)))));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(6, Variable("ave"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* computeProcedure = createProcedureNode("compute", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* programNode = createProgramNode("computeAverage", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == true);
}

String printAscending = "\
procedure printAscending {\n\
\n\
    read num1;\n\
    read num2;\n\
    noSwap = 0;\n\
\n\
    if(num1 > num2) then {\n\
        temp = num1;\n\
        num1 = num2;\n\
        num2 = temp;\n\
    } else {\n\
     noSwap = 1;\n\
    }\n\
    print num1;\n\
    print num2;\n\
    print noSwap;\n\
}";

TEST_CASE("Design Extractor identifies semantically valid program with if as valid - printAscending")
{
    List<StatementNode> statements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("noSwap"), createRefExpr(0))));


    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // If statements
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("temp"), createRefExpr("num1"))));
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("num1"), createRefExpr("num2"))));
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(7, Variable("num2"), createRefExpr("temp"))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
   // Else statements
    elseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("noSwap"), createRefExpr(1))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    statements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createGtExpr(createRefExpr("num1"), createRefExpr("num2")), ifStmtLstNode, elseStmtLstNode)));


    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(9, Variable("num1"))));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(10, Variable("num2"))));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(11, Variable("noSwap"))));


    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* computeProcedure = createProcedureNode("printAscending", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* programNode = createProgramNode("printAscending", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == true);
}


String sumDigits = "\
procedure sumDigit {\n\
\n\
    read number;\n\
    sum = 0;\n\
    while(number > 0) {\n\
        digit = number % 10;\n\
        sum = sum + digit;\n\
        number = number / 10;\n\
    }\n\
\n\
    print sum;\n\
}";


TEST_CASE("Design Extractor identifies semantically valid program with while as valid - sumDigits") 
{
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        4, Variable("digit"),createModExpr(createRefExpr("num"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("sum"), createRefExpr(0)), whileStmtLstNode)));

    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("sum"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* computeProcedure = createProcedureNode("sumDigits", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* programNode = createProgramNode("sumDigits", procedureList);


    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == true);
}

String sumDigitsDuplicate = "\
procedure sumDigit {\n\
\n\
    read number;\n\
    sum = 0;\n\
    while(number > 0) {\n\
        digit = number % 10;\n\
        sum = sum + digit;\n\
        number = number / 10;\n\
    }\n\
\n\
    print sum;\n\
}\n\
procedure sumDigit {\n\
\n\
    read number;\n\
    sum = 0;\n\
    while(number > 0) {\n\
        digit = number % 10;\n\
        sum = sum + digit;\n\
        number = number / 10;\n\
    }\n\
\n\
    print sum;\n\
}";

TEST_CASE("Design Extractor identifies semantically invalid program with duplicate procedure name as invalid - sumDigitsDuplicate")
{
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("num"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("sum"), createRefExpr(0)), whileStmtLstNode)));

    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("sum"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* sumDigit1Node = createProcedureNode("sumDigit", stmtLstNode);
    ProcedureNode* sumDigit2Node = createProcedureNode("sumDigit", stmtLstNode);

    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigit1Node));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigit2Node));
    ProgramNode* programNode = createProgramNode("sumDigitsDuplicate", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == false);
}

String multiProcedureProgram = "\
procedure main {\n\
\n\
    flag = 0;\n\
    call computeCentroid;\n\
    call printResults;\n\
}\n\
\n\
procedure readPoint{\n\
     read x;\n\
     read y;\n\
}\n\
\n\
procedure printResults {\n\
    print flag;\n\
    print cenX;\n\
    print cenY;\n\
    print normSq;\n\
}\n\
\n\
procedure computeCentroid {\n\
\n\
    count = 0;\n\
    cenX = 0;\n\
    cenY = 0;\n\
    call readPoint;\n\
    while((x != 0) && (y != 0)) {\n\
        count = count + 1;\n\
        cenX = cenX + x;\n\
        cenY = cenY + y;\n\
        call readPoint;\n\
    }\n\
    if(count == 0) then {\n\
        flag = 1;\n\
    } else {\n\
        cenX = cenX/ count;\n\
        cenY = cenY / count;\n\
    }\n\
\n\
    normSq = cenX * cenY + cenY * cenY;\n\
}";


TEST_CASE("Design Extractor identifies semantically valid program with while and if as valid - multiProcedureProgram")
{
    List<StatementNode> mainStatements;
    List<StatementNode> readPointStatements;
    List<StatementNode> printResultsStatements;
    List<StatementNode> computeCentroidStatements;

    List<ProcedureNode> procedureList;

    //main 
    mainStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(1, Variable("flag"), createRefExpr(0))));
    mainStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(2, "computeCentroid")));
    mainStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(3, "printResults")));
    StmtlstNode* mainStmtLstNode = createStmtlstNode(mainStatements);
    ProcedureNode* mainProc = createProcedureNode("main", mainStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(mainProc));

    //readPoint
    readPointStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(4, Variable("x"))));
    readPointStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(5, Variable("y"))));
    StmtlstNode* readPointStmtLstNode = createStmtlstNode(readPointStatements);
    ProcedureNode* readPointProc = createProcedureNode("readPoint", readPointStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(readPointProc));

    //printResults
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(6, Variable("flag"))));
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("cenX"))));
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("cenY"))));
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(9, Variable("normSq"))));
    StmtlstNode* printResultsStmtLstNode = createStmtlstNode(printResultsStatements);
    ProcedureNode* printResultsProc = createProcedureNode("printResults", printResultsStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(printResultsProc));


    //Compute Centroid
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("count"), createRefExpr(0))));
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(11, Variable("cenX"), createRefExpr(0))));
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("cenY"), createRefExpr(0))));
    computeCentroidStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(13, "readPoint")));

    //Compute Centroid - White statement
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(15, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(16, Variable("cenX"), createPlusExpr(createRefExpr("cenX"), createRefExpr("x")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(17, Variable("cenY"), createPlusExpr(createRefExpr("cenY"), createRefExpr("y")))));
    whileStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(18, "readPoint")));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    computeCentroidStatements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(14,
                        createAndExpr(createNeqExpr(createRefExpr("x"), (createRefExpr(0))),
                                      createNeqExpr(createRefExpr("y"), (createRefExpr(0)))),
                        whileStmtLstNode)));


    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    //Compute Centroid - If statement
    ifStatements.push_back(
    std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("flag"), createRefExpr(1))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    //Compute Centroid - Else statement
    elseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("cenX"), createDivExpr(createRefExpr("cenX"), createRefExpr("count")))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(22, Variable("cenY"), createDivExpr(createRefExpr("cenY"), createRefExpr("count")))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    computeCentroidStatements.push_back(
        std::unique_ptr<IfStatementNode>(createIfNode(19, createEqExpr(createRefExpr("count"), createRefExpr(0)), ifStmtLstNode, elseStmtLstNode)));
    
    computeCentroidStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(23, Variable("normSq"), createPlusExpr(createTimesExpr(createRefExpr("cenX"), createRefExpr("cenX")), createTimesExpr(createRefExpr("cenY"), createRefExpr("cenY"))))));
    StmtlstNode* computeCentroidStmtLstNode = createStmtlstNode(computeCentroidStatements);
    ProcedureNode* computeCentroidProc = createProcedureNode("computeCentroid", computeCentroidStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeCentroidProc));

    ProgramNode* programNode = createProgramNode("SimpleProgram", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == true);
}


String sumDigitPlusCyclicCall = "\
procedure sumDigit {\n\
\n\
    read number;\n\
    sum = 0;\n\
    while(number > 0) {\n\
        digit = number % 10;\n\
        sum = sum + digit;\n\
        number = number / 10;\n\
    }\n\
\n\
    call plus;\n\
    print sum;\n\
}\n\
procedure plus {\n\
\n\
    sum = 0;\n\
    print sum;\n\
    call sumDigit;\n\
}";

TEST_CASE("Design Extractor identifies semantically invalid program with cyclic calls as invalid - sumDigit and plus")
{
    //sumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("num"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("sum"), createRefExpr(0)), whileStmtLstNode)));

    statements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(7, "plus")));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("sum"))));
    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* sumDigitsProcedureNode = createProcedureNode("sumDigit", stmtLstNode);

    // plus
    List<StatementNode> plusStatements;
    plusStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("sum"), createRefExpr(0))));
    plusStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(10, Variable("sum"))));
    plusStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(11, "sumDigit")));
    StmtlstNode* plusStmtLstNode = createStmtlstNode(plusStatements);
    ProcedureNode* plusProcedureNode = createProcedureNode("plus", plusStmtLstNode);


    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigitsProcedureNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(plusProcedureNode));
    ProgramNode* programNode = createProgramNode("sumDigitPlusCyclicCall", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == false);
}


String sumDigitPlusMultiplyCyclicCall = "\
procedure sumDigit {\n\
\n\
    read number;\n\
    sum = 0;\n\
    while(number > 0) {\n\
        digit = number % 10;\n\
        sum = sum + digit;\n\
        number = number / 10;\n\
    }\n\
\n\
    call plus;\n\
    print sum;\n\
}\n\
\n\
procedure plus {\n\
\n\
    sum = 0;\n\
    print sum;\n\
    call multiply;\n\
}\n\
\n\
procedure multiply {\n\
\n\
    print number;\n\
    call sumDigit;\n\
}";

TEST_CASE("Design Extractor identifies semantically invalid program with cyclic calls as invalid - sumDigit, plus and minus")
{
    // sumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("num"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("sum"), createRefExpr(0)), whileStmtLstNode)));

    statements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(7, "plus")));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("sum"))));
    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* sumDigitsProcedureNode = createProcedureNode("sumDigit", stmtLstNode);

    // plus
    List<StatementNode> plusStatements;
    plusStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("sum"), createRefExpr(0))));
    plusStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(10, Variable("sum"))));
    plusStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(11, "multiply")));
    StmtlstNode* plusStmtLstNode = createStmtlstNode(plusStatements);
    ProcedureNode* plusProcedureNode = createProcedureNode("plus", plusStmtLstNode);
    
    // multiply
    List<StatementNode> multiplyStatements;
    multiplyStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(12, Variable("number"))));
    multiplyStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(13, "sumDigit")));
    StmtlstNode* multiplyStmtLstNode = createStmtlstNode(multiplyStatements);
    ProcedureNode* multiplyProcedureNode = createProcedureNode("multiply", multiplyStmtLstNode);


    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigitsProcedureNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(plusProcedureNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(multiplyProcedureNode));
    ProgramNode* programNode = createProgramNode("sumDigitPlusMultiplyCyclicCall", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == false);
}


String sumDigitPlusNonexistentProcedureCall = "\
procedure sumDigit {\n\
\n\
    read number;\n\
    sum = 0;\n\
    while(number > 0) {\n\
        digit = number % 10;\n\
        sum = sum + digit;\n\
        number = number / 10;\n\
    }\n\
\n\
    call plus;\n\
    print sum;\n\
}\n\
\n\
procedure plus {\n\
\n\
    sum = 0;\n\
    print sum;\n\
    call multiply;\n\
}";

TEST_CASE(
    "Design Extractor identifies semantically invalid program with non-existent procedure calls as invalid - sumDigit and plus")
{
    // sumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("num"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("sum"), createRefExpr(0)), whileStmtLstNode)));

    statements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(7, "plus")));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("sum"))));
    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* sumDigitsProcedureNode = createProcedureNode("sumDigit", stmtLstNode);

    // plus
    List<StatementNode> plusStatements;
    plusStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("sum"), createRefExpr(0))));
    plusStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(10, Variable("sum"))));
    plusStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(11, "multiply")));
    StmtlstNode* plusStmtLstNode = createStmtlstNode(plusStatements);
    ProcedureNode* plusProcedureNode = createProcedureNode("plus", plusStmtLstNode);

    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigitsProcedureNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(plusProcedureNode));
    ProgramNode* programNode = createProgramNode("sumDigitPlusNonexistentProcedureCall", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == false);
}


String sumDigitPlusMultiplyNestedWhileIfCyclicCall = "\
procedure printSumDigit {\n\
\n\
    read number;\n\
    sum = 0;\n\
    while(number > 0) {\n\
        digit = number % 10;\n\
        sum = sum + digit;\n\
        number = number / 10;\n\
        if(sum > 2) then {\n\
            call printSum;\n\
        } else {\n\
            call printNumber;\n\
        }\n\
    }\n\
}\n\
\n\
procedure printSum {\n\
\n\
    print sum;\n\
    call printNumber;\n\
}\n\
\n\
procedure printNumber {\n\
\n\
    print number;\n\
    call printSumDigit;\n\
}";

TEST_CASE(
    "Design Extractor identifies semantically invalid program with nested while and if cyclic calls as invalid - printSumDigit, printSum and printNumber")
{
    // printSumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("num"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));

    // Nested if in while loop
    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // If statement
    ifStatements.push_back(
        std::unique_ptr<CallStatementNode>(createCallNode(8, "printSum")));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Else statement
    elseStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(9, "printNumber")));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    whileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createGtExpr(createRefExpr("sum"), createRefExpr(2)), ifStmtLstNode, elseStmtLstNode)));

    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    // Add while stmtLstNode to printSumDigit's statement list
    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("sum"), createRefExpr(0)), whileStmtLstNode)));
    
    // Create printSumDigit's stmtlstNode
    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* printSumDigitsProcedureNode = createProcedureNode("printSumDigit", stmtLstNode);



    // printSum
    List<StatementNode> printSumStatements;
    
    printSumStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(10, Variable("sum"))));
    printSumStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(11, "printNumber")));
    StmtlstNode* printSumStmtLstNode = createStmtlstNode(printSumStatements);
    ProcedureNode* printSumProcedureNode = createProcedureNode("printSum", printSumStmtLstNode);

    // printNumber
    List<StatementNode> printNumberStatements;
    printNumberStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(12, Variable("number"))));
    printNumberStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(13, "printSumDigit")));
    StmtlstNode* printNumberStmtLstNode = createStmtlstNode(printNumberStatements);
    ProcedureNode* printNumberProcedureNode = createProcedureNode("printNumber", printNumberStmtLstNode);

    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(printSumDigitsProcedureNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(printSumProcedureNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(printNumberProcedureNode));
    ProgramNode* programNode = createProgramNode("printSumDigitPrintSumPrintNumberNestedWhileIfCyclicCall", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == false);
}

String recursivePrintAscending = "\
procedure printAscending {\n\
\n\
    read num1;\n\
    read num2;\n\
\n\
    if(num1 >= num2) then {\n\
        num2 = num2 + 1;\n\
    } else {\n\
        num1 = num1 + 1;\n\
    }\n\
    print num1;\n\
    print num2;\n\
    call printAscending;\n\
}";

TEST_CASE("Design Extractor identifies semantically invalid recursive program as invalid - recursivePrintAscending")
{
    List<StatementNode> statements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));

    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // If statements
    ifStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("num2"), createPlusExpr(createRefExpr("num2"), createRefExpr(1)))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Else statements
    elseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("num1"), createPlusExpr(createRefExpr("num2"), createRefExpr(1)))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    statements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createGteExpr(createRefExpr("num1"), createRefExpr("num2")), ifStmtLstNode, elseStmtLstNode)));

    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(6, Variable("num1"))));
    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("num2"))));
    statements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(8, "recursivePrintAscending")));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* recursivePrintAscendingProcedureNode = createProcedureNode("recursivePrintAscending", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(recursivePrintAscendingProcedureNode));
    ProgramNode* programNode = createProgramNode("recursivePrintAscending", procedureList);

    SemanticErrorsValidator seValidator(*programNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    REQUIRE(isSemanticallyValid == false);
}
