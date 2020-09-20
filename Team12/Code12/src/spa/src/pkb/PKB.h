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
Vector<String> getUsesProcedures(const String& varName);
Vector<String> getUsesVariablesFromStatement(Integer stmt);
Vector<String> getUsesVariablesFromProcedure(const String& procName);
Vector<Integer> getAllUsesStatements(StatementType stmtType);
Vector<String> getAllUsesVariablesFromStatementType(StatementType stmtType);
Vector<String> getAllUsesVariablesFromProgram();
Vector<String> getAllUsesProcedures();
Vector<Pair<Integer, String>> getAllUsesStatementTuple(StatementType stmtType);
Vector<Pair<String, String>> getAllUsesProcedureTuple();

// Modifies
void addModifiesRelationships(Integer stmtNum, StatementType stmtType, Vector<String> varNames);
void addModifiesRelationships(const String& procName, Vector<String> varNames);
Boolean checkIfProcedureModifies(const String& procName, const String& varName);
Boolean checkIfStatementModifies(Integer stmt, const String& varName);
Vector<Integer> getModifiesStatements(const String& varName, StatementType stmtType);
Vector<String> getModifiesProcedures(const String& varName);
Vector<String> getModifiesVariablesFromStatement(Integer stmt);
Vector<String> getModifiesVariablesFromProcedure(const String& procName);
Vector<Integer> getAllModifiesStatements(StatementType stmtType);
Vector<String> getAllModifiesVariablesFromStatementType(StatementType stmtType);
Vector<String> getAllModifiesVariablesFromProgram();
Vector<String> getAllModifiesProcedures();
// TODO: implement
Vector<Pair<Integer, String>> getAllModifiesStatementTuple(StatementType stmtType);
Vector<Pair<String, String>> getAllModifiesProcedureTuple();

// Parent
void addParentRelationships(Integer parent, StatementType parentType, Integer child, StatementType childType);
void addParentRelationshipsStar(Integer parent, StatementType parentType,
                                const Vector<Pair<Integer, StatementType>>& childrenStmttypePairs);
Boolean checkIfParentHolds(Integer parent, Integer child);
Boolean checkIfParentHoldsStar(Integer parent, Integer child);
Vector<StatementNumWithType> getChildStatement(Integer parent);
Vector<StatementNumWithType> getParentStatement(Integer child);
Vector<Integer> getAllChildStatementsStar(Integer parent, StatementType stmtType);
Vector<Integer> getAllParentStatementsStar(Integer child, StatementType stmtType);
Vector<Integer> getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllParentStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Integer> getAllChildStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Pair<Integer, Integer>> getAllParentTuple(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
Vector<Pair<Integer, Integer>> getAllParentTupleStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);

// Follows
void addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after, StatementType afterStmtType);
void addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                 const Vector<Pair<Integer, StatementType>>& afterStmttypePairs);
Boolean checkIfFollowsHolds(Integer before, Integer after);
Boolean checkIfFollowsHoldsStar(Integer before, Integer after);
Vector<StatementNumWithType> getAfterStatement(Integer before);
Vector<StatementNumWithType> getBeforeStatement(Integer after);
Vector<Integer> getAllAfterStatementsStar(Integer before, StatementType stmtType);
Vector<Integer> getAllBeforeStatementsStar(Integer after, StatementType stmtType);
Vector<Integer> getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Integer> getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Pair<Integer, Integer>> getAllFollowsTuple(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
Vector<Pair<Integer, Integer>> getAllFollowsTupleStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);

// Procedure
void insertIntoProcedureTable(const String& procName);
Boolean isProcedureInProgram(String procName);
Vector<String> getAllProcedures();

// Variable
void insertIntoVariableTable(const String& varName);
Boolean isVariableInProgram(String varName);
Vector<String> getAllVariables();

// Statement
void insertIntoStatementTable(Integer stmtNum, StatementType stmtType);
Boolean isStatementInProgram(Integer stmtNum);
Vector<Integer> getAllStatements(StatementType stmtType);

// Constant
void insertIntoConstantTable(Integer constant);
Boolean isConstantInProgram(Integer constant);
Vector<Integer> getAllConstants();

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
    ConstantTable constantTable;
};

#endif // PKB_H