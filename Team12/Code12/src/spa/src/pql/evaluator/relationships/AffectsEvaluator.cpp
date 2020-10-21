/**
 * Implementation of an evaluator for a such that clause that has
 * Affects and Affect* relationships between design entities
 */
#include "AffectsEvaluator.h"

#include <cassert>

#include "RelationshipsUtil.h"
#include "cfg/CfgTypes.h"

/**
 * A class to hold result lists for Affects.
 */
class AffectsTuple {
private:
    std::unordered_set<Integer> modifyingStatementResults;
    std::unordered_set<Integer> usingStatementResults;
    Vector<Pair<Integer, Integer>> affectsResults;

public:
    /**
     * Adds an Affects relationship to the results lists.
     */
    Void addAffects(Integer modifyingStmt, Integer usingStmt)
    {
        modifyingStatementResults.insert(modifyingStmt);
        usingStatementResults.insert(usingStmt);
        affectsResults.emplace_back(modifyingStmt, usingStmt);
    }

    /**
     * Gets the list of results for modifying statements
     * (left reference in the Affects relationship).
     */
    Vector<Integer> getModifyingStatements() const
    {
        return Vector<Integer>(modifyingStatementResults.begin(), modifyingStatementResults.end());
    }

    /**
     * Gets the list of results for using statements
     * (right reference in the Affects relationship).
     */
    Vector<Integer> getUsingStatements() const
    {
        return Vector<Integer>(usingStatementResults.begin(), usingStatementResults.end());
    }

    /**
     * Gets the list of results for Affects relationships.
     */
    Vector<Pair<Integer, Integer>> getAffects() const
    {
        return affectsResults;
    }
};

/**
 * Checks whether a statement number modifies a variable,
 * under the rules desribed by Affects/Affects*. Namely,
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
    return type == AssignType || type == StmtType;
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
const CfgNode* depthFirstSearch(const CfgNode* cfg, std::unordered_map<String, std::unordered_set<Integer>>& affectsMap,
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
            const CfgNode* afterIf = depthFirstSearch(cfg->childrenNodes->at(0), affectsMap, resultsLists);
            const CfgNode* afterElse = depthFirstSearch(cfg->childrenNodes->at(1), elseCopy, resultsLists);
            // if and else should eventually point to the same node
            while (afterIf != afterElse) {
                bool isIfFurtherBehind = (!afterIf->statementNodes->empty() && !afterElse->statementNodes->empty()
                                          && afterIf->statementNodes->at(0)->getStatementNumber()
                                                 < afterElse->statementNodes->at(0)->getStatementNumber())
                                         || afterElse->statementNodes->empty(); // else path is possibly end node
                if (isIfFurtherBehind) {
                    afterIf = depthFirstSearch(afterIf, affectsMap, resultsLists);
                } else {
                    afterElse = depthFirstSearch(afterElse, elseCopy, resultsLists);
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
            const CfgNode* afterWhile = depthFirstSearch(loopPath, mapAfter, resultsLists);
            // while should eventually point to this statement again
            while (afterWhile != cfg) {
                afterWhile = depthFirstSearch(afterWhile, mapBefore, resultsLists);
            }
            // run through loop until all Affects are registered
            while (mapBefore != mapAfter) {
                mapBefore = mapAfter; // copy mapAfter as the new mapBefore
                const CfgNode* afterWhileSecond = depthFirstSearch(loopPath, mapAfter, resultsLists);
                while (afterWhileSecond != cfg) {
                    afterWhileSecond = depthFirstSearch(afterWhileSecond, mapAfter, resultsLists);
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
    return nextNode;
}

Void AffectsEvaluator::evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef)
{
    if (getStatementType(leftRefVal) != AssignmentStatement || !isAffectable(rightRef.getDesignEntity().getType())) {
        resultsTable.storeResultsZero(false);
        return;
    }

    Vector<Integer> nextStatements = getAllNextStatements(leftRefVal, AnyStatement);
    Vector<Integer> affectedStatements;
    while (!nextStatements.empty()) {
    }

    resultsTable.storeResultsOne(rightRef, convertToClauseResult(affectedStatements));
}

Void AffectsEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal)
{
    if (!isAffectable(leftRef.getDesignEntity().getType()) || getStatementType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
        return;
    }

    Vector<Integer> prevStatements = getAllPreviousStatements(rightRefVal, AnyStatement);
    Vector<Integer> affectingStatements;
    while (!prevStatements.empty()) {
    }

    resultsTable.storeResultsOne(leftRef, convertToClauseResult(affectingStatements));
}

Void AffectsEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef)
{
    if (!isAffectable(leftRef.getDesignEntity().getType()) || !isAffectable(rightRef.getDesignEntity().getType())) {
        resultsTable.storeResultsZero(false);
        return;
    }
    // we can skip procedures that do not modify any variables
    Vector<String> procedures = getAllModifiesProcedures();
    AffectsTuple resultsLists;
    for (const String& proc : procedures) {
        std::unordered_map<String, std::unordered_set<Integer>> modifiedVariablesMap;
        const CfgNode* end = depthFirstSearch(getCFG(proc), modifiedVariablesMap, resultsLists);
        while (end != nullptr) {
            end = depthFirstSearch(end, modifiedVariablesMap, resultsLists);
        }
    }
    resultsTable.storeResultsTwo(leftRef, convertToClauseResult(resultsLists.getModifyingStatements()), rightRef,
                                 convertToClauseResult(resultsLists.getUsingStatements()),
                                 convertToPairedResult(resultsLists.getAffects()));
}

Void AffectsEvaluator::evaluateBothKnown(Integer leftRefVal, Integer rightRefVal)
{
    if (getStatementType(leftRefVal) != AssignmentStatement || getStatementType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
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
