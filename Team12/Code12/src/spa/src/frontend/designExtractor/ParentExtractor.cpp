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

// forward declaration for extractChildFromContainer
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
 * Helper method to join two or three Vector<StatementNumWithType>. 
 * Note that order of the elements is not preserved.
 *
 * @param v1 The first Vector<StatementNumWithType>.
 * @param v2 The second Vector<StatementNumWithType>.
 * @param v3 The third Vector<StatementNumWithType>.
 * @return A Vector<StatementNumWithType> containing
 *         elements from v1, v2 and v3 (if any).
 */
Vector<StatementNumWithType> concatenateStmtNumWithTypeVectors(const Vector<StatementNumWithType>& v1,
                                                               const Vector<StatementNumWithType>& v2,
                                  const Vector<StatementNumWithType>& v3 = Vector<StatementNumWithType>())
{
    Vector<StatementNumWithType> uniqueSet;
    uniqueSet.reserve(v1.size() + v2.size());
    if (!v1.empty()) {
        for (std::pair<Integer, StatementType> var1 : v1) {
            uniqueSet.push_back(var1);
        }
    }
    
    if (!v2.empty()) {
        for (std::pair<Integer, StatementType> var2 : v2) {
            uniqueSet.push_back(var2);
        }
    }

    if (!v3.empty()) {
        for (std::pair<Integer, StatementType> var3 : v3) {
            uniqueSet.push_back(var3);
        }
    }
    return uniqueSet;
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

    // initialise parentNodesList to store Parent* relationship
    Vector<Vector<std::pair<Integer, StatementType>>> parentNodesList;
    parentNodesList.reserve(numberOfStatements + 1);
    for (StatementNumber i = 0; i < numberOfStatements + 1; i++) {
        parentNodesList.push_back(Vector<std::pair<Integer, StatementType>>());
    }
    
    // loop through ParentList to find Parent* relationship
    for (StatementNumber childStmt = numberOfStatements; 0 < childStmt; childStmt--) {
        StatementNumber currentParentStmt = parentList->at(childStmt);

        Vector<std::pair<Integer, StatementType>> childNodeList;
        // Child has parent
        if (currentParentStmt != 0) {
            // add edge to seenNodesList 
            childNodeList.push_back(std::pair<Integer, StatementType>(childStmt, parentTable->at(childStmt)));

            // Add child node & add child nodes of child to parentNodesList
            parentNodesList.at(currentParentStmt) = concatenateStmtNumWithTypeVectors(
                childNodeList, parentNodesList.at(childStmt), parentNodesList.at(currentParentStmt));
        }
    }

    // Go through parentNodesList and store all Parent* in PKB
    for (StatementNumber i = 1; i < numberOfStatements + 1; i++) {
        if (!parentNodesList.at(i).empty()) {
            addParentRelationshipsStar(i, parentTable->at(i), parentNodesList.at(i));
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
