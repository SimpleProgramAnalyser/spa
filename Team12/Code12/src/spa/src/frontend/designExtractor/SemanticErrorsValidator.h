/**
 * Concrete API for SPA frontend DesignExtractor.
 * The design extractor traverses tha Abtract Syntax
 * Tree in the Knowledge Base, extracts relationships
 * and stores them in the Knowledge Base and checks
 * for semantic errors.
 */

#ifndef SPA_FRONTEND_SEMANTIC_ERRORS_VALIDATOR_H
#define SPA_FRONTEND_SEMANTIC_ERRORS_VALIDATOR_H

#include <utility>

#include "ast/AstTypes.h"
#include "error/InputError.h"

class SemanticErrorsValidator {
public:
    ProgramNode& programNode;

    SemanticErrorsValidator(ProgramNode& progNode);
    Boolean checkProgramValidity();
    Boolean checkWhileStatementValidity(WhileStatementNode* stmtNode);
    Boolean checkAssignmentStatementValidity(AssignmentStatementNode* stmtNode);
    Boolean checkIfStatementValidity(IfStatementNode* stmtNode);
    Boolean checkCallStatementValidity(CallStatementNode* stmtNode);
    Boolean checkReadStatementValidity(ReadStatementNode* stmtNode);
    Boolean checkPrintStatementValidity(PrintStatementNode* stmtNode);

    Boolean checkStatementValidity(StatementNode* stmtNode);
    Boolean checkExpressionValidity(const Expression* expression);
};

#endif // SPA_FRONTEND_SEMANTIC_ERROS_VALIDATOR_H