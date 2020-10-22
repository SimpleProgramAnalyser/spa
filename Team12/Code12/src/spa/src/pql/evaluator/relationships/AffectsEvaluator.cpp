/**
 * Implementation of an evaluator for a such that clause that has
 * Affects and Affect* relationships between design entities
 */
#include "AffectsEvaluator.h"

#include <cassert>
#include <functional>
#include <set>

#include "RelationshipsUtil.h"

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
 * Checks whether a statement number modifies a variable,
 * under the rules described by Affects/Affects*. Namely,
 * statement has to be an assignment, read or procedure
 * call statement and it has to modify the variable.
 *
 * @param statement Statement number to be checked.
 * @param variable Variable to be checked.
 * @return True, if statement is an Assignment, Read or
 *         Call that modifies variable. Otherwise false.
 */
bool doesProgLineModifies(Integer statement, const String& variable)
{
    StatementType type = getStatementType(statement);
    if (!(type == AssignmentStatement || type == CallStatement || type == ReadStatement)) {
        // ignore container statements, according to definition of Affects
        return false;
    } else {
        return checkIfStatementModifies(statement, variable);
    }
}

/**
 * Checks whether a particular DesignEntityType could
 * possibly have any Affects/Affects* relationships.
 *
 * @param type DesignEntityType of a synonym.
 * @return True, if the type is either "assign _;" or "stmt _;".
 */
inline bool isAffectable(DesignEntityType type)
{
    return type == AssignType || type == StmtType || type == Prog_LineType;
}

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
        switch (getStatementType(currentStmtNum)) {
        case AssignmentStatement: {
            Vector<String> usedVariables = getUsesVariablesFromStatement(currentStmtNum);
            // store Affects relationships
            for (const String& var : usedVariables) {
                auto variablePosition = affectsMap.find(var);
                if (variablePosition != affectsMap.end()) {
                    for (Integer modifyingStmt : variablePosition->second) {
                        resultsLists.addAffects(modifyingStmt, currentStmtNum);
                    }
                }
            }
            Vector<String> modifiedVariables = getModifiesVariablesFromStatement(currentStmtNum);
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
            Vector<String> modifiedVariables = getModifiesVariablesFromStatement(currentStmtNum);
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
            // if and else should eventually point to
            while (afterIf != afterElse) {
                bool isIfFurtherBehind = afterIf->nodeNumber < afterElse->nodeNumber; // else path is possibly end node
                if (isIfFurtherBehind) {
                    afterIf = affectsSearch(afterIf, affectsMap, resultsLists);
                } else {
                    afterElse = affectsSearch(afterElse, elseCopy, resultsLists);
                }
            }
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
            // PrintStatement, do nothing
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
    if (getStatementType(leftRefVal) != AssignmentStatement || !isAffectable(rightRef.getDesignEntity().getType())) {
        resultsTable.storeResultsZero(false);
        return;
    }

    if (!exploredModifierAssigns.isCached(leftRefVal)) {
        Vector<Integer> nextStatements = getAllNextStatements(leftRefVal, AnyStatement);
        // A priority queue that returns smaller statements first
        UniquePriorityQueue<Integer, std::less<Integer>> statementsQueue;
        for (Integer next : nextStatements) {
            statementsQueue.insert(next);
        }
        Vector<String> modifiedFromPkb = getModifiesVariablesFromStatement(leftRefVal);
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

            StatementType currentStatementType = getStatementType(currentStatement);
            if (currentStatementType == AssignmentStatement
                && checkIfStatementUses(currentStatement, modifiedVariable)) {
                // this assignment is Affected by leftRefVal
                affectedStatements.insert(currentStatement);
            }

            if (doesProgLineModifies(currentStatement, modifiedVariable)) {
                // this statements modifies the variable, so we are sure that
                // there can no longer be any Affects found along this branch
                continue;
            }

            // continue traversal of the branch, put all Next statements into the queue
            for (Integer next : getAllNextStatements(currentStatement, AnyStatement)) {
                statementsQueue.insert(next);
            }
        }
        cacheModifierTable.insert(leftRefVal, CacheSet(affectedStatements));
        exploredModifierAssigns.insert(leftRefVal);
    }
    resultsTable.storeResultsOne(rightRef, cacheModifierTable.get(leftRefVal).toClauseResult());
}

Void AffectsEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal)
{
    if (!isAffectable(leftRef.getDesignEntity().getType()) || getStatementType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
        return;
    }
    Vector<String> usedFromPkb = getUsesVariablesFromStatement(rightRefVal);
    if (usedFromPkb.empty()) {
        // this assign uses no variables, cannot possibly have anything Affecting it
        resultsTable.storeResultsZero(false);
        return;
    }
    // cache if needed
    if (!exploredUserAssigns.isCached(rightRefVal)) {
        Vector<Integer> prevStatements = getAllPreviousStatements(rightRefVal, AnyStatement);
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

            StatementType currentStatementType = getStatementType(currentStatement);
            bool doesCurrentModifyAnyVariables = false;
            std::unordered_set<String> remainingVariables = *statementVariablesMap.at(currentStatement);
            for (const String& variable : *statementVariablesMap.at(currentStatement)) {
                if (doesProgLineModifies(currentStatement, variable)) {
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
            for (Integer prev : getAllPreviousStatements(currentStatement, AnyStatement)) {
                statementsQueue.insert(prev);
                if (statementVariablesMap.find(prev) != statementVariablesMap.end()) {
                    // if already exists in map, merge the sets
                    for (const String& variable : remainingVariables) {
                        statementVariablesMap.at(prev)->insert(variable);
                    }
                } else if (doesCurrentModifyAnyVariables) {
                    statementVariablesMap.emplace(prev,
                                                  std::make_shared<std::unordered_set<String>>(remainingVariables));
                } else {
                    statementVariablesMap.emplace(prev, statementVariablesMap.at(currentStatement));
                }
            }
        }
        cacheUserTable.insert(rightRefVal, CacheSet(affectingStatements));
        exploredUserAssigns.insert(rightRefVal);
    }
    resultsTable.storeResultsOne(leftRef, cacheUserTable.get(rightRefVal).toClauseResult());
}

Void AffectsEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef)
{
    if (leftRef.getReferenceType() == SynonymRefType && rightRef.getReferenceType() == SynonymRefType
        && (!isAffectable(leftRef.getDesignEntity().getType())
            || !isAffectable(rightRef.getDesignEntity().getType()))) {
        resultsTable.storeResultsZero(false);
        return;
    }
    // cache if needed
    if (!cacheFullyPopulated) {
        // we can skip procedures that do not modify any variables
        Vector<String> procedures = getAllModifiesProcedures();
        AffectsTuple resultsLists;
        for (const String& proc : procedures) {
            std::unordered_map<String, std::unordered_set<Integer>> modifiedVariablesMap;
            const CfgNode* end = affectsSearch(getCFG(proc), modifiedVariablesMap, resultsLists);
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
    if (getStatementType(leftRefVal) != AssignmentStatement || getStatementType(rightRefVal) != AssignmentStatement) {
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
    Vector<String> modifiedList = getModifiesVariablesFromStatement(leftRefVal);
    // assumption that assign statements only modify one variable
    assert(modifiedList.size() == 1); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    String variableModified = modifiedList[0];
    // traverse CFG to find whether Affects(left, right), modified depth-first search
    Vector<Integer> nextStatements = getAllNextStatements(leftRefVal, AnyStatement);
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
            foundRight = checkIfStatementUses(rightRefVal, variableModified);
            break;
        } else if (doesProgLineModifies(currentStatement, variableModified)) {
            break;
        }
        nextStatements = getAllNextStatements(currentStatement, AnyStatement);
    }
    // update cache
    if (foundRight) {
        cacheModifierTable.insertPartial(leftRefVal, rightRefVal);
        cacheUserTable.insertPartial(rightRefVal, leftRefVal);
    }
    resultsTable.storeResultsZero(foundRight);
}

Void AffectsEvaluator::evaluateLeftKnownStar(Integer leftRefVal, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateRightKnownStar(const Reference& leftRef, Integer rightRefVal) {}

Void AffectsEvaluator::evaluateBothAnyStar(const Reference& leftRef, const Reference& rightRef) {}

Void AffectsEvaluator::evaluateBothKnownStar(Integer leftRefVal, Integer rightRefVal) {}

AffectsEvaluator::AffectsEvaluator(ResultsTable& resultsTable): resultsTable(resultsTable) {}

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

Void AffectsEvaluator::evaluateAffectsStarClause(const Reference& leftRef, const Reference& rightRef) {}
