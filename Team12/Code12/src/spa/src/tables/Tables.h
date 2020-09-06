/**
 * Functions related to tables.
 */

#ifndef SPA_TABLES_H
#define SPA_TABLES_H

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../ast/AstTypes.h"
#include "Types.h"

class RootNode {
public:
    RootNode();

    ProgramNode* rootNode;
    void assignRootNode(ProgramNode* rootNodeToAssign);
    ProgramNode* getRootNode();
};

class ProcedureTable {
public:
    std::unordered_set<String> setOfProceduresNames; // getAllProc
    std::vector<String> listOfProcedureNames;        // getProcName

    ProcedureTable();

    void insertIntoProcedureTable(String procName);
    std::vector<String> getAllProcedures();
    Boolean getProcedureFromName(String procName);
};

class VariableTable {
public:
    std::vector<String> listOfVariables;       // getAllVar
    std::unordered_set<String> setOfVariables; // getVarFromName

    VariableTable();

    void insertIntoVariableTable(String varName);
    Boolean getVariableFromName(String varName);
    std::vector<String> getAllVariables();
};

class StatementTable {
public:
    std::unordered_map<Integer, std::vector<Integer>> listOfAllStatement; // getAllStmt
    std::unordered_set<Integer> setOfStatements;                          // getFromIndex
    StatementTable();

    void insertIntoStatementTable(StatementNode* stmtNode, Integer lineNumber);
    Boolean getStatementFromIndex(Integer indexOfStmt);
    std::vector<Integer> getAllStatements(StatementType stmtType);
};

class UsesTable {
public:
    UsesTable();

    void addUsesRelationshipsStatement(Integer usesStatement, List<String> varList);
    void addUsesRelationshipsProcedure(String usesProcedure, List<String> varList);
    Boolean checkIfStatementUses(Integer usesStatement, String varName);
    Boolean checkifProcedureUses(String usesProcedure, String varName);
    List<Integer> getUsesStatements(String varName, StatementType stmtType);
    List<Integer> getUsesProcedures(String varName, StatementType stmtType);
    List<String> getUsesVariablesFromStatements(Integer usesStatement);
    List<String> getUsesVariablesFromProcedures(String usesProcedure);
    List<Integer> getAllUsesStatements(StatementType stmtType);
    List<String> getAllUsesProcedures();
    List<String> getAllUsesVariables(StatementType stmtType);
    List<String> getAllUsesVariables(String procName);
};

class ModifiesTable {
public:
    ModifiesTable();

    void addModifiesRelationshipStatement(Integer modifiesStatement, List<String> varList);
    void addUsesRelationshipsProcedure(String modifiesProcedure, List<String> varList);
    Boolean checkIfStatementModifies(Integer modifiesStatement, String varName);
    Boolean checkifProcedureModifies(String modifiesProcedure, String varName);
    List<Integer> getModifiesStatements(String varName, StatementType stmtType);
    List<Integer> getModifiesProcedures(String varName, StatementType stmtType);
    List<String> getVariablesModifiedByStatement(Integer modifiesStatement);
    List<String> getVariablesModifiedByProcedure(String modifiesProcedure);
    List<Integer> getAllModifiesStatements(StatementType stmtType);
    List<String> getAllModifiesProcedures();
    List<String> getAllModifiesVariables(StatementType stmtType);
    List<String> getAllModifiesVariables(String procName);
};
#endif
