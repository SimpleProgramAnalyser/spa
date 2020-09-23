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
     * A method to compare two ResultsTable for testing purposes.
     */
    bool operator==(const ResultsTable& rt) const;

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

    /**
     * Checks the relationship table for two synonyms, to
     * see whether there is any relationships between the
     * potential values of the two.
     *
     * @param leftSynonym Left synonym to be checked.
     * @param rightSynonym Right synonym to be checked.
     * @return True, if some clause has restricted the
     *         left and right to certain relationships.
     */
    Boolean checkIfHaveRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym);

    /**
     * Gets all registered relationships between two synonyms
     * in the relationships table. Note that if no such
     * relationships exist, there may be a runtime error!
     * Use checkIfHaveRelationships to determine whether
     * the relationships exist before calling this method.
     *
     * @param leftSynonym Left synonym of the relationship.
     * @param rightSynonym Right synonym of the relationship.
     * @return Pairs of the possible (left, right) values in
     *         the relationships table.
     */
    std::vector<std::pair<String, String>> getRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym);
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
    /**
     * If a relationship between two synonyms is found in the cache, then
     * it is certain that their potential values have some relationship
     * within relationshipsTable. If it is not found, a manual search of
     * relationshipsTable is required.
     */
    std::unordered_map<Synonym, std::unordered_set<Synonym>> synonymRelationshipsCache;

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
        // store the relationships in cache
        if (!valueRelationships.empty()) {
            if (synonymRelationshipsCache.find(firstSynonym) == synonymRelationshipsCache.end()) {
                synonymRelationshipsCache.insert(
                    std::pair<Synonym, std::unordered_set<Synonym>>(firstSynonym, std::unordered_set<Synonym>()));
            }
            synonymRelationshipsCache[firstSynonym].insert(secondSynonym);
            if (synonymRelationshipsCache.find(secondSynonym) == synonymRelationshipsCache.end()) {
                synonymRelationshipsCache.insert(
                    std::pair<Synonym, std::unordered_set<Synonym>>(secondSynonym, std::unordered_set<Synonym>()));
            }
            synonymRelationshipsCache[secondSynonym].insert(firstSynonym);
        }
    }

public:
    /**
     * A method to compare two RelationshipsGraph for testing purposes.
     * This method ignores differences in the synonym cache.
     */
    bool operator==(const RelationshipsGraph& rg) const;

    /**
     * A method to compare two RelationshipsGraph for testing purposes.
     * This method compares the synonym cache of the graph as well.
     */
    bool checkEqualIncludingCache(const RelationshipsGraph& rg) const;

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

    /**
     * Checks whether two synonyms have relationships in the
     * relationships table cache. If this returns false, the
     * two synonyms could still have a relationship, but it
     * would require iterating over the entire relationship
     * table to determine. If this returns true, then it is
     * certain that firstSynonym and secondSynonym have some
     * potential values that affect each other.
     *
     * @param firstSynonym The first synonym to be checked.
     * @param secondSynonym The second synonym to be checked.
     *
     * @return True, if it is certain that firstSynonym and
     *         secondSynonym has relationships in the graph.
     *         If it is uncertain, but not impossible, false.
     */
    Boolean checkCachedRelationships(const Synonym& firstSynonym, const Synonym& secondSynonym);

    /**
     * Checks if two potential values are related in the
     * relationships table.
     *
     * @param firstPv The first potential value.
     * @param secondPv The second potential value.
     * @return True, if both values exist in the table and
     *         there is a relationship between them.
     *         Otherwise, false.
     */
    Boolean checkIfRelated(const PotentialValue& firstPv, const PotentialValue& secondPv);

    /**
     * Retrieves all values related to a potential value
     * as stored in relationships table.
     *
     * @param value The potential value to look up.
     * @return List of all other potential values that are
     *         related to it.
     */
    std::vector<PotentialValue> retrieveRelationships(const PotentialValue& value);
};

#endif // SPA_PQL_RESULTS_TABLE_H
