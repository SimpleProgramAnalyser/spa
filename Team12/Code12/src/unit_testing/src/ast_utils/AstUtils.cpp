/**
 * Implementation of methods to generate Abstract Syntax
 * Trees, to aid in unit testing.
 *
 * Methods labelled with the same number describe the
 * same program in different forms.
 */

#include "AstUtils.h"

#include "ast/AstLibrary.h"

String getProgram1String()
{
    String simpleProgram = "\
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
    return simpleProgram;
}

ProgramNode* getProgram1Tree()
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
    ProgramNode* constructedAst = createProgramNode("compute", procedureList, 6);
    return constructedAst;
}

String getProgram2String()
{
    String simpleProgram = "\
procedure factorials {\n\
    limit = 100;\n\
    current = 0;\n\
    factorial = 0;\n\
    while (current < limit) {\n\
        if (current == 0) then {\n\
            factorial = 1;\n\
        } else {\n\
            factorial = current * factorial;\n\
        }\n\
        current = current + 1;\n\
    }\n\
    answer = factorial;\n\
}";

/*
Sample SIMPLE program used:

procedure factorials {
    limit = 100;
    current = 0;
    factorial = 0;
    while (current < limit) {
        if (current == 0) then {
            factorial = 1;
        } else {
            factorial = current * factorial;
        }
        current = current + 1;
    }
    answer = factorial;
}

Annotated with statement numbers:
procedure factorials {
    limit = 100;                             // 1
    current = 0;                             // 2
    factorial = 0;                           // 3
    while (current < limit) {                // 4
        if (current == 0) then {             // 5
            factorial = 1;                   // 6
        } else {                             //
            factorial = current * factorial; // 7
        }                                    //
        current = current + 1;               // 8
    }                                        //
    answer = factorial;                      // 9
}
*/
    return simpleProgram;
}

ProgramNode* getProgram2Tree()
{
    List<StatementNode> statements;
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(1, Variable("limit"), createRefExpr(100))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("current"), createRefExpr(0))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("factorial"), createRefExpr(0))));
    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factorial"), createRefExpr(1))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("factorial"), createTimesExpr(createRefExpr("current"), createRefExpr("factorial")))));
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("current"), createRefExpr(0)), createStmtlstNode(ifStatements),
                     createStmtlstNode(elseStatements))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(8, Variable("current"), createPlusExpr(createRefExpr("current"), createRefExpr(1)))));
    statements.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        4, createLtExpr(createRefExpr("current"), createRefExpr("limit")), createStmtlstNode(whileStatements))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("answer"), createRefExpr("factorial"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* computeProcedure = createProcedureNode("factorials", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* constructedAst = createProgramNode("factorials", procedureList, 9);

    return constructedAst;
}

String getProgram3String()
{
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

/* Annotated computeAverage with line numbers
procedure compute {
1.  read num1;
2.  read num2;
3.  read num3;
4.  sum = (num1 + num2) * num3;
5.  ave = sum / 3;
6.  print ave;
}
*/
    return computeAverage;
}

ProgramNode* getProgram3Tree()
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
    ProgramNode* programNode = createProgramNode("compute", procedureList, 6);

    return programNode;
}

String getProgram4String()
{
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

/* Annotated printAscending with line numbers
procedure printAscending {
1.  read num1;\n\
2.  read num2;\n\
3.  noSwap = 0;\n\
4.  if(num1 > num2) then {
5.    temp = num1;
6.    num1 = num2;
7.    num2 = temp;
    } else {
8.    noSwap = 1;
    }
9.  print num1;
10. print num2;
11. print noSwap;
}";
*/
    return printAscending;
}

ProgramNode* getProgram4Tree()
{
    List<StatementNode> statements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("num1"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("num2"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("noSwap"), createRefExpr(0))));

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
    ProgramNode* programNode = createProgramNode("printAscending", procedureList, 11);

    return programNode;
}

String getProgram5String()
{
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

/* Annotated sumDigits with line numbers
procedure sumDigit {
1.  read number;
2.  sum = 0;
3.  while(number > 0) {
4.    digit = number % 10;
5.    sum = sum + digit;
6.    number = number / 10;
    }
7.  print sum;\n\
}
*/
    return sumDigits;
}

ProgramNode* getProgram5Tree()
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
    ProcedureNode* computeProcedure = createProcedureNode("sumDigits", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* programNode = createProgramNode("sumDigit", procedureList, 7);

    return programNode;
}

String getProgram6String()
{
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

/* Annotated sumDigits duplicates with line numbers
procedure sumDigit {
1.  read number;
2.  sum = 0;
3.  while(number > 0) {
4.    digit = number % 10;
5.    sum = sum + digit;
6.    number = number / 10;
    }
7.  print sum;
}
procedure sumDigit {
8.  read number;
9.  sum = 0;
10. while(number > 0) {
11.     digit = number % 10;
12.     sum = sum + digit;
13.     number = number / 10;
    }
14. print sum;
}
*/
    return sumDigitsDuplicate;
}

ProgramNode* getProgram6Tree()
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
    ProcedureNode* sumDigitNode = createProcedureNode("sumDigit", stmtLstNode);

    // Duplicate procedure
    List<StatementNode> dupStatements;
    List<StatementNode> dupWhileStatements;
    dupStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(8, Variable("number"))));
    dupStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("sum"), createRefExpr(0))));

    dupWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(11, Variable("digit"), createModExpr(createRefExpr("num"), createRefExpr(10)))));
    dupWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(12, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    dupWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* dupWhileStmtLstNode = createStmtlstNode(dupWhileStatements);

    dupStatements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(10, createGtExpr(createRefExpr("sum"), createRefExpr(0)), dupWhileStmtLstNode)));

    dupStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(14, Variable("sum"))));

    StmtlstNode* dupStmtLstNode = createStmtlstNode(dupStatements);
    ProcedureNode* dupSumDigitNode = createProcedureNode("sumDigit", dupStmtLstNode);

    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigitNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(dupSumDigitNode));
    ProgramNode* programNode = createProgramNode("sumDigit", procedureList, 14);

    return programNode;
}

String getProgram7String()
{
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

/* Annotated multiProcedureProgram with line numbers
procedure main {
1.  flag = 0;
2.  call computeCentroid;
3.  call printResults;
}
procedure readPoint{
4.   read x;
5.   read y;
}

procedure printResults {
6.  print flag;
7.  print cenX;
8.  print cenY;
9.  print normSq;
}
procedure computeCentroid {
10.  count = 0;
11.  cenX = 0;
12.  cenY = 0;
13.  call readPoint;
14.  while((x != 0) && (y != 0)) {
15.      count = count + 1;
16.      cenX = cenX + x;
17.      cenY = cenY + y;
18.      call readPoint;
     }
19.  if(count == 0) then {
20.      flag = 1;
     } else {
21.      cenX = cenX/ count;
22.      cenY = cenY / count;
     }
23.  normSq = cenX * cenY + cenY * cenY;
}
*/
    return multiProcedureProgram;
}

ProgramNode* getProgram7Tree()
{
    List<StatementNode> mainStatements;
    List<StatementNode> readPointStatements;
    List<StatementNode> printResultsStatements;
    List<StatementNode> computeCentroidStatements;

    List<ProcedureNode> procedureList;

    // main
    mainStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(1, Variable("flag"), createRefExpr(0))));
    mainStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(2, "computeCentroid")));
    mainStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(3, "printResults")));
    StmtlstNode* mainStmtLstNode = createStmtlstNode(mainStatements);
    ProcedureNode* mainProc = createProcedureNode("main", mainStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(mainProc));

    // readPoint
    readPointStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(4, Variable("x"))));
    readPointStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(5, Variable("y"))));
    StmtlstNode* readPointStmtLstNode = createStmtlstNode(readPointStatements);
    ProcedureNode* readPointProc = createProcedureNode("readPoint", readPointStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(readPointProc));

    // printResults
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(6, Variable("flag"))));
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("cenX"))));
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("cenY"))));
    printResultsStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(9, Variable("normSq"))));
    StmtlstNode* printResultsStmtLstNode = createStmtlstNode(printResultsStatements);
    ProcedureNode* printResultsProc = createProcedureNode("printResults", printResultsStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(printResultsProc));

    // Compute Centroid
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("count"), createRefExpr(0))));
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(11, Variable("cenX"), createRefExpr(0))));
    computeCentroidStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("cenY"), createRefExpr(0))));
    computeCentroidStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(13, "readPoint")));

    // Compute Centroid - While statement
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
    // Compute Centroid - If statement
    ifStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("flag"), createRefExpr(1))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Compute Centroid - Else statement
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(21, Variable("cenX"), createDivExpr(createRefExpr("cenX"), createRefExpr("count")))));
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(22, Variable("cenY"), createDivExpr(createRefExpr("cenY"), createRefExpr("count")))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    computeCentroidStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(19, createEqExpr(createRefExpr("count"), createRefExpr(0)), ifStmtLstNode, elseStmtLstNode)));

    computeCentroidStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(23, Variable("normSq"),
                         createPlusExpr(createTimesExpr(createRefExpr("cenX"), createRefExpr("cenX")),
                                        createTimesExpr(createRefExpr("cenY"), createRefExpr("cenY"))))));
    StmtlstNode* computeCentroidStmtLstNode = createStmtlstNode(computeCentroidStatements);
    ProcedureNode* computeCentroidProc = createProcedureNode("computeCentroid", computeCentroidStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeCentroidProc));

    ProgramNode* programNode = createProgramNode("main", procedureList, 23);

    return programNode;
}

String getProgram8String() {
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

/* Annotated sumDigitPlusCyclicCall with line numbers
procedure sumDigit {
1.  read number;
2.  sum = 0;
3.  while(number > 0) {
4.    digit = number % 10;
5.    sum = sum + digit;
6.    number = number / 10;
    }
7.  call plus;
8.  print sum;
}
procedure plus {
9.  sum = 0;
10. print sum;
11. call sumDigit;
}
*/
    return sumDigitPlusCyclicCall;
}

ProgramNode* getProgram8Tree() {
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
    plusStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(11, "sumDigit")));
    StmtlstNode* plusStmtLstNode = createStmtlstNode(plusStatements);
    ProcedureNode* plusProcedureNode = createProcedureNode("plus", plusStmtLstNode);

    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigitsProcedureNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(plusProcedureNode));
    ProgramNode* programNode = createProgramNode("sumDigitPlusCyclicCall", procedureList, 11);

    return programNode;
}

String getProgram9String() {
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

/* Annotated sumDigitPlusCyclicCall with line numbers
procedure sumDigit {
1.  read number;
2.  sum = 0;
3.  while(number > 0) {
4.    digit = number % 10;
5.    sum = sum + digit;
6.    number = number / 10;
    }
7.  call plus;
8.  print sum;
}
procedure plus {
9.  sum = 0;
10. print sum;
11. call multiply;
}
procedure multiply {
12. print number;
13. call sumDigit;
}
*/
    return sumDigitPlusMultiplyCyclicCall;
}

ProgramNode* getProgram9Tree() {
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
    ProgramNode* programNode = createProgramNode("sumDigitPlusMultiplyCyclicCall", procedureList, 13);

    return programNode;
}

String getProgram10String() {
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

/* Annotated sumDigitPlusNonexistentProcedureCall with line numbers
procedure sumDigit {
1.  read number;
2.  sum = 0;
3.  while(number > 0) {
4.    digit = number % 10;
5.    sum = sum + digit;
6.    number = number / 10;
    }
7.  call plus;
8.  print sum;
}
procedure plus {
9.  sum = 0;
10. print sum;
11. call multiply;
}
*/
    return sumDigitPlusNonexistentProcedureCall;
}

ProgramNode* getProgram10Tree() {
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
    ProgramNode* programNode = createProgramNode("sumDigitPlusNonexistentProcedureCall", procedureList, 11);

    return programNode;
}

String getProgram11String() {
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

/* Annotated sumDigitPlusMultiplyNestedWhileIfCyclicCall with line numbers
procedure printSumDigit {
1.  read number;
2.  sum = 0;
3.  while(number > 0) {
4.    digit = number % 10;
5.    sum = sum + digit;
6.    number = number / 10;
7.    if(sum > 2) then {
8.      call printSum;
      } else {
9.      call printNumber;
      }
    }
}
procedure printSum {
10. print sum;
11. call printNumber;
}
procedure printNumber {
12. print number;
13. call printSumDigit;
}
*/
    return sumDigitPlusMultiplyNestedWhileIfCyclicCall;
}

ProgramNode* getProgram11Tree() {
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
    ifStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(8, "printSum")));
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
    ProgramNode* programNode
        = createProgramNode("printSumDigitPrintSumPrintNumberNestedWhileIfCyclicCall", procedureList, 13);

    return programNode;
}

String getProgram12String() {
    String recursivePrintAscending = "\
procedure recursivePrintAscending {\n\
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
    call recursivePrintAscending;\n\
}";

/* Annotated recursivePrintAscending with line numbers
procedure recursivePrintAscending {
1.  read num1;
2.  read num2;
3.  if(num1 >= num2) then {
4.      num2 = num2 + 1;
    } else {
5.      num1 = num1 + 1;
    }
6.  print num1;
7.  print num2;
8.  call recursivePrintAscending;
};
*/
    return recursivePrintAscending;
}

ProgramNode* getProgram12Tree() {
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
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("num1"), createPlusExpr(createRefExpr("num2"), createRefExpr(1)))));
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
    ProgramNode* programNode = createProgramNode("recursivePrintAscending", procedureList, 8);

    return programNode;
}
