/**
 * Implementation of methods to generate Abstract Syntax
 * Trees, to aid in unit testing.
 *
 * Methods labelled with the same number describe the
 * same program in different forms.
 */

#include "AstUtils.h"

#include "ast/AstLibrary.h"

String getProgram1String_compute()
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

ProgramNode* getProgram1Tree_compute()
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

String getProgram2String_factorials()
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

ProgramNode* getProgram2Tree_factorials()
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

String getProgram3String_computeAverage()
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

ProgramNode* getProgram3Tree_computeAverage()
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

String getProgram4String_printAscending()
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

ProgramNode* getProgram4Tree_printAscending()
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

String getProgram5String_sumDigit()
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

ProgramNode* getProgram5Tree_sumDigit()
{
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("number"), createRefExpr(0)), whileStmtLstNode)));

    statements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("sum"))));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* computeProcedure = createProcedureNode("sumDigit", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(computeProcedure));
    ProgramNode* programNode = createProgramNode("sumDigit", procedureList, 7);

    return programNode;
}

String getProgram6String_sumDigitDuplicate()
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

ProgramNode* getProgram6Tree_sumDigitDuplicate()
{
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("number"), createRefExpr(0)), whileStmtLstNode)));

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
        createAssignNode(11, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
    dupWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(12, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    dupWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* dupWhileStmtLstNode = createStmtlstNode(dupWhileStatements);

    dupStatements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(10, createGtExpr(createRefExpr("number"), createRefExpr(0)), dupWhileStmtLstNode)));

    dupStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(14, Variable("sum"))));

    StmtlstNode* dupStmtLstNode = createStmtlstNode(dupStatements);
    ProcedureNode* dupSumDigitNode = createProcedureNode("sumDigit", dupStmtLstNode);

    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(sumDigitNode));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(dupSumDigitNode));
    ProgramNode* programNode = createProgramNode("sumDigit", procedureList, 14);

    return programNode;
}

String getProgram7String_computeCentroid()
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
    normSq = cenX * cenX + cenY * cenY;\n\
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
    23.  normSq = cenX * cenX + cenY * cenY;
    }
    */
    return multiProcedureProgram;
}

String getProgram7StringMismatchedBraces_computeCentroid()
{
    String mismatchedBraces = "\
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
    if(count == 0) then " /* missing opening brace here */ "\
\n        flag = 1;\n\
    } else {\n\
        cenX = cenX/ count;\n\
        cenY = cenY / count;\n\
    }\n\
\n\
    normSq = cenX * cenX + cenY * cenY;\n\
}";
    return mismatchedBraces;
}

String getProgram7StringMismatchedBrackets_computeCentroid()
{
    String mismatchedBrackets = "\
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
    while((x != 0) && (y != 0" /* missing closing bracket here */ ") {\n\
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
    normSq = cenX * cenX + cenY * cenY;\n\
}";
    return mismatchedBrackets;
}

ProgramNode* getProgram7Tree_computeCentroid()
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

String getProgram8String_sumDigitPlusCyclicCall()
{
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

ProgramNode* getProgram8Tree_sumDigitPlusCyclicCall()
{
    // sumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("number"), createRefExpr(0)), whileStmtLstNode)));

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
    ProgramNode* programNode = createProgramNode("sumDigit", procedureList, 11);

    return programNode;
}

String getProgram9String_sumDigitPlusMultiplyCyclicCall()
{
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

ProgramNode* getProgram9Tree_sumDigitPlusMultiplyCyclicCall()
{
    // sumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("number"), createRefExpr(0)), whileStmtLstNode)));

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
    ProgramNode* programNode = createProgramNode("sumDigit", procedureList, 13);

    return programNode;
}

String getProgram10String_sumDigitPlusNonexistentProcedureCall()
{
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

ProgramNode* getProgram10Tree_sumDigitPlusNonexistentProcedureCall()
{
    // sumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("sum"), createPlusExpr(createRefExpr("sum"), createRefExpr("digit")))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("number"), createDivExpr(createRefExpr("number"), createRefExpr(10)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);

    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(3, createGtExpr(createRefExpr("number"), createRefExpr(0)), whileStmtLstNode)));

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
    ProgramNode* programNode = createProgramNode("sumDigit", procedureList, 11);

    return programNode;
}

String getProgram11String_sumDigitPlusMultiplyNestedWhileIfCyclicCall()
{
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

ProgramNode* getProgram11Tree_sumDigitPlusMultiplyNestedWhileIfCyclicCall()
{
    // printSumDigit
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("number"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("sum"), createRefExpr(0))));

    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("digit"), createModExpr(createRefExpr("number"), createRefExpr(10)))));
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
        createWhileNode(3, createGtExpr(createRefExpr("number"), createRefExpr(0)), whileStmtLstNode)));

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
    ProgramNode* programNode = createProgramNode("printSumDigit", procedureList, 13);

    return programNode;
}

String getProgram12String_recursivePrintAscending()
{
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

ProgramNode* getProgram12Tree_recursivePrintAscending()
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
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("num1"), createPlusExpr(createRefExpr("num1"), createRefExpr(1)))));
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

String getProgram13String_ifExample()
{
    String ifExample = "\
procedure ifExample {\n\
\n\
    if(num1 >= num2) then {\n\
        num2 = num2 + 1;\n\
    } else {\n\
        num1 = num1 + 1;\n\
    }\n\
}";

    return ifExample;
}

ProgramNode* getProgram13Tree_ifExample()
{
    List<StatementNode> statements;
    List<StatementNode> ifStatements;
    List<StatementNode> elseStatements;
    // If statements
    ifStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("num2"), createPlusExpr(createRefExpr("num2"), createRefExpr(1)))));
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifStatements);
    // Else statements
    elseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(3, Variable("num1"), createPlusExpr(createRefExpr("num1"), createRefExpr(1)))));
    StmtlstNode* elseStmtLstNode = createStmtlstNode(elseStatements);

    statements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(1, createGteExpr(createRefExpr("num1"), createRefExpr("num2")), ifStmtLstNode, elseStmtLstNode)));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* ifExampleProcedureNode = createProcedureNode("ifExample", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(ifExampleProcedureNode));
    ProgramNode* programNode = createProgramNode("ifExample", procedureList, 3);

    return programNode;
}

String getProgram14String_whileExample()
{
    String whileExample = "\
procedure whileExample {\n\
\n\
    while(x != 0){\n\
        x = x * 123;\
    }\n\
}";

    return whileExample;
}

ProgramNode* getProgram14Tree_whileExample()
{
    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("x"), createTimesExpr(createRefExpr("x"), createRefExpr(123)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);
    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(1, createNeqExpr(createRefExpr("x"), createRefExpr(0)), whileStmtLstNode)));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* whileExampleProcedureNode = createProcedureNode("whileExample", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(whileExampleProcedureNode));
    ProgramNode* programNode = createProgramNode("whileExample", procedureList, 2);

    return programNode;
}

String getProgram15String_complicatedConditional()
{
    String complicatedConditional = "\
procedure complicatedConditional {\n\
\n\
    while(!((x % 6 - 3 < a) && ((y == z) || (!(b != c + x))))){\n\
        x = x * 123;\
    }\n\
}";

    return complicatedConditional;
}

StringVector getProgram15StringList_complicatedConditional()
{
    StringVector complicatedConditional(46);
    String str0("procedure");
    String str1("complicatedConditional");
    String str2("{");
    String str3("while");
    String str4("(");
    String str5("!");
    String str6("(");
    String str7("(");
    String str8("x");
    String str9("%");
    String str10("6");
    String str11("-");
    String str12("3");
    String str13("<");
    String str14("a");
    String str15(")");
    String str16("&&");
    String str17("(");
    String str18("(");
    String str19("y");
    String str20("==");
    String str21("z");
    String str22(")");
    String str23("||");
    String str24("(");
    String str25("!");
    String str26("(");
    String str27("b");
    String str28("!=");
    String str29("c");
    String str30("+");
    String str31("x");
    String str32(")");
    String str33(")");
    String str34(")");
    String str35(")");
    String str36(")");
    String str37("{");
    String str38("x");
    String str39("=");
    String str40("x");
    String str41("*");
    String str42("123");
    String str43(";");
    String str44("}");
    String str45("}");

    complicatedConditional.at(0) = std::move(str0);
    complicatedConditional.at(1) = std::move(str1);
    complicatedConditional.at(2) = std::move(str2);
    complicatedConditional.at(3) = std::move(str3);
    complicatedConditional.at(4) = std::move(str4);
    complicatedConditional.at(5) = std::move(str5);
    complicatedConditional.at(6) = std::move(str6);
    complicatedConditional.at(7) = std::move(str7);
    complicatedConditional.at(8) = std::move(str8);
    complicatedConditional.at(9) = std::move(str9);
    complicatedConditional.at(10) = std::move(str10);
    complicatedConditional.at(11) = std::move(str11);
    complicatedConditional.at(12) = std::move(str12);
    complicatedConditional.at(13) = std::move(str13);
    complicatedConditional.at(14) = std::move(str14);
    complicatedConditional.at(15) = std::move(str15);
    complicatedConditional.at(16) = std::move(str16);
    complicatedConditional.at(17) = std::move(str17);
    complicatedConditional.at(18) = std::move(str18);
    complicatedConditional.at(19) = std::move(str19);
    complicatedConditional.at(20) = std::move(str20);
    complicatedConditional.at(21) = std::move(str21);
    complicatedConditional.at(22) = std::move(str22);
    complicatedConditional.at(23) = std::move(str23);
    complicatedConditional.at(24) = std::move(str24);
    complicatedConditional.at(25) = std::move(str25);
    complicatedConditional.at(26) = std::move(str26);
    complicatedConditional.at(27) = std::move(str27);
    complicatedConditional.at(28) = std::move(str28);
    complicatedConditional.at(29) = std::move(str29);
    complicatedConditional.at(30) = std::move(str30);
    complicatedConditional.at(31) = std::move(str31);
    complicatedConditional.at(32) = std::move(str32);
    complicatedConditional.at(33) = std::move(str33);
    complicatedConditional.at(34) = std::move(str34);
    complicatedConditional.at(35) = std::move(str35);
    complicatedConditional.at(36) = std::move(str36);
    complicatedConditional.at(37) = std::move(str37);
    complicatedConditional.at(38) = std::move(str38);
    complicatedConditional.at(39) = std::move(str39);
    complicatedConditional.at(40) = std::move(str40);
    complicatedConditional.at(41) = std::move(str41);
    complicatedConditional.at(42) = std::move(str42);
    complicatedConditional.at(43) = std::move(str43);
    complicatedConditional.at(44) = std::move(str44);
    complicatedConditional.at(45) = std::move(str45);

    return complicatedConditional;
}

ProgramNode* getProgram15Tree_complicatedConditional()
{
    ConditionalExpression* notNeqExpression
        = createNotExpr(createNeqExpr(createRefExpr("b"), createPlusExpr(createRefExpr("c"), createRefExpr("x"))));
    ConditionalExpression* eqExpression = createEqExpr(createRefExpr("y"), createRefExpr("z"));
    ConditionalExpression* orExpression = createOrExpr(eqExpression, notNeqExpression);
    Expression* minusExpression
        = createMinusExpr(createModExpr(createRefExpr("x"), createRefExpr(6)), createRefExpr(3));
    ConditionalExpression* gtExpression = createLtExpr(minusExpression, createRefExpr("a"));
    ConditionalExpression* andExpression = createAndExpr(gtExpression, orExpression);
    ConditionalExpression* predicate = createNotExpr(andExpression);

    List<StatementNode> statements;
    List<StatementNode> whileStatements;
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("x"), createTimesExpr(createRefExpr("x"), createRefExpr(123)))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);
    statements.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(1, predicate, whileStmtLstNode)));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* complicatedConditionalProcedureNode = createProcedureNode("complicatedConditional", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(complicatedConditionalProcedureNode));
    ProgramNode* programNode = createProgramNode("complicatedConditional", procedureList, 2);

    return programNode;
}

String getProgram16String_keywordsAsIdentifier()
{
    String keywordsAsIdentifier = "\
procedure procedure {\n\
    read print;\
    if = (3 + 4) * 3;\n\
    while (while <= else) {\n\
        call = call + 1;\n\
        else = else - if;\n\
    }\n\
    call if;\n\
}\n\
\n\
procedure if {\n\
    print print;\n\
    read procedure;\n\
    if (if == then) then{\n\
        else = then / while;\n\
    } else {\n\
        read read;\n\
    }\n\
    read = print - procedure;\n\
}";
    /* Annotated procedure with line numbers
procedure procedure {
1.  read print;
2.  if = (3 + 4) * 3;
3.  while (while <= else) {
4.      call = call + 1;
5.      else = else - if;
    }
6.  call if;
}

procedure if {
7.  print print;
8.  read procedure;
9.  if (if == then) then{
10.     else = then / while;
    } else {
11.     read read;\n
    }
12. read = print - procedure;
}
*/
    return keywordsAsIdentifier;
}

ProgramNode* getProgram16Tree_keywordsAsIdentifier()
{
    List<StatementNode> procedureStatements;
    List<StatementNode> whileStatements;
    procedureStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("print"))));
    procedureStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        2, Variable("if"), createTimesExpr(createPlusExpr(createRefExpr(3), createRefExpr(4)), createRefExpr(3)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("call"), createPlusExpr(createRefExpr("call"), createRefExpr(1)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(5, Variable("else"), createMinusExpr(createRefExpr("else"), createRefExpr("if")))));
    procedureStatements.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        3, createLteExpr(createRefExpr("while"), createRefExpr("else")), createStmtlstNode(whileStatements))));
    procedureStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(6, "if")));

    List<StatementNode> ifProcedureStatements;
    List<StatementNode> realIfStatements;
    List<StatementNode> realElseStatements;
    ifProcedureStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("print"))));
    ifProcedureStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(8, Variable("procedure"))));
    realIfStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(10, Variable("else"), createDivExpr(createRefExpr("then"), createRefExpr("while")))));
    realElseStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(11, Variable("read"))));
    ifProcedureStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("if"), createRefExpr("then")), createStmtlstNode(realIfStatements),
                     createStmtlstNode(realElseStatements))));
    ifProcedureStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(12, Variable("read"), createMinusExpr(createRefExpr("print"), createRefExpr("procedure")))));

    StmtlstNode* procStmtLstNode = createStmtlstNode(procedureStatements);
    ProcedureNode* procedureProcedure = createProcedureNode("procedure", procStmtLstNode);
    StmtlstNode* ifStmtLstNode = createStmtlstNode(ifProcedureStatements);
    ProcedureNode* ifProcedure = createProcedureNode("if", ifStmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(procedureProcedure));
    procedureList.push_back(std::unique_ptr<ProcedureNode>(ifProcedure));
    ProgramNode* programNode = createProgramNode("procedure", procedureList, 12);

    return programNode;
}

String getProgram17String_sameVariableAndProcedureName()
{
    String procedureString = "\
procedure procedure {\n\
    procedure = procedure % procedure;\n\
    read call;\n\
    call call;\n\
}\n\
\n\
procedure call {\n\
    procedure = procedure * 1;\n\
    string = 43110;\n\
    call string;\n\
}\n\
\n\
procedure string {\n\
    print string;\n\
}\n\
";
    /* Annotated procedure with line numbers
procedure procedure {
1.  procedure = procedure % procedure;
2.  read call;
3.  call call;
}

procedure call {
4.  procedure = procedure * 1;
5.  string = 43110;
6.  call string;
}

procedure string {
7.  print string;
}
    */
    return procedureString;
}

ProgramNode* getProgram17Tree_sameVariableAndProcedureName()
{
    List<StatementNode> procedureStatements;
    List<StatementNode> callStatements;
    List<StatementNode> stringStatements;
    procedureStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        1, Variable("procedure"), createModExpr(createRefExpr("procedure"), createRefExpr("procedure")))));
    procedureStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(2, Variable("call"))));
    procedureStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(3, "call")));

    callStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(4, Variable("procedure"), createTimesExpr(createRefExpr("procedure"), createRefExpr(1)))));
    callStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("string"), createRefExpr(43110))));
    callStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(6, "string")));

    stringStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(7, Variable("string"))));

    List<ProcedureNode> procedureList;
    procedureList.push_back(
        std::unique_ptr<ProcedureNode>(createProcedureNode("procedure", createStmtlstNode(procedureStatements))));
    procedureList.push_back(
        std::unique_ptr<ProcedureNode>(createProcedureNode("call", createStmtlstNode(callStatements))));
    procedureList.push_back(
        std::unique_ptr<ProcedureNode>(createProcedureNode("string", createStmtlstNode(stringStatements))));
    ProgramNode* programNode = createProgramNode("procedure", procedureList, 7);

    return programNode;
}

String getProgram18String_endWithWhile()
{
    String spheresdf = "\
procedure spheresdf {\n\
     dist = x * x + y * y + z * z;\n\
     x = dist;\n\
     depth = depth;\n\
     read p;\n\
     while (x != p) {\n\
            p = x;\n\
            x = (dist / x + x) / 2; dist = x - 1;\n\
      x = x * x + y * y / 2;} \n\
       }\
";
    /*
    Annotated with statement numbers:

    procedure spheresdf {
    1.   dist = x * x + y * y + z * z;
    2.   x = dist;
    3.   depth = depth;
    4.   read p;
    5.   while (x != p) {
    6.          p = x;
    7.          x = (dist / x + x) / 2;
    8.          dist = x - 1;
    9.          x = x * x + y * y / 2;}}
    */
    return spheresdf;
}

ProgramNode* getProgram18Tree_endWithWhile()
{
    List<StatementNode> statements;
    Expression* xxyyzz = createPlusExpr(createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                                                       createTimesExpr(createRefExpr("y"), createRefExpr("y"))),
                                        createTimesExpr(createRefExpr("z"), createRefExpr("z")));
    statements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(1, Variable("dist"), xxyyzz)));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("x"), createRefExpr("dist"))));
    statements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(3, Variable("depth"), createRefExpr("depth"))));
    statements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(4, Variable("p"))));

    List<StatementNode> whileStatements;
    whileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("p"), createRefExpr("x"))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        7, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(8, Variable("dist"), createMinusExpr(createRefExpr("x"), createRefExpr(1)))));
    whileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        9, Variable("x"),
        createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                       createDivExpr(createTimesExpr(createRefExpr("y"), createRefExpr("y")), createRefExpr(2))))));
    StmtlstNode* whileStmtLstNode = createStmtlstNode(whileStatements);
    statements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(5, createNeqExpr(createRefExpr("x"), createRefExpr("p")), whileStmtLstNode)));

    StmtlstNode* stmtLstNode = createStmtlstNode(statements);
    ProcedureNode* whileExampleProcedureNode = createProcedureNode("spheresdf", stmtLstNode);
    List<ProcedureNode> procedureList;
    procedureList.push_back(std::unique_ptr<ProcedureNode>(whileExampleProcedureNode));
    ProgramNode* programNode = createProgramNode("spheresdf", procedureList, 9);

    return programNode;
}

String getProgram19String_multipleProcedures()
{
    String multipleProcedures = "\
procedure a {\n\
    call b;\n\
    call c;\n\
    print num;\n\
    print result\n\
}\n\
\n\
procedure b {\n\
    read num;\n\
    factor = factor;\n\
    call d;\n\
}\n\
\n\
procedure c {\n\
    depth = 7;\n\
    base = 10;\n\
    result = 0;\n\
    while (base > 0) {\n\
        if (base % 2 == 0) then {\n\
            num = num / 2;\n\
        } else {\n\
            result = num + result;\n\
            call d;\n\
        }\n\
    }\n\
}\n\
\n\
procedure d {\n\
    if (depth > 0) {\n\
        result = depth;\n\
    } else {\n\
        result = result;\n\
    }\n\
}\n\
";

    /*
    Annotated with statement numbers:
    procedure a {
    1.  call b;
    2.  call c;
    3.  print num;
    4.  print result;
    }

    procedure b {
    5.  read num;
    6.  factor = factor;
    7.  call d;
    }

    procedure c {
    8.  depth = 7;
    9.  base = 10;
    10. result = 0;
    11. while (base > 0) {
    12.     if (base % 2 == 0) then {
    13.         num = num / 2;
            } else {
    14.         result = num + result;
    15.         call d;
            }
        }
    }

    procedure d {
    16. if (depth > 0) {
    17.     result = depth;
        } else {
    18.     result = result;
        }
    }
    */
    return multipleProcedures;
}

ProgramNode* getProgram19Tree_multipleProcedures()
{
    List<StatementNode> aStmts;
    List<StatementNode> bStmts;
    List<StatementNode> cStmts;
    List<StatementNode> dStmts;

    List<ProcedureNode> procedureList;

    // procedure a
    aStmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(1, "b")));
    aStmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(2, "c")));
    aStmts.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(3, Variable("num"))));
    aStmts.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(4, Variable("result"))));
    StmtlstNode* aStmtLstNode = createStmtlstNode(aStmts);
    ProcedureNode* aProc = createProcedureNode("a", aStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(aProc));

    // procedure b
    bStmts.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(5, Variable("num"))));
    bStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("factor"), createRefExpr("factor"))));
    bStmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(7, "d")));
    StmtlstNode* bStmtLstNode = createStmtlstNode(bStmts);
    ProcedureNode* bProc = createProcedureNode("b", bStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(bProc));

    // procedure c while
    List<StatementNode> cIfStatements;
    List<StatementNode> cElseStatements;
    List<StatementNode> cWhileStatements;
    cIfStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("num"), createDivExpr(createRefExpr("num"), createRefExpr(2)))));
    cElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(14, Variable("result"), createPlusExpr(createRefExpr("num"), createRefExpr("result")))));
    cElseStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(15, "d")));
    cWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(12, createEqExpr(createModExpr(createRefExpr("base"), createRefExpr(2)), createRefExpr(0)),
                     createStmtlstNode(cIfStatements), createStmtlstNode(cElseStatements))));

    // procedure c
    cStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("depth"), createRefExpr(7))));
    cStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("base"), createRefExpr(10))));
    cStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("result"), createRefExpr(0))));
    cStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        11, createGtExpr(createRefExpr("base"), createRefExpr(0)), createStmtlstNode(cWhileStatements))));
    StmtlstNode* cStmtLstNode = createStmtlstNode(cStmts);
    ProcedureNode* cProc = createProcedureNode("c", cStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(cProc));

    // procedure d
    List<StatementNode> dIfStatements;
    List<StatementNode> dElseStatements;
    dIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("result"), createRefExpr("depth"))));
    dElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("result"), createRefExpr("result"))));
    dStmts.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createGtExpr(createRefExpr("depth"), createRefExpr(0)), createStmtlstNode(dIfStatements),
                     createStmtlstNode(dElseStatements))));
    StmtlstNode* dStmtLstNode = createStmtlstNode(dStmts);
    ProcedureNode* dProc = createProcedureNode("d", dStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(dProc));

    ProgramNode* programNode = createProgramNode("a", procedureList, 18);

    return programNode;
}

String getProgram20String_multipleProceduresSpheresdf()
{
    String multipleProceduresSpheresdf = "\
procedure main { \t\n\
     read steps;\n\
     call raymarch;\n\
     print depth; }\n\
\n\
procedure raymarch {\n\
     ro = 13;\n\
     rd = 19;\n\
     read depth; \n\
    while (count < steps) {\n\
            print depth;\n\
            po = ro + rd * depth;\n\
          call spheresdf;\n\
          if (dist < epsilon) then {\n\
                  done = depth; }\n\
                else {\n\
                  depth = depth + dist;} \n\
          count = count + 1; }}\n\
\n\
procedure spheresdf {\n\
     dist = x * x + y * y + z * z;\n\
     x = dist;\n\
     depth = depth;\n\
     read p;\n\
     while (x != p) {\n\
            p = x;\n\
            x = (dist / x + x) / 2; } \n\
      dist = x - 1;\n\
      x = x * x + y * y / 2; }\n\
";
    /*
    Annotated with statement numbers:
    procedure main {
    1.     read steps;
    2.     call raymarch;
    3.     print depth; }

    procedure raymarch {
    4.     ro = 13;
    5.     rd = 19;
    6.     read depth;
    7.     while (count < steps) {
    8.          print depth;
    9.          po = ro + rd * depth;
    10.         call spheresdf;
    11.         if (dist < epsilon) then {
    12.             done = depth; }
                else {
    13.             depth = depth + dist;}
    14.          count = count + 1; }}

    procedure spheresdf {
    15.     dist = x * x + y * y + z * z;
    16.     x = dist;
    17.     depth = depth;
    18.     read p;
    19.     while (x != p) {
    20.            p = x;
    21.            x = (dist / x + x) / 2; }
    22.      dist = x - 1;
    23.      x = x * x + y * y / 2; }
        */
    return multipleProceduresSpheresdf;
}

ProgramNode* getProgram20Tree_multipleProceduresSpheresdf()
{
    List<StatementNode> mainStmts;
    List<StatementNode> raymarchStmts;
    List<StatementNode> spheresdfStmts;

    List<ProcedureNode> procedureList;

    // procedure main
    mainStmts.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(1, Variable("steps"))));
    mainStmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(2, "raymarch")));
    mainStmts.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(3, Variable("depth"))));
    StmtlstNode* mainStmtLstNode = createStmtlstNode(mainStmts);
    ProcedureNode* mainProc = createProcedureNode("main", mainStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(mainProc));

    // procedure raymarch while & if
    List<StatementNode> raymarchIfStatements;
    List<StatementNode> raymarchElseStatements;
    List<StatementNode> raymarchWhileStatements;
    raymarchIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("done"), createRefExpr("depth"))));
    raymarchElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(13, Variable("depth"), createPlusExpr(createRefExpr("depth"), createRefExpr("dist")))));

    raymarchWhileStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("depth"))));
    raymarchWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        9, Variable("po"),
        createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth"))))));
    raymarchWhileStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(10, "spheresdf")));
    raymarchWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createLtExpr(createRefExpr("dist"), createRefExpr("epsilon")),
                     createStmtlstNode(raymarchIfStatements), createStmtlstNode(raymarchElseStatements))));
    raymarchWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(14, Variable("count"), createPlusExpr(createRefExpr("count"), createRefExpr(1)))));

    // procedure raymarch
    raymarchStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("ro"), createRefExpr(13))));
    raymarchStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("rd"), createRefExpr(19))));
    raymarchStmts.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(6, Variable("depth"))));
    raymarchStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        7, createLtExpr(createRefExpr("count"), createRefExpr("steps")), createStmtlstNode(raymarchWhileStatements))));

    StmtlstNode* raymarchStmtLstNode = createStmtlstNode(raymarchStmts);
    ProcedureNode* raymarchProc = createProcedureNode("raymarch", raymarchStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(raymarchProc));

    // procedure spheresdf while
    List<StatementNode> spheresdfWhileStatements;
    spheresdfWhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("p"), createRefExpr("x"))));
    spheresdfWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        21, Variable("x"),
        createDivExpr(createPlusExpr(createDivExpr(createRefExpr("dist"), createRefExpr("x")), createRefExpr("x")),
                      createRefExpr(2)))));

    // procedure spheresdf
    spheresdfStmts.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(15, Variable("dist"),
                         createPlusExpr(createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                                                       createTimesExpr(createRefExpr("y"), createRefExpr("y"))),
                                        createTimesExpr(createRefExpr("z"), createRefExpr("z"))))));
    spheresdfStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("x"), createRefExpr("dist"))));
    spheresdfStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("depth"), createRefExpr("depth"))));
    spheresdfStmts.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(18, Variable("p"))));
    spheresdfStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        19, createNeqExpr(createRefExpr("x"), createRefExpr("p")), createStmtlstNode(spheresdfWhileStatements))));

    spheresdfStmts.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(22, Variable("dist"), createMinusExpr(createRefExpr("x"), createRefExpr(1)))));
    spheresdfStmts.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        23, Variable("x"),
        createPlusExpr(createTimesExpr(createRefExpr("x"), createRefExpr("x")),
                       createDivExpr(createTimesExpr(createRefExpr("y"), createRefExpr("y")), createRefExpr(2))))));

    StmtlstNode* spheresdfStmtLstNode = createStmtlstNode(spheresdfStmts);
    ProcedureNode* spheresdfProc = createProcedureNode("spheresdf", spheresdfStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(spheresdfProc));

    ProgramNode* programNode = createProgramNode("main", procedureList, 23);

    return programNode;
}

String getProgram21String_endWithIf()
{
    String raymarch = "\
    procedure raymarch {\n\
         ro = 13;\n\
         rd = 19;\n\
         read depth; \n\
        while (count < steps) {\n\
                print depth;\n\
                po = ro + rd * depth;\n\
              call spheresdf;\n\
              if (dist < epsilon) then {\n\
                      done = depth; }\n\
                    else {\n\
                      depth = depth + dist}}\n\
        }\n\
 ";
    /**
     * Extracted Raymarch from the procedures above
     */
    /*
    Annotated with statement numbers:
    procedure raymarch {
    1.     ro = 13;
    2.     rd = 19;
    3.     read depth;
    4.     while (count < steps) {
    5.          print depth;
    6.          po = ro + rd * depth;
    7.         if (dist < epsilon) then {
    8.             done = depth; }
                else {
    9.             depth = depth + dist;}}}
    */
    return raymarch;
}

ProgramNode* getProgram21Tree_endWithIf()
{
    List<ProcedureNode> procedureList;
    List<StatementNode> raymarchStmts;

    // procedure raymarch while & if
    List<StatementNode> raymarchIfStatements;
    List<StatementNode> raymarchElseStatements;
    List<StatementNode> raymarchWhileStatements;
    raymarchIfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("done"), createRefExpr("depth"))));
    raymarchElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(9, Variable("depth"), createPlusExpr(createRefExpr("depth"), createRefExpr("dist")))));

    raymarchWhileStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(5, Variable("depth"))));
    raymarchWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        6, Variable("po"),
        createPlusExpr(createRefExpr("ro"), createTimesExpr(createRefExpr("rd"), createRefExpr("depth"))))));
    raymarchWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createLtExpr(createRefExpr("dist"), createRefExpr("epsilon")),
                     createStmtlstNode(raymarchIfStatements), createStmtlstNode(raymarchElseStatements))));

    // procedure raymarch
    raymarchStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(1, Variable("ro"), createRefExpr(13))));
    raymarchStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("rd"), createRefExpr(19))));
    raymarchStmts.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(3, Variable("depth"))));
    raymarchStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        4, createLtExpr(createRefExpr("count"), createRefExpr("steps")), createStmtlstNode(raymarchWhileStatements))));

    StmtlstNode* raymarchStmtLstNode = createStmtlstNode(raymarchStmts);
    ProcedureNode* raymarchProc = createProcedureNode("raymarch", raymarchStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(raymarchProc));
    ProgramNode* programNode = createProgramNode("main", procedureList, 9);

    return programNode;
}

String getProgram22String_whileNestedInWhile()
{
    String readPoints = "\
        procedure readPoint {\n\
        if ((x < y) && (y < z)) then {\n\
          y = z + x;\n\
        } else {\n\
            y = x + y; }\n\
        while (x > z) { \n\
            x = y;\n\
            y = normSq + 3;\n\
            while (normSq > 3) {\n\
               print normSq;\n\
            }\n\
        }\n\
        normSq = y;\n\
    }\n\
";
    /*
     * Annotated with statement numbers:
     procedure readPoint {
     1    if ((x < y) && (y < z)) then {
     2        y = z + x;
         } else {
     3        y = x + y;}
     4    while (x > z) {
     5        x = y;
     6        y = normSq + 3;
     7        while (normSq > 3) {
     8            print normSq;}}
     9    normSq = y;}
     */
    return readPoints;
}

ProgramNode* getProgram22Tree_whileNestedInWhile()
{
    List<ProcedureNode> procedureList;
    List<StatementNode> readPointStmts;

    // procedure readPoint if & whiles
    List<StatementNode> readPointIfStatements;
    List<StatementNode> readPointElseStatements;
    List<StatementNode> readPointOuterWhileStatements;
    List<StatementNode> readPointInnerWhileStatements;

    readPointIfStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(2, Variable("y"), createPlusExpr(createRefExpr("z"), createRefExpr("x")))));
    readPointElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(3, Variable("y"), createPlusExpr(createRefExpr("x"), createRefExpr("y")))));

    readPointInnerWhileStatements.push_back(
        std::unique_ptr<PrintStatementNode>(createPrintNode(8, Variable("normSq"))));

    readPointOuterWhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("x"), createRefExpr("y"))));
    readPointOuterWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(6, Variable("y"), createPlusExpr(createRefExpr("normSq"), createRefExpr(3)))));
    readPointOuterWhileStatements.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(7, createGtExpr(createRefExpr("normSq"), createRefExpr("3")),
                        createStmtlstNode(readPointInnerWhileStatements))));

    // procedure redPoint
    readPointStmts.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(1,
                     createAndExpr(createLtExpr(createRefExpr("x"), createRefExpr("y")),
                                   createLtExpr(createRefExpr("y"), createRefExpr("z"))),
                     createStmtlstNode(readPointIfStatements), createStmtlstNode(readPointElseStatements))));
    readPointStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        4, createGtExpr(createRefExpr("x"), createRefExpr("y")), createStmtlstNode(readPointOuterWhileStatements))));
    readPointStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(9, Variable("normSq"), createRefExpr("y"))));

    StmtlstNode* readPointStmtLstNode = createStmtlstNode(readPointStmts);
    ProcedureNode* readPointProc = createProcedureNode("readPoint", readPointStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(readPointProc));
    ProgramNode* programNode = createProgramNode("main", procedureList, 9);

    return programNode;
}

String getProgram23String_nestedWhileIfs()
{
    String nestedWhileIfs = "\
      procedure doSomething {\n\
        if (TryingToMakeLongVariableNamesToTestLimits != 0) then {\n\
          while (procedure == 0) {\n\
            print p;\n\
          }\n\
          if (procedure > 0) then {\n\
            print p;\n\
          } else {\n\
            read c;\n\
          }\n\
        } else {\n\
          b = c + constantAssignment;\n\
        }\n\
        while (constantAssignment != 3) {\n\
          ifStmtConstVal = 57 % (x - 86) * y;\n\
        }\n\
     }\n\
";

    /*
    * Annotated with statement numbers:
    procedure doSomething {
    1  if (TryingToMakeLongVariableNamesToTestLimits != 0) then {
    2    while (procedure == 0) {
    3      print p; }
    4        if (procedure > 0 ) then {
    5          print p;
             } else {
    6          read c;
             }
        } else {
    7     b = c + constantAssignment;
        }
    8   while (constantAssignment != 3) {
    9     ifStmtConstVal = 57 % (x - 86) * y; } }
    */
    return nestedWhileIfs;
}

ProgramNode* getProgram23Tree_nestedWhileIfs()
{
    List<ProcedureNode> procedureList;
    List<StatementNode> doSmthStmts;

    // procedure readPoint if & whiles
    List<StatementNode> doSmthOuterIfStatements;
    List<StatementNode> doSmthOuterElseStatements;

    List<StatementNode> doSmthInnerIfStatements;
    List<StatementNode> doSmthInnerElseStatements;

    List<StatementNode> doSmthInnerWhileStatements;

    List<StatementNode> doSmthOuterWhileStatements;

    doSmthInnerWhileStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(3, Variable("p"))));

    doSmthOuterIfStatements.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        2, createEqExpr(createRefExpr("procedure"), createRefExpr(0)), createStmtlstNode(doSmthInnerWhileStatements))));

    doSmthInnerIfStatements.push_back(std::unique_ptr<PrintStatementNode>(createPrintNode(5, Variable("p"))));
    doSmthInnerElseStatements.push_back(std::unique_ptr<ReadStatementNode>(createReadNode(6, Variable("c"))));

    doSmthOuterIfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createGtExpr(createRefExpr("procedure"), createRefExpr(0)),
                     createStmtlstNode(doSmthInnerIfStatements), createStmtlstNode(doSmthInnerElseStatements))));

    doSmthOuterElseStatements.push_back(std::unique_ptr<AssignmentStatementNode>(
        createAssignNode(7, Variable("b"), createPlusExpr(createRefExpr("c"), createRefExpr("constantAssignment")))));

    doSmthStmts.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(1, createNeqExpr(createRefExpr("procedure"), createRefExpr(0)),
                     createStmtlstNode(doSmthOuterIfStatements), createStmtlstNode(doSmthOuterElseStatements))));

    doSmthOuterWhileStatements.push_back(std::unique_ptr<AssignmentStatementNode>(createAssignNode(
        9, Variable("ifStmtConstVal"),
        createDivExpr(createRefExpr(57),
                      createTimesExpr(createTimesExpr(createRefExpr("x"), createRefExpr(86)), createRefExpr("y"))))));

    doSmthStmts.push_back(std::unique_ptr<WhileStatementNode>(
        createWhileNode(8, createNeqExpr(createRefExpr("constantAssignment"), createRefExpr(3)),
                        createStmtlstNode(doSmthOuterWhileStatements))));

    StmtlstNode* doSmthStmtLstNode = createStmtlstNode(doSmthStmts);
    ProcedureNode* doSmthProc = createProcedureNode("doSmth", doSmthStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(doSmthProc));
    ProgramNode* programNode = createProgramNode("main", procedureList, 9);

    return programNode;
}

String getProgram24String_nestedIfs()
{
    String nestedIfs = "\
      procedure nested1 {\n\
        if (stmt == 1) then {\n\
          stmt = 2;\n\
        } else {\n\
          if (stmt == 3) then {\n\
            stmt = 4;\n\
          } else {\n\
            if (stmt == 5) then {\n\
              stmt = 6;\n\
            } else {\n\
              if (stmt == 7) then {\n\
                stmt = 8;\n\
              } else {\n\
                if (stmt == 9) then {\n\
                  stmt = 10;\n\
                } else {\n\
                  if (stmt == 11) then {\n\
                    stmt = 12;\n\
                  } else {\n\
                    if (stmt == 13) then {\n\
                      stmt = 14;\n\
                    } else {\n\
                      if (stmt == 15) then {\n\
                        stmt = 16;\n\
                      } else {\n\
                        if (stmt == 17) then {\n\
                          stmt = 18;\n\
                        } else {\n\
                          stmt = 19; }}}}}}}}}\n\
        stmt = 20; }\n\
";

    /*
    * Annotated with statement numbers:
      procedure nested1 {\n\
    1   if (stmt == 1) then {\n\
    2     stmt = 2;\n\
        } else {\n\
    3     if (stmt == 3) then {\n\
    4       stmt = 4;\n\
          } else {\n\
    5       if (stmt == 5) then {\n\
    6         stmt = 6;\n\
            } else {\n\
    7         if (stmt == 7) then {\n\
    8           stmt = 8;\n\
              } else {\n\
    9           if (stmt == 9) then {\n\
    10            stmt = 10;\n\
                } else {\n\
    11            if (stmt == 11) then {\n\
    12              stmt = 12;\n\
                  } else {\n\
    13              if (stmt == 13) then {\n\
    14                stmt = 14;\n\
                    } else {\n\
    15                if (stmt == 15) then {\n\
    16                  stmt = 16;\n\
                      } else {\n\
    17                  if (stmt == 17) then {\n\
    18                    stmt = 18;\n\
                        } else {\n\
    19                    stmt = 19; }}}}}}}}}\n\
    20  stmt = 20;}\n\
    */
    return nestedIfs;
}

ProgramNode* getProgram24Tree_nestedIfs()
{
    List<ProcedureNode> procedureList;
    List<StatementNode> doSmthStmts;

    // procedure doSmth ifs
    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;
    List<StatementNode> doSmth3IfStatements;
    List<StatementNode> doSmth3ElseStatements;

    List<StatementNode> doSmth4IfStatements;
    List<StatementNode> doSmth4ElseStatements;

    List<StatementNode> doSmth5IfStatements;
    List<StatementNode> doSmth5ElseStatements;

    List<StatementNode> doSmth6IfStatements;
    List<StatementNode> doSmth6ElseStatements;

    List<StatementNode> doSmth7IfStatements;
    List<StatementNode> doSmth7ElseStatements;

    List<StatementNode> doSmth8IfStatements;
    List<StatementNode> doSmth8ElseStatements;

    List<StatementNode> doSmth9IfStatements;
    List<StatementNode> doSmth9ElseStatements;

    doSmth9IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));

    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6))));

    doSmth3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));

    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4))));

    doSmth2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));

    doSmth1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));

    doSmth1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    doSmthStmts.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(1, createEqExpr(createRefExpr("procedure"), createRefExpr(0)),
                     createStmtlstNode(doSmth1IfStatements), createStmtlstNode(doSmth1ElseStatements))));

    doSmthStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(20))));

    StmtlstNode* doSmthStmtLstNode = createStmtlstNode(doSmthStmts);
    ProcedureNode* doSmthProc = createProcedureNode("doSmth", doSmthStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(doSmthProc));
    ProgramNode* programNode = createProgramNode("main", procedureList, 20);

    return programNode;
}

String getProgram25String_nestedIfsInWhile()
{
    String nestedIfs = "\
      procedure nested2 {\n\
        while (stmt == 1) {\n\
          stmt = 2;\n\
          if (stmt == 3) then {\n\
            stmt = 4;\n\
          } else {\n\
            if (stmt == 5) then {\n\
              stmt = 6;\n\
            } else {\n\
              if (stmt == 7) then {\n\
                stmt = 8;\n\
              } else {\n\
                if (stmt == 9) then {\n\
                  stmt = 10;\n\
                } else {\n\
                  if (stmt == 11) then {\n\
                    stmt = 12;\n\
                  } else {\n\
                    if (stmt == 13) then {\n\
                      stmt = 14;\n\
                    } else {\n\
                      if (stmt == 15) then {\n\
                        stmt = 16;\n\
                      } else {\n\
                        if (stmt == 17) then {\n\
                          stmt = 18;\n\
                        } else {\n\
                          stmt = 19; }}}}}}}}}\n\
        stmt = 20; }\n\
";
    /*
    * Annotated with statement numbers:
    procedure nested2 {\n\
    1   while (stmt == 1) {\n\
    2     stmt = 2;\n\
    3     if (stmt == 3) then {\n\
    4       stmt = 4;\n\
          } else {\n\
    5       if (stmt == 5) then {\n\
    6         stmt = 6;\n\
            } else {\n\
    7         if (stmt == 7) then {\n\
    8           stmt = 8;\n\
              } else {\n\
    9           if (stmt == 9) then {\n\
    10            stmt = 10;\n\
                } else {\n\
    11            if (stmt == 11) then {\n\
    12              stmt = 12;\n\
                  } else {\n\
    13              if (stmt == 13) then {\n\
    14                stmt = 14;\n\
                    } else {\n\
    15                if (stmt == 15) then {\n\
    16                  stmt = 16;\n\
                      } else {\n\
    17                  if (stmt == 17) then {\n\
    18                    stmt = 18;\n\
                        } else {\n\
    19                    stmt = 19; }}}}}}}}}\n\
    20  stmt = 20; }\n\
    */
    return nestedIfs;
}

ProgramNode* getProgram25Tree_nestedIfsInWhile()
{
    List<ProcedureNode> procedureList;
    List<StatementNode> doSmthStmts;

    // procedure doSmth while & ifs
    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;
    List<StatementNode> doSmth3IfStatements;
    List<StatementNode> doSmth3ElseStatements;

    List<StatementNode> doSmth4IfStatements;
    List<StatementNode> doSmth4ElseStatements;

    List<StatementNode> doSmth5IfStatements;
    List<StatementNode> doSmth5ElseStatements;

    List<StatementNode> doSmth6IfStatements;
    List<StatementNode> doSmth6ElseStatements;

    List<StatementNode> doSmth7IfStatements;
    List<StatementNode> doSmth7ElseStatements;

    List<StatementNode> doSmth8IfStatements;
    List<StatementNode> doSmth8ElseStatements;

    List<StatementNode> doSmthWhileStatements;

    doSmth8IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth7IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth7ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(17)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth6ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(15)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth5ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(13)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(10, Variable("stmt"), createRefExpr(10))));

    doSmth4ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(8, Variable("stmt"), createRefExpr(8))));

    doSmth3ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));

    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(6, Variable("stmt"), createRefExpr(6))));

    doSmth2ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));

    doSmth1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4))));

    doSmth1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    doSmthWhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));

    doSmthWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth1IfStatements),
                     createStmtlstNode(doSmth1ElseStatements))));

    doSmthStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(doSmthWhileStatements))));

    doSmthStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(20))));

    StmtlstNode* doSmthStmtLstNode = createStmtlstNode(doSmthStmts);
    ProcedureNode* doSmthProc = createProcedureNode("doSmth", doSmthStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(doSmthProc));
    ProgramNode* programNode = createProgramNode("main", procedureList, 20);

    return programNode;
}

String getProgram26String_nestedInterleavingIfsInWhile()
{
    String nestedIfsInWhile = "\
      procedure nested3 {\n\
        while (stmt == 1) {\n\
          if (stmt == 2) then {\n\
		    if (stmt == 3) then {\n\
		      if (stmt == 4) then {\n\
                if (stmt == 5) then {\n\
                  if (stmt == 6) then {\n\
                    if (stmt == 7) then {\n\
                      if (stmt == 8) then {\n\
                        if (stmt == 9) then {\n\
                          if (stmt == 10) then {\n\
                            if (stmt == 11) then {\n\
                              stmt = 12;\n\
                            } else {\n\
                              stmt = 13;\n\
                            }\n\
                          } else {\n\
                            stmt = 14;\n\
                          }\n\
                        } else {\n\
                          stmt = 15;\n\
                        }\n\
                      } else {\n\
                        stmt = 16;\n\
                      }\n\
                    } else {\n\
                      stmt = 17;\n\
                    }\n\
                  } else {\n\
                    stmt = 18;\n\
                  }\n\
                } else {\n\
                  stmt = 19;\n\
                }\n\
		      } else {\n\
		        stmt = 20;\n\
		      }\n\
		    } else {\n\
		      stmt = 21;\n\
		    }\n\
          } else {\n\
		    stmt = 22;\n\
	      }\n\
        }\n\
        stmt = 23; }\n\
";
    /*
    * Annotated with statement numbers:
      procedure nested3 {
    1   while (stmt == 1) {
    2     if (stmt == 2) then {
    3       if (stmt == 3) then {
    4         if (stmt == 4) then {
    5           if (stmt == 5) then {
    6             if (stmt == 6) then {
    7               if (stmt == 7) then {
    8                 if (stmt == 8) then {
    9                   if (stmt == 9) then {
    10                    if (stmt == 10) then {
    11                      if (stmt == 11) then {
    12                        stmt = 12;
                            } else {
    13                        stmt = 13;
                            }
                          } else {
    14                      stmt = 14;
                          }
                        } else {
    15                    stmt = 15;
                        }
                      } else {
    16                  stmt = 16;
                      }
                    } else {
    17                stmt = 17;
                    }
                  } else {
    18              stmt = 18;
                  }
                } else {
    19            stmt = 19;
                }
              } else {
    20          stmt = 20;
              }
            } else {
    21        stmt = 21;
            }
          } else {
    22      stmt = 22;
          }
        }
    23  stmt = 23; }
    */
    return nestedIfsInWhile;
}

ProgramNode* getProgram26Tree_nestedInterleavingIfsInWhile()
{
    List<ProcedureNode> procedureList;
    List<StatementNode> doSmthStmts;

    // procedure doSmth while & ifs
    List<StatementNode> doSmthWhileStatements;

    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;
    List<StatementNode> doSmth3IfStatements;
    List<StatementNode> doSmth3ElseStatements;

    List<StatementNode> doSmth4IfStatements;
    List<StatementNode> doSmth4ElseStatements;

    List<StatementNode> doSmth5IfStatements;
    List<StatementNode> doSmth5ElseStatements;

    List<StatementNode> doSmth6IfStatements;
    List<StatementNode> doSmth6ElseStatements;

    List<StatementNode> doSmth7IfStatements;
    List<StatementNode> doSmth7ElseStatements;

    List<StatementNode> doSmth8IfStatements;
    List<StatementNode> doSmth8ElseStatements;

    List<StatementNode> doSmth9IfStatements;
    List<StatementNode> doSmth9ElseStatements;

    List<StatementNode> doSmth10IfStatements;
    List<StatementNode> doSmth10ElseStatements;

    doSmth1ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(22))));

    doSmth2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(21))));

    doSmth3ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(20))));

    doSmth4ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(19))));

    doSmth5ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(18, Variable("stmt"), createRefExpr(18))));

    doSmth6ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(17, Variable("stmt"), createRefExpr(17))));

    doSmth7ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(16, Variable("stmt"), createRefExpr(16))));

    doSmth8ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(15, Variable("stmt"), createRefExpr(15))));

    doSmth9ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(14, Variable("stmt"), createRefExpr(14))));

    doSmth10ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(13, Variable("stmt"), createRefExpr(13))));

    doSmth10IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(12, Variable("stmt"), createRefExpr(12))));

    doSmth9IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(doSmth10IfStatements), createStmtlstNode(doSmth10ElseStatements))));

    doSmth8IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(10)), createStmtlstNode(doSmth9IfStatements),
                     createStmtlstNode(doSmth9ElseStatements))));

    doSmth7IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(doSmth8IfStatements),
                     createStmtlstNode(doSmth8ElseStatements))));

    doSmth6IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(8, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(doSmth7IfStatements),
                     createStmtlstNode(doSmth7ElseStatements))));

    doSmth5IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(7, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(doSmth6IfStatements),
                     createStmtlstNode(doSmth6ElseStatements))));

    doSmth4IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(6, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(doSmth5IfStatements),
                     createStmtlstNode(doSmth5ElseStatements))));

    doSmth3IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(5, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(doSmth4IfStatements),
                     createStmtlstNode(doSmth4ElseStatements))));

    doSmth2IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(doSmth3IfStatements),
                     createStmtlstNode(doSmth3ElseStatements))));

    doSmth1IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    doSmthWhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(2, createEqExpr(createRefExpr("stmt"), createRefExpr(2)), createStmtlstNode(doSmth1IfStatements),
                     createStmtlstNode(doSmth1ElseStatements))));

    doSmthStmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(doSmthWhileStatements))));

    doSmthStmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(23))));

    StmtlstNode* doSmthStmtLstNode = createStmtlstNode(doSmthStmts);
    ProcedureNode* doSmthProc = createProcedureNode("doSmth", doSmthStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(doSmthProc));
    ProgramNode* programNode = createProgramNode("main", procedureList, 23);

    return programNode;
}

String getProgram27String_ifInIf()
{
    String ifInIf = "\
      procedure ifInIf {\n\
        if (stmt == 1) then {\n\
          stmt = 2;\n\
        } else {\n\
          if (stmt == 3) then {\n\
            stmt = 4;\n\
          } else {\n\
            stmt = 5;\n\
          } } }\n\
";
    /*
    * Annotated with statement numbers:
      procedure ifInIf {
    1   if (stmt == 1) then {
    2     stmt = 2;
        } else {
    3     if (stmt == 3) then {
    4       stmt = 4;
          } else {
    5       stmt = 5;
          } } }
    */
    return ifInIf;
}

ProgramNode* getProgram27Tree_ifInIf()
{
    List<ProcedureNode> procedureList;
    List<StatementNode> doSmthStmts;

    // procedure doSmth while & ifs

    List<StatementNode> doSmth1IfStatements;
    List<StatementNode> doSmth1ElseStatements;

    List<StatementNode> doSmth2IfStatements;
    List<StatementNode> doSmth2ElseStatements;

    doSmth2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(4, Variable("stmt"), createRefExpr(4))));

    doSmth2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("stmt"), createRefExpr(5))));

    doSmth1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));

    doSmth1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(3, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(doSmth2IfStatements),
                     createStmtlstNode(doSmth2ElseStatements))));

    doSmthStmts.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(doSmth1IfStatements),
                     createStmtlstNode(doSmth1ElseStatements))));

    StmtlstNode* doSmthStmtLstNode = createStmtlstNode(doSmthStmts);
    ProcedureNode* doSmthProc = createProcedureNode("doSmth", doSmthStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(doSmthProc));
    ProgramNode* programNode = createProgramNode("main", procedureList, 5);

    return programNode;
}

String getProgram28String_combineIfInIfAndNested3AndNested2()
{
    String ifInIf = "\
      procedure ifInIf {\n\
        if (stmt == 1) then {\n\
          stmt = 2;\n\
          call nested3;\n\
        } else {\n\
          if (stmt == 3) then {\n\
            stmt = 4;\n\
          } else {\n\
            stmt = 5;\n\
          } } }\n\
      procedure nested3 {\n\
        while (stmt == 1) {\n\
          if (stmt == 2) then {\n\
		    if (stmt == 3) then {\n\
		      if (stmt == 4) then {\n\
                if (stmt == 5) then {\n\
                  if (stmt == 6) then {\n\
                    if (stmt == 7) then {\n\
                      if (stmt == 8) then {\n\
                        if (stmt == 9) then {\n\
                          if (stmt == 10) then {\n\
                            if (stmt == 11) then {\n\
                              stmt = 12;\n\
                            } else {\n\
                              stmt = 13;\n\
                            }\n\
                          } else {\n\
                            stmt = 14;\n\
                          }\n\
                        } else {\n\
                          stmt = 15;\n\
                        }\n\
                      } else {\n\
                        stmt = 16;\n\
                      }\n\
                    } else {\n\
                      stmt = 17;\n\
                    }\n\
                  } else {\n\
                    stmt = 18;\n\
                  }\n\
                } else {\n\
                  stmt = 19;\n\
                }\n\
		      } else {\n\
		        stmt = 20;\n\
		      }\n\
		    } else {\n\
		      stmt = 21;\n\
		    }\n\
          } else {\n\
		    stmt = 22;\n\
	      }\n\
        }\n\
        stmt = 23;\n\
        call nested2; }\n\
      procedure nested2 {\n\
        while (stmt == 1) {\n\
          stmt = 2;\n\
          if (stmt == 3) then {\n\
            stmt = 4;\n\
          } else {\n\
            if (stmt == 5) then {\n\
              stmt = 6;\n\
            } else {\n\
              if (stmt == 7) then {\n\
                stmt = 8;\n\
              } else {\n\
                if (stmt == 9) then {\n\
                  stmt = 10;\n\
                } else {\n\
                  if (stmt == 11) then {\n\
                    stmt = 12;\n\
                  } else {\n\
                    if (stmt == 13) then {\n\
                      stmt = 14;\n\
                    } else {\n\
                      if (stmt == 15) then {\n\
                        stmt = 16;\n\
                      } else {\n\
                        if (stmt == 17) then {\n\
                          stmt = 18;\n\
                        } else {\n\
                          stmt = 19; }}}}}}}}}\n\
        stmt = 20; }\n\
";
    /*
    * Annotated with statement numbers:
      procedure ifInIf {
    1   if (stmt == 1) then {
    2     stmt = 2;
    3     call nested3;
        } else {
    4     if (stmt == 3) then {
    5       stmt = 4;
    6       call nested2;
          } else {
    7       stmt = 5;
          } } }

      procedure nested3 {
    8   while (stmt == 1) {
    9     if (stmt == 2) then {
    10      if (stmt == 3) then {
    11        if (stmt == 4) then {
    12          if (stmt == 5) then {
    13            if (stmt == 6) then {
    14              if (stmt == 7) then {
    15                if (stmt == 8) then {
    16                  if (stmt == 9) then {
    17                    if (stmt == 10) then {
    18                      if (stmt == 11) then {
    19                        stmt = 12;
                            } else {
    20                        stmt = 13;
                            }
                          } else {
    21                      stmt = 14;
                          }
                        } else {
    22                    stmt = 15;
                        }
                      } else {
    23                  stmt = 16;
                      }
                    } else {
    24                stmt = 17;
                    }
                  } else {
    25              stmt = 18;
                  }
                } else {
    26            stmt = 19;
                }
              } else {
    27          stmt = 20;
              }
            } else {
    28        stmt = 21;
            }
          } else {
    29      stmt = 22;
          }
        }
    30  stmt = 23;
    31  call nested2;}

        procedure nested2 {\n\
    32  while (stmt == 1) {\n\
    33    stmt = 2;\n\
    34    if (stmt == 3) then {\n\
    35      stmt = 4;\n\
          } else {\n\
    36      if (stmt == 5) then {\n\
    37        stmt = 6;\n\
            } else {\n\
    38        if (stmt == 7) then {\n\
    39          stmt = 8;\n\
              } else {\n\
    40          if (stmt == 9) then {\n\
    41            stmt = 10;\n\
                } else {\n\
    42            if (stmt == 11) then {\n\
    43              stmt = 12;\n\
                  } else {\n\
    44              if (stmt == 13) then {\n\
    45                stmt = 14;\n\
                    } else {\n\
    46                if (stmt == 15) then {\n\
    47                  stmt = 16;\n\
                      } else {\n\
    48                  if (stmt == 17) then {\n\
    49                    stmt = 18;\n\
                        } else {\n\
    50                    stmt = 19; }}}}}}}}}\n\
    51  stmt = 20; }\n\

    */
    return ifInIf;
}

ProgramNode* getProgram28Tree_combineIfInIfAndNested3AndNested2()
{
    List<ProcedureNode> procedureList;

    // procedure ifInIf while & ifs
    List<StatementNode> ifInIfStmts;

    List<StatementNode> ifInIf1IfStatements;
    List<StatementNode> ifInIf1ElseStatements;

    List<StatementNode> ifInIf2IfStatements;
    List<StatementNode> ifInIf2ElseStatements;

    ifInIf2IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(5, Variable("stmt"), createRefExpr(4))));

    ifInIf2IfStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(6, "nested2")));

    ifInIf2ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(7, Variable("stmt"), createRefExpr(5))));

    ifInIf1IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(2, Variable("stmt"), createRefExpr(2))));

    ifInIf1IfStatements.push_back(std::unique_ptr<CallStatementNode>(createCallNode(3, "nested3")));

    ifInIf1ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(4, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(ifInIf2IfStatements),
                     createStmtlstNode(ifInIf2ElseStatements))));

    ifInIfStmts.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(1, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(ifInIf1IfStatements),
                     createStmtlstNode(ifInIf1ElseStatements))));
    StmtlstNode* ifInIfStmtLstNode = createStmtlstNode(ifInIfStmts);
    ProcedureNode* ifInIfProc = createProcedureNode("ifInIf", ifInIfStmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(ifInIfProc));

    // procedure nested while & ifs
    List<StatementNode> nested3Stmts;

    List<StatementNode> nested3WhileStatements;

    List<StatementNode> nested31IfStatements;
    List<StatementNode> nested31ElseStatements;

    List<StatementNode> nested32IfStatements;
    List<StatementNode> nested32ElseStatements;
    List<StatementNode> nested33IfStatements;
    List<StatementNode> nested33ElseStatements;

    List<StatementNode> nested34IfStatements;
    List<StatementNode> nested34ElseStatements;

    List<StatementNode> nested35IfStatements;
    List<StatementNode> nested35ElseStatements;

    List<StatementNode> nested36IfStatements;
    List<StatementNode> nested36ElseStatements;

    List<StatementNode> nested37IfStatements;
    List<StatementNode> nested37ElseStatements;

    List<StatementNode> nested38IfStatements;
    List<StatementNode> nested38ElseStatements;

    List<StatementNode> nested39IfStatements;
    List<StatementNode> nested39ElseStatements;

    List<StatementNode> nested310IfStatements;
    List<StatementNode> nested310ElseStatements;

    nested31ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(29, Variable("stmt"), createRefExpr(22))));

    nested32ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(28, Variable("stmt"), createRefExpr(21))));

    nested33ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(27, Variable("stmt"), createRefExpr(20))));

    nested34ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(26, Variable("stmt"), createRefExpr(19))));

    nested35ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(25, Variable("stmt"), createRefExpr(18))));

    nested36ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(24, Variable("stmt"), createRefExpr(17))));

    nested37ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(23, Variable("stmt"), createRefExpr(16))));

    nested38ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(22, Variable("stmt"), createRefExpr(15))));

    nested39ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(21, Variable("stmt"), createRefExpr(14))));

    nested310ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(20, Variable("stmt"), createRefExpr(13))));

    nested310IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(19, Variable("stmt"), createRefExpr(12))));

    nested39IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(18, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested310IfStatements), createStmtlstNode(nested310ElseStatements))));

    nested38IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(17, createEqExpr(createRefExpr("stmt"), createRefExpr(10)),
                     createStmtlstNode(nested39IfStatements), createStmtlstNode(nested39ElseStatements))));

    nested37IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(16, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested38IfStatements),
                     createStmtlstNode(nested38ElseStatements))));

    nested36IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(15, createEqExpr(createRefExpr("stmt"), createRefExpr(8)), createStmtlstNode(nested37IfStatements),
                     createStmtlstNode(nested37ElseStatements))));

    nested35IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(14, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested36IfStatements),
                     createStmtlstNode(nested36ElseStatements))));

    nested34IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(13, createEqExpr(createRefExpr("stmt"), createRefExpr(6)), createStmtlstNode(nested35IfStatements),
                     createStmtlstNode(nested35ElseStatements))));

    nested33IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(12, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested34IfStatements),
                     createStmtlstNode(nested34ElseStatements))));

    nested32IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(11, createEqExpr(createRefExpr("stmt"), createRefExpr(4)), createStmtlstNode(nested33IfStatements),
                     createStmtlstNode(nested33ElseStatements))));

    nested31IfStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(10, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(nested32IfStatements),
                     createStmtlstNode(nested32ElseStatements))));

    nested3WhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(9, createEqExpr(createRefExpr("stmt"), createRefExpr(2)), createStmtlstNode(nested31IfStatements),
                     createStmtlstNode(nested31ElseStatements))));

    nested3Stmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        8, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(nested3WhileStatements))));

    nested3Stmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(30, Variable("stmt"), createRefExpr(23))));

    nested3Stmts.push_back(std::unique_ptr<CallStatementNode>(createCallNode(31, "nested2")));

    StmtlstNode* nested3StmtLstNode = createStmtlstNode(nested3Stmts);
    ProcedureNode* nested3Proc = createProcedureNode("nested3", nested3StmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(nested3Proc));

    // procedure doSmth while & ifs
    List<StatementNode> nested2Stmts;

    List<StatementNode> nested21IfStatements;
    List<StatementNode> nested21ElseStatements;

    List<StatementNode> nested22IfStatements;
    List<StatementNode> nested22ElseStatements;
    List<StatementNode> nested23IfStatements;
    List<StatementNode> nested23ElseStatements;

    List<StatementNode> nested24IfStatements;
    List<StatementNode> nested24ElseStatements;

    List<StatementNode> nested25IfStatements;
    List<StatementNode> nested25ElseStatements;

    List<StatementNode> nested26IfStatements;
    List<StatementNode> nested26ElseStatements;

    List<StatementNode> nested27IfStatements;
    List<StatementNode> nested27ElseStatements;

    List<StatementNode> nested28IfStatements;
    List<StatementNode> nested28ElseStatements;

    List<StatementNode> nested2WhileStatements;

    nested28IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(49, Variable("stmt"), createRefExpr(18))));

    nested28ElseStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(50, Variable("stmt"), createRefExpr(19))));

    nested27IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(47, Variable("stmt"), createRefExpr(16))));

    nested27ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(48, createEqExpr(createRefExpr("stmt"), createRefExpr(17)),
                     createStmtlstNode(nested28IfStatements), createStmtlstNode(nested28ElseStatements))));

    nested26IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(45, Variable("stmt"), createRefExpr(14))));

    nested26ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(46, createEqExpr(createRefExpr("stmt"), createRefExpr(15)),
                     createStmtlstNode(nested27IfStatements), createStmtlstNode(nested27ElseStatements))));

    nested25IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(43, Variable("stmt"), createRefExpr(12))));

    nested25ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(44, createEqExpr(createRefExpr("stmt"), createRefExpr(13)),
                     createStmtlstNode(nested26IfStatements), createStmtlstNode(nested26ElseStatements))));

    nested24IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(41, Variable("stmt"), createRefExpr(10))));

    nested24ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(42, createEqExpr(createRefExpr("stmt"), createRefExpr(11)),
                     createStmtlstNode(nested25IfStatements), createStmtlstNode(nested25ElseStatements))));

    nested23IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(39, Variable("stmt"), createRefExpr(8))));

    nested23ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(40, createEqExpr(createRefExpr("stmt"), createRefExpr(9)), createStmtlstNode(nested24IfStatements),
                     createStmtlstNode(nested24ElseStatements))));

    nested22IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(37, Variable("stmt"), createRefExpr(6))));

    nested22ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(38, createEqExpr(createRefExpr("stmt"), createRefExpr(7)), createStmtlstNode(nested23IfStatements),
                     createStmtlstNode(nested23ElseStatements))));

    nested21IfStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(35, Variable("stmt"), createRefExpr(4))));

    nested21ElseStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(36, createEqExpr(createRefExpr("stmt"), createRefExpr(5)), createStmtlstNode(nested22IfStatements),
                     createStmtlstNode(nested22ElseStatements))));

    nested2WhileStatements.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(33, Variable("stmt"), createRefExpr(2))));

    nested2WhileStatements.push_back(std::unique_ptr<IfStatementNode>(
        createIfNode(34, createEqExpr(createRefExpr("stmt"), createRefExpr(3)), createStmtlstNode(nested21IfStatements),
                     createStmtlstNode(nested21ElseStatements))));

    nested2Stmts.push_back(std::unique_ptr<WhileStatementNode>(createWhileNode(
        32, createEqExpr(createRefExpr("stmt"), createRefExpr(1)), createStmtlstNode(nested2WhileStatements))));

    nested2Stmts.push_back(
        std::unique_ptr<AssignmentStatementNode>(createAssignNode(51, Variable("stmt"), createRefExpr(20))));

    StmtlstNode* nested2StmtLstNode = createStmtlstNode(nested2Stmts);
    ProcedureNode* nested2Proc = createProcedureNode("nested2", nested2StmtLstNode);
    procedureList.push_back(std::unique_ptr<ProcedureNode>(nested2Proc));

    ProgramNode* programNode = createProgramNode("main", procedureList, 51);
    return programNode;
}
