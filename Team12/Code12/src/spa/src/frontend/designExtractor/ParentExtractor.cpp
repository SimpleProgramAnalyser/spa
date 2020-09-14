/**
 * Implementation of Parent extractor.
 */

#include "ParentExtractor.h"

#include <array>
#include <cassert>

#include "ast/AstLibrary.h"
#include "pkb/PKB.h"

// Since only each statement can only have 1 parent, we only need a
// one-dimensional array to store the Parent relationships, where the
// index of the array is the statement number and the value is the
// statement number of its parent. If a statement has no parent, the value
// will default to 0. Index 0 is a buffer.
typedef std::vector<StatementNumber> ParentList;
typedef std::unordered_map<StatementNumber, StatementType> ParentTypeTable;

/**
 * Stores a Parent relationship in an adjacency matrix,
 * and also calls the PKB API to store the relationship
 * in the database. Note that this method is used for
 * Parent without a star.
 *
 * @param parentList The adjacency list to store
 *                      Parent relationships.
 * @param parentTable The map of statement number to
 *                     statement type, for Parent*.
 * @param before Statement number of the before statement.
 * @param beforeStmtType Type of the before statement.
 * @param after Statement number of the after statement.
 * @param afterStmtType Type of the after statement.
 *
 * @return Void.
 */
Void markParentRelationship(ParentList* parentList, ParentTypeTable* parentTable, StatementNumber parent,
                            StatementType parentStmtType, StatementNumber child, StatementType childStmtType)
{
    parentList->at(child) = parent;
    parentTable->insert(std::pair<StatementNumber, StatementType>(parent, parentStmtType));
    parentTable->insert(std::pair<StatementNumber, StatementType>(child, childStmtType));
    // call PKB to add the relationship
    addParentRelationships(parent, parentStmtType, child, childStmtType);
}

// forward declaration for extractFollowsFromContainer
ParentList* extractParentStmtlst(ParentList* parentList, ParentTypeTable* parentTable, const StmtlstNode* stmtLstNode);
// forward declaration for extractChildren
ParentList* extractChildFromContainer(ParentList* parentList, ParentTypeTable* parentTable,
                                      const std::unique_ptr<StatementNode>& containerStmt);
/**
 * Extract Parent relationships from the child statements.
 *
 * @param parentList The adjacency list to store
 *                    Parent relationships.
 * @param parentTable The map of statement number to
 *                     statement type, for Parent*.
 * @param childStatements The list of childStatements.
 * @param numberOfChildStatements The number of childStatements.
 * @param parentStmtType The statement type of the current parent statement.
 * @param parentStmtNumber The statement number of the current parent statement.
 * @return The pointer to the adjacency list,
 *         to allow for chaining of methods.
 */
ParentList* extractChildren(ParentList* parentList, ParentTypeTable* parentTable,
                            const List<StatementNode>& childStatements, size_t numberOfChildStatements,
                            StatementType parentStmtType, StatementNumber parentStmtNumber)
{
    for (size_t i = 0; i < numberOfChildStatements; i++) {
        const std::unique_ptr<StatementNode>& childStmt = childStatements.at(i);
        // mark an edge in parentMatrix: (parentStatement ---> childStatement)
        markParentRelationship(parentList, parentTable, parentStmtNumber, parentStmtType,
                               childStmt->getStatementNumber(), childStmt->getStatementType());
        if (isContainerStatement(childStmt->getStatementType())) {
            extractChildFromContainer(parentList, parentTable, childStmt);
        }
    }
    return parentList;
}

/**
 * Extracts Parent relationships within a certain
 * nesting level, given a reference to the container
 * statement. Container statements can be if or while
 * statements in SIMPLE.
 *
 * @param parentList The adjacency list to store
 *                    Parent relationships.
 * @param parentTable The map of statement number to
 *                     statement type, for Parent*.
 * @param containerStmt The container statement node to traverse.
 * @return The pointer to the adjacency list,
 *         to allow for chaining of methods.
 */
ParentList* extractChildFromContainer(ParentList* parentList, ParentTypeTable* parentTable,
                                      const std::unique_ptr<StatementNode>& containerStmt)
{
    StatementNumber parentStmtNumber = containerStmt->getStatementNumber();
    StatementType parentStmtType = containerStmt->getStatementType();
    if (parentStmtType == IfStatement) {
        // NOLINTNEXTLINE
        auto* ifStatement = static_cast<IfStatementNode*>(containerStmt.get());
        const StmtlstNode* const ifStmtLstNode = ifStatement->ifStatementList;
        const List<StatementNode>& ifStatements = ifStmtLstNode->statementList;
        size_t numberOfIfStatements = ifStatements.size();

        const StmtlstNode* const elseStmtLstNode = ifStatement->elseStatementList;
        const List<StatementNode>& elseStatements = elseStmtLstNode->statementList;
        size_t numberOfElseStatements = elseStatements.size();

        extractChildren(parentList, parentTable, ifStatements, numberOfIfStatements, parentStmtType, parentStmtNumber);
        extractChildren(parentList, parentTable, elseStatements, numberOfElseStatements, parentStmtType,
                        parentStmtNumber);
    } else {
        // NOLINTNEXTLINE
        assert(containerStmt->getStatementType() == WhileStatement);
        // NOLINTNEXTLINE
        auto* whileStatement = static_cast<WhileStatementNode*>(containerStmt.get());
        const StmtlstNode* const whileStmtLstNode = whileStatement->statementList;
        const List<StatementNode>& whileStatements = whileStmtLstNode->statementList;
        size_t numberOfWhileStatements = whileStatements.size();

        extractChildren(parentList, parentTable, whileStatements, numberOfWhileStatements, parentStmtType,
                        parentStmtNumber);
    }
    return parentList;
}

/**
 * Extract Parent relationships from the statement
 * list node of the lowest nesting level (not nested).
 * Statements in a statement list node are within the
 * same nesting level.
 *
 * @param parentList The adjacency list to store
 *                    Parent relationships.
 * @param parentTable The map of statement number to
 *                     statement type, for Parent*.
 * @param stmtLstNode The statement list node to traverse.
 * @return The pointer to the adjacency list,
 *         to allow for chaining of methods.
 */
ParentList* extractParentStmtlst(ParentList* parentList, ParentTypeTable* parentTable,
                                 const StmtlstNode* const stmtLstNode)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    size_t numberOfStatements = statements.size();
    for (size_t i = 0; i < numberOfStatements; i++) {
        const std::unique_ptr<StatementNode>& currentStatement = statements.at(i);
        if (isContainerStatement(currentStatement->getStatementType())) {
            extractChildFromContainer(parentList, parentTable, currentStatement);
        }
    }
    return parentList;
}

/**
 * Identifies Parent relationships in a given program
 * and stores them in a ParentList. This method is
 * exposed solely for unit testing purposes.
 *
 * @param rootNode Root node of an Abstract Syntax Tree.
 * @return Pointer to the ParentList created. This list must
 *         be deleted by the caller of this function!
 */
ParentList* extractParentReturnAdjacencyList(const ProgramNode& rootNode)
{
    const List<ProcedureNode>& procedures = rootNode.procedureList;
    StatementNumber numberOfStatements = rootNode.totalNumberOfStatements;
    size_t numberOfProcedures = procedures.size();

    // initiate the Parent* list for fast lookup
    auto* parentList = new ParentList();
    // initiate Parent table for fast typing of statements
    auto* parentTable = new ParentTypeTable();
    parentList->reserve(numberOfStatements + 1);
    for (StatementNumber i = 0; i < numberOfStatements + 1; i++) {
        // initiate the adjacency list with 0
        // 0 indicates the lack of a Parent relationship
        parentList->push_back(0);
    }

    // loop through procedures to get Parent relationship (no star)
    for (size_t i = 0; i < numberOfProcedures; i++) {
        extractParentStmtlst(parentList, parentTable, procedures.at(i)->statementListNode);
    }

    // loop through ParentList to find Parent* relationships
    for (StatementNumber childStmt = 1; childStmt < numberOfStatements + 1; childStmt++) {
        StatementNumber currentParentStmt = parentList->at(childStmt);
        Vector<std::pair<Integer, StatementType>> seenNodesList;
        // Child has parent
        while (currentParentStmt != 0) {
            // add this edge to the seen nodes
            seenNodesList.push_back(
                std::pair<Integer, StatementType>(currentParentStmt, parentTable->at(currentParentStmt)));
            // travel the edges to the next accessible node
            currentParentStmt = parentList->at(currentParentStmt);
        }
        if (!seenNodesList.empty()) {
            // store all Parent* in PKB
            addParentRelationshipsStar(childStmt, parentTable->at(childStmt), seenNodesList);
        }
    }
    delete parentTable;

    return parentList;
}

Void extractParent(const ProgramNode& rootNode)
{
    ParentList* list = extractParentReturnAdjacencyList(rootNode);
    // handle deletion of the adjacency list in heap
    delete list;
}
