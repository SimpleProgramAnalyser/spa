/**
 * Implementation of the PQL query evaluator.
 */

#include "Evaluator.h"

#include <stdexcept>

#include "RawResultFromClause.h"
#include "pkb/PKB.h"

static Boolean checkIfClausesEmpty(ClauseVector clauses);
static Boolean isQueryVacuouslyTrue(Vector<RawResultFromClause> results);
Vector<RawResultFromClause> filterResultsRelatedToSyn(Vector<RawResultFromClause> results);
static RawResultFromClause retrieveAllMatching(DesignEntityType entTypeOfSynonym);
RawQueryResult processSyntacticallyValidQuery(AbstractQuery abstractQuery);
RawResultFromClause processSingleSynonymQuery(Synonym synonym, ClauseVector clauses, DeclarationTable declarations);
RawResultFromClause evaluateFollowsClause(Synonym synonym, SuchThatClause* stClause, DeclarationTable declarations,
                                          Boolean isStar);
RawResultFromClause processClause(Synonym synonym, Clause* clause, DeclarationTable declarations);
RawResultFromClause processSuchThat(Synonym synonym, SuchThatClause* stClause, DeclarationTable declarations);
static StatementType mapToStatementType(DesignEntityType entType);

/*
 * An utility method to convert an integer vector to a string vector.
 *
 * @param intList An integer vector to convert.
 *
 * @return Vector<String> the converted vector.
 */
Vector<String> convertToStringVect(const Vector<Integer>& intList)
{
    Vector<String> strList;
    for (Integer i : intList) {
        strList.push_back(std::to_string(i));
    }
    return strList;
}

/**
 * Evaluates a PQL query, and interacts with the PKB,
 * for the results of that query.
 *
 * @param query The PQL query.
 *
 * @return RawQueryResult, representing the PQL
 * query results (Note: If either PQL query invalid, or is
 * valid but yields no result, an empty RawQueryResult
 * would be returned).
 */
RawQueryResult Evaluator::evaluateQuery(const AbstractQuery& query)
{
    /*
     * First check if PQL query is semantically valid.
     * If invalid, don't continue evaluating query.
     */
    if (query.isInvalid()) {
        return RawQueryResult::getSyntaxError(
            "ERROR CODE 3735929054: PQL was not parsed. SIGSYNTAX obtained. This incident will be reported.");
    }
    return processSyntacticallyValidQuery(query);
}

/*
 * Processes a PQL query and interacts with PKB if needed,
 * to obtain the results to teh query. This method is a
 * (private) helper method to the main method
 * evaluateQuery(AbstractQuery).
 *
 * @param query The PQL query.
 *
 * @return RawQueryResult, representing the PQL
 * query results (Note: If either PQL query invalid, or is
 * valid but yields no result, an empty RawQueryResult
 * would be returned).
 */
RawQueryResult processSyntacticallyValidQuery(AbstractQuery abstractQuery)
{
    Vector<Synonym> synonyms{(abstractQuery.getSelectSynonym())};
    ClauseVector clauses = abstractQuery.getClauses();
    DeclarationTable declarations = abstractQuery.getDeclarationTable();
    Vector<RawResultFromClause> results;
    if (synonyms.size() == 1) {
        /*
         * Currently, there is only exactly 1 synonym,
         * in the PQL query, so we will only need to iterate through
         * all the clauses once (with respect to that synonym).
         *
         * However, if there is a tuple of synonyms,
         * call processMultipleSynonymClauses(..) which would
         * determine the constraints of the synonyms and
         * call appropriate methods to get the result
         */
        RawResultFromClause result = processSingleSynonymQuery(synonyms.at(0), clauses, declarations);
        results.push_back(result);
    }
    return RawQueryResult(results);
}

/*
 * Processes (all) the clauses in a PQL query, with respect to
 * a single given synonym.
 *
 * For instance, given the following PQL query;
 *     assign a; statement s;
 *     Select a such that Follows(s, a) pattern a(_, "3+5")
 *
 * All clauses in the PQL, namely the Follows and Pattern
 * clauses would be processed, with respect to the (only)
 * assignment synonym a.
 *
 * If however, there are vacuously true PQL queries
 * such as the following;
 *     statement s1; statement a;
 *     Select s such that Follows(s1, a) pattern a(_, "3+5")
 *
 * Then this method, would also handle this possibility
 * (where it simply retrieves all statements (from the
 * PKB Tables API).
 *
 * This method interacts with the relevant PKB APIs,
 * to obtain the results to the query.
 *
 * @param synonym The synonym by which clauses would be
 * evaluated with respect to.
 * @param clauses The clauses to evaluate.
 * @param declarations A table containing a map of declarations (variables)
 * to their design entity (might come in handy for processing query).
 *
 * @return RawResultFromClauses, which represents results from
 * all clauses in the query (please read the documentation of
 * RawResultFromClause for more details), evaluated with respect
 * to a particular synonym.
 */
RawResultFromClause processSingleSynonymQuery(Synonym synonym, ClauseVector clauses, DeclarationTable declarations)
{
    Vector<RawResultFromClause> results;
    /*
     * First check if clauses are empty.
     *
     * If so, retrieve all entity related to that synonym,
     * from Tables API.
     */
    Boolean areClausesEmpty = checkIfClausesEmpty(clauses);
    if (areClausesEmpty) {
        DesignEntityType entTypeOfSynonym = declarations.getDesignEntityOfSynonym(synonym).getType();
        return retrieveAllMatching(entTypeOfSynonym);
    }

    for (int i = 0; i < clauses.count(); ++i) {
        Clause* clause = clauses.get(i);
        RawResultFromClause result = processClause(synonym, clause, declarations);
        /*
         * If the clause is not related to the synonym,
         * and it yields no results, then we can conclude
         * immediately, that this query clauses (evaluated
         * with respect to the synonym in question),
         * returns no results.
         *
         * As such, we don't have to continue processing
         * the rest of the Clauses, and can just terminate
         * early here.
         */
        if (!result.checkIsClauseRelatedToSynonym() && result.isEmpty()) {
            return RawResultFromClauses::emptyRawResultFromClauses();
        }

        results.push_back(result);
    }

    /*
     * Here there is another possibility, where query can be
     * vacuously true, i.e, when all of the clauses/clause
     * results are not related to the synonym
     */
    areClausesEmpty = isQueryVacuouslyTrue(results);

    if (areClausesEmpty) {
        DesignEntityType entTypeOfSynonym = declarations.getDesignEntityOfSynonym(synonym).getType();
        RawResultFromClause result = retrieveAllMatching(entTypeOfSynonym);

        // Reset the vector, for we need to update new results.
        results.clear();
        results.push_back(result);
    }

    /*
     * Filter all clause/clause results that are not
     * related to the synonym.
     *
     * If the query was vacuously true, and
     * results were updated (on the if-condition
     * directly on top), then, the RawResultFromClause
     * returned, would automatically have the flag
     * that it is related to synonym (the
     * retrieveResultsForVacuouslyTrue(..) method
     * inserts this flag (please read more on
     * its documentation, for more information).
     */
    results = filterResultsRelatedToSyn(results);
    return RawResultFromClauses(results);
}

/*
 * Processes a single clause in a PQL query, with respect to
 * a given synonym. All results obtained from the clauses will be
 * with respect to that particular synonym.
 *
 * This method also interacts with the PKB APIs to obtain
 * the desired results.
 *
 * @param synonym The synonym by which the clause would be
 * evaluated, with respect to.
 * @param clause The clause to evaluate.
 * @param declarations A table containing a map of declarations (variables)
 * to their design entity (might come in handy for processing query).
 *
 * @return Vector<String>, which represents the results from only 1
 * particular clause in the query (please read the documentation
 * of RawQueryResult for more details) evaluated with respect to
 * a particular synonym.
 */
RawResultFromClause processClause(Synonym synonym, Clause* clause, DeclarationTable declarations)
{
    if (clause->getType() == SuchThatClauseType) {
        // NOLINTNEXTLINE
        return processSuchThat(synonym, static_cast<SuchThatClause*>(clause), declarations);
    } else if (clause->getType() == PatternClauseType) {
        // TODO: Pattern matching
    } else {
        throw std::runtime_error("Unknown clause type in processClause");
    }
}

/*
 * Processes a single such that clause in a PQL query, with
 * respect to a given synonym. All results obtained from
 * the clauses will be with respect to that particular synonym.
 *
 * This method also interacts with the PKB, if needed
 * to obtain the results for the such that clause.
 *
 * @param synonym The synonym by which the clause would be
 * evaluated with respect to.
 * @param stClause The SuchThatClause to evaluate.
 * @param declarations A table containing a map of declarations (variables)
 * to their design entity (might come in handy for processing query).
 *
 * @return Vector<String>, which represents the results from only 1
 * particular (such that) clause in the query (please read the
 * documentation of RawQueryResult for more details) evaluated
 * with respect to a particular synonym.
 */
Vector<String> processSuchThat(const Synonym& synonym, SuchThatClause* stClause, const DeclarationTable& declarations)
{
    Relationship rel = stClause->getRelationship();
    RelationshipReferenceType relRefType = rel.getRelationship();
    switch (relRefType) {
    case FollowsType:
        return evaluateFollowsClause(synonym, stClause, declarations, false);
    case FollowsStarType:
        return evaluateFollowsClause(synonym, stClause, declarations, true);
    // case ParentType:
    // case ParentStarType:
    // case UsesStatementType:
    // case UsesProcedureType:
    // case ModifiesStatementType:
    // case ModifiesProcedureType:
    default:
        throw std::runtime_error("Unknown relationship type in processSuchThat");
    }
}

/*
 * Given a Vector<RawResultFromClause>, filter out all RawResultFromClause
 * that are not related to the synonym (i.e, isClauseRelatedToSynonym is false).
 */
Vector<RawResultFromClause> filterResultsRelatedToSyn(Vector<RawResultFromClause> results)
{
    Vector<RawResultFromClause> filteredResults;

    Integer len = results.size();

    for (int i = 0; i < len; ++i) {
        RawResultFromClause result = results.at(i);

        if (result.checkIsClauseRelatedToSynonym()) {
            filteredResults.push_back(result);
        }
    }

    return filteredResults;
}

/*
 * Given a synonym, retrieves the results for vacuously true queries,
 * i.e, retrieve all entity objects related to the design entity type
 * of the synonym (e.g, statement, while loops, procedures, etc).
 *
 * This method returns a RawResultFromClass, through which it will
 * set the isClauseRelatedToSynonym flag in the RawResultFromClass
 * to true.
 *
 * @param entTypeOfSynonym The design entity type of the synonym.
 *
 * @return RawResultFromClause representing the results, from
 * the vacuously true statement.
 */
RawResultFromClause retrieveAllMatching(DesignEntityType entTypeOfSynonym)
{
    Vector<String> results;
    if (isStatementDesignEntity(entTypeOfSynonym)) {
        results = convertToStringVect(getAllStatements(mapToStatementType(entTypeOfSynonym)));
    } else if (entTypeOfSynonym == VariableType) {
        results = getAllVariables();
    } else if (entTypeOfSynonym == ProcedureType) {
        results = getAllProcedures();
    } else if (entTypeOfSynonym == ConstantType) {
        // TODO: ??? Constant ???
    } else {
        throw std::runtime_error("Unknown DesignEntityType in retrieveAllMatching");
    }
    return RawResultFromClause(results);
}

/*
 * Given a DesignEntityType, this function maps it to
 * a StatementType, which is a type compatible for calling
 * the PKB API
 *
 * @param entType The DesignEntityType to map to.
 *
 * @return StatementType This represents the corresponding,
 * mapped StatementType object.
 */
StatementType mapToStatementType(DesignEntityType entType)
{
    StatementType mappedStmtType;
    // TODO: Use a hash table instead
    if (entType == StmtType) {
        mappedStmtType = AnyStatement;
    } else if (entType == ReadType) {
        mappedStmtType = ReadStatement;
    } else if (entType == PrintType) {
        mappedStmtType = PrintStatement;
    } else if (entType == CallType) {
        mappedStmtType = CallStatement;
    } else if (entType == WhileType) {
        mappedStmtType = WhileStatement;
    } else if (entType == IfType) {
        mappedStmtType = IfStatement;
    } else if (entType == AssignType) {
        mappedStmtType = AssignmentStatement;
    } else {
        throw std::runtime_error("Wrong entity type in mapToStatementType");
    }

    return mappedStmtType;
}

/*
 * This method handles/checks case of a PQL query being
 * vacuously true, when the query does not have any clauses.
 *
 * @return True if vacuously true, false otherwise.
 */
Boolean checkIfClausesEmpty(ClauseVector clauses)
{
    return clauses.count() == 0;
}

/*
 * Processes a single (such that, Follows) clause in a PQL query, with
 * respect to a given synonym. All results obtained from the clauses
 * will be with respect to that particular synonym. This method also
 * interacts with the PKB to obtain the results to the query.
 *
 * @param synonym The variable that is to be selected.
 * @param stClause The Follows clause to evaluate.
 * @param declarations Table containing a map of variables
 *                     to their design entity type.
 * @param isStar Whether the Follows clause comes with an
 *               asterisk, indicating the transitive closure.
 *
 * @return Results for the Synonym in the Follows clause.
 */
Vector<String> evaluateFollowsClause(const Synonym& synonym, SuchThatClause* stClause,
                                     const DeclarationTable& declarations, Boolean isStar)
{
    Vector<String> result;
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
    if (leftRefType == IntegerRefType && (rightRefType == SynonymRefType || rightRefType == WildcardRefType)) {
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
        result = convertToStringVect(tempResult);
    } else if (leftRefType == SynonymRefType && rightRefType == IntegerRefType) {
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
        result = convertToStringVect(tempResult);
    } else if (leftRefType == IntegerRefType && rightRefType == IntegerRefType) {
        /*
         * For this case, in iteration 1, the clause is perpertually not
         * related to the select synonym (because the wildcard to converted
         * to any StmtType, hence no checking needs to be done here (if
         * clause is related to select synonym).
         */
        Integer leftRefVal = std::stoi(leftRef.getValue());
        Integer rightRefVal = std::stoi(rightRef.getValue());
        Boolean followsHolds = (isStar ? checkIfFollowsHoldsStar : checkIfFollowsHolds)(leftRefVal, rightRefVal);
        if (followsHolds) {
            result.push_back("true");
        }
    } else if (leftRefType == SynonymRefType && rightRefType == SynonymRefType) {
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
        result = convertToStringVect(lookupPkbFunction(leftRefStmtType, rightRefStmtType));
    } else {
        throw std::runtime_error("Error in evaluateFollowsClause: No synonyms or integers in Follows clause");
    }
    return result;
}
