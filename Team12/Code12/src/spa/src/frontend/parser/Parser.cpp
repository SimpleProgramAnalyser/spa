/**
 * Implementation of SPA frontend parser.
 */

#include "Parser.h"

#include <cassert>

#include "Token.h"
#include "ast/AstLibrary.h"
#include "lexer/Lexer.h"

typedef Integer BracketsDepth;
typedef Integer TokenListIndex;

/**
 * This class is used to wrap the result of parsing
 * (an Abstract Syntax Tree node) with a value which
 * refers to the index of the next unparsed Token in
 * the list of Tokens. Also, if this index is set to
 * a negative number, it signifies a syntax error.
 *
 * @tparam T Type of the AST node.
 */
template <typename T>
class ParserReturnType {
public:
    T astNode;
    TokenListIndex nextUnparsedToken;

    explicit ParserReturnType(T nullValue): astNode(std::move(nullValue)), nextUnparsedToken(0) {}
    ParserReturnType(T ast, TokenListIndex next): astNode(std::move(ast)), nextUnparsedToken(next) {}
};

/**
 * Given the index of the first bracket "(" in a list of
 * tokens, find the index of the matching closing bracket ")".
 * This may not necessarily be the closest ")".
 *
 * @param programTokens The tokens representing the program.
 * @param indexOfFirstBracket Index of the opening bracket.
 *
 * @return Index of the closed bracket, or -1 if not found.
 *         Return -2 if the given index is not an opening bracket,
 *         or if the index is invalid.
 */
TokenListIndex getBracketEnd(frontend::TokenList* programTokens, TokenListIndex indexOfFirstBracket)
{
    TokenListIndex numberOfTokens = programTokens->size();
    // boundary checks
    if (indexOfFirstBracket < 0 || indexOfFirstBracket >= numberOfTokens
        || programTokens->at(indexOfFirstBracket)->tokenTag != frontend::BracketOpenTag) {
        // indexOfFirstBracket out of bounds, or not an opening bracket
        return -2;
    }

    BracketsDepth bracketsSeen = 1;
    TokenListIndex endBracket = indexOfFirstBracket + 1;
    while (endBracket < numberOfTokens && bracketsSeen > 0) {
        frontend::Tag currentToken = programTokens->at(endBracket)->tokenTag;
        if (currentToken == frontend::BracketOpenTag) {
            bracketsSeen++;
        } else if (currentToken == frontend::BracketClosedTag) {
            bracketsSeen--;
        }
        endBracket++;
    }

    if (endBracket >= numberOfTokens) {
        // did not find the closing bracket in the entire program
        return -1;
    } else {
        return endBracket - 1;
    }
}

/**
 * Returns a ParserReturnType signifying a syntax error.
 * (nextUnparsedToken is set to -1)
 *
 * @tparam T The type of node in ParserReturnType.
 * @return ParserReturnType of type T, with astNode set
 *         to nullptr and nextUnparsedToken set to -1.
 */
template <typename T>
ParserReturnType<std::unique_ptr<T>> getSyntaxError()
{
    return ParserReturnType<std::unique_ptr<T>>(std::unique_ptr<T>{}, -1);
}

ParserReturnType<std::unique_ptr<ReferenceExpression>> parseReferenceExpression(frontend::TokenList* programTokens,
                                                                                TokenListIndex index)
{
    // TODO, implement
    return ParserReturnType<std::unique_ptr<ReferenceExpression>>(std::unique_ptr<ReferenceExpression>{}, -1);
}

ParserReturnType<std::unique_ptr<ArithmeticExpression>>
parseArithmeticExpression(frontend::TokenList* programTokens, TokenListIndex startIndex, TokenListIndex endIndex)
{
    // TODO, implement
    return ParserReturnType<std::unique_ptr<ArithmeticExpression>>(std::unique_ptr<ArithmeticExpression>{}, -1);
}

ParserReturnType<std::unique_ptr<Expression>> parseExpression(frontend::TokenList* programTokens,
                                                              TokenListIndex startIndex, TokenListIndex endIndex)
{
    // possible combinations, where e is an expression:
    // ---------------------------------------------------
    //                           reference expression
    //                          ^-------------------^
    //  arithmetic expression   |                   |   unwrap and call recursively
    // ^---------------------^  |                   |  ^-^
    // e+e, e-e, e*e, e/e, e%e, var_name, const_value, (e)

    TokenListIndex numberOfTokens = programTokens->size();
    if (startIndex > endIndex || endIndex >= numberOfTokens) {
        // bounds error in indexes
        return ParserReturnType<std::unique_ptr<Expression>>(std::unique_ptr<Expression>{}, -1);
    } else if (startIndex == endIndex
               && (frontend::isIdentifierTag(programTokens->at(startIndex)->tokenTag)
                   || programTokens->at(startIndex)->tokenTag == frontend::ConstantTag)) {
        ParserReturnType<std::unique_ptr<ReferenceExpression>> refExp
            = parseReferenceExpression(programTokens, startIndex);
        // upcast to Expression
        return ParserReturnType<std::unique_ptr<Expression>>(std::move(refExp.astNode), refExp.nextUnparsedToken);
    } else if (startIndex == endIndex) {
        // syntax error in expression
        return getSyntaxError<Expression>();
    }

    // unwrap brackets and call expression again, if possible
    if (programTokens->at(startIndex)->tokenTag == frontend::BracketOpenTag
        && getBracketEnd(programTokens, startIndex) == endIndex) {

        return parseExpression(programTokens, startIndex + 1, endIndex - 1);
    }

    ParserReturnType<std::unique_ptr<ArithmeticExpression>> arithExp
        = parseArithmeticExpression(programTokens, startIndex, endIndex);
    // upcast to Expression
    return ParserReturnType<std::unique_ptr<Expression>>(std::move(arithExp.astNode), arithExp.nextUnparsedToken);
}

Expression* parseExpression(StringList* lexedExpression)
{
    return parseExpression(frontend::tokeniseSimple(lexedExpression), 0,
                           lexedExpression->size() /* assuming token list has same length */)
        .astNode.release();
}

ParserReturnType<std::unique_ptr<RelationalExpression>>
parseRelationalExpression(frontend::TokenList* programTokens, TokenListIndex startIndex, TokenListIndex endIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();
    if (startIndex >= endIndex || endIndex >= numberOfTokens) {
        // bounds error in indexes
        return getSyntaxError<RelationalExpression>();
    }
    // find the relational operator
    TokenListIndex relationalOperator = startIndex;
    frontend::Tag currentToken = programTokens->at(relationalOperator)->tokenTag;
    while (relationalOperator <= endIndex && !frontend::isRelationalOperatorTag(currentToken)) {
        relationalOperator++;
        currentToken = programTokens->at(relationalOperator)->tokenTag;
    }
    // check if found
    if (relationalOperator >= numberOfTokens) {
        // error, name or constant without relational operator
        return getSyntaxError<RelationalExpression>();
    }
    // parse first expression
    ParserReturnType<std::unique_ptr<Expression>> firstExpr
        = parseExpression(programTokens, startIndex, relationalOperator - 1);
    if (firstExpr.nextUnparsedToken < 0) {
        // syntax error in first expression
        return getSyntaxError<RelationalExpression>();
    }
    // parse second expression, assume it is until the endIndex
    ParserReturnType<std::unique_ptr<Expression>> secondExpr
        = parseExpression(programTokens, firstExpr.nextUnparsedToken + 1 /* skip over operator */, endIndex);
    if (secondExpr.nextUnparsedToken < 0) {
        // syntax error in second expression
        return getSyntaxError<RelationalExpression>();
    }
    RelationalExpression* (*createExpressionFunction)(Expression*, Expression*);
    // find out which function to use
    switch (programTokens->at(firstExpr.nextUnparsedToken)->tokenTag) {
    case frontend::GtTag:
        createExpressionFunction = &createGtExpr;
        break;
    case frontend::GteTag:
        createExpressionFunction = &createGteExpr;
        break;
    case frontend::LtTag:
        createExpressionFunction = &createLtExpr;
        break;
    case frontend::LteTag:
        createExpressionFunction = &createLteExpr;
        break;
    case frontend::NeqTag:
        createExpressionFunction = &createNeqExpr;
        break;
    case frontend::EqTag:
        createExpressionFunction = &createEqExpr;
        break;
    default:
        throw std::runtime_error("invalid relational operator in parseConditionalExpression");
    }
    // create the conditional expression
    return ParserReturnType<std::unique_ptr<RelationalExpression>>(
        std::unique_ptr<RelationalExpression>(
            createExpressionFunction(firstExpr.astNode.release(), secondExpr.astNode.release())),
        secondExpr.nextUnparsedToken);
}

ParserReturnType<std::unique_ptr<ConditionalExpression>>
parseConditionalExpression(frontend::TokenList* programTokens, TokenListIndex startIndex, TokenListIndex endIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();
    if (startIndex >= endIndex || endIndex >= numberOfTokens) {
        // bounds error in indexes
        return getSyntaxError<ConditionalExpression>();
    }

    Boolean isRelational = false;
    Boolean isNegated = false;
    // cache result of bracket matching if possible
    TokenListIndex possibleEndBracketForExpression = -1;
    frontend::Tag firstToken = programTokens->at(startIndex)->tokenTag;
    if (firstToken == frontend::NotConditionalTag) {
        // conditional expression is of form "!" "(" "cond_expression" ")"
        isNegated = true;
    } else if (frontend::isIdentifierTag(firstToken) || firstToken == frontend::ConstantTag) {
        // conditional expression is of form "expr" "operator" "expr"
        isRelational = true;
    } else if (firstToken == frontend::BracketOpenTag) {
        // conditional expression could be of form "expr" "relational_operator" "expr"
        // or could be of form "cond_expr" "&&_or_||" "cond_expr"
        // (also, form "(" "cond_expr" ")")
        TokenListIndex endBracket = getBracketEnd(programTokens, startIndex);
        if (endBracket < 0 || endBracket > endIndex) {
            // syntax error in brackets, or no operator after brackets
            return getSyntaxError<ConditionalExpression>();
        } else if (endBracket == endIndex) {
            // case where there are double brackets
            // e.g. while ((...))
            ParserReturnType<std::unique_ptr<ConditionalExpression>> innerCondition
                = parseConditionalExpression(programTokens, startIndex + 1, endBracket - 1);
            if (innerCondition.nextUnparsedToken < 0 || innerCondition.nextUnparsedToken != endBracket) {
                // syntax error in inner conditional
                return getSyntaxError<ConditionalExpression>();
            } else {
                innerCondition.nextUnparsedToken = endBracket + 1; // ignore last bracket
                return innerCondition;
            }
        }
        frontend::Tag operatorToken = programTokens->at(endBracket + 1)->tokenTag;
        switch (operatorToken) {
        case frontend::AndConditionalTag:
        case frontend::OrConditionalTag:
            // is not relational
            possibleEndBracketForExpression = endBracket; // cache location of end bracket
            break;
        case frontend::GtTag:
        case frontend::GteTag:
        case frontend::LtTag:
        case frontend::LteTag:
        case frontend::NeqTag:
        case frontend::EqTag:
            isRelational = true;
            break;
        default:
            // wrong operator after brackets
            return getSyntaxError<ConditionalExpression>();
        }
    } else {
        // unrecognised token, syntax error in conditional
        return getSyntaxError<ConditionalExpression>();
    }

    if (isRelational) {
        // "expr" "rel_operator" "expr"
        ParserReturnType<std::unique_ptr<RelationalExpression>> relationalExpr
            = parseRelationalExpression(programTokens, startIndex, endIndex);
        if (relationalExpr.nextUnparsedToken < 0) {
            // syntax error in relational expression
            return getSyntaxError<ConditionalExpression>();
        } else {
            // upcast to ConditionalExpression
            return ParserReturnType<std::unique_ptr<ConditionalExpression>>(
                std::unique_ptr<ConditionalExpression>(std::move(relationalExpr.astNode)),
                relationalExpr.nextUnparsedToken);
        }
    } else if (isNegated) {
        // "!" "(" "cond_expr" ")"
        TokenListIndex endBracket = getBracketEnd(programTokens, startIndex + 1);
        if (endBracket < 0) {
            // error with not conditional syntax, or expression bracket syntax
            return getSyntaxError<ConditionalExpression>();
        }
        ParserReturnType<std::unique_ptr<ConditionalExpression>> negatedCondition
            = parseConditionalExpression(programTokens, startIndex + 2, endBracket - 1);
        if (negatedCondition.nextUnparsedToken < 0) {
            // syntax error in sub-conditional expression
            return negatedCondition;
        }
        return ParserReturnType<std::unique_ptr<ConditionalExpression>>(
            std::unique_ptr<ConditionalExpression>(createNotExpr(negatedCondition.astNode.release())),
            /* add one to skip closed bracket */
            negatedCondition.nextUnparsedToken + 1);
    } else {
        // "(" "cond_expr" ")" "&&_or_||" "(" "cond_expr" ")"
        assert(possibleEndBracketForExpression != -1); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        // parse first condition
        ParserReturnType<std::unique_ptr<ConditionalExpression>> firstCondition
            = parseConditionalExpression(programTokens, startIndex + 1, possibleEndBracketForExpression - 1);
        if (firstCondition.nextUnparsedToken < 0) {
            // syntax error in first sub-conditional expression
            return firstCondition;
        }
        assert(programTokens // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
                       ->at(firstCondition.nextUnparsedToken + 1)
                       ->tokenTag
                   == frontend::AndConditionalTag
               || programTokens->at(firstCondition.nextUnparsedToken + 1)->tokenTag == frontend::OrConditionalTag);
        // match brackets for second condition
        TokenListIndex secondEndBracket = getBracketEnd(programTokens, firstCondition.nextUnparsedToken + 2);
        if (secondEndBracket < 0) {
            // syntax error with second condition brackets
            return getSyntaxError<ConditionalExpression>();
        }
        // parse second condition
        ParserReturnType<std::unique_ptr<ConditionalExpression>> secondCondition
            = parseConditionalExpression(programTokens, firstCondition.nextUnparsedToken + 3, secondEndBracket - 1);
        if (secondCondition.nextUnparsedToken < 0) {
            // syntax error in second sub-conditional expression
            return secondCondition;
        }
        assert(secondCondition.nextUnparsedToken // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
               == endIndex - 1);
        // finally, create the conditional expression
        if (programTokens->at(firstCondition.nextUnparsedToken)->tokenTag == frontend::AndConditionalTag) {
            return ParserReturnType<std::unique_ptr<ConditionalExpression>>(
                std::unique_ptr<ConditionalExpression>(
                    createAndExpr(firstCondition.astNode.release(), secondCondition.astNode.release())),
                secondCondition.nextUnparsedToken);
        } else {
            // programTokens->at(firstCondition.nextUnparsedToken)->tokenTag == frontend::OrConditionalTag
            return ParserReturnType<std::unique_ptr<ConditionalExpression>>(
                std::unique_ptr<ConditionalExpression>(
                    createOrExpr(firstCondition.astNode.release(), secondCondition.astNode.release())),
                secondCondition.nextUnparsedToken);
        }
    }
}

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
        return getSyntaxError<CallStatementNode>();
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
        return getSyntaxError<PrintStatementNode>();
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
        return getSyntaxError<ReadStatementNode>();
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
        TokenListIndex tokenPointer = getBracketEnd(programTokens, startIndex + 1) + 1;
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
            return getSyntaxError<IfStatementNode>();
        }
        // check if "if" conditional parsed correctly
        if (ifCondition.nextUnparsedToken < 0) {
            // error in parsing conditional expression
            return getSyntaxError<IfStatementNode>();
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
            return getSyntaxError<IfStatementNode>();
        }
        // now, parse the "else" statements
        if (programTokens->at(ifStatements.nextUnparsedToken)->tokenTag == frontend::ElseKeywordTag) {
            elseStatements = parseStatementList(programTokens, ifStatements.nextUnparsedToken + 1 /* skip "else" */);
        } else {
            // no "else" keyword, wrong if/else syntax
            return getSyntaxError<IfStatementNode>();
        }
        // check if "else" statements parsed correctly
        if (elseStatements.nextUnparsedToken < 0) {
            return getSyntaxError<IfStatementNode>();
        }
        return ParserReturnType<std::unique_ptr<IfStatementNode>>(
            std::unique_ptr<IfStatementNode>(createIfNode(ifStmtNum, ifCondition.astNode.release(),
                                                          ifStatements.astNode.release(),
                                                          elseStatements.astNode.release())),
            elseStatements.nextUnparsedToken);
    }
    // if syntax is invalid
    return getSyntaxError<IfStatementNode>();
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
        TokenListIndex tokenPointer = getBracketEnd(programTokens, startIndex + 1) + 1;
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
            return getSyntaxError<WhileStatementNode>();
        }
        // check if while condition parsed correctly
        if (loopControlCondition.nextUnparsedToken < 0) {
            // error in parsing conditional expression
            return getSyntaxError<WhileStatementNode>();
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
            return getSyntaxError<WhileStatementNode>();
        }
        return ParserReturnType<std::unique_ptr<WhileStatementNode>>(
            std::unique_ptr<WhileStatementNode>(
                createWhileNode(whileStmtNum, loopControlCondition.astNode.release(), statements.astNode.release())),
            statements.nextUnparsedToken);
    }
    // while syntax is invalid
    return getSyntaxError<WhileStatementNode>();
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
            return getSyntaxError<AssignmentStatementNode>();
        }
        // check if expression parsed correctly
        if (expression.nextUnparsedToken < 0) {
            // syntax error in expression
            return getSyntaxError<AssignmentStatementNode>();
        } else {
            // next unparsed token should be the semicolon
            assert(expression.nextUnparsedToken // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
                   == tokenPointer);
            statementsSeen++;
            return ParserReturnType<std::unique_ptr<AssignmentStatementNode>>(
                std::unique_ptr<AssignmentStatementNode>(
                    createAssignNode(statementsSeen, assignedVariable, expression.astNode.release())),
                expression.nextUnparsedToken + 1 /* skip semicolon */);
        }
    } else {
        // invalid assign syntax, not of form "variable" "=" ...
        return getSyntaxError<AssignmentStatementNode>();
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
        return getSyntaxError<StatementNode>();
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
        return getSyntaxError<StmtlstNode>();
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
        return getSyntaxError<ProcedureNode>();
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
