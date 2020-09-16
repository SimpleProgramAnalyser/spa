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
Boolean doExpressionsMatch(Expression* programExpression, Expression* clauseExpression) {}

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

std::vector<String> matchAssignStatement(AssignmentStatementNode* assign, PatternClause* pnClause,
                                         SynonymAndClauseRelationship relationship)
{}

std::vector<String> findAssignInStatementList(StmtlstNode* stmtLstNode, PatternClause* pnClause,
                                              SynonymAndClauseRelationship relationship)
{}

std::vector<String> evaluateAssignPattern(const Synonym& synonym, PatternClause* pnClause,
                                          const DeclarationTable& declarations)
{
    std::vector<String> result;
    ProgramNode* ast = getRootNode();

    return result;
}

std::vector<String> evaluatePattern(const Synonym& synonym, PatternClause* pnClause,
                                    const DeclarationTable& declarations)
{
    if (pnClause->getStatementType() == AssignPatternType) {
        return evaluateAssignPattern(synonym, pnClause, declarations);
    } else {
        throw std::runtime_error("Unknown PatternClause type in evaluatePattern");
    }
}
