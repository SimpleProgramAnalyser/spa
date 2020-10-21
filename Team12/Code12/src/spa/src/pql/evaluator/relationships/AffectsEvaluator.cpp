/**
 * Implementation of an evaluator for a such that clause that has
 * Affects and Affect* relationships between design entities
 */
#include "AffectsEvaluator.h"

#include "RelationshipsUtil.h"

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

Void AffectsEvaluator::evaluateLeftKnown(Integer leftRefVal, const Reference& rightRef)
{
    if (getStatementType(leftRefVal) != AssignmentStatement || !isAffectable(rightRef.getDesignEntity().getType())) {
        resultsTable.storeResultsZero(false);
        return;
    }
}

Void AffectsEvaluator::evaluateRightKnown(const Reference& leftRef, Integer rightRefVal)
{
    if (!isAffectable(leftRef.getDesignEntity().getType()) || getStatementType(rightRefVal) != AssignmentStatement) {
        resultsTable.storeResultsZero(false);
        return;
    }
}

Void AffectsEvaluator::evaluateBothAny(const Reference& leftRef, const Reference& rightRef)
{
    if (!isAffectable(leftRef.getDesignEntity().getType()) || !isAffectable(rightRef.getDesignEntity().getType())) {
        resultsTable.storeResultsZero(false);
        return;
    }
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
            // next should either be 2, 1, 0 choices
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
