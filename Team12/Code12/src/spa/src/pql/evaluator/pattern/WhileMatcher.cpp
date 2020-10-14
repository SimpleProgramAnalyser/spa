/**
 * Implementation of While Matcher for Query Evaluator,
 * used to evaluate while pattern clauses.
 */

#include "WhileMatcher.h"

#include "PatternMatcherUtil.h"
#include "pkb/PKB.h"

Void getVariableInExpr(const Expression* expr, std::unordered_set<String>& variables)
{
    if (expr->isArithmetic()) {
        auto arithmeticExpr = static_cast<const ArithmeticExpression*>(expr);
        const Expression* leftExpr = arithmeticExpr->leftFactor;
        const Expression* rightExpr = arithmeticExpr->rightFactor;

        getVariableInExpr(leftExpr, variables);
        getVariableInExpr(rightExpr, variables);
    } else {
        // Reference Expression
        auto refExpr = static_cast<const ReferenceExpression*>(expr);
        const BasicDataType* basicDataType = refExpr->basicData;
        if (basicDataType->isConstant()) {
            return;
        }

        const auto* variable = static_cast<const Variable*>(basicDataType);
        variables.insert(variable->varName);
    }
}

Void getLiteralVariablesInCondExpr(const ConditionalExpression* condExpr, std::unordered_set<String>& variables)
{
    auto conditionType = condExpr->getConditionalType();
    switch (conditionType) {
    case NotConditionalExpression: {
        auto notCondExpr = static_cast<const NotExpression*>(condExpr);
        const ConditionalExpression* negatedCondExpression = notCondExpr->expression;

        getLiteralVariablesInCondExpr(negatedCondExpression, variables);
        break;
    }
    case OrConditionalExpression: {
        auto orCondExpr = static_cast<const OrExpression*>(condExpr);
        const ConditionalExpression* leftCondExpr = orCondExpr->leftExpression;
        const ConditionalExpression* rightCondExpr = orCondExpr->rightExpression;

        getLiteralVariablesInCondExpr(leftCondExpr, variables);
        getLiteralVariablesInCondExpr(rightCondExpr, variables);
        break;
    }
    case AndConditionalExpression: {
        auto andCondExpr = static_cast<const AndExpression*>(condExpr);
        const ConditionalExpression* leftCondExpr = andCondExpr->leftExpression;
        const ConditionalExpression* rightCondExpr = andCondExpr->rightExpression;

        getLiteralVariablesInCondExpr(leftCondExpr, variables);
        getLiteralVariablesInCondExpr(rightCondExpr, variables);
        break;
    }
    case RelationalConditionalExpression: {
        auto relCondExpr = static_cast<const RelationalExpression*>(condExpr);
        const Expression* leftExpr = relCondExpr->leftFactor;
        const Expression* rightExpr = relCondExpr->rightFactor;

        getVariableInExpr(leftExpr, variables);
        getVariableInExpr(rightExpr, variables);
        break;
    }
    default:
        throw std::runtime_error("Unknown or invalid expression type in getLiteralVariablesInCondExpr");
    }
}

PatternMatcherTuple matchWhileStatement(WhileStatementNode* whileNode, PatternClause* pnClause, StatementNumber stmtNumber)
{
    Reference controlVariableRef = pnClause->getEntRef();
    ReferenceType refType = controlVariableRef.getReferenceType();
    PatternMatcherTuple results;

    assert(refType == WildcardRefType || refType == LiteralRefType || refType == SynonymRefType);

    if (refType == WildcardRefType) {
        results.addTargetStatement(stmtNumber);
    } else {
        std::unordered_set<String> variables;
        getLiteralVariablesInCondExpr(whileNode->predicate, variables);

        if (refType == LiteralRefType) {
            String literalVariable = controlVariableRef.getValue();
            auto gotLiteralVariable = variables.find(literalVariable);

            if (gotLiteralVariable == variables.end()) {
                return results;
            }

            results.addTargetStatement(stmtNumber);
        } else {
            // SynonymRefType
            results.addTargetStatement(stmtNumber, variables);
        }
    }

    return results;
}

PatternMatcherTuple findWhileInStatementList(const StmtlstNode* const stmtLstNode, PatternClause* pnClause)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    PatternMatcherTuple results;
    for (const std::unique_ptr<StatementNode>& currStmt : statements) {
        StatementType stmtType = currStmt->getStatementType();
        if (stmtType == WhileStatement) {
            // NOLINTNEXTLINE
            auto* whileNode = static_cast<WhileStatementNode*>(currStmt.get());
            StatementNumber stmtNumber = currStmt->getStatementNumber();
            PatternMatcherTuple resultsFromWhile = matchWhileStatement(whileNode, pnClause, stmtNumber);

            // find and match nested while statements
            PatternMatcherTuple resultsFromNestedWhile = findWhileInStatementList(whileNode->statementList, pnClause);
            results.concatTuple(resultsFromWhile);
            results.concatTuple(resultsFromNestedWhile);
        } else if (stmtType == IfStatement) {
            // NOLINTNEXTLINE
            auto* ifNode = static_cast<IfStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromIf = findWhileInStatementList(ifNode->ifStatementList, pnClause);
            PatternMatcherTuple resultsFromElse = findWhileInStatementList(ifNode->elseStatementList, pnClause);
            results.concatTuple(resultsFromIf);
            results.concatTuple(resultsFromElse);
        }
    }

    return results;
}

Void evaluateWhilePattern(PatternClause* pnClause, ResultsTable* resultsTable)
{
    // get all results from AST
    ProgramNode* ast = getRootNode();
    const List<ProcedureNode>& procedureList = ast->procedureList;
    PatternMatcherTuple allResults;
    for (const std::unique_ptr<ProcedureNode>& proc : procedureList) {
        PatternMatcherTuple resultsFromProcedure = findWhileInStatementList(proc->statementListNode, pnClause);
        allResults.concatTuple(resultsFromProcedure);
    }

    // store results in ResultTable
    ReferenceType controlVariableRefType = pnClause->getEntRef().getReferenceType();
    switch (controlVariableRefType) {
    case WildcardRefType:
    case LiteralRefType:
        resultsTable->storeResultsOne(pnClause->getPatternSynonym(), allResults.getTargetStatements());
        break;
    case SynonymRefType:
        resultsTable->storeResultsTwo(pnClause->getPatternSynonym(), allResults.getTargetStatements(),
                                      pnClause->getEntRef(), convertToPairedResult(allResults.getRelationships()));
        break;
    default:
        throw std::runtime_error("Unknown or invalid reference type in evaluateWhilePattern");
    }
}
