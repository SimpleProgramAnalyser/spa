/**
 * Implementation of Follows extractor.
 */

#include "FollowsExtractor.h"

#include <array>
#include <cassert>

#include "ast/AstLibrary.h"
#include "pkb/relationships/Follows.h"

// Since only one statement can Follow another, we only need a
// one-dimensional array to store the Follows relationships
using FollowsList = std::vector<StatementNumber>;

/**
 * Stores a Follows relationship in an adjacency matrix,
 * and also calls the PKB API to store the relationship
 * in the database. Note that this method is used for
 * Follows without a star.
 *
 * @param followsList The adjacency list to store
 *                      Follows relationships.
 * @param before Statement number of the before statement.
 * @param after Statement number of the after statement.
 * @return Void.
 */
Void markFollowsRelationship(FollowsList* followsList, StatementNumber before, StatementNumber after)
{
    followsList->at(before) = after;
    // call PKB to add the relationship
    addFollowsRelationships(before, after);
}

// forward declaration for extractFollowsFromContainer
FollowsList* extractFollowsStmtlst(FollowsList* followsList, const StmtlstNode* stmtLstNode);

/**
 * Extracts Follows relationships within a certain
 * nesting level, given a reference to the container
 * statement. Container statements can be if or while
 * statements in SIMPLE.
 *
 * @param followsList The adjacency list to store
 *                    Follows relationships.
 * @param containerStmt The container statement node to traverse.
 * @return The pointer to the adjacency list,
 *         to allow for chaining of methods.
 */
FollowsList* extractFollowsFromContainer(FollowsList* followsList, const std::unique_ptr<StatementNode>& containerStmt)
{
    if (containerStmt->getStatementType() == IfStatement) {
        // NOLINTNEXTLINE
        auto* ifStatement = static_cast<IfStatementNode*>(containerStmt.get());
        extractFollowsStmtlst(followsList, ifStatement->ifStatementList);
        extractFollowsStmtlst(followsList, ifStatement->elseStatementList);
    } else {
        // NOLINTNEXTLINE
        assert(containerStmt->getStatementType() == WhileStatement);
        // NOLINTNEXTLINE
        auto* whileStatement = static_cast<WhileStatementNode*>(containerStmt.get());
        extractFollowsStmtlst(followsList, whileStatement->statementList);
    }
    return followsList;
}

/**
 * Extract Follows relationships from a statement
 * list node. Statements in a statement list node
 * are within the same nesting level.
 *
 * @param followsList The adjacency list to store
 *                    Follows relationships.
 * @param stmtLstNode The statement list node to traverse.
 * @return The pointer to the adjacency list,
 *         to allow for chaining of methods.
 */
FollowsList* extractFollowsStmtlst(FollowsList* followsList, const StmtlstNode* const stmtLstNode)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    size_t numberOfStatements = statements.size();
    for (size_t i = 0; i < numberOfStatements - 1 /* last statement cannot follow another */; i++) {
        const std::unique_ptr<StatementNode>& currentStatement = statements.at(i);
        const std::unique_ptr<StatementNode>& nextStatement = statements.at(i + 1);
        // mark an edge in followsMatrix: (currentStatement ---> nextStatement)
        markFollowsRelationship(followsList, currentStatement->getStatementNumber(),
                                nextStatement->getStatementNumber());

        if (isContainerStatement(currentStatement->getStatementType())) {
            extractFollowsFromContainer(followsList, currentStatement);
        }
    }
    return followsList;
}

/**
 * Identifies Follows relationships in a given program
 * and stores them in a FollowsList. This method is
 * exposed solely for unit testing purposes.
 *
 * @param rootNode Root node of an Abstract Syntax Tree.
 * @return Pointer to the FollowsList created. This list must
 *         be deleted by the caller of this function!
 */
FollowsList* extractFollowsReturnAdjacencyList(const ProgramNode& rootNode)
{
    const List<ProcedureNode>& procedures = rootNode.procedureList;
    StatementNumber numberOfStatements = rootNode.totalNumberOfStatements;
    size_t numberOfProcedures = procedures.size();

    // initiate the Follows* list for fast lookup
    auto* followsList = new FollowsList();
    followsList->reserve(numberOfStatements + 1);
    for (StatementNumber i = 0; i < numberOfStatements + 1; i++) {
        // initiate the adjacency list with 0
        // 0 indicates the lack of a Follows relationship
        followsList->push_back(0);
    }

    // loop through procedures to get Follows relationship (no star)
    for (size_t i = 0; i < numberOfProcedures; i++) {
        extractFollowsStmtlst(followsList, procedures.at(i)->statementListNode);
    }

    // loop through FollowsList to find Follows* relationships
    for (StatementNumber beforeStmt = 0; beforeStmt < numberOfStatements; beforeStmt++) {
        StatementNumber currentAfterStmt = followsList->at(beforeStmt);
        Vector<Integer> seenNodesList;
        while (currentAfterStmt != 0) {
            // add this edge to the seen nodes
            seenNodesList.push_back(currentAfterStmt);
            // travel the edges to the next accessible node
            currentAfterStmt = followsList->at(currentAfterStmt);
        }
        // store all Follows* in PKB
        addFollowsRelationshipsStar(beforeStmt, seenNodesList);
    }

    return followsList;
}

Void extractFollows(const ProgramNode& rootNode)
{
    FollowsList* list = extractFollowsReturnAdjacencyList(rootNode);
    // handle deletion of the adjacency list in heap
    delete list;
}
