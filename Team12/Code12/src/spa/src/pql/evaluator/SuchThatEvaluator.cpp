/**
 * Implementation of Such That Evaluator for Query
 * Evaluator, used to evaluate Such That clauses in
 * queries given to SIMPLE Program Analyser.
 */
#include "SuchThatEvaluator.h"

#include <pkb/PKB.h>

// Helper methods for PKB
inline Vector<Integer> verifyStatementType(const StatementType& stmtType, Vector<StatementNumWithType>& result)
{
    if (!result.empty() && result.at(0).second == stmtType) {
        return std::vector<Integer>(result.at(0).first);
    } else {
        return std::vector<Integer>();
    }
}

inline Vector<Integer> getAllBeforeStatements(Integer after, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getBeforeStatement(after);
    return verifyStatementType(stmtType, result);
}

inline Vector<Integer> getAllAfterStatements(Integer before, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getAfterStatement(before);
    return verifyStatementType(stmtType, result);
}

inline Vector<Integer> getAllParentStatements(Integer child, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getParentStatement(child);
    return verifyStatementType(stmtType, result);
}

inline Vector<Integer> getAllChildStatements(Integer parent, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getChildStatement(parent);
    return verifyStatementType(stmtType, result);
}

Void evaluateFollowsClause(Reference leftRef, Reference rightRef, Boolean isStar, ResultsTable* resultsTable);
Void evaluateModifiesClause(Reference leftRef, Reference rightRef, ResultsTable* resultsTable);
Void evaluateParentClause(Reference leftRef, Reference rightRef, Boolean isStar, ResultsTable* resultsTable);
Void evaluateUsesClause(Reference leftRef, Reference rightRef, ResultsTable* resultsTable);

Void evaluateSuchThat(SuchThatClause* stClause, ResultsTable* resultsTable)
{
    Relationship rel = stClause->getRelationship();
    RelationshipReferenceType relRefType = rel.getRelationship();
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
    switch (relRefType) {
    case FollowsType:
        return evaluateFollowsClause(leftRef, rightRef, false, resultsTable);
    case FollowsStarType:
        return evaluateFollowsClause(leftRef, rightRef, true, resultsTable);
    case ParentType:
        return evaluateParentClause(leftRef, rightRef, false, resultsTable);
    case ParentStarType:
        return evaluateParentClause(leftRef, rightRef, true, resultsTable);
    case UsesType:
    case UsesStatementType:
    case UsesProcedureType:
        return evaluateUsesClause(leftRef, rightRef, resultsTable);
    case ModifiesType:
    case ModifiesStatementType:
    case ModifiesProcedureType:
        return evaluateModifiesClause(leftRef, rightRef, resultsTable);
    default:
        throw std::runtime_error("Unknown relationship type in evaluateSuchThat");
    }
}

/**
 * Checks if a ReferenceType corresponds to multiple
 * query results, namely if the ReferenceType is
 * the Synonym type or Wildcard type.
 *
 * @param refType The ReferenceType to be checked.
 * @return True, if the Reference holding this
 *         type can match more than one result.
 *         Otherwise, false.
 */
Boolean canMatchMultiple(const ReferenceType& refType)
{
    return (refType == SynonymRefType || refType == WildcardRefType);
}

/**
 * The opposite of canMatchMultiple, checks if a
 * ReferenceType can only correspond to one
 * design entity in the program.
 *
 * @param refType The ReferenceType to be checked.
 * @return True, if the Reference holding this
 *         type can only match one entity.
 *         Otherwise, false.
 */
Boolean canMatchOnlyOne(const ReferenceType& refType)
{
    return (refType == IntegerRefType || refType == LiteralRefType);
}

/*
 * Processes a single (such that, Follows) clause in a PQL query, with
 * respect to a given synonym. All results obtained from the clauses
 * will be stored in the results table. This method also
 * interacts with the PKB to obtain the results to the query.
 *
 * @param leftRef The left reference in the Follows relationship.
 * @param rightRef The right reference in the Follows relationship.
 * @param isStar Whether the Follows clause comes with an
 *               asterisk, indicating the transitive closure.
 * @param resultsTable Table containing past results, to store
 *                     the results of evaluating this clause
 */
Void evaluateFollowsClause(Reference leftRef, Reference rightRef, Boolean isStar, ResultsTable* resultsTable)
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    /*
     * Here, we filter by reference types of the follows clause
     * operands, and there are a few possible combinations,
     * each of them corresponds to a particular group of
     * PKB API calls.
     *
     * The possible combinations are:
     * 1. Follows(3, s)
     *    Where the left operand is an integer
     *    (line number) and the right operand is
     *    a synonym.
     *
     * 2. Follows(s, 3)
     *    Where the left operand is a synonym type
     *    and the left operand is a integer (line
     *    number).
     *
     * 3. Follows(3, _)
     *    Where the left operand is an integer
     *    (line number) and the right operand
     *    is a wildcard.
     *
     * 4. Follows(_, 3)
     *    Where the left operand is a wildcard,
     *    and the right operand is a wildcard.
     *
     * 5. Follows(2, 3)
     *    Both operands are integers.
     *    (It is not hard to see that the left and right
     *    operands in this case, would never be related to
     *    the synonym (as per iteration 1 requirements)).
     *
     * 6. Follows(s1, s2)
     *    Where both operands are synonym types.
     *
     * 7. Follows(st, st)
     *    The left operand must be the same as the right
     *    operand. For Follows, this is impossible.
     */
    if (leftRefType == IntegerRefType && canMatchMultiple(rightRefType)) {
        Integer leftValue = std::stoi(leftRef.getValue());
        // get the DesignEntityType of the right operand
        // if it is wildcard, equivalent to any statement
        DesignEntityType rightSynonymType
            = rightRef.isWildCard() ? StmtType : resultsTable->getTypeOfSynonym(rightRef.getValue());
        /*
         * Here there are many combinations/possibilities, of what the
         * right DesignEntityType can be, however with a some mapping,
         * function, we can simplify things.
         *
         * The function, would map DesignEntityType (pql/preprocessor/AqTypes.h)
         * to StatementType (in Types.h). It is declared in EvaluatorUtils.cpp.
         *
         * Note: Here we make a critical assumption, that we can never
         * have invalid DesignEntityType which would hence get mapped
         * to spurious StatementType. This is guaranteed due to our
         * (robust) validation at the PQL Preprocessor side.
         */
        Vector<Integer> tempResult = (isStar ? getAllAfterStatementsStar
                                             : getAllAfterStatements)(leftValue, mapToStatementType(rightSynonymType));
        resultsTable->filterTable(rightRef, convertToClauseResult(tempResult));
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        // get the DesignEntityType of the left operand
        DesignEntityType leftSynonymType
            = leftRef.isWildCard() ? StmtType : resultsTable->getTypeOfSynonym(leftRef.getValue());
        Integer rightValue = std::stoi(rightRef.getValue());
        Vector<Integer> tempResult = (isStar ? getAllBeforeStatementsStar
                                             : getAllBeforeStatements)(rightValue, mapToStatementType(leftSynonymType));
        resultsTable->filterTable(leftRef, convertToClauseResult(tempResult));
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        /*
         * For this case, in iteration 1, the clause is perpetually not
         * related to the select synonym (because the wildcard to converted
         * to any StmtType, hence no checking needs to be done here (if
         * clause is related to select synonym).
         */
        Integer leftRefVal = std::stoi(leftRef.getValue());
        Integer rightRefVal = std::stoi(rightRef.getValue());
        Boolean followsHolds = (isStar ? checkIfFollowsHoldsStar : checkIfFollowsHolds)(leftRefVal, rightRefVal);
        if (followsHolds) {
            std::vector<String> tempResult{"trueFollows"};
            resultsTable->filterTable(leftRef, tempResult);
        } else {
            // we store an empty list to denote a lack of results
            resultsTable->filterTable(leftRef, std::vector<String>());
        }
    } else if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // Check if left == right, for Follows this will always return empty
        resultsTable->filterTable(leftRef, std::vector<String>());
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        // In this case, we can only get the DesignEntityType of both the left and right operands
        StatementType leftRefStmtType = leftRef.isWildCard()
                                            ? AnyStatement
                                            : mapToStatementType(resultsTable->getTypeOfSynonym(leftRef.getValue()));
        StatementType rightRefStmtType = rightRef.isWildCard()
                                             ? AnyStatement
                                             : mapToStatementType(resultsTable->getTypeOfSynonym(rightRef.getValue()));
        if (isStar) {
            resultsTable->filterTable(
                leftRef, convertToClauseResult(getAllBeforeStatementsTypedStar(leftRefStmtType, rightRefStmtType)));
            resultsTable->filterTable(
                rightRef, convertToClauseResult(getAllAfterStatementsTypedStar(leftRefStmtType, rightRefStmtType)));
        } else {
            resultsTable->filterTable(
                leftRef, convertToClauseResult(getAllBeforeStatementsTyped(leftRefStmtType, rightRefStmtType)));
            resultsTable->filterTable(
                rightRef, convertToClauseResult(getAllAfterStatementsTyped(leftRefStmtType, rightRefStmtType)));
        }
    } else {
        throw std::runtime_error("Error in evaluateFollowsClause: No synonyms or integers in Follows clause");
    }
}

/**
 * Processes a single Uses clause in a PQL query.
 *
 * @param leftRef The left reference in the Uses relationship.
 * @param rightRef The right reference in the Uses relationship.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @param resultsTable Table containing past results, to store
 *                     the results of evaluating this clause
 */
Void evaluateUsesClause(Reference leftRef, Reference rightRef, ResultsTable* resultsTable)
{
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();

    if (canMatchOnlyOne(leftRefType) && canMatchMultiple(rightRefType)) {
        result = leftRefType == IntegerRefType ? getUsesVariablesFromStatement(std::stoi(leftRef.getValue()))
                                               : getUsesVariablesFromProcedure(leftRef.getValue());
    } else if (canMatchMultiple(leftRefType) && canMatchOnlyOne(rightRefType)) {
        if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            result = convertToClauseResult(getUsesStatements(
                rightRef.getValue(),
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else {
            // declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType() == ProcedureType
            result = getUsesProcedures(rightRef.getValue());
        }
    } else if (canMatchOnlyOne(leftRefType) && canMatchOnlyOne(rightRefType)) {
        Boolean usesHolds;
        if (leftRefType == IntegerRefType) {
            usesHolds = checkIfStatementUses(std::stoi(leftRef.getValue()), rightRef.getValue());
        } else {
            // leftRefType == LiteralRefType
            usesHolds = checkIfProcedureUses(leftRef.getValue(), rightRef.getValue());
        }
        if (usesHolds) {
            result.push_back("trueUses");
        }
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        if (leftRef.getValue() == synonym
            && isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select stmt
            result = convertToClauseResult(getAllUsesStatements(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else if (leftRef.getValue() == synonym) {
            // select procedure
            result = getAllUsesProcedures();
        } else if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select variable with statement
            result = getAllUsesVariables(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType()));
        } else {
            // select variable with procedure
            result = getAllUsesVariables(leftRef.getValue());
        }
    } else {
        throw std::runtime_error("Error in evaluateUsesClause: invalid arguments in Uses");
    }
    return result;
}

/**
 * Processes a single Modifies clause in a PQL query.
 *
 * @param synonym The name that is to be selected.
 * @param stClause The Modifies clause to evaluate.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @return Results for the synonym in the Modifies clause.
 */
ClauseResult Evaluator::evaluateModifiesClause(const Synonym& synonym, SuchThatClause* stClause,
                                               const DeclarationTable& declarations)
{
    ClauseResult result;
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();

    if (canMatchOnlyOne(leftRefType) && canMatchMultiple(rightRefType)) {
        result = leftRefType == IntegerRefType ? getModifiesVariablesFromStatement(std::stoi(leftRef.getValue()))
                                               : getModifiesVariablesFromProcedure(leftRef.getValue());
    } else if (canMatchMultiple(leftRefType) && canMatchOnlyOne(rightRefType)) {
        if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            result = convertToClauseResult(getModifiesStatements(
                rightRef.getValue(),
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else {
            // declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType() == ProcedureType
            result = getModifiesProcedures(rightRef.getValue());
        }
    } else if (canMatchOnlyOne(leftRefType) && canMatchOnlyOne(rightRefType)) {
        Boolean modifies;
        if (leftRefType == IntegerRefType) {
            modifies = checkIfStatementModifies(std::stoi(leftRef.getValue()), rightRef.getValue());
        } else {
            // leftRefType == LiteralRefType
            modifies = checkIfProcedureModifies(leftRef.getValue(), rightRef.getValue());
        }
        if (modifies) {
            result.push_back("trueModifies");
        }
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        if (leftRef.getValue() == synonym
            && isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select stmt
            result = convertToClauseResult(getAllModifiesStatements(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())));
        } else if (leftRef.getValue() == synonym) {
            // select procedure
            result = getAllModifiesProcedures();
        } else if (isStatementDesignEntity(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType())) {
            // select variable with statement
            result = getAllModifiesVariables(
                mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType()));
        } else {
            // select variable with procedure
            result = getAllModifiesVariables(leftRef.getValue());
        }
    } else {
        throw std::runtime_error("Error in evaluateModifiesClause: invalid arguments in Modifies");
    }
    return result;
}

/**
 * Processes a single Parent clause in a PQL query.
 *
 * @param synonym The name that is to be selected.
 * @param stClause The Parent clause to evaluate.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @return Results for the synonym in the Parent clause.
 */
ClauseResult Evaluator::evaluateParentClause(const Synonym& synonym, SuchThatClause* stClause,
                                             const DeclarationTable& declarations, Boolean isStar)
{
    ClauseResult result;
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    if (leftRefType == IntegerRefType && canMatchMultiple(rightRefType)) {
        Integer leftValue = std::stoi(leftRef.getValue());
        DesignEntityType rightSynonymType
            = rightRef.isWildCard() ? StmtType : declarations.getDesignEntityOfSynonym(rightRef.getValue()).getType();
        Vector<Integer> tempResult = (isStar ? getAllChildStatementsStar
                                             : getAllChildStatements)(leftValue, mapToStatementType(rightSynonymType));
        result = convertToClauseResult(tempResult);
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        DesignEntityType leftSynonymType
            = leftRef.isWildCard() ? StmtType : declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType();
        Integer rightValue = std::stoi(rightRef.getValue());
        Vector<Integer> tempResult = (isStar ? getAllParentStatementsStar
                                             : getAllParentStatements)(rightValue, mapToStatementType(leftSynonymType));
        result = convertToClauseResult(tempResult);
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        Integer leftRefVal = std::stoi(leftRef.getValue());
        Integer rightRefVal = std::stoi(rightRef.getValue());
        Boolean followsHolds = (isStar ? checkIfParentHoldsStar : checkIfParentHolds)(leftRefVal, rightRefVal);
        if (followsHolds) {
            result.push_back("trueParent");
        }
    } else if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // Check if left == right, for Parent this will always return empty
        return result;
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        StatementType leftRefStmtType
            = leftRef.isWildCard()
                  ? AnyStatement
                  : mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType());
        StatementType rightRefStmtType
            = rightRef.isWildCard()
                  ? AnyStatement
                  : mapToStatementType(declarations.getDesignEntityOfSynonym(rightRef.getValue()).getType());
        std::vector<Integer> (*lookupPkbFunction)(StatementType, StatementType);
        if (leftRef.getValue() == synonym) {
            lookupPkbFunction = isStar ? getAllParentStatementsTypedStar : getAllParentStatementsTyped;
        } else {
            lookupPkbFunction = isStar ? getAllChildStatementsTypedStar : getAllChildStatementsTyped;
        }
        result = convertToClauseResult(lookupPkbFunction(leftRefStmtType, rightRefStmtType));
    } else {
        throw std::runtime_error("Error in evaluateParentClause: No synonyms or integers in Parent clause");
    }
    return result;
}
