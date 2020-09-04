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

    ParserReturnType(T ast, TokenListIndex next): astNode(std::move(ast)), nextUnparsedToken(next) {}
};

ParserReturnType<ArithmeticExpression*> parseArithmeticExpression(frontend::TokenList* programTokens) {}

ArithmeticExpression* parseArithmeticExpression(StringList* lexedArithmeticExpression)
{
    return parseArithmeticExpression(frontend::tokeniseSimple(lexedArithmeticExpression)).astNode;
}

ParserReturnType<std::unique_ptr<CallStatementNode>> parseCallStmt(frontend::TokenList* programTokens,
                                                                   TokenListIndex startIndex)
{
    return ParserReturnType<std::unique_ptr<CallStatementNode>>(std::unique_ptr<CallStatementNode>{}, 0);
}

ParserReturnType<std::unique_ptr<PrintStatementNode>> parsePrintStmt(frontend::TokenList* programTokens,
                                                                     TokenListIndex startIndex)
{
    return ParserReturnType<std::unique_ptr<PrintStatementNode>>(std::unique_ptr<PrintStatementNode>{}, 0);
}

ParserReturnType<std::unique_ptr<ReadStatementNode>> parseReadStmt(frontend::TokenList* programTokens,
                                                                   TokenListIndex startIndex)
{
    return ParserReturnType<std::unique_ptr<ReadStatementNode>>(std::unique_ptr<ReadStatementNode>{}, 0);
}

ParserReturnType<std::unique_ptr<IfStatementNode>> parseIfStmt(frontend::TokenList* programTokens,
                                                               TokenListIndex startIndex)
{
    return ParserReturnType<std::unique_ptr<IfStatementNode>>(std::unique_ptr<IfStatementNode>{}, 0);
}

ParserReturnType<std::unique_ptr<WhileStatementNode>> parseWhileStmt(frontend::TokenList* programTokens,
                                                                     TokenListIndex startIndex)
{
    return ParserReturnType<std::unique_ptr<WhileStatementNode>>(std::unique_ptr<WhileStatementNode>{}, 0);
}

ParserReturnType<std::unique_ptr<AssignmentStatementNode>> parseAssignStmt(frontend::TokenList* programTokens,
                                                                           TokenListIndex startIndex)
{
    return ParserReturnType<std::unique_ptr<AssignmentStatementNode>>(std::unique_ptr<AssignmentStatementNode>{}, 0);
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
        case frontend::CallKeywordTag:
            ParserReturnType<std::unique_ptr<CallStatementNode>> resultCall = parseCallStmt();
            statement = resultCall.astNode.release();
            break;
        case frontend::PrintKeywordTag:
            ParserReturnType<std::unique_ptr<PrintStatementNode>> resultPrint = parsePrintStmt();
            statement = resultPrint.astNode.release();
            break;
        case frontend::ReadKeywordTag:
            ParserReturnType<std::unique_ptr<ReadStatementNode>> resultRead = parseReadStmt();
            statement = resultRead.astNode.release();
            break;
        case frontend::IfKeywordTag; ParserReturnType<std::unique_ptr<IfStatementNode>> resultIf = parseIfStmt();
            statement = resultIf.astNode.release(); break; case frontend::WhileKeywordTag:
            ParserReturnType<std::unique_ptr<WhileStatementNode>> resultWhile = parseWhileStmt();
            statement = resultWhile.astNode.release();
            break;
        case frontend::IdentifierTag:
            // [[fallthrough]];
        default:
            // assignment statement
            ParserReturnType<std::unique_ptr<AssignmentStatementNode>> resultAssign = parseAssignStmt();
            statement = resultAssign.astNode.release();
        }
    } else {
        // empty statement
        isSyntaxError = true;
    }

    if (isSyntaxError) {
        return ParserReturnType<std::unique_ptr<StatementNode>>(std::unique_ptr<StatementNode>{}, -1);
    } else {
        return ParserReturnType<std::unique_ptr<StatementNode>>(std::unique_ptr<StatementNode>(std::move(statement)),
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
        TokenListIndex currentStatementStartIndex = nextUnparsed;
        while (currentStatementStartIndex < numberOfTokens && currentStatementStartIndex > 0
               && programTokens->at(currentStatementStartIndex)->tokenTag != frontend::BracesClosedTag) {

            ParserReturnType<std::unique_ptr<StatementNode>> result
                = parseStatement(programTokens, currentStatementStartIndex);
            statements.push_back(std::move(result.astNode));
            currentStatementStartIndex = result.nextUnparsedToken;
        }

        if (programTokens->at(currentStatementStartIndex)->tokenTag != frontend::BracesClosedTag) {
            // syntax error, unmatched brackets in statementlist
            isSyntaxError = true;
        } else if (currentStatementStartIndex < 0) {
            // syntax error in statement
            isSyntaxError = true;
        } else {
            // no errors
        }
    } else {
        // syntax error, empty statement list
        isSyntaxError = true;
    }

    if (isSyntaxError) {
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
    auto* procedures = new List<ProcedureNode>();
    while (currentIndex < numberOfTokens) {
        if (tokenisedProgram->at(currentIndex)->tokenTag == frontend::ProcedureKeywordTag) {
            ParserReturnType<std::unique_ptr<ProcedureNode>> p = parseProcedure(tokenisedProgram, currentIndex);
            procedures->push_back(std::move(p.astNode));
            currentIndex = p.nextUnparsedToken;
        } else {
            // syntax error, SIMPLE program must consist of procedures
        }
    }
    delete tokenisedProgram;
}
