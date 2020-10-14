/**
 * Implementation of Assign Matcher for Query Evaluator,
 * used to evaluate assign pattern clauses.
 */

#include "AssignMatcher.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "PatternMatcherUtil.h"
#include "pkb/PKB.h"

/**
 * Checks whether the clauseExpression can be found
 * within the programExpression. If it is not found,
 * check the subtrees of the programExpression
 * by calling doExpressionsMatchRecursive recursively.
 *
 * @param programExpression The tree of an Expression from
 *                          the SIMPLE program, to match the
 *                          clauseExpression to.
 * @param clauseExpression ExpressionSpec object from a
 *                         query clause, to match with a
 *                         subtree in programExpression.
 * @return True, if clauseExpression matches some subtree
 *         within programExpression. Otherwise, false.
 */
Boolean doExpressionsMatchRecursive(const Expression* const programExpression, Expression* clauseExpression)
{
    if (!programExpression->isArithmetic()) {
        // the expression is a reference, either constant or variable
        return *(clauseExpression) == *(programExpression);
    } else {
        // arithmetic expression
        auto* arithExp = static_cast<const ArithmeticExpression* const>(programExpression); // NOLINT
        return *(clauseExpression) == *(programExpression)
               || doExpressionsMatchRecursive(arithExp->leftFactor, clauseExpression)
               || doExpressionsMatchRecursive(arithExp->rightFactor, clauseExpression);
    }
}

/**
 * For a given query ExpressionSpec, check whether
 * it matches an SIMPLE program expression.
 *
 * @param programExpression The tree of an Expression from
 *                          the SIMPLE program, to match the
 *                          expSpec to.
 * @param expSpec The expression spec in the query.
 *
 * @return True, if the expression spec matches the
 *         programExpression. Otherwise, false.
 */
Boolean doesExpressionSpecMatch(const Expression* programExpression, const ExpressionSpec& expSpec)
{
    ExpressionSpecType type = expSpec.expressionSpecType;
    switch (type) {
    case WildcardExpressionType:
        // always matches with a wildcard
        return true;
    case LiteralExpressionType:
        // check the top level for equality only
        return *(programExpression) == *(expSpec.getExpression());
    case ExtendableLiteralExpressionType:
        // check all subtrees of the expression
        return doExpressionsMatchRecursive(programExpression, expSpec.getExpression());
    default:
        throw std::runtime_error("Unknown or invalid expression type in doesExpressionSpecMatch");
    }
}

/**
 * Given the Abstract Syntax Tree node that represents
 * a SIMPLE program assignment statement, check if the
 * pattern clause matches the assign statement. If the
 * pattern clause matches but the variable assigned to
 * is not found in the resultsTable, return false.
 *
 * @param assign The node representing the assignment statement.
 * @param pnClause The pattern clause from the query.
 *
 * @return True, if the assign statement matches the clause.
 *         False, if there was no match.
 */
Boolean matchAssignStatement(AssignmentStatementNode* assign, PatternClause* pnClause)
{
    // check whether the variable that is being assigned to matches
    ReferenceType refType = pnClause->getEntRef().getReferenceType();
    if ((refType == LiteralRefType && assign->variable.varName == pnClause->getEntRef().getValue())
        || refType == WildcardRefType || refType == SynonymRefType) {
        // check whether expression matches
        return doesExpressionSpecMatch(assign->expression, pnClause->getExprSpec());
    } else {
        return false;
    }
}

/**
 * Given a statement list node, find assign statements in the
 * statement list that matches the given pattern clause of
 * some query, and then gathers a list of results to store
 * in the results table.
 *
 * @param stmtLstNode The statement list node to traverse.
 * @param pnClause The pattern clause to find.
 * @param constraints The constraints of this clause.
 *
 * @return The list of matching statements and variables.
 */
PatternMatcherTuple findAssignInStatementList(const StmtlstNode* const stmtLstNode, PatternClause* pnClause)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    PatternMatcherTuple results;
    for (const std::unique_ptr<StatementNode>& currStmt : statements) {
        StatementType stmtType = currStmt->getStatementType();
        if (stmtType == AssignmentStatement) {
            // NOLINTNEXTLINE
            auto* assign = static_cast<AssignmentStatementNode*>(currStmt.get());
            Boolean hasMatch = matchAssignStatement(assign, pnClause);
            // if there is a match, check the relationship to add to list of results
            if (hasMatch) {
                results.addTargetStatement(currStmt->getStatementNumber(), assign->variable.varName);
            }
        } else if (stmtType == IfStatement) {
            // NOLINTNEXTLINE
            auto* ifNode = static_cast<IfStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromIf = findAssignInStatementList(ifNode->ifStatementList, pnClause);
            PatternMatcherTuple resultsFromElse = findAssignInStatementList(ifNode->elseStatementList, pnClause);
            results.concatTuple(resultsFromIf);
            results.concatTuple(resultsFromElse);
        } else if (stmtType == WhileStatement) {
            // NOLINTNEXTLINE
            auto* whileNode = static_cast<WhileStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromWhile = findAssignInStatementList(whileNode->statementList, pnClause);
            results.concatTuple(resultsFromWhile);
        }
    }
    return results;
}

Void evaluateAssignPattern(PatternClause* pnClause, ResultsTable* resultsTable)
{
    ProgramNode* ast = getRootNode();
    const List<ProcedureNode>& procedureList = ast->procedureList;
    PatternMatcherTuple allResults;
    for (const std::unique_ptr<ProcedureNode>& proc : procedureList) {
        PatternMatcherTuple resultsFromProcedure = findAssignInStatementList(proc->statementListNode, pnClause);
        allResults.concatTuple(resultsFromProcedure);
    }
    // store results in ResultTable
    resultsTable->storeResultsTwo(pnClause->getPatternSynonym(), allResults.getTargetStatements(),
                                  pnClause->getEntRef(), convertToPairedResult(allResults.getRelationships()));
}
