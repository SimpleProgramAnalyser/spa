/**
 * Implementation of an evaluator for a such that clause
 * describing a Modifies relationship between design entities
 */
#include "ModifiesEvaluator.h"

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
    resultsTable->filterTable(rightRef, tempResult);
}

Void ModifiesEvaluator::evaluateRightKnown() const
{
    DesignEntityType leftType = resultsTable->getTypeOfSynonym(leftRef.getValue());
    if (isStatementDesignEntity(leftType)) {
        resultsTable->filterTable(
            leftRef, convertToClauseResult(getModifiesStatements(rightRef.getValue(), mapToStatementType(leftType))));
    } else {
        // left ref is a procedure
        resultsTable->filterTable(leftRef, getModifiesProcedures(rightRef.getValue()));
    }
}

Void ModifiesEvaluator::evaluateBothAny() const
{
    Boolean isStatementLeft
        = leftRefType == SynonymRefType && isStatementDesignEntity(resultsTable->getTypeOfSynonym(leftRef.getValue()));
    Boolean leftHasConstraints = refHasConstraints(leftRef, resultsTable);
    Boolean rightHasConstraints = refHasConstraints(rightRef, resultsTable);
    if (leftHasConstraints || rightHasConstraints) {
        ClauseResult previousResultsForLeft = resultsTable->get(leftRef.getValue());
        ClauseResult previousResultsForRight = resultsTable->get(rightRef.getValue());
        std::vector<String> tempResultForLeft;
        std::vector<String> tempResultForRight;
        std::vector<std::pair<String, String>> tempResultForRelationships;
        // do a Cartesian product of both result lists and check each pair
        for (const String& strLeft : previousResultsForLeft) {
            for (const String& strRight : previousResultsForRight) {
                Boolean followsHolds;
                if (isStatementLeft) {
                    followsHolds = checkIfStatementModifies(std::stoi(strLeft), strRight);
                } else {
                    followsHolds = checkIfProcedureModifies(strLeft, strRight);
                }
                if (followsHolds) {
                    tempResultForLeft.push_back(strLeft);
                    tempResultForRight.push_back(strRight);
                    tempResultForRelationships.emplace_back(strLeft, strRight);
                }
            }
        }
        resultsTable->filterTable(leftRef, tempResultForLeft);
        resultsTable->filterTable(rightRef, tempResultForRight);
        resultsTable->associateRelationships(tempResultForRelationships, leftRef, rightRef);
    } else if (isStatementLeft) {
        StatementType leftStmtType = mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
        // select stmt
        resultsTable->filterTable(leftRef, convertToClauseResult(getAllModifiesStatements(leftStmtType)));
        // select variable with statement
        resultsTable->filterTable(rightRef, getAllModifiesVariablesFromStatementType(leftStmtType));
        // select all tuples Modifies(stmt, variable)
        resultsTable->associateRelationships(getAllModifiesStatementTuple(leftStmtType), leftRef, rightRef);
    } else if (leftRefType == SynonymRefType) {
        // select procedure
        resultsTable->filterTable(leftRef, getAllModifiesProcedures());
        // select variable with procedure
        resultsTable->filterTable(rightRef, getModifiesVariablesFromProcedure(leftRef.getValue()));
        // select all tuples Modifies(procedure, variable)
        resultsTable->associateRelationships(getAllModifiesProcedureTuple(), leftRef, rightRef);
    } else {
        throw std::runtime_error("Unknown case in ModifiesExtractor::evaluateBothAny");
    }
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
    if (modifiesHolds) {
        std::vector<String> tempResult{"trueModifies"};
        resultsTable->filterTable(leftRef, tempResult);
    } else {
        // we store an empty list to denote a lack of results
        resultsTable->filterTable(leftRef, std::vector<String>());
    }
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