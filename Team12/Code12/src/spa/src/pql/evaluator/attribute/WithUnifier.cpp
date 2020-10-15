/**
 * Implementation of WithUnifier for Query Evaluator.
 * WithUnifier performs a unification to obtain results.x`
 */

#include "WithUnifier.h"

#include "pkb/PKB.h"
#include "pql/evaluator/relationships/RelationshipsUtil.h"

Void unifyLeftKnown(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable) {}

Void unifyRightKnown(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    unifyLeftKnown(rightRef, leftRef, resultsTable);
}

Void unifyBothKnown(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    bool unificationSuccessful = false;
    if (leftRef == rightRef) {
        unificationSuccessful = true;
    } else if (leftRef.getReferenceType() == IntegerRefType && rightRef.getReferenceType() == IntegerRefType) {
        unificationSuccessful = std::stoi(leftRef.getValue()) == std::stoi(rightRef.getValue());
    }
    resultsTable->storeResultsZero(unificationSuccessful);
}

Void unifyBothAny(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    if (leftRef == rightRef) {
        resultsTable->storeResultsZero(true);
    }
}

Void evaluateWith(WithClause* whClause, ResultsTable* resultsTable)
{
    const Reference& leftRef = whClause->getLeftReference();
    const Reference& rightRef = whClause->getRightReference();
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    // assume leftRef and rightRef are not wildcard,
    void (*unifyFunction)(const Reference&, const Reference&, ResultsTable*)
        = canMatchMultiple(leftRefType) ? (canMatchMultiple(rightRefType) ? unifyBothAny : unifyRightKnown)
                                        : (canMatchMultiple(rightRefType) ? unifyLeftKnown : unifyBothKnown);
    unifyFunction(leftRef, rightRef, resultsTable);
}
