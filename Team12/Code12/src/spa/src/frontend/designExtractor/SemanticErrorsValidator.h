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

typedef std::vector<std::vector<bool>> Matrix;

class SemanticErrorsValidator {
public:
    /**
     * Graph of calls, stored as an adjacency matrix
     * using index of a procedure as a node number
     *
     * Rows and columns are guaranteed to have a size
     * equal to the number of procedures in the program.
     *
     * If procedure a calls procedure b, this is stored
     * in the matrix as
     * adjacencyMatrixOfCalls.at(a).at(b) = true, and
     * if a does not call b, the value will default to false.
     */
    Matrix adjacencyMatrixOfCalls;

    explicit SemanticErrorsValidator(ProgramNode& progNode);

    /**
     * The main method of Semantic Errors Validator,
     * checks the program for semantic errors like
     * cyclic calls, calls to non-existent procedures
     * and duplicate procedure names.
     *
     * Also populates the Calls graph when
     * it is run for the first time.
     *
     * @return True, if program is semantically valid.
     */
    Boolean isProgramValid();

    /**
     * Sorts the Calls graph by reverse topological order
     * (edges or calls will point backward, so the
     * first procedure in the sorted list would not
     * call any other procedure).
     *
     * This is possible if the program is valid, since
     * there would be no cyclic calls, the Calls graph
     * would be a directed acyclic graph.
     *
     * If isProgramValid has not been run, or the
     * program is semantically invalid, this will
     * return an empty std::vector.
     *
     * @return List of procedure indexes in reverse
     *         topological order (least calls to most).
     */
    std::vector<int> reverseTopologicalSort();

private:
    ProgramNode& programNode;
    // Ensure that Semantic Errors Validator only runs once
    // for a given program
    Boolean hasExecutedValidityChecks;
    // Cache the result of isProgramValid upon first run
    Boolean programValidity;

    Boolean checkForAllSemanticErrors();
    Void populateCallGraphWithStatementNode(StatementNode* stmtNode,
                                            std::unordered_map<std::string, int>& procedureNameSet,
                                            int currProcNameIndex);
    Void populateCallGraphWithIfStatement(IfStatementNode* stmtNode,
                                          std::unordered_map<std::string, int>& procedureNameSet,
                                          int currProcNameIndex);
    Void populateCallGraphWithWhileStatement(WhileStatementNode* stmtNode,
                                             std::unordered_map<std::string, int>& procedureNameSet,
                                             int currProcNameIndex);
    Void addCallEdge(CallStatementNode* stmtNode, std::unordered_map<std::string, int>& procedureNameSet,
                     int currProcNameIndex);
    Boolean checkCallStatementCallsValidProcedure(StatementNode* stmtNode,
                                                  std::unordered_map<std::string, int>& procedureNameSet,
                                                  int currProcNameIndex);
    static int getProcNameIndex(std::unordered_map<std::string, int> procedureNameSet, const std::string& procName);
    bool isCyclic(size_t procListSize);
    bool isCyclicUtil(int v, bool visited[], bool* recStack);
};

#endif // SPA_FRONTEND_SEMANTIC_ERRORS_VALIDATOR_H