/**
 * Implementation of an evaluator for a such that clause that has
 * Affects and Affect* relationships between design entities
 */

#include "AffectsEvaluator.h"

#include <cassert>
#include <set>
#include <stdexcept>

#include "AffectsUtils.h"
#include "pql/evaluator/relationships/RelationshipsUtil.h"

/**
 * Implementation of a priority queue using a std::set
 * to ensure unique elements within the queue.
 */
template <typename T, typename Comparator>
class UniquePriorityQueue {
private:
    std::set<T, Comparator> orderedSet;

public:
    // Inserts an item into the priority queue. Duplicates will be ignored.
    void insert(T item)
    {
        orderedSet.insert(item);
    }

    // Checks if the priority queue is empty or not.
    bool empty() const
    {
        return orderedSet.empty();
    }

    // Returns a copy of the top-most item from the priority queue.
    T peek() const
    {
        return *orderedSet.begin();
    }

    // Deletes the top-most item from the priority queue.
    void pop()
    {
        orderedSet.erase(orderedSet.begin());
    }

    // Removes the top-most item from the priority queue and returns it.
    T popOff()
    {
        auto startPosition = orderedSet.begin();
        T item = *startPosition;
        orderedSet.erase(startPosition);
        return item;
    }
};

/**
 * Copies entries from one unordered_map into another.
 *
 * @param finalMap The map to copy into and modify.
 * @param mapToCopy The map to copy entries.
 */
void combineIntoFirst(std::unordered_map<String, std::unordered_set<Integer>>& finalMap,
                      const std::unordered_map<String, std::unordered_set<Integer>>& mapToCopy)
{
    for (const std::pair<String, std::unordered_set<Integer>> entry : mapToCopy) {
        if (finalMap.find(entry.first) == finalMap.end()) {
            finalMap.emplace(entry.first, std::unordered_set<Integer>());
        }
        for (Integer setItem : entry.second) {
            finalMap.at(entry.first).insert(setItem);
        }
    }
}

Void AffectsTuple::addAffects(Integer modifyingStmt, Integer usingStmt)
{
    modifyingStatementResults.insert(modifyingStmt);
    usingStatementResults.insert(usingStmt);
    affectsResults.emplace_back(modifyingStmt, usingStmt);
}

Vector<Integer> AffectsTuple::getModifyingStatements() const
{
    return Vector<Integer>(modifyingStatementResults.begin(), modifyingStatementResults.end());
}

Vector<Integer> AffectsTuple::getUsingStatements() const
{
    return Vector<Integer>(usingStatementResults.begin(), usingStatementResults.end());
}

Vector<Pair<Integer, Integer>> AffectsTuple::getAffects() const
{
    return affectsResults;
}

/**
 * Performs a recursive depth-first search over the CFG, adding
 * Affects relationships as assignment statements are traversed.
 *
 * @param cfg The CFG node to explore.
 * @param affectsMap The map storing an association between modified
 *                   variables and the assignment statements which
 *                   last modified them, if any.
 * @param resultsLists The AffectsTuple to store results in when new
 *                     Affects relationships are found in the search.
 *
 * @return The next unexplored CFG node.
 */
const CfgNode* AffectsEvaluator::affectsSearch(const CfgNode* cfg,
                                               std::unordered_map<String, std::unordered_set<Integer>>& affectsMap,
                                               AffectsTuple& resultsLists)
{
    const CfgNode* nextNode = nullptr;
    for (const StatementNode* stmtNode : *(cfg->statementNodes)) {
        StatementNumber currentStmtNum = stmtNode->getStatementNumber();
        switch (facade->getType(currentStmtNum)) {
        case AssignmentStatement: {
            Vector<String> usedVariables = facade->getUsed(currentStmtNum);
            // store Affects relationships
            for (const String& var : usedVariables) {
                auto variablePosition = affectsMap.find(var);
                if (variablePosition != affectsMap.end()) {
                    for (Integer modifyingStmt : variablePosition->second) {
                        resultsLists.addAffects(modifyingStmt, currentStmtNum);
                    }
                }
            }
            Vector<String> modifiedVariables = facade->getModified(currentStmtNum);
            for (const String& var : modifiedVariables) {
                if (affectsMap.find(var) == affectsMap.end()) {
                    affectsMap.emplace(var, std::unordered_set<Integer>());
                    affectsMap.at(var).insert(currentStmtNum);
                } else {
                    affectsMap.at(var).clear();
                    affectsMap.at(var).insert(currentStmtNum);
                }
            }
            nextNode = cfg->childrenNodes->empty() ? nullptr : cfg->childrenNodes->at(0);
            break;
        }
        case ReadStatement:
        case CallStatement: {
            Vector<String> modifiedVariables = facade->getModified(currentStmtNum);
            for (const String& var : modifiedVariables) {
                affectsMap.erase(var);
            }
            nextNode = cfg->childrenNodes->empty() ? nullptr : cfg->childrenNodes->at(0);
            break;
        }
        case IfStatement: {
            std::unordered_map<String, std::unordered_set<Integer>> elseCopy = affectsMap;
            // if statement will point to two different paths
            assert(cfg->childrenNodes->size() == 2); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            const CfgNode* afterIf = affectsSearch(cfg->childrenNodes->at(0), affectsMap, resultsLists);
            const CfgNode* afterElse = affectsSearch(cfg->childrenNodes->at(1), elseCopy, resultsLists);
            const CfgNode* joinNode = cfg->ifJoinNode;
            assert(joinNode != nullptr); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            // if and else should eventually point to joinNode
            while (afterIf != afterElse) {
                bool isIfFinished = afterIf == joinNode; // preferentially evaluate if first
                if (!isIfFinished) {
                    afterIf = affectsSearch(afterIf, affectsMap, resultsLists);
                } else {
                    afterElse = affectsSearch(afterElse, elseCopy, resultsLists);
                }
            }
            assert(afterElse == joinNode); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            // combine both maps
            combineIntoFirst(affectsMap, elseCopy);
            nextNode = afterIf;
            break;
        }
        case WhileStatement: {
            // while statement will point to two different paths
            assert(cfg->childrenNodes->size() == 2); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            const CfgNode* firstPath = cfg->childrenNodes->at(0);
            const CfgNode* secondPath = cfg->childrenNodes->at(1);
            // check statement numbers to see which one is the loop path
            bool isFirstLoop = (!firstPath->statementNodes->empty() && !secondPath->statementNodes->empty()
                                && firstPath->statementNodes->at(0)->getStatementNumber()
                                       < secondPath->statementNodes->at(0)->getStatementNumber())
                               || secondPath->statementNodes->empty(); // second path is possibly end node
            const CfgNode* exitPath = isFirstLoop ? secondPath : firstPath;
            const CfgNode* loopPath = isFirstLoop ? firstPath : secondPath;
            // initiate tracking maps
            std::unordered_map<String, std::unordered_set<Integer>> mapBefore = affectsMap;
            std::unordered_map<String, std::unordered_set<Integer>> mapAfter = affectsMap;
            // run the first round through the while loop
            const CfgNode* afterWhile = affectsSearch(loopPath, mapAfter, resultsLists);
            // while should eventually point to this statement again
            while (afterWhile != cfg) {
                afterWhile = affectsSearch(afterWhile, mapBefore, resultsLists);
            }
            // run through loop until all Affects are registered
            while (mapBefore != mapAfter) {
                mapBefore = mapAfter; // copy mapAfter as the new mapBefore
                const CfgNode* afterWhileSecond = affectsSearch(loopPath, mapAfter, resultsLists);
                while (afterWhileSecond != cfg) {
                    afterWhileSecond = affectsSearch(afterWhileSecond, mapAfter, resultsLists);
                }
            }
            combineIntoFirst(affectsMap, mapAfter);
            nextNode = exitPath;
            break;
        }
        default: {
            // PrintStatement, cannot modify variables so no need to check
            nextNode = cfg->childrenNodes->empty() ? nullptr : cfg->childrenNodes->at(0);
        }
        }
    }
    if (cfg->statementNodes->empty() && !cfg->childrenNodes->empty()) {
        assert(cfg->childrenNodes->size() == 1); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        nextNode = cfg->childrenNodes->at(0);
    }
    return nextNode;
}

Void AffectsEvaluator::evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef)
{
    if (facade->getType(leftRefVal) != AssignmentStatement || !isAffectable(rightRef)) {
        resultsTable.storeResultsZero(false);
        return;
    }

    if (!exploredModifierAssigns.isCached(leftRefVal)) {
        cacheModifierAssigns(leftRefVal);
    }

    resultsTable.storeResultsOne(rightRef, cacheModifierTable.get(leftRefVal).toClauseResult());
}

Void AffectsEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal)
{
    if (!isAffectable(leftRef) || facade->getType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
        return;
    }
    Vector<String> usedFromPkb = facade->getUsed(rightRefVal);
    if (usedFromPkb.empty()) {
        // this assign uses no variables, cannot possibly have anything Affecting it
        resultsTable.storeResultsZero(false);
        return;
    }
    // cache if needed
    if (!exploredUserAssigns.isCached(rightRefVal)) {
        cacheUserAssigns(rightRefVal, usedFromPkb);
    }
    resultsTable.storeResultsOne(leftRef, cacheUserTable.get(rightRefVal).toClauseResult());
}

Void AffectsEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef)
{
    if (leftRef.getReferenceType() == SynonymRefType && rightRef.getReferenceType() == SynonymRefType
        && (!isAffectable(leftRef) || !isAffectable(rightRef))) {
        resultsTable.storeResultsZero(false);
        return;
    }
    // cache if needed
    cacheAll();
    if (leftRef.getReferenceType() == SynonymRefType && leftRef == rightRef) {
        // case where both synonyms the same, e.g. Affects(a, a)
        Vector<Integer> selfAffected;
        for (const std::pair<Integer, Integer>& affectsRelation : allAffectsTuples) {
            if (affectsRelation.first == affectsRelation.second) {
                selfAffected.push_back(affectsRelation.first);
            }
        }
        resultsTable.storeResultsOne(leftRef.getValue(), convertToClauseResult(selfAffected));
    } else {
        resultsTable.storeResultsTwo(leftRef, convertToClauseResult(allModifierAssigns), rightRef,
                                     convertToClauseResult(allUserAssigns), convertToPairedResult(allAffectsTuples));
    }
}

Void AffectsEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal)
{
    if (facade->getType(leftRefVal) != AssignmentStatement || facade->getType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
        return;
    }
    // check cache
    if (cacheFullyPopulated) {
        resultsTable.storeResultsZero(cacheModifierTable.check(leftRefVal, rightRefVal));
        return;
    }
    if (cacheModifierTable.check(leftRefVal, rightRefVal) || cacheUserTable.check(rightRefVal, leftRefVal)) {
        resultsTable.storeResultsZero(true);
        return;
    }
    Vector<String> modifiedList = facade->getModified(leftRefVal);
    // assumption that assign statements only modify one variable
    assert(modifiedList.size() == 1); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    String variableModified = modifiedList[0];
    // traverse CFG to find whether Affects(left, right), modified depth-first search
    Vector<Integer> nextStatements = facade->getNext(leftRefVal);
    bool foundRight = false;
    while (!nextStatements.empty()) {
        Integer currentStatement;
        if (nextStatements.size() == 2) {
            Integer firstValue = nextStatements[0];
            Integer secondValue = nextStatements[1];
            bool isFirstSmaller = firstValue < secondValue;
            Integer lowerValue = isFirstSmaller ? firstValue : secondValue;
            Integer higherValue = isFirstSmaller ? secondValue : firstValue;
            if (lowerValue <= rightRefVal && rightRefVal < higherValue) {
                currentStatement = lowerValue;
            } else {
                currentStatement = higherValue;
            }
        } else {
            // next should either be 2, 1, 0 choices (0 exits the while loop)
            assert(nextStatements.size() == 1); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            currentStatement = nextStatements[0];
        }
        if (currentStatement == rightRefVal) {
            // rightRefVal is an assign due to initial guard clause
            foundRight = facade->doesStatementUse(rightRefVal, variableModified);
            break;
        } else if (facade->doesStatementModify(currentStatement, variableModified)) {
            break;
        }
        nextStatements = facade->getNext(currentStatement);
    }
    // update cache
    if (foundRight) {
        cacheModifierTable.insertPartial(leftRefVal, rightRefVal);
        cacheUserTable.insertPartial(rightRefVal, leftRefVal);
    }
    resultsTable.storeResultsZero(foundRight);
}

Void AffectsEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef)
{
    if (facade->getType(leftRefVal) != AssignmentStatement || !isAffectable(rightRef)) {
        resultsTable.storeResultsZero(false);
        return;
    }

    CacheSet modifierStarAnyStmtResults
        = evaluateModifierStar(leftRefVal, -1); // -1 is used to indicate no previous statement
    ClauseResult clauseResult = modifierStarAnyStmtResults.toClauseResult();
    resultsTable.storeResultsOne(rightRef, clauseResult);
}

Void AffectsEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal)
{
    if (facade->getType(rightRefVal) != AssignmentStatement || !isAffectable(leftRef)) {
        resultsTable.storeResultsZero(false);
        return;
    }

    CacheSet userStarAnyStmtResults = evaluateUserStar(rightRefVal, -1); // -1 is used to indicate no previous statement
    ClauseResult clauseResult = userStarAnyStmtResults.toClauseResult();
    resultsTable.storeResultsOne(leftRef, clauseResult);
}

Void AffectsEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef)
{
    if (!isAffectable(leftRef) || !isAffectable(rightRef)) {
        resultsTable.storeResultsZero(false);
        return;
    }

    Vector<StatementNumber> allAssignStatements = facade->getAssigns();

    if (leftRef.isWildCard() && rightRef.isWildCard()) {
        // return true if there are any normal modifies
        for (StatementNumber stmtNum : allAssignStatements) {
            if (!exploredModifierAssigns.isCached(stmtNum)) {
                cacheModifierAssigns(stmtNum);
            }

            if (cacheModifierTable.get(stmtNum).empty()) {
                continue;
            }

            resultsTable.storeResultsZero(true);
            return;
        }

        resultsTable.storeResultsZero(false);
        return;
    }

    cacheAll();
    Vector<StatementNumber> results;

    if (leftRef.isWildCard() || rightRef.isWildCard()) {
        // return all that has a normal modifies
        if (leftRef.isWildCard()) {
            for (StatementNumber stmtNum : allAssignStatements) {
                if (exploredUserAssigns.isCached(stmtNum) && !cacheUserTable.get(stmtNum).empty()) {
                    results.push_back(stmtNum);
                }
            }
        } else {
            for (StatementNumber stmtNum : allAssignStatements) {
                if (exploredModifierAssigns.isCached(stmtNum) && !cacheModifierTable.get(stmtNum).empty()) {
                    results.push_back(stmtNum);
                }
            }
        }

        ClauseResult clauseResult = convertToClauseResult(results);
        resultsTable.storeResultsOne(rightRef, clauseResult);
        return;
    }

    if (leftRef == rightRef) {
        // return all that has a Affects* with itself
        for (StatementNumber stmtNum : allAssignStatements) {
            CacheSet modifierStarAnyStmtResults = evaluateModifierStar(stmtNum, -1);
            if (modifierStarAnyStmtResults.isCached(stmtNum)) {
                results.push_back(stmtNum);
            }
        }

        ClauseResult clauseResult = convertToClauseResult(results);
        resultsTable.storeResultsOne(leftRef, clauseResult);
        return;
    }

    // leftRef != rightRef && both != wildcard
    Vector<Pair<Integer, Integer>> pairedResults;
    for (StatementNumber stmtNum : allAssignStatements) {
        CacheSet modifierStarAnyStmtResults = evaluateModifierStar(stmtNum, -1);
        Vector<StatementNumber> resultsList = modifierStarAnyStmtResults.toList();
        for (auto result : resultsList) {
            Pair<Integer, Integer> pairResult = std::make_pair(stmtNum, result);
            pairedResults.push_back(pairResult);
        }
    }

    resultsTable.storeResultsTwo(leftRef.getValue(), rightRef.getValue(), convertToPairedResult(pairedResults));
}

Void AffectsEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal)
{
    if (facade->getType(leftRefVal) != AssignmentStatement || facade->getType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
        return;
    }
    CacheSet modifierStarAnyStmtResults = evaluateModifierStar(leftRefVal, -1);
    resultsTable.storeResultsZero(modifierStarAnyStmtResults.isCached(rightRefVal));
}

Void AffectsEvaluator::cacheAll()
{
    if (!cacheFullyPopulated) {
        // we just need certain procedures for computation of Affects
        Vector<String> procedures = facade->getRelevantProcedures();
        AffectsTuple resultsLists;
        for (const String& proc : procedures) {
            std::unordered_map<String, std::unordered_set<Integer>> modifiedVariablesMap;
            const CfgNode* end = affectsSearch(facade->getCfg(proc), modifiedVariablesMap, resultsLists);
            while (end != nullptr) {
                end = affectsSearch(end, modifiedVariablesMap, resultsLists);
            }
        }
        // store in cache
        allModifierAssigns = resultsLists.getModifyingStatements();
        allUserAssigns = resultsLists.getUsingStatements();
        allAffectsTuples = resultsLists.getAffects();
        for (Integer modifier : allModifierAssigns) {
            exploredModifierAssigns.insert(modifier);
        }
        for (Integer user : allUserAssigns) {
            exploredUserAssigns.insert(user);
        }
        for (const std::pair<Integer, Integer>& affectsRelation : allAffectsTuples) {
            cacheModifierTable.insertPartial(affectsRelation.first, affectsRelation.second);
            cacheUserTable.insertPartial(affectsRelation.second, affectsRelation.first);
        }
        cacheFullyPopulated = true;
    }
}

Void AffectsEvaluator::cacheModifierAssigns(Integer leftRefVal)
{
    Vector<Integer> nextStatements = facade->getNext(leftRefVal);
    // A priority queue that returns smaller statements first
    UniquePriorityQueue<Integer, std::less<Integer>> statementsQueue;
    for (Integer next : nextStatements) {
        statementsQueue.insert(next);
    }
    Vector<String> modifiedFromPkb = facade->getModified(leftRefVal);
    // assignments can only modify one variable
    assert(modifiedFromPkb.size() == 1); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    std::unordered_set<Integer> affectedStatements;
    String modifiedVariable = modifiedFromPkb.at(0);
    // a set to break out of a loop
    std::unordered_set<Integer> visitedStatementsSet;

    while (!statementsQueue.empty()) {
        Integer currentStatement = statementsQueue.popOff();
        if (visitedStatementsSet.find(currentStatement) != visitedStatementsSet.end()) {
            // if we have visited before, we ignore
            continue;
        }
        visitedStatementsSet.insert(currentStatement);

        StatementType currentStatementType = facade->getType(currentStatement);
        if (currentStatementType == AssignmentStatement
            && facade->doesStatementUse(currentStatement, modifiedVariable)) {
            // this assignment is Affected by leftRefVal
            affectedStatements.insert(currentStatement);
        }

        if (facade->doesStatementModify(currentStatement, modifiedVariable)) {
            // this statements modifies the variable, so we are sure that
            // there can no longer be any Affects found along this branch
            continue;
        }

        // continue traversal of the branch, put all Next statements into the queue
        for (Integer next : facade->getNext(currentStatement)) {
            statementsQueue.insert(next);
        }
    }
    cacheModifierTable.insert(leftRefVal, CacheSet(affectedStatements));
    exploredModifierAssigns.insert(leftRefVal);
}

Void AffectsEvaluator::cacheUserAssigns(Integer rightRefVal, Vector<String> usedFromPkb)
{
    Vector<Integer> prevStatements = facade->getPrevious(rightRefVal);
    std::shared_ptr<std::unordered_set<String>> originalUsedVariables
        = std::make_shared<std::unordered_set<String>>(usedFromPkb.begin(), usedFromPkb.end());
    // A priority queue that returns larger statements first
    UniquePriorityQueue<Integer, std::greater<Integer>> statementsQueue;
    // A map to tell which statements still can modify which variables
    std::unordered_map<Integer, std::shared_ptr<std::unordered_set<String>>> statementVariablesMap;
    for (Integer prev : prevStatements) {
        statementsQueue.insert(prev);
        statementVariablesMap.emplace(prev, originalUsedVariables);
    }
    std::unordered_set<Integer> affectingStatements;
    // a set to break out of a loop
    std::unordered_set<Integer> visitedStatementsSet;

    while (!statementsQueue.empty()) {
        Integer currentStatement = statementsQueue.popOff();
        if (visitedStatementsSet.find(currentStatement) != visitedStatementsSet.end()) {
            // if we have visited before, we ignore
            continue;
        }
        visitedStatementsSet.insert(currentStatement);

        StatementType currentStatementType = facade->getType(currentStatement);
        bool doesCurrentModifyAnyVariables = false;
        std::unordered_set<String> remainingVariables = *statementVariablesMap.at(currentStatement);
        for (const String& variable : *statementVariablesMap.at(currentStatement)) {
            if (facade->doesStatementModify(currentStatement, variable)) {
                doesCurrentModifyAnyVariables = true;
                remainingVariables.erase(variable);
            }
        }
        if (currentStatementType == AssignmentStatement && doesCurrentModifyAnyVariables) {
            // this assignment Affects rightRefVal
            affectingStatements.insert(currentStatement);
        }

        if (doesCurrentModifyAnyVariables && remainingVariables.empty()) {
            // before this statement, all variables used in rightRefVal have been
            // modified, so there can no longer be any Affects found along this branch
            continue;
        }

        // continue traversal of the branch, put all Previous statements into a queue
        // also, associate the current set with Previous statements
        for (Integer prev : facade->getPrevious(currentStatement)) {
            statementsQueue.insert(prev);
            if (statementVariablesMap.find(prev) != statementVariablesMap.end()) {
                // if already exists in map, merge the sets
                for (const String& variable : remainingVariables) {
                    statementVariablesMap.at(prev)->insert(variable);
                }
            } else if (doesCurrentModifyAnyVariables) {
                statementVariablesMap.emplace(prev, std::make_shared<std::unordered_set<String>>(remainingVariables));
            } else {
                statementVariablesMap.emplace(prev, statementVariablesMap.at(currentStatement));
            }
        }
    }
    cacheUserTable.insert(rightRefVal, CacheSet(affectingStatements));
    exploredUserAssigns.insert(rightRefVal);
}

CacheSet AffectsEvaluator::getModifierAssigns(Integer stmtNum)
{
    return cacheModifierTable.get(stmtNum);
}

AffectsEvaluator::AffectsEvaluator(ResultsTable& resultsTable, AffectsEvaluatorFacade* facade):
    cacheUserTable(), cacheModifierTable(), exploredUserAssigns(), exploredModifierAssigns(), allModifierAssigns(),
    allUserAssigns(), allAffectsTuples(), cacheFullyPopulated(false), cacheUserStarTable(), cacheModifierStarTable(),
    partiallyCacheUserStarSet(), partiallyCacheModifierStarSet(), exploredUserStarAssigns(),
    exploredModifierStarAssigns(), visitedUserStarAssigns(), visitedModifierStarAssigns(), resultsTable(resultsTable),
    facade(facade)
{}

Void AffectsEvaluator::evaluateAffectsClause(const Reference& leftRef, const Reference& rightRef)
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
        throw std::runtime_error(
            "Error in AffectsEvaluator::evaluateAffectsClause: No synonyms or integers in Affects clause");
    }
}

Void AffectsEvaluator::evaluateAffectsStarClause(const Reference& leftRef, const Reference& rightRef)
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
            "Error in AffectsEvaluator::evaluateAffectsStarClause: No synonyms or integers in Affects* clause");
    }
}

CacheSet AffectsEvaluator::getCacheModifierStarStatement(StatementNumber stmtNum, StatementNumber prevModifierStmtNum)
{
    // Check if statement number has been explored
    if (exploredModifierStarAssigns.isCached(stmtNum)) {
        return cacheModifierStarTable.get(stmtNum);
    }

    if (visitedModifierStarAssigns.isCached(stmtNum)) {
        // evaluation is still processing in a while loop,
        // and encountered a statement that is also currently
        // being processed in the recursion stack, hence it
        // is unable to determine its completed Modifier CacheSet
        partiallyCacheModifierStarSet.insert(stmtNum);
        return cacheModifierStarTable.get(stmtNum);
    }

    visitedModifierStarAssigns.insert(stmtNum);

    // if the statement has not been evaluated before, evaluate it
    if (!exploredModifierAssigns.isCached(stmtNum)) {
        cacheModifierAssigns(stmtNum);
    }

    Vector<StatementNumber> allUserStatements = cacheModifierTable.get(stmtNum).toList();
    CacheSet currentCacheSet(allUserStatements);
    cacheModifierStarTable.insert(stmtNum, currentCacheSet);
    for (StatementNumber userStatement : allUserStatements) {
        if (userStatement == stmtNum) {
            continue;
        }

        CacheSet nextModifierCacheSet = getCacheModifierStarStatement(userStatement, stmtNum);
        currentCacheSet.combine(nextModifierCacheSet);
    }

    cacheModifierStarTable.getReference(stmtNum)->combine(currentCacheSet);
    exploredModifierStarAssigns.insert(stmtNum);
    return cacheModifierStarTable.get(stmtNum);
}

CacheSet AffectsEvaluator::getCacheUserStarStatement(StatementNumber stmtNum, StatementNumber prevUserStmtNum)
{
    // Check if statement number has been explored
    if (exploredUserStarAssigns.isCached(stmtNum)) {
        return cacheUserStarTable.get(stmtNum);
    }

    if (visitedUserStarAssigns.isCached(stmtNum)) {
        // evaluation is still processing in a while loop,
        // and encountered a statement that is also currently
        // being processed in the recursion stack, hence it
        // is unable to determine its completed User CacheSet
        partiallyCacheUserStarSet.insert(stmtNum);
        return CacheSet();
    }

    visitedUserStarAssigns.insert(stmtNum);

    // if the statement has not been evaluated before, evaluate it
    if (!exploredUserAssigns.isCached(stmtNum)) {
        Vector<String> usedFromPkb = facade->getUsed(stmtNum);
        if (usedFromPkb.empty()) {
            // this assign uses no variables, cannot possibly have anything Affecting it
            return CacheSet();
        }
        cacheUserAssigns(stmtNum, usedFromPkb);
    }

    Vector<StatementNumber> allModifierStatements = cacheUserTable.get(stmtNum).toList();
    CacheSet currentCacheSet(allModifierStatements);
    cacheUserStarTable.insert(stmtNum, currentCacheSet);
    for (StatementNumber modifierStatement : allModifierStatements) {
        // TODO: will the order of iteration affect the partially cached results? should it be sorted?
        if (modifierStatement == stmtNum) {
            continue;
        }

        CacheSet nextUserCacheSet = getCacheUserStarStatement(modifierStatement, stmtNum);
        currentCacheSet.combine(nextUserCacheSet);
    }

    cacheUserStarTable.getReference(stmtNum)->combine(currentCacheSet);
    exploredUserStarAssigns.insert(stmtNum);
    return cacheUserStarTable.get(stmtNum);
}

CacheSet AffectsEvaluator::evaluateModifierStar(StatementNumber stmtNum, StatementNumber prevModifierStmtNum)
{
    CacheSet results = getCacheModifierStarStatement(stmtNum, prevModifierStmtNum);
    cleanup(partiallyCacheModifierStarSet, cacheModifierStarTable);
    return results;
}

CacheSet AffectsEvaluator::evaluateUserStar(StatementNumber stmtNum, StatementNumber prevUserStmtNum)
{
    CacheSet results = getCacheUserStarStatement(stmtNum, prevUserStmtNum);
    cleanup(partiallyCacheUserStarSet, cacheUserStarTable);
    return results;
}

/**
 * Combines statements who have partially evaluated statements
 * in the CacheTable, with the now fully evaluated statements'
 * Affects* statements.
 *
 * @param partiallyCacheSet Either Modifier or User partial set.
 * @param cacheTable        Either Modifier or User cache table.
 */
void AffectsEvaluator::cleanup(CacheSet& partiallyCacheSet, CacheTable& cacheTable)
{
    std::unordered_set<StatementNumber> keys = cacheTable.keys();
    Vector<StatementNumber> wasPartiallyEvaluatedList = partiallyCacheSet.toVector();
    for (StatementNumber wasPartiallyEvaluated : wasPartiallyEvaluatedList) {
        CacheSet fullyEvaluatedCacheSet = cacheTable.get(wasPartiallyEvaluated);
        Vector<StatementNumber> fullEvaluatedCacheVector = fullyEvaluatedCacheSet.toVector();
        for (StatementNumber cachedStmt : fullEvaluatedCacheVector) {
            if (keys.find(cachedStmt) == keys.end()) {
                continue;
            }

            CacheSet* cachedStmtSet = cacheTable.getReference(cachedStmt);
            if (cachedStmtSet->isCached(wasPartiallyEvaluated)) {
                cachedStmtSet->combine(fullyEvaluatedCacheSet);
            }
        }
    }
}