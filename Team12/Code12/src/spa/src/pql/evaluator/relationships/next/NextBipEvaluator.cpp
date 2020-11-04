/**
 * Implementation of an evaluator for a such that clause that
 * has NextBip and NextBip* relationships between design entities.
 */

#include "NextBipEvaluator.h"

Void NextBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    DesignEntityType rightSynonymType = rightRef.isWildCard() ? StmtType : rightRef.getDesignEntity().getType();
    CacheSet nextStarBipAnyStmtResults = NextEvaluator::getCacheNextStatement(leftRefVal);
    Vector<StatementNumber> filteredList
        = nextStarBipAnyStmtResults.filterStatementType(mapToStatementType(rightSynonymType)).toList();

    ProcedureName leftRefContainingProcName = facade->getProcedureOfStmt(leftRefVal);
    if (leftRefContainingProcName.empty()) {
        resultsTable.storeResultsZero(false);
        return;
    }

    Vector<StatementNumber> finalValidResults = filterValidResults(leftRefContainingProcName, true, filteredList);
    ClauseResult results = convertToClauseResult(finalValidResults);
    resultsTable.storeResultsOne(rightRef, results);
}

Void NextBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    DesignEntityType rightSynonymType = leftRef.isWildCard() ? StmtType : leftRef.getDesignEntity().getType();
    CacheSet prevStarBipAnyStmtResults = NextEvaluator::getCachePrevStatement(rightRefVal);
    Vector<StatementNumber> filteredList
        = prevStarBipAnyStmtResults.filterStatementType(mapToStatementType(rightSynonymType)).toList();

    ProcedureName rightRefContainingProcName = facade->getProcedureOfStmt(rightRefVal);
    if (rightRefContainingProcName.empty()) {
        resultsTable.storeResultsZero(false);
        return;
    }

    Vector<StatementNumber> finalValidResults = filterValidResults(rightRefContainingProcName, false, filteredList);
    ClauseResult results = convertToClauseResult(finalValidResults);
    resultsTable.storeResultsOne(leftRef, results);
}

Void NextBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    // TODO
    NextEvaluator::evaluateBothAnyStar(leftRef, rightRef);
}

Void NextBipEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    Vector<StatementNumber> nextStarBipAnyStmtResults = NextEvaluator::getCacheNextStatement(leftRefVal).toList();
    ProcedureName leftRefContainingProcName = facade->getProcedureOfStmt(leftRefVal);
    if (leftRefContainingProcName.empty()) {
        resultsTable.storeResultsZero(false);
        return;
    }
    Vector<StatementNumber> filteredValidResults
        = filterValidResults(leftRefContainingProcName, true, nextStarBipAnyStmtResults);

    Boolean isTrue = std::find(filteredValidResults.begin(), filteredValidResults.end(), rightRefVal)
                     != filteredValidResults.end();

    return resultsTable.storeResultsZero(isTrue);
}

NextBipEvaluator::NextBipEvaluator(ResultsTable& resultsTable, NextEvaluatorFacade* facade):
    NextEvaluator(resultsTable, facade)
{}

Vector<StatementNumber> NextBipEvaluator::filterValidResults(ProcedureName procName, Boolean isCaller,
                                                             Vector<StatementNumber> list)
{
    Vector<StatementNumber> validResults;

    for (StatementNumber i : list) {
        ProcedureName otherProcName = facade->getProcedureOfStmt(i);
        if (procName == otherProcName) {
            validResults.push_back(i);
        } else if (isCaller && facade->checksIfCallsStarHolds(procName, otherProcName)) {
            validResults.push_back(i);
        } else if (!isCaller && facade->checksIfCallsStarHolds(otherProcName, procName)) {
            validResults.push_back(i);
        }
    }

    return validResults;
}