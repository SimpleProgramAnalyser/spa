#ifndef PKB_H
#define PKB_H

#include <pkb/PkbTypes.h>
#include <pkb/relationships/Follows.h>
#include <pkb/relationships/Modifies.h>
#include <pkb/relationships/Parent.h>
#include <pkb/relationships/Uses.h>
#include <pkb/tables/Tables.h>

/**
 * Holds API methods for the Program Knowledge Base (PKB).
 */

void addUsesRelationships(Integer stmtNum, StatementType stmtType, const Vector<String>& varNames);
void addUsesRelationships(const String& procName, const Vector<String>& varNames);
Boolean checkIfProcedureUses(const String& procName, const String& varName);
Boolean checkIfStatementUses(Integer stmt, const String& varName);
Vector<Integer> getUsesStatements(const String& varName, StatementType stmtType);
Vector<String> getUsesProcedures(const String& procName);
Vector<String> getUsesVariablesFromStatement(Integer stmt);
Vector<String> getUsesVariablesFromProcedure(const String& procName);
Vector<Integer> getAllUsesStatements(StatementType stmtType);
Vector<String> getAllUsesVariables(StatementType stmtType);
Vector<String> getAllUsesVariables(const String& procName);
Vector<String> getAllUsesProcedures();

// Modifies
void addModifiesRelationships(Integer stmtNum, Vector<String> varNames);
void addModifiesRelationships(String procName, Vector<String> varNames);
String checkIfProcedureModifies(String procName, String varName);
Integer checkIfStatementModifies(Integer stmt, String varName);
Vector<Integer> getModifiesStatements(String varName, StatementType stmtType);
Vector<String> getModifiesProcedures(String procName);
Vector<String> getModifiesVariablesFromStatement(Integer stmt);
Vector<String> getModifiesVariablesFromProcedure(String procName);
Vector<Integer> getAllModifiesStatements(StatementType stmtType);
Vector<String> getAllModifiesVariables(StatementType stmtType);
Vector<String> getAllModifiesVariables(String procName);
Vector<String> getAllModifiesProcedures();

// Parent
void addParentRelationships(Integer parent, Integer child);
void addParentRelationshipsStar(Integer parent, Vector<Integer> children);
Boolean checkIfParentHolds(Integer parent, Integer child);
Boolean checkIfParentHoldsStar(Integer parent, Integer child);
Vector<Integer> getAllChildStatements(Integer parent, StatementType stmtType);
Vector<Integer> getAllParentStatements(Integer parent, StatementType stmtType);
Vector<Integer> getAllChildStatementsStar(Integer parent, StatementType stmtType);
Vector<Integer> getAllParentStatementsStar(Integer parent, StatementType stmtType);
Vector<Integer> getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllParentStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllChildStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);

// Follows
void addFollowsRelationships(Integer parent, Integer child);
void addFollowsRelationshipsStar(Integer parent, Vector<Integer> children);
Boolean checkIfFollowsHolds(Integer parent, Integer child);
Boolean checkIfFollowsHoldsStar(Integer parent, Integer child);
Vector<Integer> getAllAfterStatements(Integer parent, StatementType stmtType);
Vector<Integer> getAllBeforeStatements(Integer parent, StatementType stmtType);
Vector<Integer> getAllAfterStatementsStar(Integer parent, StatementType stmtType);
Vector<Integer> getAllBeforeStatementsStar(Integer parent, StatementType stmtType);
Vector<Integer> getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);

// Procedure
void insertIntoProcedureTable(String procName);
Vector<String> getAllProcedures();
Boolean getProcedureFromName(String procName);

// Variable
void insertIntoVariableTable(String varName);
Boolean getVariableFromName(String varName);
Vector<String> getAllVariables();

// Statement
void insertIntoStatementTable(StatementNode* stmtNode, Integer lineNumber);
Boolean getStatementFromIndex(Integer indexOfStmt);
Vector<Integer> getAllStatements(StatementType stmtType);

// Root node
void assignRootNode(ProgramNode* rootNodeToAssign);
ProgramNode* getRootNode();


class PKB {
public:
    ProgramNode* rootNode{};
    StatementTable statementTable;
    VariableTable variableTable;
    ProcedureTable procedureTable;
    FollowsTable followsTable;
    ParentTable parentTable;
    UsesTable usesTable;
    ModifiesTable modifiesTable;
};

#endif // PKB_H