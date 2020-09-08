/**
 * Implementation of Follows extractor.
 */

#include "FollowsExtractor.h"

#include <array>

using Matrix = std::vector<std::vector<std::size_t>>;

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
Matrix* extractFollowsStmtlst(Matrix* followsMatrix, const StmtlstNode* const stmtLstNode) {
    List<StatementNode> statements = stmtLstNode->statementList;
    size_t numberOfStatements = statements.size();
    for (size_t i = 0; i < numberOfStatements; i++) {
        std::unique_ptr<StatementNode>& currentStatement = statements.at(i);
        if (isContainerStatement(currentStatement->getStatementType())) {

        }
    }
    return followsMatrix;
}

Void extractFollows(ProgramNode& rootNode) {
    List<ProcedureNode> procedures = rootNode.procedureList;
    StatementNumber numberOfStatements = rootNode.totalNumberOfStatements;
    size_t numberOfProcedures = procedures.size();

    // initiate the Follows* matrix for fast lookup
    Matrix adjacencyMatrix;
    for (size_t i = 0; i < numberOfStatements; i++) {
        adjacencyMatrix.push_back(std::vector<size_t>(numberOfStatements));
    }

    // loop through procedures
    for (size_t i = 0; i < numberOfProcedures; i++) {
        extractFollowsStmtlst(&adjacencyMatrix, procedures.at(i)->statementListNode);
    }

}
