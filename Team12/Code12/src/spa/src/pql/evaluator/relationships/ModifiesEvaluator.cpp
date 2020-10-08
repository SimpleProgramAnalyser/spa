/**
 * Implementation of an evaluator for a such that clause
 * describing a Modifies relationship between design entities
 */
#include "ModifiesEvaluator.h"

#include <stdexcept>

#include "RelationshipsUtil.h"

class ModifiesEvaluator {
private:
    Reference leftRef;
    Reference rightRef;
    ResultsTable* resultsTable;
    // these will determine whether it is a statement or procedure Modifies
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
    ModifiesEvaluator(Reference leftRef, Reference rightRef, ResultsTable* resultsTable):
        leftRef(std::move(leftRef)), rightRef(std::move(rightRef)), resultsTable(resultsTable),
        leftRefType(leftRef.getReferenceType())
    {}
    Void evaluateModifiesClause();
};

Void evaluateModifiesClause(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    ModifiesEvaluator evaluator(leftRef, rightRef, resultsTable);
    evaluator.evaluateModifiesClause();
}

Void ModifiesEvaluator::evaluateLeftKnown() const
{
    ClauseResult tempResult = leftRefType == IntegerRefType
                                  ? getModifiesVariablesFromStatement(std::stoi(leftRef.getValue()))
                                  : getModifiesVariablesFromProcedure(leftRef.getValue());
    resultsTable->storeResultsOne(rightRef, tempResult);
}

Void ModifiesEvaluator::evaluateRightKnown() const
{
    DesignEntityType leftType = resultsTable->getTypeOfSynonym(leftRef.getValue());
    if (isStatementDesignEntity(leftType)) {
        resultsTable->storeResultsOne(
            leftRef, convertToClauseResult(getModifiesStatements(rightRef.getValue(), mapToStatementType(leftType))));
    } else {
        // left ref is a procedure
        resultsTable->storeResultsOne(leftRef, getModifiesProcedures(rightRef.getValue()));
    }
}

Void ModifiesEvaluator::evaluateBothAny() const
{
    Boolean isStatementLeft
        = leftRefType == SynonymRefType && isStatementDesignEntity(resultsTable->getTypeOfSynonym(leftRef.getValue()));
    ClauseResult leftResults;
    ClauseResult rightResults;
    PairedResult tuples;
    if (isStatementLeft) {
        StatementType leftStmtType = mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
        // select stmt
        leftResults = convertToClauseResult(getAllModifiesStatements(leftStmtType));
        // select variable with statement
        rightResults = getAllModifiesVariablesFromStatementType(leftStmtType);
        // select all tuples Modifies(stmt, variable)
        tuples = convertToPairedResult(getAllModifiesStatementTuple(leftStmtType));
    } else if (leftRefType == SynonymRefType) {
        // select procedure
        leftResults = getAllModifiesProcedures();
        // select variable with procedure
        rightResults = getAllModifiesVariablesFromProgram();
        // select all tuples Modifies(procedure, variable)
        tuples = getAllModifiesProcedureTuple();
    } else {
        throw std::runtime_error("Unknown case in ModifiesExtractor::evaluateBothAny");
    }
    resultsTable->storeResultsTwo(leftRef, leftResults, rightRef, rightResults, tuples);
}

Void ModifiesEvaluator::evaluateBothKnown() const
{
    Boolean modifiesHolds;
    if (leftRefType == IntegerRefType) {
        modifiesHolds = checkIfStatementModifies(std::stoi(leftRef.getValue()), rightRef.getValue());
    } else {
        // leftRefType == LiteralRefType
        modifiesHolds = checkIfProcedureModifies(leftRef.getValue(), rightRef.getValue());
    }
    std::vector<String> tempResult;
    if (modifiesHolds) {
        // we add a placeholder item to denote presence of results
        tempResult.emplace_back("trueModifies");
    }
    resultsTable->storeResultsOne(leftRef, tempResult);
}

Void ModifiesEvaluator::evaluateModifiesClause()
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
        throw std::runtime_error("Error in ModifiesExtractor::evaluateModifiesClause: invalid arguments in Modifies");
    }
}