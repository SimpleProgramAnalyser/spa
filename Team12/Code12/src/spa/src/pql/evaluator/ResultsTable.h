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
#include <unordered_set>

#include "EvaluatorUtils.h"

// Forward declaration of RelationshipsGraph
class RelationshipsGraph;

class ResultsTable {
private:
    std::unordered_map<Synonym, ClauseResult> resultsMap;
    DeclarationTable declarations;
    std::unique_ptr<RelationshipsGraph> relationships;
    Boolean hasResult;

    Boolean checkIfSynonymInMap(const Synonym& syn);
    void filterAfterVerification(const Synonym& syn, const ClauseResult& results);
    ClauseResult findCommonElements(const ClauseResult& newResults, const Synonym& synonym);

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
    explicit ResultsTable(DeclarationTable decls);

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
     * @param results New results for this reference. This list
     *                can contain duplicates, but the duplicate
     *                values will be removed eventually.
     */
    void filterTable(const Reference& ref, const ClauseResult& results);

    /**
     * Associates some results with a synonym, if the synonym
     * does not already exist in the table. If the synonym
     * exists, and is mapped to some results, find the common
     * results in the table list and the new list provided,
     * and store the common results in the table list instead.
     * This effectively removes those results that are not in
     * both lists, from the association table.
     *
     * @param ref Synonym for a Design Entity to associate with.
     * @param results New results for this reference. This list
     *                can contain duplicates, but the duplicate
     *                values will be removed eventually.
     */
    void filterTable(const Synonym& syn, const ClauseResult& results);

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

    /**
     * Disassociates a certain value from a synonym in
     * the results table, if that value exists.
     *
     * @param synonym The synonym in the query.
     * @param value The result to eliminate.
     */
    void eliminatePotentialValue(const Synonym& synonym, const String& value);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms. ResultsTable assumes that these
     * values already exist in the results table. If the values
     * do not exist, the behaviour of other methods is undefined.
     *
     * If the references are not synonyms, do nothing.
     *
     * @param valueRelationships List of relationships between
     *                           potential values.
     * @param leftRef The reference that the first value
     *                     corresponds to.
     * @param rightRef The reference that the second
     *                     value corresponds to.
     */
    void associateRelationships(Vector<Pair<String, String>> valueRelationships, const Reference& leftRef,
                                const Reference& rightRef);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms. ResultsTable assumes that these
     * values already exist in the results table. If the values
     * do not exist, the behaviour of other methods is undefined.
     *
     * If the references are not synonyms, do nothing.
     *
     * @param valueRelationships List of relationships between
     *                           potential values.
     * @param leftRef The reference that the first value
     *                     corresponds to.
     * @param rightRef The reference that the second
     *                     value corresponds to.
     */
    void associateRelationships(Vector<Pair<Integer, String>> valueRelationships, const Reference& leftRef,
                                const Reference& rightRef);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms. ResultsTable assumes that these
     * values already exist in the results table. If the values
     * do not exist, the behaviour of other methods is undefined.
     *
     * If the references are not synonyms, do nothing.
     *
     * @param valueRelationships List of relationships between
     *                           potential values.
     * @param leftRef The reference that the first value
     *                     corresponds to.
     * @param rightRef The reference that the second
     *                     value corresponds to.
     */
    void associateRelationships(Vector<Pair<String, Integer>> valueRelationships, const Reference& leftRef,
                                const Reference& rightRef);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms. ResultsTable assumes that these
     * values already exist in the results table. If the values
     * do not exist, the behaviour of other methods is undefined.
     *
     * If the references are not synonyms, do nothing.
     *
     * @param valueRelationships List of relationships between
     *                           potential values.
     * @param leftRef The reference that the first value
     *                     corresponds to.
     * @param rightRef The reference that the second
     *                     value corresponds to.
     */
    void associateRelationships(Vector<Pair<Integer, Integer>> valueRelationships, const Reference& leftRef,
                                const Reference& rightRef);
};

/*
 * Given a synonym type, retrieve all entity objects related to the
 * design entity type of the synonym (e.g, statement, while loops,
 * procedures, etc).
 *
 * @param entTypeOfSynonym The design entity type of the synonym.
 *
 * @return ClauseResult representing the results, from
 *         the vacuously true statement.
 */
ClauseResult retrieveAllMatching(DesignEntityType entTypeOfSynonym);

// Identity function for strings
inline String stringId(String str)
{
    return str;
}

/**
 * A helper class for results table, the Relationships
 * Graph keeps track of relationships between possible
 * values of synonyms in the query.
 */
class RelationshipsGraph {
private:
    std::unordered_map<PotentialValue, std::unordered_set<PotentialValue, PotentialValueHasher>, PotentialValueHasher>
        relationshipsTable;

    void associate(const PotentialValue& firstKey, const PotentialValue& secondKey);
    void deleteEdge(const PotentialValue& firstKey, const PotentialValue& secondKey);
    Boolean checkIfPotentialValueHasRelationships(const PotentialValue& pv);

    template <typename T, typename U, Value (*firstToString)(T), Value (*secondToString)(U)>
    void insert(Vector<Pair<T, U>> valueRelationships, const Synonym& firstSynonym, const Synonym& secondSynonym)
    {
        for (Pair<T, U> value : valueRelationships) {
            PotentialValue firstKey(firstSynonym, firstToString(value.first));
            PotentialValue secondKey(secondSynonym, secondToString(value.second));
            associate(firstKey, secondKey);
        }
    }

public:
    /**
     * Adds a list of relationships between potential values
     * of certain synonyms.
     */
    void insertRelationships(Vector<Pair<String, String>> valueRelationships, const Synonym& firstSynonym,
                             const Synonym& secondSynonym);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms.
     */
    void insertRelationships(Vector<Pair<String, Integer>> valueRelationships, const Synonym& firstSynonym,
                             const Synonym& secondSynonym);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms.
     */
    void insertRelationships(Vector<Pair<Integer, String>> valueRelationships, const Synonym& firstSynonym,
                             const Synonym& secondSynonym);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms.
     */
    void insertRelationships(Vector<Pair<Integer, Integer>> valueRelationships, const Synonym& firstSynonym,
                             const Synonym& secondSynonym);

    /**
     * Deletes a potential value from the graph, as well as
     * all edges associated with it. Update the result
     * table if other potential values can no longer exist.
     *
     * This method is to be called whenever a potential value
     * is filtered out from the results table, to automatically
     * delete related values as well (if possible).
     *
     * @param pv The potential value to delete.
     * @param resultsTable The results table to update, if related
     *                     synonyms can be removed as well
     */
    void deleteFromGraph(const PotentialValue& pv, ResultsTable* resultsTable);
};

#endif // SPA_PQL_RESULTS_TABLE_H
