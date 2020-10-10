/**
 * Implementation of an evaluator for a such that clause
 * describing a Uses relationship between design entities
 */
#include "UsesEvaluator.h"

#include <stdexcept>

#include "RelationshipsUtil.h"

class UsesEvaluator {
private:
    Reference leftRef;
    Reference rightRef;
    ResultsTable* resultsTable;
    // these will determine whether it is a statement or procedure Uses
    ReferenceType leftRefType;

    // case where left is known, right is variable
    Void evaluateLeftKnown() const;
    // case where left is variable, right is known (string)
    Void evaluateRightKnown() const;
    // case where both are variable
    Void evaluateBothAny() const;
    // case where both are known
    Void evaluateBothKnown() const;

public:
    UsesEvaluator(Reference leftRef, Reference rightRef, ResultsTable* resultsTable):
        leftRef(std::move(leftRef)), rightRef(std::move(rightRef)), resultsTable(resultsTable),
        leftRefType(leftRef.getReferenceType())
    {}
    Void evaluateUsesClause();
};

Void evaluateUsesClause(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    UsesEvaluator evaluator(leftRef, rightRef, resultsTable);
    evaluator.evaluateUsesClause();
}

Void UsesEvaluator::evaluateLeftKnown() const
{
    ClauseResult tempResult = leftRefType == IntegerRefType
                                  ? getUsesVariablesFromStatement(std::stoi(leftRef.getValue()))
                                  : getUsesVariablesFromProcedure(leftRef.getValue());
    resultsTable->storeResultsOne(rightRef, tempResult);
}

Void UsesEvaluator::evaluateRightKnown() const
{
    DesignEntityType leftType = resultsTable->getTypeOfSynonym(leftRef.getValue());
    if (isStatementDesignEntity(leftType)) {
        resultsTable->storeResultsOne(
            leftRef, convertToClauseResult(getUsesStatements(rightRef.getValue(), mapToStatementType(leftType))));
    } else {
        // left ref is a procedure
        resultsTable->storeResultsOne(leftRef, getUsesProcedures(rightRef.getValue()));
    }
}

Void UsesEvaluator::evaluateBothAny() const
{
    Boolean isStatementLeft
        = leftRefType == SynonymRefType && isStatementDesignEntity(resultsTable->getTypeOfSynonym(leftRef.getValue()));
    ClauseResult leftResults;
    ClauseResult rightResults;
    PairedResult tuples;
    if (isStatementLeft) {
        StatementType leftStmtType = mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
        // select stmt
        leftResults = convertToClauseResult(getAllUsesStatements(leftStmtType));
        // select variable with statement
        rightResults = getAllUsesVariablesFromStatementType(leftStmtType);
        // select all tuples Uses(stmt, variable)
        tuples = convertToPairedResult(getAllUsesStatementTuple(leftStmtType));
    } else if (leftRefType == SynonymRefType) {
        // select procedure
        leftResults = getAllUsesProcedures();
        // select variable with procedure
        rightResults = getAllUsesVariablesFromProgram();
        // select all tuples Uses(procedure, variable)
        tuples = getAllUsesProcedureTuple();
    } else {
        throw std::runtime_error("Unknown case in UsesExtractor::evaluateBothAny");
    }
    resultsTable->storeResultsTwo(leftRef, leftResults, rightRef, rightResults, tuples);
}

Void UsesEvaluator::evaluateBothKnown() const
{
    Boolean usesHolds;
    if (leftRefType == IntegerRefType) {
        usesHolds = checkIfStatementUses(std::stoi(leftRef.getValue()), rightRef.getValue());
    } else {
        // leftRefType == LiteralRefType
        usesHolds = checkIfProcedureUses(leftRef.getValue(), rightRef.getValue());
    }
    std::vector<String> tempResult;
    if (usesHolds) {
        // we add a placeholder item to denote presence of results
        tempResult.emplace_back("trueUses");
    }
    resultsTable->storeResultsOne(leftRef, tempResult);
}

Void UsesEvaluator::evaluateUsesClause()
{
    ReferenceType rightRefType = rightRef.getReferenceType();
    if (canMatchOnlyOne(leftRefType) && canMatchMultiple(rightRefType)) {
        evaluateLeftKnown();
    } else if (canMatchMultiple(leftRefType) && canMatchOnlyOne(rightRefType)) {
        evaluateRightKnown();
    } else if (canMatchOnlyOne(leftRefType) && canMatchOnlyOne(rightRefType)) {
        evaluateBothKnown();
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        evaluateBothAny();
    } else {
        throw std::runtime_error("Error in UsesExtractor::evaluateUsesClause: invalid arguments in Uses");
    }
}
