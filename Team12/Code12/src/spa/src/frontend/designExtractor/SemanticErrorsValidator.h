/**
 * Concrete API for SPA frontend DesignExtractor.
 * The design extractor traverses tha Abtract Syntax
 * Tree in the Knowledge Base, extracts relationships
 * and stores them in the Knowledge Base and checks
 * for semantic errors.
 */

#ifndef SPA_FRONTEND_SEMANTIC_ERRORS_VALIDATOR_H
#define SPA_FRONTEND_SEMANTIC_ERRORS_VALIDATOR_H

#include <unordered_map>
#include <utility>

#include "ast/AstTypes.h"
#include "error/InputError.h"

class SemanticErrorsValidator {
public:
    ProgramNode& programNode;

    SemanticErrorsValidator(ProgramNode& progNode);
    Boolean isProgramValid();

    Void populateCallGraphWithStatementNode(StatementNode* stmtNode,
                                            std::unordered_map<std::string, int>& procedureNameSet,
                                            std::vector<std::vector<int>>& adjList, int currProcNameIndex);
    Void populateCallGraphWithIfStatement(IfStatementNode* stmtNode,
                                          std::unordered_map<std::string, int>& procedureNameSet,
                                          std::vector<std::vector<int>>& adjList, int currProcNameIndex);
    Void populateCallGraphWithWhileStatement(WhileStatementNode* stmtNode,
                                             std::unordered_map<std::string, int>& procedureNameSet,
                                             std::vector<std::vector<int>>& adjList, int currProcNameIndex);
    Void addCallEdge(CallStatementNode* stmtNode, std::unordered_map<std::string, int>& procedureNameSet,
                     std::vector<std::vector<int>>& adjList, int currProcNameIndex);
    Boolean checkCallStatementCallsValidProcedure(StatementNode* stmtNode,
                                                  std::unordered_map<std::string, int>& procedureNameSet,
                                                  int currProcNameIndex);
    int getProcNameIndex(std::unordered_map<std::string, int> procedureNameSet, std::string procName);
    bool isCyclic(std::vector<std::vector<int>>& adjList, size_t procListSize);
    bool isCyclicUtil(int v, bool visited[], bool* recStack, std::vector<std::vector<int>>& adjList);
};

#endif // SPA_FRONTEND_SEMANTIC_ERROS_VALIDATOR_H