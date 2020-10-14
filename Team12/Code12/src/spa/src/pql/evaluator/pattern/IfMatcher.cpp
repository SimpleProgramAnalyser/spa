/**
 * Implementation of If Matcher for Query Evaluator,
 * used to evaluate if pattern clauses.
 */

#include "IfMatcher.h"

PatternMatcherTuple matchIfStatement(IfStatementNode* ifNode, PatternClause* pnClause, StatementNumber stmtNumber)
{
    Reference controlVariableRef = pnClause->getEntRef();
    ReferenceType refType = controlVariableRef.getReferenceType();
    PatternMatcherTuple results;

    assert(refType == WildcardRefType || refType == LiteralRefType || refType == SynonymRefType);

    if (refType == WildcardRefType) {
        results.addTargetStatement(stmtNumber);
    } else {
        std::unordered_set<String> variables;
        getLiteralVariablesInCondExpr(ifNode->predicate, variables);

        if (refType == LiteralRefType) {
            String literalVariable = controlVariableRef.getValue();
            auto gotLiteralVariable = variables.find(literalVariable);

            if (gotLiteralVariable == variables.end()) {
                return results;
            }

            results.addTargetStatement(stmtNumber);
        } else {
            // SynonymRefType
            results.addTargetStatement(stmtNumber, variables);
        }
    }

    return results;
}

PatternMatcherTuple findIfInStatementList(const StmtlstNode* const stmtLstNode, PatternClause* pnClause)
{
    const List<StatementNode>& statements = stmtLstNode->statementList;
    PatternMatcherTuple results;
    for (const std::unique_ptr<StatementNode>& currStmt : statements) {
        StatementType stmtType = currStmt->getStatementType();
        if (stmtType == IfStatement) {
            // NOLINTNEXTLINE
            auto* ifNode = static_cast<IfStatementNode*>(currStmt.get());
            StatementNumber stmtNumber = currStmt->getStatementNumber();
            PatternMatcherTuple resultsFromIf = matchIfStatement(ifNode, pnClause, stmtNumber);
            results.concatTuple(resultsFromIf);

            PatternMatcherTuple resultsFromNestedIf = findIfInStatementList(ifNode->ifStatementList, pnClause);
            PatternMatcherTuple resultsFromNestedElse = findIfInStatementList(ifNode->elseStatementList, pnClause);
            results.concatTuple(resultsFromNestedIf);
            results.concatTuple(resultsFromNestedElse);
        } else if (stmtType == WhileStatement) {
            // NOLINTNEXTLINE
            auto* nestedWhileNode = static_cast<WhileStatementNode*>(currStmt.get());
            PatternMatcherTuple resultsFromNestedWhile = findIfInStatementList(nestedWhileNode->statementList, pnClause);
            results.concatTuple(resultsFromNestedWhile);
        }
    }

    return results;
}

Void evaluateIfPattern(PatternClause* pnClause, ResultsTable* resultsTable) {
    // get all results from AST
    ProgramNode* ast = getRootNode();
    const List<ProcedureNode>& procedureList = ast->procedureList;
    PatternMatcherTuple allResults;
    for (const std::unique_ptr<ProcedureNode>& proc : procedureList) {
        PatternMatcherTuple resultsFromProcedure = findIfInStatementList(proc->statementListNode, pnClause);
        allResults.concatTuple(resultsFromProcedure);
    }

    // store results in ResultTable
    ReferenceType controlVariableRefType = pnClause->getEntRef().getReferenceType();
    switch (controlVariableRefType) {
        case WildcardRefType:
        case LiteralRefType:
            resultsTable->storeResultsOne(pnClause->getPatternSynonym(), allResults.getTargetStatements());
            break;
        case SynonymRefType:
            resultsTable->storeResultsTwo(pnClause->getPatternSynonym(), allResults.getTargetStatements(),
                                          pnClause->getEntRef(), convertToPairedResult(allResults.getRelationships()));
            break;
        default:
            throw std::runtime_error("Unknown or invalid reference type in evaluateIfPattern");
    }
}
