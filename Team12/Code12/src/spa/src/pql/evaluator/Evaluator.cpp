/**
 * Implemenetation of the PQL query evaluator.
 */

#include "Evaluator.h"

#include "pkb/PKB.h"
#include "pql/preprocessor/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

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
RawQueryResult Evaluator::evaluateQuery(AbstractQuery query)
{
    /*
     * First check if PQL query is semantically valid.
     * If invalid, don't continue evaluating query.
     */
    if (query.isInvalid()) {
        return RawQueryResult::emptyRawQueryResult();
    }

    /*
     * Otherwise, at this point, the PQL query,
     * should be semantically (and of course
     * syntatically) valid.
     */

    Boolean queryVacuouslyTrue = isQueryVacuouslyTrue(query);

    RawQueryResult result = processQuery(query);

    /*
     * If query is vacuously true, retrieve all entity
     * related to that synonym, from Tables API
     */

    return result;
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
RawQueryResult Evaluator::processQuery(AbstractQuery query)
{
    Vector<Vector<Vector<String>>> results;

    /*
     * Currently, there is only exactly 1 synonym,
     * in the PQL query, so we will only need to iterate through
     * all the clauses once.
     *
     * Had there been a tuple of synonyms <a, p, s>,
     * where a, p, and s, are assignment, procedures
     * and statements respectively, iterate through
     * all synonyms and call processQueryClauses(..).
     */
    Synonym synonym = query.getSelectSynonym();
    ClauseVector clauses = query.getClauses();
    DeclarationTable declarations = query.getDeclarationTable();

    Vector<Vector<String>> result = processQueryClauses(synonym, clauses, declarations);

    results.push_back(result);

    RawQueryResult rawQueryResult(results);

    return rawQueryResult;
}

/*
 * Processes (all) the clauses in a PQL query, with respect to
 * a given synonym. All results obtained from the clauses will be
 * with respect to that particular synonym.
 *
 * For instance, given the following PQL query;
 *     assign a; statement s;
 *     Select a such that Follows(s, a) pattern a(_, "3+5")
 *
 * All clauses in the PQL, namely the Follows and Pattern
 * clauses would be procdessed, with respect to the
 * assignment synonym a. That is, all results returned
 * would be matching assignment statements.
 *
 * If instead, the PQL query was the following;
 *     statement s1; statement a;
 *     Select s such that Follows(s1, a) pattern a(_, "3+5")
 *
 * Then similarly, all clauses in the PQL, would be
 * processed, however results returned would be matching
 * (generic) statements.
 *
 * This method also interacts with the PKB, if needed
 * to obtain the reuslts to the query.
 *
 * @param synonym The synonym by which clauses would be
 * evaluated, with respect to..
 * @param clauses The clauses to evaluate.
 * @param declarations A table containing a map of declarations (variables)
 * to their design entity (might come in handy for processing query).
 *
 * @return Vector<Vector<String>>, where each Vector<String>
 * element corresponds to the results from all clauses in the query
 * (please read the documentation of RawQueryResult for more details),
 * evaluated with respect to a particular synonym.
 */
Vector<Vector<String>> Evaluator::processQueryClauses(Synonym synonym, ClauseVector clauses,
                                                      DeclarationTable declarations)
{
    Vector<Vector<String>> results;

    for (int i = 0; i < clauses.totalNumClauses(); ++i) {
        Clause* clause = clauses.get(i);

        Vector<String> result = processQueryClause(synonym, clause, declarations);

        results.push_back(result);
    }

    return results;
}

/*
 * Processes a single clause in a PQL query, with respect to
 * a given synonym. All results obtained from the clauses will be
 * with respect to that particular synonym.
 *
 * This method also interacts with the PKB, if needed
 * to obtain the results to the query.
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
Vector<String> Evaluator::processQueryClause(Synonym synonym, Clause* clause, DeclarationTable declarations)
{
    Vector<String> result;

    if (clause->getType() == SuchThatClauseType) {
        SuchThatClause* stClause = dynamic_cast<SuchThatClause*>(clause);

        result = processQuerySuchThatClause(synonym, stClause, declarations);

    } else if (clause->getType() == PatternClauseType) {
    }

    return result;
}

/*
 * Processes a single (such that) clause in a PQL query, with
 * respect to a given synonym. All results obtained from
 * the clauses will be with respect to that particular synonym.
 *
 * This method also interacts with the PKB, if needed
 * to obtain the reuslts to the query.
 *
 * @param synonym The synonym by which the clause would be
 * evaluated, with respect to..
 * @param stClause The SuchThatClause to evaluate.
 * @param declarations A table containing a map of declarations (variables)
 * to their design entity (might come in handy for processing query).
 *
 * @return Vector<String>, which represents the results from only 1
 * particular (such that) clause in the query (please read the
 * documentation of RawQueryResult for more details) evaluated
 * with respect to a particular synonym.
 */
Vector<String> Evaluator::processQuerySuchThatClause(Synonym synonym, SuchThatClause* stClause,
                                                     DeclarationTable declarations)
{
    Relationship rel = stClause->getRelationship();
    RelationshipReferenceType relRefType = rel.getRelationship();

    Vector<String> result;

    if (relRefType == FollowsType) {
        result = processQuerySuchThatFollowsClause(synonym, stClause, declarations);
    } else if (relRefType == FollowsStarType) {
    } else if (relRefType == ParentType) {
    } else if (relRefType == ParentStarType) {
    } else if (relRefType == UsesType) {
    } else if (relRefType == UsesStatementType) {
    } else if (relRefType == UsesProcedureType) {
    } else if (relRefType == ModifiesType) {
    } else if (relRefType == ModifiesStatementType) {
    } else if (relRefType == ModifiesProcedureType) {
    }

    /*if (relationship.
    Reference leftRef = relationship.getLeftRef();
    Reference rightRef = relationship.getRightRef();*/

    return result;
}

/*
 * Processes a single (such that, Follows) clause in a PQL query, with
 * respect to a given synonym. All results obtained from
 * the clauses will be with respect to that particular synonym.
 *
 * This method also interacts with the PKB, if needed
 * to obtain the reuslts to the query.
 *
 * @param synonym The synonym by which the clause would be
 * evaluated, with respect to..
 * @param stClause The SuchThatClause (Follows type) to
 * evaluate.
 * @param declarations A table containing a map of declarations (variables)
 * to their design entity (might come in handy for processing query).
 *
 * @return Vector<String>, which represents the results from only 1
 * particular (such that, Follows) clause in the query (please read the
 * documentation of RawQueryResult for more details) evaluated
 * with respect to a particular synonym.
 */
Vector<String> Evaluator::processQuerySuchThatFollowsClause(Synonym synonym, SuchThatClause* stClause,
                                                            DeclarationTable declarations)
{
    Relationship rel = stClause->getRelationship();
    RelationshipReferenceType relRefType = rel.getRelationship();

    Vector<String> result;

    Reference leftRef = rel.getLeftRef();
    Reference rightRef = rel.getRightRef();

    /*
     * For follows, we can have _ as the first argument,
     * e.g, Follows(_, 3).
     *
     * Additionally, like other relationships, we can have
     * the _ wildcard, as the second arugment too.
     */
    if (leftRef.isWildCard()) {
    } else if (rightRef.isWildCard()) {
    }

    DesignEntity leftRefDesignEntity = declarations.getDesignEntityOfSynonym(leftRef.getValue());
    DesignEntity rightRefDesignEntity = declarations.getDesignEntityOfSynonym(rightRef.getValue());

    DesignEntityType leftRefDesignEntityType = leftRefDesignEntity.getType();
    DesignEntityType rightRefDesignEntityType = rightRefDesignEntity.getType();

    /*
     * Here there are many combinations/possibilities, of the left and
     * right operands design entity types and we shall (exhaustively)
     * handle them one-by-one (via a bunch of if-else).
     *
     * Upon getting the right combination, we will then call the
     * PKB API to get the results.
     */

    if (leftRefDesignEntityType == StmtType && rightRefDesignEntityType == AssignType) {
        Vector<Integer> tempResult = getAllBeforeStatementsTyped(AnyStatement, AssignmentStatement);
        tempResult.push_back(12345);
        result = convertToStringVect(tempResult);
    }

    return result;
}

/*
 * A (private) Utility method to convert an integer vector to a string
 * vector.
 *
 * @param intList An integer vector to convert.
 *
 * @return Vector<String> the converted vector.
 */
Vector<String> Evaluator::convertToStringVect(Vector<Integer> intList)
{
    Vector<String> strList;

    Integer len = intList.size();

    for (int i = 0; i < len; ++i) {
        int elem = intList.at(i);

        String str = std::to_string(elem);

        strList.push_back(str);
    }

    return strList;
}

/*
 * Checks if the PQL query is vacously true,
 * where the synonym (in select) is not directly
 * used in the clauses. An example of such queries
 * include; variable v; statement s; Select v such
 * that Follows(3, s).
 *
 * @return True if vacuously true, false otherwise.
 */
Boolean Evaluator::isQueryVacuouslyTrue(AbstractQuery query)
{
    Synonym selectSynonym = query.getSelectSynonym();
    ClauseVector clauses = query.getClauses();
    DeclarationTable declarations = query.getDeclarationTable();

    /*
     * Iterate through all clauses, one of which must at least
     * have be related directly to the select synonym.
     */
    for (int i = 0; i < clauses.totalNumClauses(); ++i) {
        Clause* clause = clauses.get(i);

        if (clause->getType() == SuchThatClauseType) {
            SuchThatClause* stClause = dynamic_cast<SuchThatClause*>(clause);

            Relationship rel = stClause->getRelationship();

            ReferenceValue leftVal = rel.getLeftRef().getValue();
            ReferenceValue rightVal = rel.getRightRef().getValue();

            /*
             * If any synonym matches select synonym, we terminate
             * early, knowing that query is indeed not vacuously true.
             */
            if (leftVal == selectSynonym || rightVal == selectSynonym) {
                return false;
            }
        } else if (clause->getType() == PatternClauseType) {
            PatternClause* pClause = dynamic_cast<PatternClause*>(clause);

            ReferenceValue val = pClause->getEntRef().getValue();

            /*
             * If the synonym matches select synonym, we terminate
             * early, knowing that query is indeed not vacuously true.
             */
            if (val == selectSynonym) {
                return false;
            }
        }
    }

    // Else the query is vacuously true.
    return true;
}
