/**
 * Implementation of SPA frontend parser.
 */

#include "Parser.h"

#include <cassert>
#include <stdexcept>

#include "Token.h"
#include "ast/AstLibrary.h"
#include "lexer/Lexer.h"
#include "pkb/PKB.h"

typedef Integer BracketsDepth;
typedef Integer ErrorCode;

/**
 * Given the index of a closing bracket ")" in a list of
 * tokens, scan the list of program tokens in reverse order,
 * and find the index of the matching open bracket "(".
 * This may not necessarily be the closest "(".
 *
 * @param programTokens The tokens representing the program.
 * @param indexOfClosedBracket Index of the closed bracket.
 *
 * @return Index of the open bracket, or -1 if not found.
 *         Return -2 if the given index is not a closed bracket,
 *         or if the index is out-of-bounds.
 */
TokenListIndex getBracketStart(frontend::TokenList* programTokens, TokenListIndex indexOfClosedBracket)
{
    TokenListIndex numberOfTokens = programTokens->size();
    // boundary checks
    if (indexOfClosedBracket < 0 || indexOfClosedBracket >= numberOfTokens
        || programTokens->at(indexOfClosedBracket).tokenTag != frontend::BracketClosedTag) {
        // indexOfFirstBracket out of bounds, or not an opening bracket
        return -2;
    }

    BracketsDepth bracketsSeen = 1;
    TokenListIndex startBracket = indexOfClosedBracket - 1;
    while (startBracket >= 0 && bracketsSeen > 0) {
        frontend::Tag currentToken = programTokens->at(startBracket).tokenTag;
        if (currentToken == frontend::BracketClosedTag) {
            bracketsSeen++;
        } else if (currentToken == frontend::BracketOpenTag) {
            bracketsSeen--;
        }
        // go towards the start of the program
        startBracket--;
    }

    if (bracketsSeen > 0) {
        // did not find the closing bracket in the entire program
        return -1;
    } else {
        return startBracket + 1;
    }
}

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
        || programTokens->at(indexOfFirstBracket).tokenTag != frontend::BracketOpenTag) {
        // indexOfFirstBracket out of bounds, or not an opening bracket
        return -2;
    }

    BracketsDepth bracketsSeen = 1;
    TokenListIndex endBracket = indexOfFirstBracket + 1;
    while (endBracket < numberOfTokens && bracketsSeen > 0) {
        frontend::Tag currentToken = programTokens->at(endBracket).tokenTag;
        if (currentToken == frontend::BracketOpenTag) {
            bracketsSeen++;
        } else if (currentToken == frontend::BracketClosedTag) {
            bracketsSeen--;
        }
        endBracket++;
    }

    if (bracketsSeen > 0) {
        // did not find the closing bracket in the entire program
        return -1;
    } else {
        return endBracket - 1;
    }
}

/**
 * Recursively gets the last statement number inside the stmtList.
 * The last statement number is not simply the last statement in
 * the statement list node, as the last statement could be a
 * container statement holding the true last statement.
 *
 * @param stmtlstNode The statement list node to traverse.
 * @return The last statement number that can be found from
 *         within the statement list node.
 */
StatementNumber getLastStatementNumber(const StmtlstNode* stmtlstNode)
{
    StatementType lastStatementType = (stmtlstNode->statementList.end() - 1)->get()->getStatementType();
    switch (lastStatementType) {
    case IfStatement: {
        // NOLINTNEXTLINE
        IfStatementNode* ifStatementNode = static_cast<IfStatementNode*>((stmtlstNode->statementList.end() - 1)->get());
        // else statements will contain later statement numbers than if statements
        return getLastStatementNumber(ifStatementNode->elseStatementList);
    }
    case WhileStatement: {
        WhileStatementNode* whileStatementNode
            // NOLINTNEXTLINE
            = static_cast<WhileStatementNode*>((stmtlstNode->statementList.end() - 1)->get());
        return getLastStatementNumber(whileStatementNode->statementList);
    }
    default: {
        return (stmtlstNode->statementList.end() - 1)->get()->getStatementNumber();
    }
    }
}

/**
 * Returns a ParserReturnType signifying a syntax error.
 * (nextUnparsedToken is set to negative value)
 *
 * @tparam T The type of node in ParserReturnType.
 * @param errorCode The code for the error.
 * @return ParserReturnType of type T, with astNode set
 *         to nullptr and nextUnparsedToken set to -1.
 */
template <typename T>
ParserReturnType<std::unique_ptr<T>> getSyntaxError(ErrorCode errorCode)
{
    return ParserReturnType<std::unique_ptr<T>>(std::unique_ptr<T>{}, -errorCode);
}

ParserReturnType<std::unique_ptr<ReferenceExpression>>
parseReferenceExpression(frontend::TokenList* programTokens, TokenListIndex index, Boolean isUpdatingPkb)
{
    TokenListIndex numberOfTokens = programTokens->size();
    if (index < 0 || index >= numberOfTokens) {
        // bounds error in index
        return getSyntaxError<ReferenceExpression>(18);
    }
    frontend::Tag tokenTag = programTokens->at(index).tokenTag;
    if (frontend::isIdentifierTag(tokenTag)) {
        String variable = programTokens->at(index).rawString;
        if (isUpdatingPkb) {
            insertIntoVariableTable(variable);
        }
        return ParserReturnType<std::unique_ptr<ReferenceExpression>>(
            std::unique_ptr<ReferenceExpression>(createRefExpr(variable)), index + 1);
    } else if (tokenTag == frontend::ConstantTag) {
        // stoi should succeed unless there is a bug in the tokeniser
        Integer constant = std::stoi(programTokens->at(index).rawString);
        if (isUpdatingPkb) {
            insertIntoConstantTable(constant);
        }
        return ParserReturnType<std::unique_ptr<ReferenceExpression>>(
            std::unique_ptr<ReferenceExpression>(createRefExpr(constant)), index + 1);
    } else {
        // syntax error, not a reference expression
        return getSyntaxError<ReferenceExpression>(19);
    }
}

// forward declaration for parseArithmeticExpression
ParserReturnType<std::unique_ptr<Expression>> parseExpression(frontend::TokenList* programTokens,
                                                              TokenListIndex startIndex, TokenListIndex endIndex,
                                                              Boolean isUpdatingPkb);

ParserReturnType<std::unique_ptr<ArithmeticExpression>> parseArithmeticExpression(frontend::TokenList* programTokens,
                                                                                  TokenListIndex startIndex,
                                                                                  TokenListIndex endIndex,
                                                                                  Boolean isUpdatingPkb)
{
    // ==========================================================================================
    // define expression => ex; parseExpression function => pE;
    // Possible patterns for recursion:
    // (if brackets are seen, find end bracket and ignore operators inside brackets)
    //
    // Single operator, consecutive:
    // ex + ex + ex + ex  |  ex * ex * ex * ex
    // ex + ex - ex + ex  |  ex * ex / ex * ex
    // ex - ex - ex - ex  |  ex / ex / ex / ex
    //                   -> left precedence
    //                   -> pE(ex o ex o ex) o pE(ex)
    //
    // Multiple operator, non-consecutive
    // ex * ex - ex / ex
    //                   -> use operator precedence
    //                   -> pE(ex * ex) - pE(ex / ex)
    //                   -> create a minus expression
    //
    // Multiple operator, with consecutive patterns
    // ex + ex * ex - ex / ex
    //                   -> first, apply operator precedence
    //                   -> pE(ex) + pE(ex * ex) - pE(ex / ex)
    //                   -> then, apply left precedence
    //                   -> pE(ex + ex * ex) - pE(ex / ex)
    //                   -> create a minus expression
    // ==========================================================================================
    // Example expression:  ex + ex * ex / ex - ex + ex / ex * ex - ex + ex
    //                   -> first, apply operator precedence
    //                   -> pE(ex) + pE(ex * ex / ex) - ex + pE(ex / ex * ex) - ex + ex
    //                   -> then, apply left precedence
    //                   -> pE(ex + ex * ex / ex - ex + ex / ex * ex - ex) + pE(ex)
    //                   -> create a plus expression
    //
    // Description of parsing algorithm for arithmetic expression:
    // 1. Find rightmost + or - operator
    // 2. If found
    //     2a. Left = pE everything on the left of the operator
    //     2b. Right = pE everything on the right of the operator
    //     2c. Return + or - node, with left = Left and right = Right
    // 3. If not found
    //     3a. Find rightmost * or / operator
    //     3b. If found
    //         3b1. Left = pE everything on the left of the operator
    //         3b2. Right = pE everything on the right of the operator
    //         3b3. Return * or / node, with left = Left and right = Right
    //     3c. If not found
    //         3c1. Return syntax error

    TokenListIndex numberOfTokens = programTokens->size();
    if (startIndex > endIndex || endIndex >= numberOfTokens) {
        // bounds error in indexes
        return getSyntaxError<ArithmeticExpression>(20);
    }

    // set up variables
    ArithmeticExpression* (*createExpressionFunction)(Expression*, Expression*);
    ParserReturnType<std::unique_ptr<Expression>> leftExpr(nullptr);
    ParserReturnType<std::unique_ptr<Expression>> rightExpr(nullptr);

    // find last + or - from right
    Boolean hasPlusOrMinus = false;
    TokenListIndex operatorIndex = endIndex;
    while (operatorIndex >= startIndex) {
        frontend::Tag currentTag = programTokens->at(operatorIndex).tokenTag;
        if (currentTag == frontend::PlusTag || currentTag == frontend::MinusTag) {
            hasPlusOrMinus = true;
            break;
        } else if (currentTag == frontend::BracketClosedTag) {
            // jump over brackets
            operatorIndex = getBracketStart(programTokens, operatorIndex);
        }
        operatorIndex--;
    }
    // currentIndex should now point to + or -
    if (hasPlusOrMinus) {
        frontend::Tag currentOperator = programTokens->at(operatorIndex).tokenTag;
        if (currentOperator == frontend::PlusTag) {
            createExpressionFunction = &createPlusExpr;
        } else if (currentOperator == frontend::MinusTag) {
            createExpressionFunction = &createMinusExpr;
        } else {
            throw std::runtime_error("error in parseArithmeticExpression, programTokens list mutated");
        }
    } else {
        // find last * or / or % from right
        operatorIndex = endIndex;
        while (operatorIndex >= startIndex) {
            frontend::Tag currentTag = programTokens->at(operatorIndex).tokenTag;
            if (currentTag == frontend::TimesTag || currentTag == frontend::DivideTag
                || currentTag == frontend::ModuloTag) {

                break;
            } else if (currentTag == frontend::BracketClosedTag) {
                // jump over brackets
                operatorIndex = getBracketStart(programTokens, operatorIndex);
            }
            operatorIndex--;
        }

        if (operatorIndex < startIndex) {
            // syntax error, no operator in arithmetic expression
            return getSyntaxError<ArithmeticExpression>(21);
        }
        frontend::Tag currentOperator = programTokens->at(operatorIndex).tokenTag;
        if (currentOperator == frontend::TimesTag) {
            createExpressionFunction = &createTimesExpr;
        } else if (currentOperator == frontend::DivideTag) {
            createExpressionFunction = &createDivExpr;
        } else if (currentOperator == frontend::ModuloTag) {
            createExpressionFunction = &createModExpr;
        } else {
            throw std::runtime_error("error in parseArithmeticExpression, programTokens list mutated");
        }
    }

    // create left expression
    leftExpr = parseExpression(programTokens, startIndex, operatorIndex - 1, isUpdatingPkb);
    // check validity of left expression
    if (leftExpr.hasError()) {
        // syntax error in left expression
        return getSyntaxError<ArithmeticExpression>(leftExpr.getErrorCode());
    }
    // create right expression
    rightExpr = parseExpression(programTokens, operatorIndex + 1, endIndex, isUpdatingPkb);
    // check validity of right expression
    if (rightExpr.hasError()) {
        // syntax error in right expression
        return getSyntaxError<ArithmeticExpression>(rightExpr.getErrorCode());
    }
    // return final expression
    return ParserReturnType<std::unique_ptr<ArithmeticExpression>>(
        std::unique_ptr<ArithmeticExpression>(
            createExpressionFunction(leftExpr.astNode.release(), rightExpr.astNode.release())),
        endIndex + 1);
}

ParserReturnType<std::unique_ptr<Expression>> parseExpression(frontend::TokenList* programTokens,
                                                              TokenListIndex startIndex, TokenListIndex endIndex,
                                                              Boolean isUpdatingPkb)
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
        return getSyntaxError<Expression>(22);
    }

    if (startIndex == endIndex
        && (frontend::isIdentifierTag(programTokens->at(startIndex).tokenTag)
            || programTokens->at(startIndex).tokenTag == frontend::ConstantTag)) {
        ParserReturnType<std::unique_ptr<ReferenceExpression>> refExp
            = parseReferenceExpression(programTokens, startIndex, isUpdatingPkb);
        // upcast to Expression
        return ParserReturnType<std::unique_ptr<Expression>>(std::move(refExp.astNode), refExp.nextUnparsedToken);
    } else if (startIndex == endIndex) {
        // syntax error in expression, one token long but not constant or variable
        return getSyntaxError<Expression>(23);
    }

    // unwrap brackets and call expression again, if possible
    if (programTokens->at(startIndex).tokenTag == frontend::BracketOpenTag
        && getBracketEnd(programTokens, startIndex) == endIndex) {

        ParserReturnType<std::unique_ptr<Expression>> expr
            = parseExpression(programTokens, startIndex + 1, endIndex - 1, isUpdatingPkb);
        if (expr.hasError()) {
            return getSyntaxError<Expression>(expr.getErrorCode());
        }
        // need to increment nextUnparsedToken by 1 because of the last bracket
        return ParserReturnType<std::unique_ptr<Expression>>(std::move(expr.astNode), expr.nextUnparsedToken + 1);
    }

    ParserReturnType<std::unique_ptr<ArithmeticExpression>> arithExp
        = parseArithmeticExpression(programTokens, startIndex, endIndex, isUpdatingPkb);
    // upcast to Expression
    return ParserReturnType<std::unique_ptr<Expression>>(std::move(arithExp.astNode), arithExp.nextUnparsedToken);
}

Expression* parseExpression(const StringVector& lexedExpression)
{
    frontend::TokenList tokenised = frontend::tokeniseSimple(lexedExpression);
    Expression* parsed = parseExpression(&tokenised, 0, tokenised.size() - 1, false).astNode.release();
    return parsed;
}

ParserReturnType<std::unique_ptr<RelationalExpression>>
parseRelationalExpression(frontend::TokenList* programTokens, TokenListIndex startIndex, TokenListIndex endIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();
    if (startIndex >= endIndex || endIndex >= numberOfTokens) {
        // bounds error in indexes
        return getSyntaxError<RelationalExpression>(24);
    }
    // find the relational operator
    TokenListIndex relationalOperator = startIndex;
    frontend::Tag currentToken = programTokens->at(relationalOperator).tokenTag;
    while (relationalOperator <= endIndex && !frontend::isRelationalOperatorTag(currentToken)) {
        relationalOperator++;
        currentToken = programTokens->at(relationalOperator).tokenTag;
    }
    // check if found
    if (relationalOperator < startIndex || relationalOperator >= endIndex) {
        // error, name or constant without relational operator
        return getSyntaxError<RelationalExpression>(25);
    }
    // parse first expression
    ParserReturnType<std::unique_ptr<Expression>> firstExpr
        = parseExpression(programTokens, startIndex, relationalOperator - 1, true);
    if (firstExpr.hasError()) {
        // syntax error in first expression
        return getSyntaxError<RelationalExpression>(firstExpr.getErrorCode());
    }
    // parse second expression, assume it is until the endIndex
    ParserReturnType<std::unique_ptr<Expression>> secondExpr
        = parseExpression(programTokens, firstExpr.nextUnparsedToken + 1 /* skip over operator */, endIndex, true);
    if (secondExpr.hasError()) {
        // syntax error in second expression
        return getSyntaxError<RelationalExpression>(secondExpr.getErrorCode());
    }
    RelationalExpression* (*createExpressionFunction)(Expression*, Expression*);
    // find out which function to use
    switch (programTokens->at(firstExpr.nextUnparsedToken).tokenTag) {
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
        return getSyntaxError<ConditionalExpression>(26);
    }

    Boolean isRelational = false;
    Boolean isNegated = false;
    // cache result of bracket matching if possible
    TokenListIndex possibleEndBracketForExpression = -1;
    frontend::Tag firstToken = programTokens->at(startIndex).tokenTag;
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
            return getSyntaxError<ConditionalExpression>(27);
        } else if (endBracket == endIndex) {
            // case where there are double brackets
            // e.g. while ((...))
            ParserReturnType<std::unique_ptr<ConditionalExpression>> innerCondition
                = parseConditionalExpression(programTokens, startIndex + 1, endBracket - 1);
            if (innerCondition.hasError()) {
                // syntax error in inner conditional
                return getSyntaxError<ConditionalExpression>(innerCondition.getErrorCode());
            } else if (innerCondition.nextUnparsedToken != endBracket) {
                return getSyntaxError<ConditionalExpression>(28);
            } else {
                innerCondition.nextUnparsedToken = endBracket + 1; // ignore last bracket
                return innerCondition;
            }
        }
        frontend::Tag operatorToken = programTokens->at(endBracket + 1).tokenTag;
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
        // try to parse relational if arithmetic
        case frontend::PlusTag:
        case frontend::MinusTag:
        case frontend::DivideTag:
        case frontend::TimesTag:
        case frontend::ModuloTag:
            isRelational = true;
            break;
        default:
            // wrong operator after brackets
            return getSyntaxError<ConditionalExpression>(29);
        }
    } else {
        // unrecognised token, syntax error in conditional
        return getSyntaxError<ConditionalExpression>(30);
    }

    if (isRelational) {
        // "expr" "rel_operator" "expr"
        ParserReturnType<std::unique_ptr<RelationalExpression>> relationalExpr
            = parseRelationalExpression(programTokens, startIndex, endIndex);
        if (relationalExpr.hasError()) {
            // syntax error in relational expression
            return getSyntaxError<ConditionalExpression>(relationalExpr.getErrorCode());
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
            return getSyntaxError<ConditionalExpression>(31);
        }
        ParserReturnType<std::unique_ptr<ConditionalExpression>> negatedCondition
            = parseConditionalExpression(programTokens, startIndex + 2, endBracket - 1);
        if (negatedCondition.hasError()) {
            // syntax error in sub-conditional expression
            return negatedCondition;
        }
        assert(negatedCondition.nextUnparsedToken // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
               == endBracket);
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
        if (firstCondition.hasError()) {
            // syntax error in first sub-conditional expression
            return firstCondition;
        }
        assert(programTokens // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
                       ->at(firstCondition.nextUnparsedToken + 1)
                       .tokenTag
                   == frontend::AndConditionalTag
               || programTokens->at(firstCondition.nextUnparsedToken + 1).tokenTag == frontend::OrConditionalTag);
        // match brackets for second condition
        TokenListIndex secondEndBracket = getBracketEnd(programTokens, firstCondition.nextUnparsedToken + 2);
        if (secondEndBracket < 0) {
            // syntax error with second condition brackets
            return getSyntaxError<ConditionalExpression>(32);
        }
        // parse second condition
        ParserReturnType<std::unique_ptr<ConditionalExpression>> secondCondition
            = parseConditionalExpression(programTokens, firstCondition.nextUnparsedToken + 3, secondEndBracket - 1);
        if (secondCondition.nextUnparsedToken < 0) {
            // syntax error in second sub-conditional expression
            return secondCondition;
        }
        if (secondCondition.nextUnparsedToken != endIndex) {
            return getSyntaxError<ConditionalExpression>(33);
        }
        // finally, create the conditional expression
        if (programTokens->at(firstCondition.nextUnparsedToken + 1).tokenTag == frontend::AndConditionalTag) {
            return ParserReturnType<std::unique_ptr<ConditionalExpression>>(
                std::unique_ptr<ConditionalExpression>(
                    createAndExpr(firstCondition.astNode.release(), secondCondition.astNode.release())),
                secondCondition.nextUnparsedToken + 1);
        } else {
            // programTokens->at(firstCondition.nextUnparsedToken).tokenTag == frontend::OrConditionalTag
            return ParserReturnType<std::unique_ptr<ConditionalExpression>>(
                std::unique_ptr<ConditionalExpression>(
                    createOrExpr(firstCondition.astNode.release(), secondCondition.astNode.release())),
                secondCondition.nextUnparsedToken + 1);
        }
    }
}

// global variable to help in numbering statements, this
// variable is to be incremented before a statement is numbered
static int statementsSeen = 0;

ParserReturnType<std::unique_ptr<CallStatementNode>> parseCallStmt(frontend::TokenList* programTokens,
                                                                   TokenListIndex startIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();

    if ((numberOfTokens - startIndex > 2) && programTokens->at(startIndex).tokenTag == frontend::CallKeywordTag
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1).tokenTag)
        && programTokens->at(startIndex + 2).tokenTag == frontend::SemicolonTag) {

        statementsSeen++;
        return ParserReturnType<std::unique_ptr<CallStatementNode>>(
            std::unique_ptr<CallStatementNode>(
                createCallNode(statementsSeen, programTokens->at(startIndex + 1).rawString)),
            startIndex + 3);
    } else {
        // syntax error in call statement
        return getSyntaxError<CallStatementNode>(6);
    }
}

ParserReturnType<std::unique_ptr<PrintStatementNode>> parsePrintStmt(frontend::TokenList* programTokens,
                                                                     TokenListIndex startIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();

    if ((numberOfTokens - startIndex > 2) && programTokens->at(startIndex).tokenTag == frontend::PrintKeywordTag
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1).tokenTag)
        && programTokens->at(startIndex + 2).tokenTag == frontend::SemicolonTag) {

        String rawString = programTokens->at(startIndex + 1).rawString;
        statementsSeen++;
        insertIntoVariableTable(rawString);
        return ParserReturnType<std::unique_ptr<PrintStatementNode>>(
            std::unique_ptr<PrintStatementNode>(createPrintNode(statementsSeen, Variable(rawString))), startIndex + 3);
    } else {
        // syntax error in print statement
        return getSyntaxError<PrintStatementNode>(7);
    }
}

ParserReturnType<std::unique_ptr<ReadStatementNode>> parseReadStmt(frontend::TokenList* programTokens,
                                                                   TokenListIndex startIndex)
{
    // boundary check
    TokenListIndex numberOfTokens = programTokens->size();

    if ((numberOfTokens - startIndex > 2) && programTokens->at(startIndex).tokenTag == frontend::ReadKeywordTag
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1).tokenTag)
        && programTokens->at(startIndex + 2).tokenTag == frontend::SemicolonTag) {

        String rawString = programTokens->at(startIndex + 1).rawString;
        statementsSeen++;
        insertIntoVariableTable(rawString);
        return ParserReturnType<std::unique_ptr<ReadStatementNode>>(
            std::unique_ptr<ReadStatementNode>(createReadNode(statementsSeen, Variable(rawString))), startIndex + 3);
    } else {
        // syntax error in read statement
        return getSyntaxError<ReadStatementNode>(8);
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
    if ((numberOfTokens - startIndex > 1) && (programTokens->at(startIndex).tokenTag == frontend::IfKeywordTag)
        && programTokens->at(startIndex + 1).tokenTag == frontend::BracketOpenTag) {

        // find conditional expression and parse
        TokenListIndex tokenPointer = getBracketEnd(programTokens, startIndex + 1) + 1;
        // now token pointer points to the token directly after
        // the closing bracket of if conditional
        //
        // we expect the keyword "then":
        // "if" "(" ... ")" "then" ...
        if (programTokens->at(tokenPointer - 1).tokenTag == frontend::BracketClosedTag
            && programTokens->at(tokenPointer).tokenTag == frontend::ThenKeywordTag
            && tokenPointer - 2 > startIndex + 2) {

            ifCondition = parseConditionalExpression(programTokens, startIndex + 2, tokenPointer - 2);
        } else {
            // syntax error in if conditional, or missing "then" keyword
            return getSyntaxError<IfStatementNode>(9);
        }
        // check if "if" conditional parsed correctly
        if (ifCondition.hasError()) {
            // error in parsing conditional expression
            return getSyntaxError<IfStatementNode>(ifCondition.getErrorCode());
        }
        statementsSeen++;
        StatementNumber ifStmtNum = statementsSeen;
        // now, parse the "if" statements
        if (tokenPointer < numberOfTokens && programTokens->at(tokenPointer + 1).tokenTag == frontend::BracesOpenTag) {
            ifStatements = parseStatementList(programTokens, tokenPointer + 1 /* skip "then"*/);
        } else {
            // syntax error in if statement braces
            return getSyntaxError<IfStatementNode>(10);
        }
        // check if "if" statements parsed correctly
        if (ifStatements.hasError()) {
            // syntax error in statement list
            return getSyntaxError<IfStatementNode>(ifStatements.getErrorCode());
        } else if (ifStatements.nextUnparsedToken > numberOfTokens) {
            // missing else statement
            return getSyntaxError<IfStatementNode>(11);
        }
        // now, parse the "else" statements
        if (programTokens->at(ifStatements.nextUnparsedToken).tokenTag == frontend::ElseKeywordTag) {
            elseStatements = parseStatementList(programTokens, ifStatements.nextUnparsedToken + 1 /* skip "else" */);
        } else {
            // no "else" keyword, wrong if/else syntax
            return getSyntaxError<IfStatementNode>(12);
        }
        // check if "else" statements parsed correctly
        if (elseStatements.hasError()) {
            return getSyntaxError<IfStatementNode>(elseStatements.getErrorCode());
        }
        return ParserReturnType<std::unique_ptr<IfStatementNode>>(
            std::unique_ptr<IfStatementNode>(createIfNode(ifStmtNum, ifCondition.astNode.release(),
                                                          ifStatements.astNode.release(),
                                                          elseStatements.astNode.release())),
            elseStatements.nextUnparsedToken);
    }
    // if syntax is invalid
    return getSyntaxError<IfStatementNode>(13);
}

ParserReturnType<std::unique_ptr<WhileStatementNode>> parseWhileStmt(frontend::TokenList* programTokens,
                                                                     TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    ParserReturnType<std::unique_ptr<ConditionalExpression>> loopControlCondition(nullptr);
    ParserReturnType<std::unique_ptr<StmtlstNode>> statements(nullptr);

    // check if while syntax is valid
    if ((numberOfTokens - startIndex > 1) && (programTokens->at(startIndex).tokenTag == frontend::WhileKeywordTag)
        && programTokens->at(startIndex + 1).tokenTag == frontend::BracketOpenTag) {

        // find conditional expression and parse
        TokenListIndex tokenPointer = getBracketEnd(programTokens, startIndex + 1);

        if (tokenPointer >= 0 && tokenPointer < numberOfTokens
            && programTokens->at(tokenPointer).tokenTag == frontend::BracketClosedTag
            && tokenPointer - 1 > startIndex + 2) {

            loopControlCondition = parseConditionalExpression(programTokens, startIndex + 2, tokenPointer - 1);
        } else {
            // syntax error in while condition
            return getSyntaxError<WhileStatementNode>(14);
        }
        // check if while condition parsed correctly
        if (loopControlCondition.hasError()) {
            // error in parsing conditional expression
            return getSyntaxError<WhileStatementNode>(loopControlCondition.getErrorCode());
        }
        statementsSeen++;
        StatementNumber whileStmtNum = statementsSeen;
        // now, parse the statements
        tokenPointer++;
        // now token pointer points to the token directly after
        // the closing bracket of while conditional
        //
        // we expect a open braces for statement list:
        // "while" "(" ... ")" "{" ...
        if (tokenPointer < numberOfTokens && programTokens->at(tokenPointer).tokenTag == frontend::BracesOpenTag) {
            statements = parseStatementList(programTokens, tokenPointer);
        }
        // check if statements parsed correctly
        if (statements.hasError()) {
            // syntax error in statement list
            return getSyntaxError<WhileStatementNode>(statements.getErrorCode());
        }
        return ParserReturnType<std::unique_ptr<WhileStatementNode>>(
            std::unique_ptr<WhileStatementNode>(
                createWhileNode(whileStmtNum, loopControlCondition.astNode.release(), statements.astNode.release())),
            statements.nextUnparsedToken);
    }
    // while syntax is invalid
    return getSyntaxError<WhileStatementNode>(15);
}

ParserReturnType<std::unique_ptr<AssignmentStatementNode>> parseAssignStmt(frontend::TokenList* programTokens,
                                                                           TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    ParserReturnType<std::unique_ptr<Expression>> expression(nullptr);
    // verify assign syntax
    if ((numberOfTokens - startIndex) > 3 && frontend::isIdentifierTag(programTokens->at(startIndex).tokenTag)
        && programTokens->at(startIndex + 1).tokenTag == frontend::AssignmentTag) {

        String rawString = programTokens->at(startIndex).rawString;
        insertIntoVariableTable(rawString);
        Variable assignedVariable = Variable(rawString);
        // find end of assign expression (semicolon)
        TokenListIndex tokenPointer = startIndex + 2;
        frontend::Tag currentToken = programTokens->at(tokenPointer).tokenTag;
        while (tokenPointer < numberOfTokens - 1 && currentToken != frontend::SemicolonTag) {
            tokenPointer++;
            currentToken = programTokens->at(tokenPointer).tokenTag;
        }
        // check if semicolon was found
        if (tokenPointer < numberOfTokens && programTokens->at(tokenPointer).tokenTag == frontend::SemicolonTag
            && tokenPointer - 1 >= startIndex + 2) {

            expression = parseExpression(programTokens, startIndex + 2, tokenPointer - 1, true);
        } else {
            // missing semicolon for assign statement
            return getSyntaxError<AssignmentStatementNode>(16);
        }
        // check if expression parsed correctly
        if (expression.hasError()) {
            // syntax error in expression
            return getSyntaxError<AssignmentStatementNode>(expression.getErrorCode());
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
        return getSyntaxError<AssignmentStatementNode>(17);
    }
}

ParserReturnType<std::unique_ptr<StatementNode>> parseStatement(frontend::TokenList* programTokens,
                                                                TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    StatementNode* statement;
    TokenListIndex nextUnparsed = startIndex;
    Boolean isCall = false; // flag for CallStatement

    if (numberOfTokens - startIndex > 1) {
        // determine statement type
        frontend::Tag statementType = programTokens->at(startIndex).tokenTag;
        if (programTokens->at(startIndex + 1).tokenTag == frontend::AssignmentTag) {
            // assignment statement
            ParserReturnType<std::unique_ptr<AssignmentStatementNode>> resultAssign
                = parseAssignStmt(programTokens, startIndex);
            statement = resultAssign.astNode.release();
            nextUnparsed = resultAssign.nextUnparsedToken;
        } else {
            switch (statementType) {
            case frontend::CallKeywordTag: {
                ParserReturnType<std::unique_ptr<CallStatementNode>> resultCall
                    = parseCallStmt(programTokens, startIndex);
                statement = resultCall.astNode.release();
                nextUnparsed = resultCall.nextUnparsedToken;
                isCall = true;
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
                ParserReturnType<std::unique_ptr<ReadStatementNode>> resultRead
                    = parseReadStmt(programTokens, startIndex);
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
            default: {
                return getSyntaxError<StatementNode>(5);
            }
            }
        }
    } else {
        // empty statement
        return getSyntaxError<StatementNode>(4);
    }

    if (nextUnparsed < 0) {
        return getSyntaxError<StatementNode>(-nextUnparsed);
    }
    isCall ? insertIntoStatementTable(statement->getStatementNumber(),
                                      // NOLINTNEXTLINE
                                      static_cast<CallStatementNode*>(statement)->procedureName)
           : insertIntoStatementTable(statement->getStatementNumber(), statement->getStatementType());
    return ParserReturnType<std::unique_ptr<StatementNode>>(std::unique_ptr<StatementNode>(statement), nextUnparsed);
}

ParserReturnType<std::unique_ptr<StmtlstNode>> parseStatementList(frontend::TokenList* programTokens,
                                                                  TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    List<StatementNode> statements;
    TokenListIndex nextUnparsed = startIndex + 1; // ignore "{"
    Boolean isSyntaxError = false;                // flag for syntax error

    if (numberOfTokens - startIndex > 1 && programTokens->at(startIndex).tokenTag == frontend::BracesOpenTag) {
        // iterate through the tokens
        while (nextUnparsed < numberOfTokens && nextUnparsed > 0
               && programTokens->at(nextUnparsed).tokenTag != frontend::BracesClosedTag) {

            ParserReturnType<std::unique_ptr<StatementNode>> result = parseStatement(programTokens, nextUnparsed);
            statements.push_back(std::move(result.astNode));
            nextUnparsed = result.nextUnparsedToken;
        }

        if (nextUnparsed < 0) {
            // syntax error in statements
            isSyntaxError = true;
        } else if (nextUnparsed >= numberOfTokens
                   || programTokens->at(nextUnparsed).tokenTag != frontend::BracesClosedTag) {
            // syntax error due to unmatched brackets in statement list
            return getSyntaxError<StmtlstNode>(34);
        } else {
            // no errors
        }
    } else {
        // syntax error, empty statement list or missing braces
        return getSyntaxError<StmtlstNode>(3);
    }

    if (isSyntaxError || nextUnparsed < 0) {
        return getSyntaxError<StmtlstNode>(-nextUnparsed);
    } else {
        return ParserReturnType<std::unique_ptr<StmtlstNode>>(
            std::unique_ptr<StmtlstNode>(createStmtlstNode(statements)), nextUnparsed + 1 /* ignore closing braces */);
    }
}

ParserReturnType<std::unique_ptr<ProcedureNode>> parseProcedure(frontend::TokenList* programTokens,
                                                                TokenListIndex startIndex)
{
    // set up variables
    TokenListIndex numberOfTokens = programTokens->size();
    String procedureName;
    StmtlstNode* statementListNode;
    TokenListIndex nextUnparsed;
    Boolean isSyntaxError = false; // flag for syntax error

    // check if procedure is valid
    if ((numberOfTokens - startIndex > 2) && (programTokens->at(startIndex).tokenTag == frontend::ProcedureKeywordTag)
        && frontend::isIdentifierTag(programTokens->at(startIndex + 1).tokenTag)) {

        procedureName = programTokens->at(startIndex + 1).rawString;
        ParserReturnType<std::unique_ptr<StmtlstNode>> result = parseStatementList(programTokens, startIndex + 2);
        if (result.hasError()) {
            // syntax error in statement list
            isSyntaxError = true;
        } else {
            statementListNode = result.astNode.release();
        }
        nextUnparsed = result.nextUnparsedToken;
    } else {
        // syntax error for procedure, not of form "procedure name" ...
        return getSyntaxError<ProcedureNode>(2);
    }

    if (isSyntaxError) {
        return getSyntaxError<ProcedureNode>(-nextUnparsed);
    } else {
        StatementNumber first = statementListNode->statementList.begin()->get()->getStatementNumber();
        StatementNumber last = getLastStatementNumber(statementListNode);
        insertIntoProcedureTable(procedureName, first, last);
        return ParserReturnType<std::unique_ptr<ProcedureNode>>(
            std::unique_ptr<ProcedureNode>(createProcedureNode(procedureName, statementListNode)), nextUnparsed);
    }
}

ParserReturnType<ProgramNode*> parseSimpleReturnNode(const String& rawProgram)
{
    StringVector programFragments = splitProgram(rawProgram);
    frontend::TokenList tokenisedProgram = frontend::tokeniseSimple(std::move(programFragments));
    // start at index 0
    TokenListIndex currentIndex = 0;
    int numberOfTokens = tokenisedProgram.size();
    List<ProcedureNode> procedures;
    // reset statement numbers to 0
    statementsSeen = 0;
    while (currentIndex >= 0 && currentIndex < numberOfTokens) {
        if (tokenisedProgram.at(currentIndex).tokenTag == frontend::ProcedureKeywordTag) {
            ParserReturnType<std::unique_ptr<ProcedureNode>> p = parseProcedure(&tokenisedProgram, currentIndex);
            procedures.push_back(std::move(p.astNode));
            currentIndex = p.nextUnparsedToken;
        } else {
            // syntax error, SIMPLE program must consist of procedures
            return {nullptr, -1};
        }
    }

    if (currentIndex < 0) {
        return {nullptr, currentIndex};
    } else {
        return {createProgramNode(procedures.at(0)->procedureName, procedures, statementsSeen), numberOfTokens};
    }
}
