/**
 * Implementation of an evaluator for a such that clause that
 * has NextBip and NextBip* relationships between design entities.
 */

#include "NextBipEvaluator.h"

#include "pql/evaluator/relationships/bip/BipFacade.h"
#include "pql/evaluator/relationships/bip/BipUtils.h"

Boolean findAllNextBipFromCfgNode(CfgNode* currentCfgNode, StatementNumber startingStmtNum,
                                  Boolean isFirstRecursiveCall, std::unordered_set<size_t>& visited, CacheSet& results)
{
    size_t currentNodeNumber = currentCfgNode->nodeNumber;
    if (visited.find(currentNodeNumber) != visited.end()) {
        return false;
    } else {
        visited.insert(currentNodeNumber);
    }

    Boolean hasVisitedStartingNode = false;
    Vector<StatementNode*> stmtList = *(currentCfgNode->statementNodes);
    for (StatementNode* stmtNode : stmtList) {
        StatementNumber stmtNum = stmtNode->getStatementNumber();
        hasVisitedStartingNode = stmtNum == startingStmtNum;
        results.insert(stmtNum);
    }

    if (isFirstRecursiveCall) {
        hasVisitedStartingNode = false;
    }

    Vector<CfgNode*> childrenList = *(currentCfgNode->childrenNodes);
    for (CfgNode* childCfgNode : childrenList) {
        hasVisitedStartingNode = findAllNextBipFromCfgNode(childCfgNode, startingStmtNum, false, visited, results)
                                 || hasVisitedStartingNode;
    }

    return hasVisitedStartingNode;
}

CacheSet NextBipEvaluator::processLeftKnownStar(Integer leftRefVal)
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
        hasNextBipToItself
            = findAllNextBipFromCfgNode(startingCfgNode, leftRefVal, true, visited, results) || hasNextBipToItself;
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
    CacheSet results = processLeftKnownStar(leftRefVal);
    if (results.empty()) {
        resultsTable.storeResultsZero(false);
    }

    DesignEntityType rightSynonymType
        = rightRef.isWildCard() ? StmtType : resultsTable.getTypeOfSynonym(rightRef.getValue());
    ClauseResult filteredResults
        = results.filterStatementType(mapToStatementType(rightSynonymType)).toClauseResult();
    resultsTable.storeResultsOne(rightRef, filteredResults);
}

Void NextBipEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    StatementType prevRefStmtType
        = leftRef.isWildCard() ? AnyStatement : mapToStatementType(leftRef.getDesignEntity().getType());
    Vector<StatementNumber> allLeftStatements = facade->getStatements(prevRefStmtType);

    CacheSet results;
    for (StatementNumber stmtNum : allLeftStatements) {
        CacheSet allNextBipStarOfLeftRef = processLeftKnownStar(stmtNum);
        if (allNextBipStarOfLeftRef.isCached(rightRefVal)) {
            results.insert(stmtNum);
        }
    }

    resultsTable.storeResultsOne(leftRef, results.toClauseResult());
}

Void NextBipEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{

    StatementType prevRefStmtType
        = leftRef.isWildCard() ? AnyStatement : mapToStatementType(leftRef.getDesignEntity().getType());
    StatementType nextRefStmtType
        = rightRef.isWildCard() ? AnyStatement : mapToStatementType(rightRef.getDesignEntity().getType());

    if (leftRef.isWildCard() && rightRef.isWildCard()) {
        // check if NextBipTable has at least one NextBip Relationship
        Vector<StatementNumber> allStatements = facade->getStatements(AnyStatement);
        for (StatementNumber stmtNum : allStatements) {
            if (!facade->getNext(stmtNum, AnyStatement).empty()) {
                resultsTable.storeResultsZero(true);
                return;
            }
        }

        resultsTable.storeResultsZero(false);
        return;
    }

    if (leftRef.isWildCard()) {
        Vector<StatementNumber> nextBipTypeStatements = facade->getStatements(nextRefStmtType);
        Vector<StatementNumber> results;

        for (StatementNumber stmtNum : nextBipTypeStatements) {
            // if it has any normal Previous relationship, add to results
            Vector<StatementNumber> allPrevStatements = facade->getPrevious(stmtNum, AnyStatement);
            if (!allPrevStatements.empty()) {
                results.push_back(stmtNum);
            }
        }

        ClauseResult clauseResults = convertToClauseResult(results);
        resultsTable.storeResultsOne(rightRef, clauseResults);
        return;
    }

    if (rightRef.isWildCard()) {
        Vector<StatementNumber> prevTypeStatements = facade->getStatements(prevRefStmtType);
        Vector<StatementNumber> results;

        for (StatementNumber stmtNum : prevTypeStatements) {
            // if it has any normal Next relationship, add to results
            Vector<StatementNumber> allNextStatements = facade->getNext(stmtNum, AnyStatement);
            if (!allNextStatements.empty()) {
                results.push_back(stmtNum);
            }
        }

        ClauseResult clauseResults = convertToClauseResult(results);
        resultsTable.storeResultsOne(leftRef, clauseResults);
        return;
    }

    // Both are same Synonyms
    if (leftRef == rightRef) {
        Vector<StatementNumber> prevTypeStatements = facade->getStatements(prevRefStmtType);
        Vector<StatementNumber> results;
        for (StatementNumber stmtNum : prevTypeStatements) {

            CacheSet nextStarAnyStmtResults = processLeftKnownStar(stmtNum);
            if (nextStarAnyStmtResults.isCached(stmtNum)) {
                results.push_back(stmtNum);
            }
        }

        ClauseResult clauseResults = convertToClauseResult(results);
        resultsTable.storeResultsOne(leftRef, clauseResults);
        return;
    }

    // Both are different Synonyms
    Vector<StatementNumber> prevTypeStatements = facade->getStatements(prevRefStmtType);
    Vector<Pair<Integer, String>> pairedResults;
    for (StatementNumber stmtNum : prevTypeStatements) {
        CacheSet nextStarAnyStmtResults = processLeftKnownStar(stmtNum);
        ClauseResult filteredResults = nextStarAnyStmtResults.filterStatementType(nextRefStmtType).toClauseResult();
        // Store results
        for (const String& result : filteredResults) {
            Pair<Integer, String> pairResult = std::make_pair(stmtNum, result);
            pairedResults.push_back(pairResult);
        }
    }
    resultsTable.storeResultsTwo(leftRef.getValue(), rightRef.getValue(), convertToPairedResult(pairedResults));
}

Void NextBipEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    CacheSet results = processLeftKnownStar(leftRefVal);
    resultsTable.storeResultsZero(results.isCached(rightRefVal));
}

NextBipEvaluator::NextBipEvaluator(ResultsTable& resultsTable, NextBipFacade* facade):
    NextEvaluator(resultsTable, facade), bipFacade(facade)
{}
