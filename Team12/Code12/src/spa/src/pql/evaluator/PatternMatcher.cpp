/**
 * Implementation of Pattern Matcher for Query Evaluator,
 * used to evaluate Pattern clauses in queries
 * given to SIMPLE Program Analyser.
 */
#include "PatternMatcher.h"

#include "pkb/PKB.h"

/**
 * Checks whether the clauseExpression can be found
 * within the programExpression. If it is not found,
 * check the subtrees of the programExpression
 * by calling doExpressionsMatch recursively.
 *
 * @param programExpression The tree of an Expression from
 *                          the SIMPLE program, to match the
 *                          clauseExpression to.
 * @param clauseExpression The tree of an Expression from a
 *                         query clause, to match with a
 *                         subtree in programExpression.
 * @return True, if clauseExpression matches some subtree
 *         within programExpression. Otherwise, false.
 */
Boolean doExpressionsMatch(const Expression* const programExpression, Expression* clauseExpression)
{
    if (!programExpression->isArithmetic()) {
        // the expression is a reference, either constant or variable
        return clauseExpression == programExpression;
    } else {
        // arithmetic expression
        auto* arithExp = static_cast<const ArithmeticExpression* const>(programExpression); // NOLINT
        return doExpressionsMatch(arithExp->leftFactor, clauseExpression)
               && doExpressionsMatch(arithExp->rightFactor, clauseExpression);
    }
}

/**
 * An enum that represents certain states that the
 * PatternMatcher can run in, to determine what
 * results should be returned by the PatternMatcher.
 *
 * The enum corresponds to the relationship between
 * the "Select"ed synonym of the query and the pattern
 * clause of the query. Specifically, whether the synonym
 * appears in the pattern clause, and the type of synonym.
 */
enum class SynonymAndClauseRelationship { Unrelated, Statement, Variable };

/**
 * Given a synonym and an assign pattern clause,
 * determines the relationship between the provided
 * synonym and the clause provided.
 *
 * @param synonym Select synonym as specified in the query.
 * @param pnClause Pattern clause as specified in the query.
 *
 * @return The relationship between the two entities.
 */
SynonymAndClauseRelationship determineRelationshipForAssignPattern(const Synonym& synonym, PatternClause* pnClause)
{
    if (pnClause->getPatternSynonym() == synonym) {
        return SynonymAndClauseRelationship::Statement;
    }
    Reference assignedToVariable = pnClause->getEntRef();
    if (assignedToVariable.isNonStatementSynonym() && assignedToVariable.getValue() == synonym) {
        return SynonymAndClauseRelationship::Variable;
    } else if (assignedToVariable.isWildCard() || assignedToVariable.isNonStatementSynonym()) {
        return SynonymAndClauseRelationship::Unrelated;
    } else {
        throw std::runtime_error("Could not match pattern clause in determineRelationshipForAssignPattern");
    }
}

/**
 * Given the Abstract Syntax Tree node that represents
 * a SIMPLE program assignment statement, check if the
 * pattern clause matches the assign statement.
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
        return doExpressionsMatch(assign->expression, pnClause->getExprSpec().getExpression());
    } else {
        return false;
    }
}

/**
 * Given a statement list node, find assign statements in the
 * statement list that matches the given pattern clause of
 * some query, and then return a list of results based on
 * the relationship between the synonym and the clause.
 *
 * @param stmtLstNode The statement list node to traverse.
 * @param pnClause The pattern clause to find.
 * @param relationship Relationship between the synonym
 *                     of the query and the pattern clause.
 * @return Returns a list of query results.
 */
std::vector<String> findAssignInStatementList(const StmtlstNode* const stmtLstNode, PatternClause* pnClause,
                                              SynonymAndClauseRelationship relationship)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    std::unordered_set<String> resultsList;
    for (const std::unique_ptr<StatementNode>& currStmt : statements) {
        StatementType stmtType = currStmt->getStatementType();
        if (stmtType == AssignmentStatement) {
            // NOLINTNEXTLINE
            auto* assign = static_cast<AssignmentStatementNode*>(currStmt.get());
            Boolean hasMatch = matchAssignStatement(assign, pnClause);
            // if there is a match, check the relationship to add to list of results
            if (hasMatch && relationship == SynonymAndClauseRelationship::Variable) {
                resultsList.insert(assign->variable.varName);
            } else if (hasMatch) {
                // relationship == SynonymAndClauseRelationship::Statement or ::Unrelated
                resultsList.insert(std::to_string(currStmt->getStatementNumber()));
            }
        } else if (stmtType == IfStatement) {
            // NOLINTNEXTLINE
            auto* ifNode = static_cast<IfStatementNode*>(currStmt.get());
            std::insert_iterator<std::unordered_set<String>> setInserter
                = std::inserter(resultsList, resultsList.end());
            std::vector<String> resultsFromIf
                = findAssignInStatementList(ifNode->ifStatementList, pnClause, relationship);
            std::vector<String> resultsFromElse
                = findAssignInStatementList(ifNode->elseStatementList, pnClause, relationship);
            std::copy(resultsFromIf.begin(), resultsFromIf.end(), setInserter);
            std::copy(resultsFromElse.begin(), resultsFromElse.end(), setInserter);
        } else if (stmtType == WhileStatement) {
            // NOLINTNEXTLINE
            auto* whileNode = static_cast<WhileStatementNode*>(currStmt.get());
            std::vector<String> resultsFromWhile
                = findAssignInStatementList(whileNode->statementList, pnClause, relationship);
            std::copy(resultsFromWhile.begin(), resultsFromWhile.end(), std::inserter(resultsList, resultsList.end()));
        }
    }
    return std::vector<String>(resultsList.begin(), resultsList.end());
}

/**
 * Given an assignment pattern clause, retrieves the SIMPLE
 * program from the Program Knowledge Base (root node of
 * Abstract Syntax Tree) and tries to find the assignment
 * statements, or assigned to variables, that match the
 * expression pattern specified.
 *
 * @param synonym The Select synonym in the query.
 * @param pnClause A pattern clause in the query.
 * @param declarations Declarations table for the query.
 *
 * @return Returns a list of results found.
 */
std::vector<String> evaluateAssignPattern(const Synonym& synonym, PatternClause* pnClause)
{
    std::unordered_set<String> uniqueResult;
    ProgramNode* ast = getRootNode();
    List<ProcedureNode> procedureList = ast->procedureList;
    SynonymAndClauseRelationship relationship = determineRelationshipForAssignPattern(synonym, pnClause);
    for (std::unique_ptr<ProcedureNode>& proc : procedureList) {
        std::vector<String> resultsFromProcedure
            = findAssignInStatementList(proc->statementListNode, pnClause, relationship);
        std::copy(resultsFromProcedure.begin(), resultsFromProcedure.end(),
                  std::inserter(uniqueResult, uniqueResult.end()));
    }
    return std::vector<String>(uniqueResult.begin(), uniqueResult.end());
}

std::vector<String> evaluatePattern(const Synonym& synonym, PatternClause* pnClause)
{
    if (pnClause->getStatementType() == AssignPatternType) {
        return evaluateAssignPattern(synonym, pnClause);
    } else {
        throw std::runtime_error("Unknown PatternClause type in evaluatePattern");
    }
}
