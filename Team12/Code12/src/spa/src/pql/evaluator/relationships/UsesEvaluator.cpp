/**
 * Implementation of an evaluator for a such that clause
 * describing a Uses relationship between design entities
 */
#include "UsesEvaluator.h"

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
    Void evaluateBothAnyConstrained(Boolean isStatementLeft) const;
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
    resultsTable->filterTable(rightRef, tempResult);
}

Void UsesEvaluator::evaluateRightKnown() const
{
    DesignEntityType leftType = resultsTable->getTypeOfSynonym(leftRef.getValue());
    if (isStatementDesignEntity(leftType)) {
        resultsTable->filterTable(
            leftRef, convertToClauseResult(getUsesStatements(rightRef.getValue(), mapToStatementType(leftType))));
    } else {
        // left ref is a procedure
        resultsTable->filterTable(leftRef, getUsesProcedures(rightRef.getValue()));
    }
}

Void UsesEvaluator::evaluateBothAnyConstrained(Boolean isStatementLeft) const
{
    std::vector<String> tempResultForLeft;
    std::vector<String> tempResultForRight;
    std::vector<std::pair<String, String>> tempResultForRelationships;
    Boolean leftAndRightRelated = refsHaveRelationship(leftRef, rightRef, resultsTable);
    // check if variables have relations determined by previous clauses
    if (leftAndRightRelated) {
        // refsHaveRelationship guarantees both left and right are synonyms
        std::vector<std::pair<String, String>> relationsList
            = resultsTable->getRelationships(leftRef.getValue(), rightRef.getValue());
        for (const std::pair<String, String>& relation : relationsList) {
            Boolean usesHolds;
            if (isStatementLeft) {
                usesHolds = checkIfStatementUses(std::stoi(relation.first), relation.second);
            } else {
                usesHolds = checkIfProcedureUses(relation.first, relation.second);
            }
            if (usesHolds) {
                tempResultForLeft.push_back(relation.first);
                tempResultForRight.push_back(relation.second);
                tempResultForRelationships.push_back(relation);
            }
        }
    } else {
        ClauseResult previousResultsForLeft = resultsTable->get(leftRef.getValue());
        ClauseResult previousResultsForRight = resultsTable->get(rightRef.getValue());
        // do a Cartesian product of both result lists and check each pair
        for (const String& strLeft : previousResultsForLeft) {
            for (const String& strRight : previousResultsForRight) {
                Boolean usesHolds;
                if (isStatementLeft) {
                    usesHolds = checkIfStatementUses(std::stoi(strLeft), strRight);
                } else {
                    usesHolds = checkIfProcedureUses(strLeft, strRight);
                }
                if (usesHolds) {
                    tempResultForLeft.push_back(strLeft);
                    tempResultForRight.push_back(strRight);
                    tempResultForRelationships.emplace_back(strLeft, strRight);
                }
            }
        }
    }
    resultsTable->filterTable(leftRef, tempResultForLeft);
    resultsTable->filterTable(rightRef, tempResultForRight);
    resultsTable->associateRelationships(tempResultForRelationships, leftRef, rightRef);
}

Void UsesEvaluator::evaluateBothAny() const
{
    Boolean isStatementLeft
        = leftRefType == SynonymRefType && isStatementDesignEntity(resultsTable->getTypeOfSynonym(leftRef.getValue()));
    Boolean leftHasConstraints = refHasConstraints(leftRef, resultsTable);
    Boolean rightHasConstraints = refHasConstraints(rightRef, resultsTable);
    if ((leftHasConstraints || rightHasConstraints)) {
        evaluateBothAnyConstrained(isStatementLeft);
    } else if (isStatementLeft) {
        StatementType leftStmtType = mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
        // select stmt
        resultsTable->filterTable(leftRef, convertToClauseResult(getAllUsesStatements(leftStmtType)));
        // select variable with statement
        resultsTable->filterTable(rightRef, getAllUsesVariablesFromStatementType(leftStmtType));
        // select all tuples Uses(stmt, variable)
        resultsTable->associateRelationships(getAllUsesStatementTuple(leftStmtType), leftRef, rightRef);
    } else if (leftRefType == SynonymRefType) {
        // select procedure
        resultsTable->filterTable(leftRef, getAllUsesProcedures());
        // select variable with procedure
        resultsTable->filterTable(rightRef, getUsesVariablesFromProcedure(leftRef.getValue()));
        // select all tuples Uses(procedure, variable)
        resultsTable->associateRelationships(getAllUsesProcedureTuple(), leftRef, rightRef);
    } else {
        throw std::runtime_error("Unknown case in UsesExtractor::evaluateBothAny");
    }
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
    if (usesHolds) {
        std::vector<String> tempResult{"trueUses"};
        resultsTable->filterTable(leftRef, tempResult);
    } else {
        // we store an empty list to denote a lack of results
        resultsTable->filterTable(leftRef, std::vector<String>());
    }
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
