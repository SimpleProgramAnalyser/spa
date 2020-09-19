/**
 * Implementation of Pattern Matcher for Query Evaluator,
 * used to evaluate Pattern clauses in queries
 * given to SIMPLE Program Analyser.
 */
#include "PatternMatcher.h"

#include <algorithm>

#include "pkb/PKB.h"

/**
 * A class to hold result lists for matching
 * assign patterns and variables.
 */
class PatternMatcherTuple {
private:
    std::vector<String> assignStatementResults;
    std::unordered_set<String> variableResults;

public:
    PatternMatcherTuple() = default;

    PatternMatcherTuple(std::vector<String> assignStatementResults, std::vector<String> variableResults):
        assignStatementResults(std::move(assignStatementResults))
    {
        std::copy(variableResults.begin(), variableResults.end(),
                  std::inserter(variableResults, variableResults.end()));
    }

    /**
     * Adds a matching assign statement to the result lists.
     */
    Void addAssignStatement(Integer assignStatementNumber, const String& variable)
    {
        assignStatementResults.push_back(std::to_string(assignStatementNumber));
        variableResults.insert(variable);
    }

    /**
     * Adds entries from another PatternMatcherTuple to
     * this PatternMatcherTuple.
     */
    Void concatTuple(const PatternMatcherTuple& pmt)
    {
        this->assignStatementResults.insert(this->assignStatementResults.cend(), pmt.assignStatementResults.cbegin(),
                                            pmt.assignStatementResults.cend());
        std::copy(pmt.variableResults.begin(), pmt.variableResults.end(),
                  std::inserter(this->variableResults, this->variableResults.end()));
    }

    /**
     * Gets the list of results for matching statements.
     */
    std::vector<String> getAssignStatements() const
    {
        return assignStatementResults;
    }

    /**
     * Gets the list of results for matching variables.
     */
    std::vector<String> getVariables() const
    {
        return std::vector<String>(variableResults.begin(), variableResults.end());
    }
};

/**
 * Checks whether an item is equal to some item
 * in a vector, making use of operator==().
 *
 * @tparam T Type of the item.
 * @param vector The vector to search.
 * @param item The item to match against.
 *
 * @return Returns true if item was found in vector.
 *         Otherwise, if item != any element of vector,
 *         return false.
 */
template <typename T>
Boolean isItemInVector(std::vector<T> vector, T item)
{
    return std::find(vector.begin(), vector.end(), item) != vector.end();
}

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
Boolean doesExpressionSpecMatch(const Expression* programExpression, ExpressionSpec expSpec)
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
 * @param resultsTable The results table that contains results
 *                     from evaluation of other clauses.
 *
 * @return True, if the assign statement matches the clause.
 *         False, if there was no match.
 */
Boolean matchAssignStatement(AssignmentStatementNode* assign, PatternClause* pnClause, ResultsTable* resultsTable)
{
    // check whether the variable that is being assigned to matches
    ReferenceType refType = pnClause->getEntRef().getReferenceType();
    if ((refType == LiteralRefType && assign->variable.varName == pnClause->getEntRef().getValue())
        || refType == WildcardRefType
        || (refType == SynonymRefType
            && !resultsTable->checkIfSynonymHasConstraints(pnClause->getEntRef().getValue()))) {
        // check whether expression matches
        return doesExpressionSpecMatch(assign->expression, pnClause->getExprSpec());
    } else if (refType == SynonymRefType) {
        // assigned to variable is not in in the constrained list
        Variable astVariable = assign->variable;
        std::vector<String> previousResults = resultsTable->get(pnClause->getEntRef().getValue());
        return isItemInVector<std::string>(previousResults, astVariable.toString())
               && doesExpressionSpecMatch(assign->expression, pnClause->getExprSpec());
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
 * @param resultsTable The results table to store the results in.
 *
 * @return The list of matching statements and variables.
 */
PatternMatcherTuple findAssignInStatementList(const StmtlstNode* const stmtLstNode, PatternClause* pnClause,
                                              ResultsTable* resultsTable)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    PatternMatcherTuple results;
    Boolean stmtHasConstraints = resultsTable->checkIfSynonymHasConstraints(pnClause->getPatternSynonym());
    for (const std::unique_ptr<StatementNode>& currStmt : statements) {
        StatementType stmtType = currStmt->getStatementType();
        if (stmtType == AssignmentStatement) {
            if (stmtHasConstraints
                && !isItemInVector<std::string>(resultsTable->get(pnClause->getPatternSynonym()),
                                                std::to_string(currStmt->getStatementNumber()))) {
                // this assign statement is not counted, due to previous results
                continue;
            }
            // NOLINTNEXTLINE
            auto* assign = static_cast<AssignmentStatementNode*>(currStmt.get());
            Boolean hasMatch = matchAssignStatement(assign, pnClause, resultsTable);
            // if there is a match, check the relationship to add to list of results
            if (hasMatch) {
                results.addAssignStatement(currStmt->getStatementNumber(), assign->variable.varName);
            }
        } else if (stmtType == IfStatement) {
            // NOLINTNEXTLINE
            auto* ifNode = static_cast<IfStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromIf
                = findAssignInStatementList(ifNode->ifStatementList, pnClause, resultsTable);
            PatternMatcherTuple resultsFromElse
                = findAssignInStatementList(ifNode->elseStatementList, pnClause, resultsTable);
            results.concatTuple(resultsFromIf);
            results.concatTuple(resultsFromElse);
        } else if (stmtType == WhileStatement) {
            // NOLINTNEXTLINE
            auto* whileNode = static_cast<WhileStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromWhile
                = findAssignInStatementList(whileNode->statementList, pnClause, resultsTable);
            results.concatTuple(resultsFromWhile);
        }
    }
    return results;
}

/**
 * Given an assignment pattern clause, retrieves the SIMPLE
 * program from the Program Knowledge Base (root node of
 * Abstract Syntax Tree) and tries to find the assignment
 * statements, or assigned to variables, that match the
 * expression pattern specified.
 *
 * @param pnClause A pattern clause in the query.
 * @param resultsTable The results table to store the results in.
 */
Void evaluateAssignPattern(PatternClause* pnClause, ResultsTable* resultsTable)
{
    ProgramNode* ast = getRootNode();
    const List<ProcedureNode>& procedureList = ast->procedureList;
    PatternMatcherTuple allResults;
    for (const std::unique_ptr<ProcedureNode>& proc : procedureList) {
        PatternMatcherTuple resultsFromProcedure
            = findAssignInStatementList(proc->statementListNode, pnClause, resultsTable);
        allResults.concatTuple(resultsFromProcedure);
    }
    // store results in ResultTable
    resultsTable->filterTable(pnClause->getPatternSynonym(), allResults.getAssignStatements());
    resultsTable->filterTable(pnClause->getEntRef(), allResults.getVariables());
}

Void evaluatePattern(PatternClause* pnClause, ResultsTable* resultsTable)
{
    if (pnClause->getStatementType() == AssignPatternType) {
        evaluateAssignPattern(pnClause, resultsTable);
    } else {
        throw std::runtime_error("Unknown PatternClause type in evaluatePattern");
    }
}
