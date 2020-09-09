/**
 * Implementation of Follows extractor.
 */

#include "FollowsExtractor.h"

#include <array>
#include <cassert>

#include "ast/AstLibrary.h"
#include "pkb/relationships/Follows.h"

using Matrix = std::vector<std::vector<bool>>;

/**
 * Stores a Follow relationship in an adjacency matrix,
 * and also calls the PKB API to store the relationship
 * in the database. Note that this method is used for
 * Follows without a star.
 *
 * @param followsMatrix The adjacency matrix to store
 *                      Follows relationships.
 * @param before Statement number of the before statement.
 * @param after Statement number of the after statement.
 * @return Void.
 */
Void markFollowsRelationship(Matrix* followsMatrix, StatementNumber before, StatementNumber after)
{
    followsMatrix->at(before).at(after) = true;
    // call PKB to add the relationship
    addFollowsRelationships(before, after);
}

// forward declaration for extractFollowsFromContainer
Matrix* extractFollowsStmtlst(Matrix* followsMatrix, const StmtlstNode* stmtLstNode);

/**
 * Extracts Follows relationships within a certain
 * nesting level, given a reference to the container
 * statement. Container statements can be if or while
 * statements in SIMPLE.
 *
 * @param followsMatrix The adjacency matrix to store
 *                      Follows relationships.
 * @param containerStmt The container statement node to traverse.
 * @return The pointer to the adjacency matrix, to allow
 *         for chaining of methods.
 */
Matrix* extractFollowsFromContainer(Matrix* followsMatrix, const std::unique_ptr<StatementNode>& containerStmt)
{
    if (containerStmt->getStatementType() == IfStatement) {
        // NOLINTNEXTLINE
        auto* ifStatement = static_cast<IfStatementNode*>(containerStmt.get());
        extractFollowsStmtlst(followsMatrix, ifStatement->ifStatementList);
        extractFollowsStmtlst(followsMatrix, ifStatement->elseStatementList);
    } else {
        assert(containerStmt->getStatementType() == WhileStatement);
        // NOLINTNEXTLINE
        auto* whileStatement = static_cast<WhileStatementNode*>(containerStmt.get());
        extractFollowsStmtlst(followsMatrix, whileStatement->statementList);
    }
    return followsMatrix;
}

/**
 * Extract Follows relationships from a statement
 * list node. Statements in a statement list node
 * are within the same nesting level.
 *
 * @param followsMatrix The adjacency matrix to store
 *                      Follows relationships.
 * @param stmtLstNode The statement list node to traverse.
 * @return The pointer to the adjacency matrix, to allow
 *         for chaining of methods.
 */
Matrix* extractFollowsStmtlst(Matrix* followsMatrix, const StmtlstNode* const stmtLstNode)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    size_t numberOfStatements = statements.size();
    for (size_t i = 0; i < numberOfStatements - 1 /* last statement cannot follow another */; i++) {
        const std::unique_ptr<StatementNode>& currentStatement = statements.at(i);
        const std::unique_ptr<StatementNode>& nextStatement = statements.at(i + 1);
        // mark an edge in followsMatrix: (currentStatement ---> nextStatement)
        markFollowsRelationship(followsMatrix, currentStatement->getStatementNumber(),
                                nextStatement->getStatementNumber());

        if (isContainerStatement(currentStatement->getStatementType())) {
            extractFollowsFromContainer(followsMatrix, currentStatement);
        }
    }
    return followsMatrix;
}

Void extractFollows(ProgramNode& rootNode)
{
    const List<ProcedureNode>& procedures = rootNode.procedureList;
    StatementNumber numberOfStatements = rootNode.totalNumberOfStatements;
    size_t numberOfProcedures = procedures.size();

    // initiate the Follows* matrix for fast lookup
    Matrix adjacencyMatrix;
    adjacencyMatrix.reserve(numberOfStatements);
    for (size_t i = 0; i < numberOfStatements; i++) {
        adjacencyMatrix.push_back(std::vector<bool>(numberOfStatements));
    }

    // loop through procedures to get Follows relationship (no star)
    for (size_t i = 0; i < numberOfProcedures; i++) {
        extractFollowsStmtlst(&adjacencyMatrix, procedures.at(i)->statementListNode);
    }

    // TODO: Follows*
}
