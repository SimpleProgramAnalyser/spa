/**
 * Implementation of an evaluator for a such that clause that
 * has Calls and Calls* relationship between design entities
 */
#include "CallsEvaluator.h"

#include <stdexcept>

#include "RelationshipsUtil.h"

class CallsEvaluator {
private:
    Reference leftRef;
    Reference rightRef;
    Boolean isStar;
    ResultsTable* resultsTable;
    Boolean (*pkbBothKnownFunction)(const String&, const String&);

    // case where left is known (integer), right is variable
    Void evaluateLeftKnown() const;
    // case where left is variable, right is known (integer)
    Void evaluateRightKnown() const;
    // case where both are variable
    Void evaluateBothAny() const;
    // case where both are known
    Void evaluateBothKnown(const String& leftRefVal, const String& rightRefVal) const;

public:
    CallsEvaluator(Reference leftRef, Reference rightRef, Boolean isStar, ResultsTable* resultsTable):
        leftRef(std::move(leftRef)), rightRef(std::move(rightRef)), isStar(isStar), resultsTable(resultsTable),
        pkbBothKnownFunction(isStar ? checkIfCallsHoldsStar : checkIfCallsHolds)
    {}
    Void evaluateCallsClause() const;
};

Void evaluateCallsClause(const Reference& leftRef, const Reference& rightRef, Boolean isStar,
                         ResultsTable* resultsTable)
{
    CallsEvaluator evaluator(leftRef, rightRef, isStar, resultsTable);
    evaluator.evaluateCallsClause();
}

Void CallsEvaluator::evaluateLeftKnown() const
{
    Vector<ProcedureName> (*function)(const ProcedureName&) = getAllCallees;
    Vector<ProcedureName> (*starFunction)(const ProcedureName&) = getAllCalleesStar;
    resultsTable->storeResultsOne(rightRef, (isStar ? function : starFunction)(leftRef.getValue()));
}

Void CallsEvaluator::evaluateRightKnown() const
{
    Vector<ProcedureName> (*function)(const ProcedureName&) = getAllCallers;
    Vector<ProcedureName> (*starFunction)(const ProcedureName&) = getAllCallersStar;
    resultsTable->storeResultsOne(leftRef, (isStar ? function : starFunction)(rightRef.getValue()));
}

Void CallsEvaluator::evaluateBothAny() const
{
    ClauseResult leftResults;
    ClauseResult rightResults;
    PairedResult tuples;
    if (isStar) {
        leftResults = getAllCallees();
        rightResults = getAllCallers();
        tuples = getAllCallsTuple();
    } else {
        leftResults = getAllCalleesStar();
        rightResults = getAllCallersStar();
        tuples = getAllCallsTupleStar();
    }
    resultsTable->storeResultsTwo(leftRef, leftResults, rightRef, rightResults, tuples);
}

Void CallsEvaluator::evaluateBothKnown(const String& leftRefVal, const String& rightRefVal) const
{
    resultsTable->storeResultsZero(pkbBothKnownFunction(leftRefVal, rightRefVal));
}

Void CallsEvaluator::evaluateCallsClause() const
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    if (leftRefType == LiteralRefType && canMatchMultiple(rightRefType)) {
        evaluateLeftKnown();
    } else if (canMatchMultiple(leftRefType) && rightRefType == LiteralRefType) {
        evaluateRightKnown();
    } else if (leftRefType == LiteralRefType && rightRefType == LiteralRefType) {
        evaluateBothKnown(leftRef.getValue(), rightRef.getValue());
    } else if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // if left == right, for Calls this will always return empty
        // this is because recursive calls are not allowed
        resultsTable->storeResultsOne(leftRef, std::vector<String>());
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        evaluateBothAny();
    } else {
        throw std::runtime_error(
            "Error in CallsEvaluator::evaluateCallsClause: No synonyms or string literals in Calls clause");
    }
}
