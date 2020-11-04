/**
 * Implementation of an evaluator for a such that clause that
 * has NextBip and NextBip* relationships between design entities.
 */

#include "NextBipEvaluator.h"

#include "pql/evaluator/relationships/bip/BipFacade.h"
#include "pql/evaluator/relationships/bip/BipUtils.h"

//#include "pkb/tables/Tables.h"
//#include "pkb/PKB.h"
//#include "ast/AstTypes.h"
//#include "pql/evaluator/relationships/affects/AffectsBipEvaluator.h"

Boolean findAllNextBipFromStmtPos(CfgNode* currentCfgNode, size_t startingNodeNumber,
                                  std::unordered_set<size_t>& visited, CacheSet& results)
{
    size_t currentNodeNumber = currentCfgNode->nodeNumber;
    if (visited.find(currentNodeNumber) != visited.end()) {
        return false;
    } else {
        visited.insert(currentNodeNumber);
    }

    Vector<StatementNode*> stmtList = *(currentCfgNode->statementNodes);
    for (StatementNode* stmtNode : stmtList) {
        StatementNumber stmtNum = stmtNode->getStatementNumber();
        results.insert(stmtNum);
    }

    Vector<CfgNode*> childrenList = *(currentCfgNode->childrenNodes);
    Boolean hasVisitedStartingNode = false;
    for (CfgNode* childCfgNode : childrenList) {
        hasVisitedStartingNode = childCfgNode->nodeNumber == startingNodeNumber;
        hasVisitedStartingNode
            = findAllNextBipFromStmtPos(childCfgNode, startingNodeNumber, visited, results) || hasVisitedStartingNode;
    }

    return hasVisitedStartingNode;
}

CacheSet NextBipEvaluator::processLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    Vector<StatementPositionInCfg> allCgfNodes = findAllCorrespondingPositions(leftRefVal, *bipFacade);
    CacheSet results;
    std::unordered_set<size_t> visited;

    if (allCgfNodes.empty()) {
        return CacheSet();
    }

    Boolean hasNextBipToItself = false; // if the node has a NextBip* relationship with itself
    for (StatementPositionInCfg startingPosition : allCgfNodes) {
        CfgNode* startingCfgNode = startingPosition.getNodePosition();
        size_t startingNodeNumber = startingCfgNode->nodeNumber;
        hasNextBipToItself
            = findAllNextBipFromStmtPos(startingCfgNode, startingNodeNumber, visited, results) || hasNextBipToItself;
    }

    // Since we only added those Next* statements in the same statement list,
    // those Previous* statements in the same statement list will not be added if
    // the starting Node is in a while loop. Hence, if it has a NextBip* relationship
    // with itself, we need to add those Previous* statements that we didn't add
    // previously.
    Vector<StatementNode*> stmtList = *(allCgfNodes.at(0).getNodePosition()->statementNodes);
    for (StatementNode* stmtNode : stmtList) {
        StatementNumber stmtNum = stmtNode->getStatementNumber();
        if (!hasNextBipToItself && stmtNum <= leftRefVal) {
            results.remove(stmtNum);
        }
    }

    return results;
}

Void NextBipEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    CacheSet results = processLeftKnownStar(leftRefVal, rightRef);
    if (results.empty()) {
        resultsTable.storeResultsZero(false);
    }
    resultsTable.storeResultsOne(rightRef, results.toClauseResult());
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
    //    Vector<StatementNumber> finalValidResults = filterValidResults(rightRefContainingProcName, false,
    //    filteredList); ClauseResult results = convertToClauseResult(finalValidResults);
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
{
    CacheSet results = processLeftKnownStar(leftRefVal, Reference(WildcardRefType, "_"));
    resultsTable.storeResultsZero(results.isCached(rightRefVal));
}

NextBipEvaluator::NextBipEvaluator(ResultsTable& resultsTable, NextBipFacade* facade):
    NextEvaluator(resultsTable, facade), bipFacade(facade)
{}
