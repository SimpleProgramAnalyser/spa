/**
 * Implementation of SPA frontend parser.
 */

#include "Parser.h"

#include <cassert>

#include "Token.h"
#include "ast/AstLibrary.h"
#include "lexer/Lexer.h"

typedef int BracketsDepth;
typedef int TokenListIndex;

template <typename T>
class ParserReturnType {
public:
    T astNode;
    TokenListIndex nextUnparsedToken;

    explicit ParserReturnType(T nullValue): astNode(std::move(nullValue)), nextUnparsedToken(0) {}
    ParserReturnType(T ast, TokenListIndex next): astNode(std::move(ast)), nextUnparsedToken(next) {}
};

ParserReturnType<std::unique_ptr<ArithmeticExpression>>
parseArithmeticExpression(frontend::TokenList* programTokens, TokenListIndex startIndex, TokenListIndex endIndex)
{}

ArithmeticExpression* parseArithmeticExpression(StringList* lexedArithmeticExpression)
{
    return parseArithmeticExpression(frontend::tokeniseSimple(lexedArithmeticExpression), 0,
                                     lexedArithmeticExpression->size())
        .astNode.release();
}

ParserReturnType<std::unique_ptr<Expression>> parseExpression(frontend::TokenList* programTokens,
                                                              TokenListIndex startIndex, TokenListIndex endIndex)
{
    ParserReturnType<std::unique_ptr<ArithmeticExpression>> arithmeticExp
        = parseArithmeticExpression(programTokens, startIndex, endIndex);
    // upcast to Expression
    return ParserReturnType<std::unique_ptr<Expression>>(std::move(arithmeticExp.astNode),
                                                         arithmeticExp.nextUnparsedToken);
}

ParserReturnType<std::unique_ptr<ConditionalExpression>>
parseConditionalExpression(frontend::TokenList* programTokens, TokenListIndex startIndex, TokenListIndex endIndex)
{}

// global variable to help in numbering statements
static int statementsSeen = 0;

ParserReturnType<std::unique_ptr<CallStatementNode>> parseCallStmt(frontend::TokenList* programTokens,
                                                                   TokenListIndex startIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();

    if ((numberOfTokens - startIndex > 2) && programTokens->at(startIndex)->tokenTag == frontend::CallKeywordTag
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1)->tokenTag)
        && programTokens->at(startIndex + 2)->tokenTag == frontend::SemicolonTag) {

        statementsSeen++;
        return ParserReturnType<std::unique_ptr<CallStatementNode>>(
            std::unique_ptr<CallStatementNode>(
                createCallNode(statementsSeen, programTokens->at(startIndex + 1)->rawString)),
            startIndex + 3);
    } else {
        // syntax error in call statement
        return ParserReturnType<std::unique_ptr<CallStatementNode>>(std::unique_ptr<CallStatementNode>{}, -1);
    }
}

ParserReturnType<std::unique_ptr<PrintStatementNode>> parsePrintStmt(frontend::TokenList* programTokens,
                                                                     TokenListIndex startIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();

    if ((numberOfTokens - startIndex > 2) && programTokens->at(startIndex)->tokenTag == frontend::PrintKeywordTag
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1)->tokenTag)
        && programTokens->at(startIndex + 2)->tokenTag == frontend::SemicolonTag) {

        statementsSeen++;
        return ParserReturnType<std::unique_ptr<PrintStatementNode>>(
            std::unique_ptr<PrintStatementNode>(
                createPrintNode(statementsSeen, Variable(programTokens->at(startIndex + 1)->rawString))),
            startIndex + 3);
    } else {
        // syntax error in print statement
        return ParserReturnType<std::unique_ptr<PrintStatementNode>>(std::unique_ptr<PrintStatementNode>{}, -1);
    }
}

ParserReturnType<std::unique_ptr<ReadStatementNode>> parseReadStmt(frontend::TokenList* programTokens,
                                                                   TokenListIndex startIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();

    if ((numberOfTokens - startIndex > 2) && programTokens->at(startIndex)->tokenTag == frontend::ReadKeywordTag
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1)->tokenTag)
        && programTokens->at(startIndex + 2)->tokenTag == frontend::SemicolonTag) {

        statementsSeen++;
        return ParserReturnType<std::unique_ptr<ReadStatementNode>>(
            std::unique_ptr<ReadStatementNode>(
                createReadNode(statementsSeen, Variable(programTokens->at(startIndex + 1)->rawString))),
            startIndex + 3);
    } else {
        // syntax error in read statement
        return ParserReturnType<std::unique_ptr<ReadStatementNode>>(std::unique_ptr<ReadStatementNode>{}, -1);
    }
}

// forward declaration for "if" and "while" statement parsing
ParserReturnType<std::unique_ptr<StmtlstNode>> parseStatementList(frontend::TokenList* programTokens,
                                                                  TokenListIndex startIndex);

ParserReturnType<std::unique_ptr<IfStatementNode>> parseIfStmt(frontend::TokenList* programTokens,
                                                               TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    ParserReturnType<std::unique_ptr<ConditionalExpression>> ifCondition(nullptr);
    ParserReturnType<std::unique_ptr<StmtlstNode>> ifStatements(nullptr);
    ParserReturnType<std::unique_ptr<StmtlstNode>> elseStatements(nullptr);

    // check if "if" syntax is valid
    if ((numberOfTokens - startIndex > 1) && (programTokens->at(startIndex)->tokenTag == frontend::IfKeywordTag)
        && programTokens->at(startIndex + 1)->tokenTag == frontend::BracketOpenTag) {

        // find conditional expression and parse
        BracketsDepth bracketsSeen = 1;
        TokenListIndex tokenPointer = startIndex + 2;
        while (tokenPointer < numberOfTokens && bracketsSeen > 0) {
            frontend::Tag currentToken = programTokens->at(tokenPointer)->tokenTag;
            if (currentToken == frontend::BracketOpenTag) {
                bracketsSeen++;
            } else if (currentToken == frontend::BracketClosedTag) {
                bracketsSeen--;
            }
            tokenPointer++;
        }
        // now token pointer points to the token directly after
        // the closing bracket of if conditional
        //
        // we expect the keyword "then":
        // "if" "(" ... ")" "then" ...
        if (programTokens->at(tokenPointer - 1)->tokenTag == frontend::BracketClosedTag
            && programTokens->at(tokenPointer)->tokenTag == frontend::ThenKeywordTag
            && tokenPointer - 2 > startIndex + 2) {

            ifCondition = parseConditionalExpression(programTokens, startIndex + 2, tokenPointer - 2);
        } else {
            // syntax error in if conditional, or missing "then" keyword
            return ParserReturnType<std::unique_ptr<IfStatementNode>>(std::unique_ptr<IfStatementNode>{}, -1);
        }
        // check if "if" conditional parsed correctly
        if (ifCondition.nextUnparsedToken < 0) {
            // error in parsing conditional expression
            return ParserReturnType<std::unique_ptr<IfStatementNode>>(std::unique_ptr<IfStatementNode>{}, -1);
        }
        statementsSeen++;
        StatementNumber ifStmtNum = statementsSeen;
        // now, parse the "if" statements
        if (tokenPointer < numberOfTokens && programTokens->at(tokenPointer)->tokenTag == frontend::BracesOpenTag) {
            ifStatements = parseStatementList(programTokens, tokenPointer + 1 /* skip "then"*/);
        }
        // check if "if" statements parsed correctly
        if (ifStatements.nextUnparsedToken < 0 || ifStatements.nextUnparsedToken > numberOfTokens) {
            // syntax error in statement list, or no else statement
            return ParserReturnType<std::unique_ptr<IfStatementNode>>(std::unique_ptr<IfStatementNode>{}, -1);
        }
        // now, parse the "else" statements
        if (programTokens->at(ifStatements.nextUnparsedToken)->tokenTag == frontend::ElseKeywordTag) {
            elseStatements = parseStatementList(programTokens, ifStatements.nextUnparsedToken + 1 /* skip "else" */);
        } else {
            // no "else" keyword, wrong if/else syntax
            return ParserReturnType<std::unique_ptr<IfStatementNode>>(std::unique_ptr<IfStatementNode>{}, -1);
        }
        // check if "else" statements parsed correctly
        if (elseStatements.nextUnparsedToken < 0) {
            return ParserReturnType<std::unique_ptr<IfStatementNode>>(std::unique_ptr<IfStatementNode>{}, -1);
        }
        return ParserReturnType<std::unique_ptr<IfStatementNode>>(
            std::unique_ptr<IfStatementNode>(createIfNode(ifStmtNum, ifCondition.astNode.release(),
                                                          ifStatements.astNode.release(),
                                                          elseStatements.astNode.release())),
            elseStatements.nextUnparsedToken);
    }
    // if syntax is invalid
    return ParserReturnType<std::unique_ptr<IfStatementNode>>(std::unique_ptr<IfStatementNode>{}, -1);
}

ParserReturnType<std::unique_ptr<WhileStatementNode>> parseWhileStmt(frontend::TokenList* programTokens,
                                                                     TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    ParserReturnType<std::unique_ptr<ConditionalExpression>> loopControlCondition(nullptr);
    ParserReturnType<std::unique_ptr<StmtlstNode>> statements(nullptr);

    // check if while syntax is valid
    if ((numberOfTokens - startIndex > 1) && (programTokens->at(startIndex)->tokenTag == frontend::WhileKeywordTag)
        && programTokens->at(startIndex + 1)->tokenTag == frontend::BracketOpenTag) {

        // find conditional expression and parse
        BracketsDepth bracketsSeen = 1;
        TokenListIndex tokenPointer = startIndex + 2;
        while (tokenPointer < numberOfTokens && bracketsSeen > 0) {
            frontend::Tag currentToken = programTokens->at(tokenPointer)->tokenTag;
            if (currentToken == frontend::BracketOpenTag) {
                bracketsSeen++;
            } else if (currentToken == frontend::BracketClosedTag) {
                bracketsSeen--;
            }
            tokenPointer++;
        }
        // now token pointer points to the token directly after
        // the closing bracket of while conditional
        //
        // we expect a open braces for statement list:
        // "while" "(" ... ")" "{" ...
        if (programTokens->at(tokenPointer - 1)->tokenTag == frontend::BracketClosedTag
            && tokenPointer - 2 > startIndex + 2) {

            loopControlCondition = parseConditionalExpression(programTokens, startIndex + 2, tokenPointer - 2);
        } else {
            // syntax error in while condition
            return ParserReturnType<std::unique_ptr<WhileStatementNode>>(std::unique_ptr<WhileStatementNode>{}, -1);
        }
        // check if while condition parsed correctly
        if (loopControlCondition.nextUnparsedToken < 0) {
            // error in parsing conditional expression
            return ParserReturnType<std::unique_ptr<WhileStatementNode>>(std::unique_ptr<WhileStatementNode>{}, -1);
        }
        statementsSeen++;
        StatementNumber whileStmtNum = statementsSeen;
        // now, parse the statements
        if (tokenPointer < numberOfTokens && programTokens->at(tokenPointer)->tokenTag == frontend::BracesOpenTag) {
            statements = parseStatementList(programTokens, tokenPointer);
        }
        // check if statements parsed correctly
        if (statements.nextUnparsedToken < 0) {
            // syntax error in statement list
            return ParserReturnType<std::unique_ptr<WhileStatementNode>>(std::unique_ptr<WhileStatementNode>{}, -1);
        }
        return ParserReturnType<std::unique_ptr<WhileStatementNode>>(
            std::unique_ptr<WhileStatementNode>(
                createWhileNode(whileStmtNum, loopControlCondition.astNode.release(), statements.astNode.release())),
            statements.nextUnparsedToken);
    }
    // while syntax is invalid
    return ParserReturnType<std::unique_ptr<WhileStatementNode>>(std::unique_ptr<WhileStatementNode>{}, -1);
}

ParserReturnType<std::unique_ptr<AssignmentStatementNode>> parseAssignStmt(frontend::TokenList* programTokens,
                                                                           TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    ParserReturnType<std::unique_ptr<Expression>> expression(nullptr);
    // verify assign syntax
    if ((numberOfTokens - startIndex) > 3 && frontend::isIdentifierTag(programTokens->at(startIndex)->tokenTag)
        && programTokens->at(startIndex + 1)->tokenTag == frontend::AssignmentTag) {

        Variable assignedVariable = Variable(programTokens->at(startIndex)->rawString);
        // find end of assign expression (semicolon)
        TokenListIndex tokenPointer = startIndex + 2;
        frontend::Tag currentToken = programTokens->at(tokenPointer)->tokenTag;
        while (tokenPointer < numberOfTokens && currentToken != frontend::SemicolonTag) {
            tokenPointer++;
            currentToken = programTokens->at(tokenPointer)->tokenTag;
        }
        // check if semicolon was found
        if (tokenPointer < numberOfTokens && programTokens->at(tokenPointer)->tokenTag == frontend::SemicolonTag
            && tokenPointer - 1 > startIndex + 2) {

            expression = parseExpression(programTokens, startIndex + 2, tokenPointer - 1);
        } else {
            // missing semicolon for assign statement
            return ParserReturnType<std::unique_ptr<AssignmentStatementNode>>(
                std::unique_ptr<AssignmentStatementNode>{}, 0);
        }
        // check if expression parsed correctly
        if (expression.nextUnparsedToken < 0) {
            // syntax error in expression
            return ParserReturnType<std::unique_ptr<AssignmentStatementNode>>(
                std::unique_ptr<AssignmentStatementNode>{}, 0);
        } else {
            // next unparsed token should be the semicolon
            assert(expression.nextUnparsedToken == tokenPointer);
            statementsSeen++;
            return ParserReturnType<std::unique_ptr<AssignmentStatementNode>>(
                std::unique_ptr<AssignmentStatementNode>(
                    createAssignNode(statementsSeen, assignedVariable, expression.astNode.release())),
                expression.nextUnparsedToken + 1 /* skip semicolon */);
        }
    } else {
        // invalid assign syntax, not of form "variable" "=" ...
        return ParserReturnType<std::unique_ptr<AssignmentStatementNode>>(std::unique_ptr<AssignmentStatementNode>{},
                                                                          0);
    }
}

ParserReturnType<std::unique_ptr<StatementNode>> parseStatement(frontend::TokenList* programTokens,
                                                                TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    StatementNode* statement;
    TokenListIndex nextUnparsed = startIndex;
    Boolean isSyntaxError = false; // flag for syntax error

    if (numberOfTokens - startIndex > 1) {
        // determine statement type
        switch (programTokens->at(startIndex)->tokenTag) {
        case frontend::CallKeywordTag: {
            ParserReturnType<std::unique_ptr<CallStatementNode>> resultCall = parseCallStmt(programTokens, startIndex);
            statement = resultCall.astNode.release();
            nextUnparsed = resultCall.nextUnparsedToken;
            break;
        }
        case frontend::PrintKeywordTag: {
            ParserReturnType<std::unique_ptr<PrintStatementNode>> resultPrint
                = parsePrintStmt(programTokens, startIndex);
            statement = resultPrint.astNode.release();
            nextUnparsed = resultPrint.nextUnparsedToken;
            break;
        }
        case frontend::ReadKeywordTag: {
            ParserReturnType<std::unique_ptr<ReadStatementNode>> resultRead = parseReadStmt(programTokens, startIndex);
            statement = resultRead.astNode.release();
            nextUnparsed = resultRead.nextUnparsedToken;
            break;
        }
        case frontend::IfKeywordTag: {
            ParserReturnType<std::unique_ptr<IfStatementNode>> resultIf = parseIfStmt(programTokens, startIndex);
            statement = resultIf.astNode.release();
            nextUnparsed = resultIf.nextUnparsedToken;
            break;
        }
        case frontend::WhileKeywordTag: {
            ParserReturnType<std::unique_ptr<WhileStatementNode>> resultWhile
                = parseWhileStmt(programTokens, startIndex);
            statement = resultWhile.astNode.release();
            nextUnparsed = resultWhile.nextUnparsedToken;
            break;
        }
        case frontend::IdentifierTag:
            // [[fallthrough]];
        default: {
            // assignment statement
            ParserReturnType<std::unique_ptr<AssignmentStatementNode>> resultAssign
                = parseAssignStmt(programTokens, startIndex);
            statement = resultAssign.astNode.release();
            nextUnparsed = resultAssign.nextUnparsedToken;
        }
        }
    } else {
        // empty statement
        isSyntaxError = true;
    }

    if (isSyntaxError || nextUnparsed < 0) {
        return ParserReturnType<std::unique_ptr<StatementNode>>(std::unique_ptr<StatementNode>{}, -1);
    } else {
        return ParserReturnType<std::unique_ptr<StatementNode>>(std::unique_ptr<StatementNode>(statement),
                                                                nextUnparsed);
    }
}

ParserReturnType<std::unique_ptr<StmtlstNode>> parseStatementList(frontend::TokenList* programTokens,
                                                                  TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    List<StatementNode> statements;
    TokenListIndex nextUnparsed = startIndex;
    Boolean isSyntaxError = false; // flag for syntax error

    if (numberOfTokens - startIndex > 1 && programTokens->at(startIndex)->tokenTag == frontend::BracesOpenTag) {
        // iterate through the tokens
        while (nextUnparsed < numberOfTokens && nextUnparsed > 0
               && programTokens->at(nextUnparsed)->tokenTag != frontend::BracesClosedTag) {

            ParserReturnType<std::unique_ptr<StatementNode>> result = parseStatement(programTokens, nextUnparsed);
            statements.push_back(std::move(result.astNode));
            nextUnparsed = result.nextUnparsedToken;
        }

        if (programTokens->at(nextUnparsed)->tokenTag != frontend::BracesClosedTag) {
            // syntax error, unmatched brackets in statement list
            isSyntaxError = true;
        } else {
            // no errors
        }
    } else {
        // syntax error, empty statement list or missing braces
        isSyntaxError = true;
    }

    if (isSyntaxError || nextUnparsed < 0) {
        return ParserReturnType<std::unique_ptr<StmtlstNode>>(std::unique_ptr<StmtlstNode>{}, -1);
    } else {
        return ParserReturnType<std::unique_ptr<StmtlstNode>>(
            std::unique_ptr<StmtlstNode>(createStmtlstNode(statements)), nextUnparsed);
    }
}

ParserReturnType<std::unique_ptr<ProcedureNode>> parseProcedure(frontend::TokenList* programTokens,
                                                                TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    String procedureName;
    StmtlstNode* statementListNode;
    TokenListIndex nextUnparsed = startIndex;
    Boolean isSyntaxError = false; // flag for syntax error

    // check if procedure is valid
    if ((numberOfTokens - startIndex > 2) && (programTokens->at(startIndex)->tokenTag == frontend::ProcedureKeywordTag)
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1)->tokenTag)) {

        procedureName = programTokens->at(startIndex + 1)->rawString;
        ParserReturnType<std::unique_ptr<StmtlstNode>> result = parseStatementList(programTokens, startIndex + 2);
        if (result.nextUnparsedToken < 0) {
            // syntax error in statement list
            isSyntaxError = true;
        } else {
            statementListNode = result.astNode.release();
            nextUnparsed = result.nextUnparsedToken;
        }
    } else {
        // syntax error for procedure, not of form "procedure name" ...
        isSyntaxError = true;
    }

    if (isSyntaxError) {
        delete statementListNode;
        return ParserReturnType<std::unique_ptr<ProcedureNode>>(std::unique_ptr<ProcedureNode>{}, -1);
    } else {
        return ParserReturnType<std::unique_ptr<ProcedureNode>>(
            std::unique_ptr<ProcedureNode>(createProcedureNode(procedureName, statementListNode)), nextUnparsed);
    }
}

Void parseSimple(const String& rawProgram)
{
    StringList* programFragments = splitProgram(rawProgram);
    frontend::TokenList* tokenisedProgram = frontend::tokeniseSimple(programFragments);
    delete programFragments;
    // start at index 0
    TokenListIndex currentIndex = 0;
    int numberOfTokens = tokenisedProgram->size();
    List<ProcedureNode> procedures;
    // reset statement numbers to 0
    statementsSeen = 0;
    while (currentIndex < numberOfTokens) {
        if (tokenisedProgram->at(currentIndex)->tokenTag == frontend::ProcedureKeywordTag) {
            ParserReturnType<std::unique_ptr<ProcedureNode>> p = parseProcedure(tokenisedProgram, currentIndex);
            procedures.push_back(std::move(p.astNode));
            currentIndex = p.nextUnparsedToken;
        } else {
            // syntax error, SIMPLE program must consist of procedures
        }
        createProgramNode("SIMPLE program", procedures);
    }
    // delete tokens once tree has been created
    delete tokenisedProgram;
}
