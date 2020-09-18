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

Void evaluateSuchThat(SuchThatClause* stClause, ResultsTable* resultsTable)
{
    Relationship rel = stClause->getRelationship();
    RelationshipReferenceType relRefType = rel.getRelationship();
    switch (relRefType) {
    case FollowsType:
        return evaluateFollowsClause(synonym, stClause, declarations, false);
    case FollowsStarType:
        return evaluateFollowsClause(synonym, stClause, declarations, true);
    case ParentType:
        return evaluateParentClause(synonym, stClause, declarations, false);
    case ParentStarType:
        return evaluateParentClause(synonym, stClause, declarations, true);
    case UsesType:
    case UsesStatementType:
    case UsesProcedureType:
        return evaluateUsesClause(synonym, stClause, declarations);
    case ModifiesType:
    case ModifiesStatementType:
    case ModifiesProcedureType:
        return evaluateModifiesClause(synonym, stClause, declarations);
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
 * will be with respect to that particular synonym. This method also
 * interacts with the PKB to obtain the results to the query.
 *
 * @param synonym The synonym that is to be selected.
 * @param stClause The Follows clause to evaluate.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 * @param isStar Whether the Follows clause comes with an
 *               asterisk, indicating the transitive closure.
 *
 * @return Results for the Synonym in the Follows clause.
 */
ClauseResult Evaluator::evaluateFollowsClause(const Synonym& synonym, SuchThatClause* stClause,
                                              const DeclarationTable& declarations, Boolean isStar)
{
    ClauseResult result;
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
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
        /*
         * Check if clause is related to synonym, recall that a clause is related to
         * synonym if and only if at least one of its operands is equals to given
         * synonym in question.
         */
        Integer leftValue = std::stoi(leftRef.getValue());
        // get the DesignEntityType of the right operand
        DesignEntityType rightSynonymType
            = rightRef.isWildCard() ? StmtType : declarations.getDesignEntityOfSynonym(rightRef.getValue()).getType();
        /*
         * Here there are many combinations/possibilities, of what the
         * right DesignEntityType can be, however with a some mapping,
         * function, we can simplify things.
         *
         * The function, would map DesignEntityType (pql/preprocessor/AqTypes.h)
         * to StatementType (in Types.h).
         *
         * This function, is declared as a private member (of this class), please
         * refer to its documentation for more details.
         *
         * Note: Here we make a critical assumption, that we can never
         * have invalid DesignEntityType which would hence get mapped
         * to spurious StatementType. This is guaranteed due to our
         * (robust) validation at the PQL Preprocessor side.
         */
        Vector<Integer> tempResult = (isStar ? getAllAfterStatementsStar
                                             : getAllAfterStatements)(leftValue, mapToStatementType(rightSynonymType));
        result = convertToClauseResult(tempResult);
    } else if (canMatchMultiple(leftRefType) && rightRefType == IntegerRefType) {
        /*
         * Check if clause is related to synonym, recall that a clause is related to
         * synonym if and only if at least one of its operands is equals to given
         * synonym in question.
         */
        // get the DesignEntityType of the left operand
        DesignEntityType leftSynonymType
            = leftRef.isWildCard() ? StmtType : declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType();
        Integer rightValue = std::stoi(rightRef.getValue());
        /*
         * Here there are many combinations/possibilities, of what the
         * right DesignEntityType can be, however with a some mapping,
         * function, we can simplify things.
         *
         * The function, would map DesignEntityType (pql/preprocessor/AqTypes.h)
         * to StatementType (in Types.h).
         *
         * This function, is declared as a private member (of this class), please
         * refer to its documentation for more details.
         *
         * Note: Here we make a critical assumption, that we can never
         * have invalid DesignEntityType which would hence get mapped
         * to spurious StatementType. This is guaranteed due to our
         * (robust) validation at the PQL Preprocessor side.
         */
        Vector<Integer> tempResult = (isStar ? getAllBeforeStatementsStar
                                             : getAllBeforeStatements)(rightValue, mapToStatementType(leftSynonymType));
        result = convertToClauseResult(tempResult);
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
            result.push_back("trueFollows");
        }
    } else if (!leftRef.isWildCard() && leftRef.getValue() == rightRef.getValue()) {
        // Check if left == right, for Follows this will always return empty
        return result;
    } else if (canMatchMultiple(leftRefType) && canMatchMultiple(rightRefType)) {
        /*
         * Check if clause is related to synonym, recall that a clause is related to
         * synonym if and only if at least one of its operands is equals to given
         * synonym in question.
         */
        // In this case, we can only get the DesignEntityType of both the left and right operands
        StatementType leftRefStmtType
            = leftRef.isWildCard()
                  ? AnyStatement
                  : mapToStatementType(declarations.getDesignEntityOfSynonym(leftRef.getValue()).getType());
        StatementType rightRefStmtType
            = rightRef.isWildCard()
                  ? AnyStatement
                  : mapToStatementType(declarations.getDesignEntityOfSynonym(rightRef.getValue()).getType());
        /*
         * Here, there are 3 cases, and here's how we would handle them;
         * 1. The left operand is related (appears) in the synonym.
         *    Then, we just call the 'before' PKB API;
         *    getAllBeforeStatementsTyped(..)
         * 2. The right operand is related (appears) in the synonym.
         *    Similarly, we just call the 'after' PKB API;
         *    getAllAfterStatementsTyped(..)
         * 3. Neither operands is related (appears) to the synonym.
         *    This case is the most tricky, and technically,
         *    we might have to call the 'before' and 'after' PKB API,
         *    to determine if there are indeed results for this
         *    clause.
         *
         *    However, we can just call the 'before' PKB API,
         *    and if we get empty (or non-empty) result, it is guaranteed that
         *    the 'after' PKB API method, would also yield an empty (or non-empty)
         *    result (for we would get a Contradiction, had we not adopted this
         *    reasoning).
         *
         *    Hence, we don't have to check the 'after' PKB API,
         *    to determine if there are results there, should we get an
         *    empty result when calling 'before' API, because the
         *    'after' would be empty too.
         */
        std::vector<Integer> (*lookupPkbFunction)(StatementType, StatementType);
        if (leftRef.getValue() == synonym) {
            lookupPkbFunction = isStar ? getAllBeforeStatementsTypedStar : getAllBeforeStatementsTyped;
        } else {
            lookupPkbFunction = isStar ? getAllAfterStatementsTypedStar : getAllAfterStatementsTyped;
        }
        result = convertToClauseResult(lookupPkbFunction(leftRefStmtType, rightRefStmtType));
    } else {
        throw std::runtime_error("Error in evaluateFollowsClause: No synonyms or integers in Follows clause");
    }
    return result;
}

/**
 * Processes a single Uses clause in a PQL query.
 *
 * @param synonym The name that is to be selected.
 * @param stClause The Uses clause to evaluate.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 *
 * @return Results for the synonym in the Uses clause.
 */
ClauseResult Evaluator::evaluateUsesClause(const Synonym& synonym, SuchThatClause* stClause,
                                           const DeclarationTable& declarations)
{
    ClauseResult result;
    Reference leftRef = stClause->getRelationship().getLeftRef();
    Reference rightRef = stClause->getRelationship().getRightRef();
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
