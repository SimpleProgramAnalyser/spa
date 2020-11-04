/**
 * Implementation of an evaluator for a such that clause that
 * has NextBip and NextBip* relationships between design entities.
 */

#include "NextBipEvaluator.h"
#include "pkb/tables/Tables.h"
#include "pkb/PKB.h"
#include "ast/AstTypes.h"
#include "pql/evaluator/relationships/affects/AffectsBipEvaluator.h"

Void NextBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{

//    ProcedureName leftProcedure = facade->getProcedureOfStmt(leftRefVal);
    // for all procedure's CFGBip
    // Get starting node
    // Traverse from starting node
        // Mark each *CFG node* as visited
        // Get all nextbip statement
        // Recurse on nextbip statement



//    DesignEntityType rightSynonymType = rightRef.isWildCard() ? StmtType : rightRef.getDesignEntity().getType();
//    CacheSet nextStarBipAnyStmtResults = NextBipEvaluator::getCacheNextStatement(leftRefVal);
//    Vector<StatementNumber> filteredList
//        = nextStarBipAnyStmtResults.filterStatementType(mapToStatementType(rightSynonymType)).toList();
//
//    ProcedureName leftRefContainingProcName = facade->getProcedureOfStmt(leftRefVal);
//    if (leftRefContainingProcName.empty()) {
//        resultsTable.storeResultsZero(false);
//        return;
//    }
//
//    Vector<StatementNumber> finalValidResults = filterValidResults(leftRefContainingProcName, true, filteredList);
//    ClauseResult results = convertToClauseResult(finalValidResults);
//    resultsTable.storeResultsOne(rightRef, results);
}

Void NextBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
//    DesignEntityType rightSynonymType = leftRef.isWildCard() ? StmtType : leftRef.getDesignEntity().getType();
//    CacheSet prevStarBipAnyStmtResults = NextEvaluator::getCachePrevStatement(rightRefVal);
//    Vector<StatementNumber> filteredList
//        = prevStarBipAnyStmtResults.filterStatementType(mapToStatementType(rightSynonymType)).toList();
//
//    ProcedureName rightRefContainingProcName = facade->getProcedureOfStmt(rightRefVal);
//    if (rightRefContainingProcName.empty()) {
//        resultsTable.storeResultsZero(false);
//        return;
//    }
//
//    Vector<StatementNumber> finalValidResults = filterValidResults(rightRefContainingProcName, false, filteredList);
//    ClauseResult results = convertToClauseResult(finalValidResults);
//    resultsTable.storeResultsOne(leftRef, results);
}

Void NextBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
//    StatementType prevRefStmtType
//        = leftRef.isWildCard() ? AnyStatement : mapToStatementType(leftRef.getDesignEntity().getType());
//    StatementType nextRefStmtType
//        = rightRef.isWildCard() ? AnyStatement : mapToStatementType(rightRef.getDesignEntity().getType());
//
//    if (leftRef.isWildCard() && rightRef.isWildCard()) {
//        // check if NextBipTable has at least one NextBip Relationship
//        Vector<StatementNumber> allStatements = facade->getStatements(AnyStatement);
//        for (StatementNumber stmtNum : allStatements) {
//            if (!facade->getNext(stmtNum, AnyStatement).empty()) {
//                resultsTable.storeResultsZero(true);
//                return;
//            }
//        }
//
//        resultsTable.storeResultsZero(false);
//        return;
//    }
//
//    if (leftRef.isWildCard()) {
//        Vector<StatementNumber> nextBipTypeStatements = facade->getStatements(nextRefStmtType);
//        Vector<StatementNumber> results;
//
//        for (StatementNumber stmtNum : nextBipTypeStatements) {
//            // if it has any normal Previous relationship, add to results
//            Vector<StatementNumber> allPrevStatements = facade->getPrevious(stmtNum, AnyStatement);
//            if (!allPrevStatements.empty()) {
//                results.push_back(stmtNum);
//            }
//        }
//
//        ClauseResult clauseResults = convertToClauseResult(results);
//        resultsTable.storeResultsOne(rightRef, clauseResults);
//        return;
//    }
//
//    if (leftRef.isWildCard()) {
//        Vector<StatementNumber> nextBipTypeStatements = facade->getStatements(nextRefStmtType);
//        Vector<StatementNumber> results;
//
//        for (StatementNumber stmtNum : nextBipTypeStatements) {
//            // if it has any normal Previous relationship, add to results
//            Vector<StatementNumber> allPrevStatements = facade->getPrevious(stmtNum, AnyStatement);
//            if (!allPrevStatements.empty()) {
//                results.push_back(stmtNum);
//            }
//        }
//
//        ClauseResult clauseResults = convertToClauseResult(results);
//        resultsTable.storeResultsOne(rightRef, clauseResults);
//        return;
//    }
}

Void NextBipEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
//{
//    Vector<StatementNumber> nextStarBipAnyStmtResults = NextEvaluator::getCacheNextStatement(leftRefVal).toList();
//    ProcedureName leftRefContainingProcName = facade->getProcedureOfStmt(leftRefVal);
//    if (leftRefContainingProcName.empty()) {
//        resultsTable.storeResultsZero(false);
//        return;
//    }
//    Vector<StatementNumber> filteredValidResults
//        = filterValidResults(leftRefContainingProcName, true, nextStarBipAnyStmtResults);
//
//    Boolean isTrue = std::find(filteredValidResults.begin(), filteredValidResults.end(), rightRefVal)
//                     != filteredValidResults.end();
//
//    resultsTable.storeResultsZero(isTrue);
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