/**
 * ResultsTable is a class holding the results of
 * evaluating each clause in a SIMPLE program query.
 *
 * It provides functionality to narrow down a list
 * of results for multiple synonyms, whenever new
 * clauses are evaluated and new constraints known.
 */

#ifndef SPA_PQL_RESULTS_TABLE_H
#define SPA_PQL_RESULTS_TABLE_H

#include <unordered_map>

#include "EvaluatorUtils.h"

class ResultsTable {
private:
    std::unordered_map<Synonym, ClauseResult> resultsMap;
    const DeclarationTable& declarations;
    Boolean hasResult;

    Boolean checkIfSynonymInMap(const Synonym& syn);

public:
    /**
     * Constructor for a ResultsTable. The declarations table
     * is required so that the ResultsTable will know what
     * to return, in the event that a synonym is not
     * associated with any results.
     *
     * Since ResultsTable is meant for narrowing down results,
     * it will assume that a synonym not in the table has no
     * restrictions at all (i.e. can match all possible values).
     *
     * @param declarations Declarations of all synonyms in
     *                     the query.
     */
    explicit ResultsTable(const DeclarationTable& decls);

    /**
     * Associates some results with a synonym, if the synonym
     * does not already exist in the table. If the synonym
     * exists, and is mapped to some results, find the common
     * results in the table list and the new list provided,
     * and store the common results in the table list instead.
     * This effectively removes those results that are not in
     * both lists, from the association table.
     *
     * If the reference is not a synonym, do nothing (unless the
     * result list is empty). If the result list is empty,
     * invalidate the entire result table.
     *
     * @param ref Reference for a Design Entity to associate with.
     * @param results New results for this reference.
     */
    void filterTable(const Reference& ref, const ClauseResult& results);

    /**
     * Retrieves the list of results in the table for
     * a certain synonym. If the synonym does not exist
     * in the table, retrieve all possible values for
     * the synonym from the Program Knowledge Base.
     *
     * @param syn The synonym to look up.
     * @return List of results for the synonym.
     */
    ClauseResult get(const Synonym& syn);

    /**
     * Retrieves the type of synonym from the
     * declarations table.
     *
     * @param syn Synonym to look up.
     * @return The type of the synonym. If the synonym is
     *         not in the table, return NonExistentType.
     */
    DesignEntityType getTypeOfSynonym(const Synonym& syn);

    /**
     * Returns true if the result table is marked as having
     * no results at all, which happens if a clause returns
     * no results. This would ensure that the entire program
     * query returns no results.
     *
     * @return True, if so far all clauses have stored results
     *         in the table. False, if a certain clause stored
     *         an empty result.
     */
    Boolean hasResults() const;

    /**
     * Checks if a synonym has been restricted to only match a
     * subset of all possible values due to another clause's
     * results that were previously stored in the table.
     *
     * @return True, if synonym has been restricted.
     */
    Boolean checkIfSynonymHasConstraints(const Synonym& syn);
};

/**
 * Given two clause results, find common elements in
 * both of the clause results and return these
 * common elements in a single clause result.
 *
 * @param firstList The first clause result.
 * @param secondList The second clause result.
 * @return A single list of results.
 */
ClauseResult findCommonElements(const ClauseResult& firstList, const ClauseResult& secondList);

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
 * @return Results
 * representing the results, from
 * the vacuously true statement.
 */
ClauseResult retrieveAllMatching(DesignEntityType entTypeOfSynonym);

#endif // SPA_PQL_RESULTS_TABLE_H
