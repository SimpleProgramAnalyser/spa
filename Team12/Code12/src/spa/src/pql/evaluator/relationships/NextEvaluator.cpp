/**
 * Implementation of an evaluator for a such that clause that
 * has Next and Next* relationships between design entities
 */
#include "NextEvaluator.h"

#include <algorithm>
#include <stdexcept>

#include "RelationshipsUtil.h"

StatementNumber getLastStatementNumberInWhileLoop(StatementNumber currentStmtNum, StatementNumber whileStmtNum);

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
    if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // If left == right, Next(s, s) will always be false.
        //
        // This is because of SIMPLE rules where a while loop must
        // have at least 1 statement in its statement list.
        resultsTable.storeResultsZero(false);
    }
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

    // Both are same Synonyms
    if (leftRef == rightRef) {
        Vector<StatementNumber> prevTypeStatements = getAllStatements(prevRefStmtType);
        Vector<StatementNumber> results;
        for (StatementNumber stmtNum : prevTypeStatements) {
            CacheSet nextStarAnyStmtResults = getCacheNextStatement(stmtNum);
            if (nextStarAnyStmtResults.isCached(stmtNum)) {
                results.push_back(stmtNum);
            }
        }

        ClauseResult clauseResults = convertToClauseResult(results);
        resultsTable.storeResultsOne(leftRef, clauseResults);
        return;
    }

    // Both are different Synonyms
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

NextEvaluator::NextEvaluator(ResultsTable& resultsTable):
    resultsTable(resultsTable), cacheNextStarTable(), cachePrevStarTable(), exploredNextStatements(),
    exploredPrevStatements()
{}

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
    if (exploredNextStatements.isCached(stmtNum)) {
        return cacheNextStarTable.get(stmtNum);
    }

    Vector<StatementNumber> nextStatementList = getAllNextStatements(stmtNum, AnyStatement);
    CacheSet currentCacheSet(nextStatementList);

    if (getStatementType(stmtNum) == WhileStatement) {
        StatementNumber nextStatementNumber = stmtNum + 1;

        // First evaluate the statement that the current
        // statement has a Next relationship with
        // and is not in the current while's statement
        // list, before evaluating the while loop.
        // This allows the statements in the while loop
        // to get the Next* statements that are after
        // the while loop. If there is only one Next
        // relationship, that means this while is the
        // last statement (other than its statement list)
        // in the procedure, then there is no need to
        // evaluate any other Next relationship first
        if (nextStatementList.size() == 2) {
            StatementNumber nextNonWhileStatementNumber
                = nextStatementList.at(0) == nextStatementNumber ? nextStatementList.at(1) : nextStatementList.at(0);
            CacheSet nextCacheSet = getCacheNextStatement(nextNonWhileStatementNumber);
            currentCacheSet.combine(nextCacheSet);
        }

        StatementNumber lastStatementNumberInWhileLoop
            = getLastStatementNumberInWhileLoop(nextStatementNumber, stmtNum);

        // Add all the statements in while statement's
        // statement list to the cache set of while statement
        for (int i = stmtNum; i <= lastStatementNumberInWhileLoop; i++) {
            currentCacheSet.insert(i);
        }

        // TODO: Currently commented out since it may be unnecessary to set the results for all the statements in
        // while's statement list. Can be used to test optimisation

        // Add while statement's cache set to all statements
        // in statement list. This is done on the basis that
        // all the statements in the statement list should
        // have the same set of Next* Relationships.
        for (int i = stmtNum + 1; i <= lastStatementNumberInWhileLoop; i++) {
            cacheNextStarTable.insert(i, currentCacheSet);
            exploredNextStatements.insert(i);
        }

        cacheNextStarTable.insert(stmtNum, currentCacheSet);
        exploredNextStatements.insert(stmtNum);
        return currentCacheSet;
    } else {
        for (auto nextStmtNum : nextStatementList) {
            CacheSet nextCacheSet = getCacheNextStatement(nextStmtNum);
            currentCacheSet.combine(nextCacheSet);
        }

        cacheNextStarTable.insert(stmtNum, currentCacheSet);
        exploredNextStatements.insert(stmtNum);
        return currentCacheSet;
    }
}

CacheSet NextEvaluator::getCachePrevStatement(StatementNumber stmtNum)
{
    // Check if statement number has been explored
    if (exploredPrevStatements.isCached(stmtNum)) {
        return cachePrevStarTable.get(stmtNum);
    }

    Vector<StatementNumber> prevStatementList = getAllPreviousStatements(stmtNum, AnyStatement);
    CacheSet currentCacheSet(prevStatementList);
    StatementNumber prevStatementNumber = stmtNum - 1;

    if (getStatementType(stmtNum) == WhileStatement) {
        if (prevStatementList.size() != 1) {
            // this while statement is NOT the first statement in the procedure
            CacheSet prevNonWhileStatement = getCachePrevStatement(prevStatementNumber);
            currentCacheSet.combine(prevNonWhileStatement);
        }

        StatementNumber lastStatementNumberInWhileLoop = getLastStatementNumberInWhileLoop(stmtNum + 1, stmtNum);
        for (int i = stmtNum; i <= lastStatementNumberInWhileLoop; i++) {
            currentCacheSet.insert(i);
        }

        // Add while statement's cache set to all statements
        // in statement list. This is done on the basis that
        // all the statements in the statement list should
        // have the same set of Next* Relationships.
        for (int i = stmtNum + 1; i <= lastStatementNumberInWhileLoop; i++) {
            cachePrevStarTable.insert(i, currentCacheSet);
            exploredPrevStatements.insert(i);
        }
    } else {
        for (auto prevStmtNum : prevStatementList) {
            CacheSet prevCacheSet = getCachePrevStatement(prevStmtNum);
            currentCacheSet.combine(prevCacheSet);
        }
    }

    cachePrevStarTable.insert(stmtNum, currentCacheSet);
    exploredPrevStatements.insert(stmtNum);
    return currentCacheSet;
}

/**
 * Retrieves the last statement number of the given while loop.
 * @param currentStmtNum    Current statement number to evaluate.
 * @param whileStmtNum      Statement number of the while statement.
 * @return                  Last statement number in the
 */
StatementNumber getLastStatementNumberInWhileLoop(StatementNumber currentStmtNum, StatementNumber whileStmtNum)
{
    Vector<StatementNumber> prevStatementList = getAllPreviousStatements(whileStmtNum, AnyStatement);
    StatementNumber maxNextStmtNum = *std::max_element(prevStatementList.begin(), prevStatementList.end());

    if (getStatementType(maxNextStmtNum) == WhileStatement) {
        return getLastStatementNumberInWhileLoop(currentStmtNum, maxNextStmtNum);
    }

    return maxNextStmtNum;
}