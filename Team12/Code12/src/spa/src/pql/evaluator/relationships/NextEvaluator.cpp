/**
 * Implementation of an evaluator for a such that clause that
 * has Next and Next* relationships between design entities
 */
#include "NextEvaluator.h"

#include <stdexcept>

#include "RelationshipsUtil.h"

Void NextEvaluator::evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef) const
{
    DesignEntityType rightSynonymType
        = rightRef.isWildCard() ? StmtType : resultsTable.getTypeOfSynonym(rightRef.getValue());
    resultsTable.storeResultsOne(
        rightRef, convertToClauseResult(getAllNextStatements(leftRefVal, mapToStatementType(rightSynonymType))));
}

Void NextEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal) const
{
    DesignEntityType leftSynonymType
        = leftRef.isWildCard() ? StmtType : resultsTable.getTypeOfSynonym(leftRef.getValue());
    resultsTable.storeResultsOne(
        leftRef, convertToClauseResult(getAllPreviousStatements(rightRefVal, mapToStatementType(leftSynonymType))));
}

Void NextEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef) const
{
    // get the DesignEntityType of prev synonym and next synonym
    StatementType prevRefStmtType
        = leftRef.isWildCard() ? AnyStatement : mapToStatementType(resultsTable.getTypeOfSynonym(leftRef.getValue()));
    StatementType nextRefStmtType
        = rightRef.isWildCard() ? AnyStatement : mapToStatementType(resultsTable.getTypeOfSynonym(rightRef.getValue()));
    ClauseResult leftResults = convertToClauseResult(getAllPreviousStatementsTyped(prevRefStmtType, nextRefStmtType));
    ClauseResult rightResults = convertToClauseResult(getAllNextStatementsTyped(prevRefStmtType, nextRefStmtType));
    PairedResult tuples = convertToPairedResult(getAllNextTuples(prevRefStmtType, nextRefStmtType));
    resultsTable.storeResultsTwo(leftRef, leftResults, rightRef, rightResults, tuples);
}

Void NextEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal) const
{
    resultsTable.storeResultsZero(checkIfNextHolds(leftRefVal, rightRefVal));
}

Void NextEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    DesignEntityType rightSynonymType = rightRef.isWildCard() ? StmtType : rightRef.getDesignEntity().getType();
    CacheSet nextStarAnyStmtResults = getCacheNextStatement(leftRefVal);
    ClauseResult filteredResults = nextStarAnyStmtResults.filterStatementType(mapToStatementType(rightSynonymType));
    resultsTable.storeResultsOne(rightRef, filteredResults);
}

Void NextEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    DesignEntityType leftSynonymType = leftRef.isWildCard() ? StmtType : leftRef.getDesignEntity().getType();
    CacheSet prevStarAnyStmtResults = getCachePrevStatement(rightRefVal);
    ClauseResult filteredResults = prevStarAnyStmtResults.filterStatementType(mapToStatementType(leftSynonymType));
    resultsTable.storeResultsOne(leftRef, filteredResults);
}

Void NextEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    // get the DesignEntityType of prev synonym and next synonym
    StatementType prevRefStmtType
        = leftRef.isWildCard() ? AnyStatement : mapToStatementType(leftRef.getDesignEntity().getType());
    StatementType nextRefStmtType
        = rightRef.isWildCard() ? AnyStatement : mapToStatementType(rightRef.getDesignEntity().getType());

    if (leftRef.isWildCard() && rightRef.isWildCard()) {
        // check if NextTable has at least one Next Relationship
        Vector<StatementNumber> allStatements = getAllStatements(AnyStatement);
        for (StatementNumber stmtNum : allStatements) {
            if (!getAllNextStatements(stmtNum, AnyStatement).empty()) {
                resultsTable.storeResultsZero(true);
                return;
            }
        }

        resultsTable.storeResultsZero(false);
        return;
    }

    if (leftRef.isWildCard()) {
        Vector<StatementNumber> nextTypeStatements = getAllStatements(nextRefStmtType);
        Vector<StatementNumber> results;

        for (StatementNumber stmtNum : nextTypeStatements) {
            // if it has any normal Previous relationship, add to results
            Vector<StatementNumber> allPrevStatements = getAllPreviousStatements(stmtNum, AnyStatement);
            if (!allPrevStatements.empty()) {
                results.push_back(stmtNum);
            }
        }

        ClauseResult clauseResults = convertToClauseResult(results);
        resultsTable.storeResultsOne(rightRef, clauseResults);
        return;
    }

    if (rightRef.isWildCard()) {
        Vector<StatementNumber> prevTypeStatements = getAllStatements(prevRefStmtType);
        Vector<StatementNumber> results;

        for (StatementNumber stmtNum : prevTypeStatements) {
            // if it has any normal Next relationship, add to results
            Vector<StatementNumber> allNextStatements = getAllNextStatements(stmtNum, AnyStatement);
            if (!allNextStatements.empty()) {
                results.push_back(stmtNum);
            }
        }

        ClauseResult clauseResults = convertToClauseResult(results);
        resultsTable.storeResultsOne(leftRef, clauseResults);
        return;
    }

    // Both are Synonyms
    Vector<StatementNumber> prevTypeStatements = getAllStatements(prevRefStmtType);
    Vector<Pair<Integer, String>> pairedResults;
    for (StatementNumber stmtNum : prevTypeStatements) {
        CacheSet nextStarAnyStmtResults = getCacheNextStatement(stmtNum);
        ClauseResult filteredResults = nextStarAnyStmtResults.filterStatementType(nextRefStmtType);
        /** Store result */
        for (const String& result : filteredResults) {
            Pair<Integer, String> pairResult = std::make_pair(stmtNum, result);
            pairedResults.push_back(pairResult);
        }
    }
    resultsTable.storeResultsTwo(leftRef.getValue(), rightRef.getValue(), convertToPairedResult(pairedResults));
}

Void NextEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    CacheSet nextStarAnyStmtResults = getCacheNextStatement(leftRefVal);
    resultsTable.storeResultsZero(nextStarAnyStmtResults.isCached(rightRefVal));
}

NextEvaluator::NextEvaluator(ResultsTable& resultsTable): resultsTable(resultsTable) {}

Void NextEvaluator::evaluateNextClause(const Reference& leftRef, const Reference& rightRef)
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    if (leftRefType == IntegerRefType && canMatchMultiple(rightRefType)) {
        evaluateLeftKnown(std::stoi(leftRef.getValue()), rightRef);
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        evaluateRightKnown(leftRef, std::stoi(rightRef.getValue()));
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        evaluateBothKnown(std::stoi(leftRef.getValue()), std::stoi(rightRef.getValue()));
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        evaluateBothAny(leftRef, rightRef);
    } else {
        throw std::runtime_error("Error in NextEvaluator::evaluateNextClause: No synonyms or integers in Next clause");
    }
}

Void NextEvaluator::evaluateNextStarClause(const Reference& leftRef, const Reference& rightRef)
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    if (leftRefType == IntegerRefType && canMatchMultiple(rightRefType)) {
        evaluateLeftKnownStar(std::stoi(leftRef.getValue()), rightRef);
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        evaluateRightKnownStar(leftRef, std::stoi(rightRef.getValue()));
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        evaluateBothKnownStar(std::stoi(leftRef.getValue()), std::stoi(rightRef.getValue()));
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        evaluateBothAnyStar(leftRef, rightRef);
    } else {
        throw std::runtime_error(
            "Error in NextEvaluator::evaluateNextStarClause: No synonyms or integers in Next* clause");
    }
}

CacheSet NextEvaluator::getCacheNextStatement(StatementNumber stmtNum)
{
    // Check if statement number has been explored
    if (cacheNextStarTable.isCached(stmtNum)) {
        return cacheNextStarTable.get(stmtNum);
    }

    visitedNextStatements.insert(stmtNum);

    Vector<StatementNumber> nextStatementList = getAllNextStatements(stmtNum, AnyStatement);
    CacheSet currentCacheSet(nextStatementList);
    for (auto nextStmtNum : nextStatementList) {
        if (isNextVisited(nextStmtNum)) {
            continue;
        }

        CacheSet nextCacheSet = getCacheNextStatement(nextStmtNum);
        currentCacheSet.combine(nextCacheSet);
    }

    cacheNextStarTable.insert(stmtNum, currentCacheSet);
    return currentCacheSet;
}

CacheSet NextEvaluator::getCachePrevStatement(StatementNumber stmtNum)
{
    // Check if statement number has been explored
    if (cachePrevStarTable.isCached(stmtNum)) {
        return cachePrevStarTable.get(stmtNum);
    }

    visitedPrevStatements.insert(stmtNum);

    Vector<StatementNumber> prevStatementList = getAllPreviousStatements(stmtNum, AnyStatement);
    CacheSet currentCacheSet(prevStatementList);
    for (auto prevStmtNum : prevStatementList) {
        if (isPrevVisited(prevStmtNum)) {
            continue;
        }

        CacheSet prevCacheSet = getCachePrevStatement(prevStmtNum);
        currentCacheSet.combine(prevCacheSet);
    }

    cachePrevStarTable.insert(stmtNum, currentCacheSet);
    return currentCacheSet;
}

Boolean NextEvaluator::isNextVisited(StatementNumber stmtNum) const
{
    return visitedNextStatements.isCached(stmtNum);
}

Boolean NextEvaluator::isPrevVisited(StatementNumber stmtNum) const
{
    return visitedPrevStatements.isCached(stmtNum);
}
