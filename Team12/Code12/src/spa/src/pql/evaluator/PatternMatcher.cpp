/**
 * Implementation of Pattern Matcher for Query Evaluator,
 * used to evaluate Pattern clauses in queries
 * given to SIMPLE Program Analyser.
 */
#include "PatternMatcher.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "pkb/PKB.h"

/**
 * A class to hold result lists for matching
 * assign patterns and variables.
 */
class PatternMatcherTuple {
private:
    std::vector<String> assignStatementResults;
    std::unordered_set<String> variableResults;
    std::vector<std::pair<Integer, String>> relationshipsResults;

public:
    PatternMatcherTuple() = default;

    /**
     * Adds a matching assign statement to the result lists.
     */
    Void addAssignStatement(Integer assignStatementNumber, const String& variable)
    {
        assignStatementResults.push_back(std::to_string(assignStatementNumber));
        variableResults.insert(variable);
        relationshipsResults.emplace_back(assignStatementNumber, variable);
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
        this->relationshipsResults.insert(this->relationshipsResults.cend(), pmt.relationshipsResults.cbegin(),
                                          pmt.relationshipsResults.cend());
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

    /**
     * Gets the list of results for matching relationships.
     */
    std::vector<std::pair<Integer, String>> getRelationships() const
    {
        return relationshipsResults;
    }
};

/**
 * An enum describing possible constraints for
 * a pattern clause to follow.
 */
enum class ConstraintType {
    NoConstraints,
    StatementSynonymConstrained,
    VariableSynonymConstrained,
    BothSynonymsConstrained
};

/**
 * Encapsulates constraints for a pattern clause,
 * as well as tables for each synonym or relationship.
 */
class ConstraintChecker {
private:
    const ConstraintType type;
    const std::unordered_set<String> singleConstrainedSynonymSet;
    const std::unordered_map<String, std::unordered_set<String>> statementVariableRelationships;

public:
    /**
     * Constructs a ConstraintChecker.
     *
     * @param type Type of constraint in the pattern clause.
     * @param singleSynonymSet A set of possible values for a synonym,
     *                         if only one synonym is constrained.
     * @param relationshipsMap A map of statement values to variable
     *                         values, if both synonyms are constrained.
     */
    ConstraintChecker(ConstraintType type, std::unordered_set<String> singleSynonymSet,
                      std::unordered_map<String, std::unordered_set<String>> relationshipsMap):
        type(type),
        singleConstrainedSynonymSet(std::move(singleSynonymSet)),
        statementVariableRelationships(std::move(relationshipsMap))
    {}

    /**
     * Checks whether an assignment statement matches
     * the constraints described in this object.
     *
     * @param assignStatement The assignment statement from
     *                        the Abstract Syntax Tree.
     * @return True, if the assignment statement should be
     *         evaluated. False, if it should not.
     */
    Boolean doesAssignStatementFitConstraints(AssignmentStatementNode* assignStatement) const
    {
        if (type == ConstraintType::NoConstraints) {
            return true;
        } else if (type == ConstraintType::StatementSynonymConstrained) {
            return singleConstrainedSynonymSet.find(std::to_string(assignStatement->getStatementNumber()))
                   != singleConstrainedSynonymSet.end();
        } else if (type == ConstraintType::VariableSynonymConstrained) {
            return singleConstrainedSynonymSet.find(assignStatement->variable.varName)
                   != singleConstrainedSynonymSet.end();
        } else {
            // type == ConstraintType::BothSynonymsConstrained
            String statementNumber = std::to_string(assignStatement->getStatementNumber());
            return statementVariableRelationships.find(statementNumber) != statementVariableRelationships.end()
                   && statementVariableRelationships.at(statementNumber).find(assignStatement->variable.varName)
                          != statementVariableRelationships.at(statementNumber).end();
        }
    }
};

/**
 * Given a pattern clause and results table, determine
 * the constraint of the clause and store useful tables
 * in a ConstraintChecker.
 *
 * @param pnClause The pattern clause to check.
 * @param resultsTable The results
 * @return ConstraintChecker to allow checking of constraints.
 */
ConstraintChecker determineConstraintAndPopulateTables(PatternClause* pnClause, ResultsTable* resultsTable)
{
    ConstraintType type;
    std::unordered_set<String> singleSynonymSet;
    std::unordered_map<String, std::unordered_set<String>> relationshipsMap;
    Boolean statementSynonymHasConstraints = resultsTable->checkIfSynonymHasConstraints(pnClause->getPatternSynonym());
    Boolean variableSynonymHasConstraints
        = pnClause->getEntRef().getReferenceType() == SynonymRefType
          && resultsTable->checkIfSynonymHasConstraints(pnClause->getEntRef().getValue());
    if (statementSynonymHasConstraints && variableSynonymHasConstraints) {
        type = ConstraintType::BothSynonymsConstrained;
        std::vector<std::pair<String, String>> relationshipsList
            = resultsTable->getRelationships(pnClause->getPatternSynonym(), pnClause->getEntRef().getValue());
        for (const std::pair<String, String>& relationship : relationshipsList) {
            if (relationshipsMap.find(relationship.first) == relationshipsMap.end()) {
                relationshipsMap.insert(
                    std::pair<String, std::unordered_set<String>>(relationship.first, std::unordered_set<String>()));
            }
            relationshipsMap[relationship.first].insert(relationship.second);
        }
    } else if (statementSynonymHasConstraints) {
        type = ConstraintType::StatementSynonymConstrained;
        std::vector<String> matchingStatementsList = resultsTable->get(pnClause->getPatternSynonym());
        std::copy(matchingStatementsList.begin(), matchingStatementsList.end(),
                  std::inserter(singleSynonymSet, singleSynonymSet.end()));
    } else if (variableSynonymHasConstraints) {
        type = ConstraintType::VariableSynonymConstrained;
        std::vector<String> matchingVariablesList = resultsTable->get(pnClause->getEntRef().getValue());
        std::copy(matchingVariablesList.begin(), matchingVariablesList.end(),
                  std::inserter(singleSynonymSet, singleSynonymSet.end()));
    } else {
        type = ConstraintType::NoConstraints;
    }
    return ConstraintChecker(type, singleSynonymSet, relationshipsMap);
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
PatternMatcherTuple findAssignInStatementList(const StmtlstNode* const stmtLstNode, PatternClause* pnClause,
                                              const ConstraintChecker& constraints)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    PatternMatcherTuple results;
    for (const std::unique_ptr<StatementNode>& currStmt : statements) {
        StatementType stmtType = currStmt->getStatementType();
        if (stmtType == AssignmentStatement) {
            // NOLINTNEXTLINE
            auto* assign = static_cast<AssignmentStatementNode*>(currStmt.get());
            if (!constraints.doesAssignStatementFitConstraints(assign)) {
                // this assign statement is not counted, due to previous results
                continue;
            }
            Boolean hasMatch = matchAssignStatement(assign, pnClause);
            // if there is a match, check the relationship to add to list of results
            if (hasMatch) {
                results.addAssignStatement(currStmt->getStatementNumber(), assign->variable.varName);
            }
        } else if (stmtType == IfStatement) {
            // NOLINTNEXTLINE
            auto* ifNode = static_cast<IfStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromIf
                = findAssignInStatementList(ifNode->ifStatementList, pnClause, constraints);
            PatternMatcherTuple resultsFromElse
                = findAssignInStatementList(ifNode->elseStatementList, pnClause, constraints);
            results.concatTuple(resultsFromIf);
            results.concatTuple(resultsFromElse);
        } else if (stmtType == WhileStatement) {
            // NOLINTNEXTLINE
            auto* whileNode = static_cast<WhileStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromWhile
                = findAssignInStatementList(whileNode->statementList, pnClause, constraints);
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
    ConstraintChecker constraints = determineConstraintAndPopulateTables(pnClause, resultsTable);
    for (const std::unique_ptr<ProcedureNode>& proc : procedureList) {
        PatternMatcherTuple resultsFromProcedure
            = findAssignInStatementList(proc->statementListNode, pnClause, constraints);
        allResults.concatTuple(resultsFromProcedure);
    }
    // store results in ResultTable
    resultsTable->filterTable(pnClause->getPatternSynonym(), allResults.getAssignStatements());
    resultsTable->filterTable(pnClause->getEntRef(), allResults.getVariables());
    resultsTable->associateRelationships(
        allResults.getRelationships(), Reference(SynonymRefType, pnClause->getPatternSynonym()), pnClause->getEntRef());
}

Void evaluatePattern(PatternClause* pnClause, ResultsTable* resultsTable)
{
    if (pnClause->getStatementType() == AssignPatternType) {
        evaluateAssignPattern(pnClause, resultsTable);
    } else {
        throw std::runtime_error("Unknown PatternClause type in evaluatePattern");
    }
}
