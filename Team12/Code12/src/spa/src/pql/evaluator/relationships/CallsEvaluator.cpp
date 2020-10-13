/**
 * Implementation of an evaluator for a such that clause that
 * has Calls and Calls* relationship between design entities
 */
#include "CallsEvaluator.h"

#include "pkb/PKB.h"

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

Void CallsEvaluator::evaluateLeftKnown() const {}

Void CallsEvaluator::evaluateRightKnown() const {}

Void CallsEvaluator::evaluateBothAny() const {}

Void CallsEvaluator::evaluateBothKnown(const String& leftRefVal, const String& rightRefVal) const {}

Void CallsEvaluator::evaluateCallsClause() const {}
