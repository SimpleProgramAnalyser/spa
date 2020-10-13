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

#include <functional>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "EvaluatorUtils.h"

typedef std::queue<std::function<void()>> EvaluationQueue;
typedef std::unordered_set<String> ResultsSet;

// Forward declaration of RelationshipsGraph
class RelationshipsGraph;

class ResultsTable {
private:
    std::unordered_map<Synonym, ResultsSet> resultsMap;
    DeclarationTable declarations;
    std::unique_ptr<RelationshipsGraph> relationships;
    EvaluationQueue queue;
    Boolean hasResult;
    Boolean hasEvaluated;

    Boolean checkIfSynonymInMap(const Synonym& syn);
    void filterAfterVerification(const Synonym& syn, const ClauseResult& results);
    ResultsSet findCommonElements(const ClauseResult& newResults, const Synonym& synonym);

    /**
     * Creates a evaluation closure for one synonym's results.
     *
     * @return The evaluation closure.
     */
    static std::function<void()> createEvaluatorOne(ResultsTable* table, const Synonym& syn,
                                                    const ClauseResult& results);

    /**
     * Creates a evaluation closure for two linked synonym's results.
     *
     * @return The evaluation closure.
     */
    static std::function<void()> createEvaluatorTwo(ResultsTable* table, const Synonym& s1, const Synonym& s2,
                                                    const PairedResult& tuples);

    /**
     * Merges the results for one synonym for the ResultsTable provided.
     * This method assumes that the results are not empty.
     */
    static void mergeOneSynonym(ResultsTable* table, const Synonym& syn, const ClauseResult& results);

    /**
     * Merges the results for two synonyms for the ResultsTable provided.
     * This method assumes both results are not empty.
     */
    static void mergeTwoSynonyms(ResultsTable* table, const Synonym& s1, const Synonym& s2, const PairedResult& tuples);

    /**
     * Initiates merging of the results, if not yet merged.
     */
    void mergeResults();

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
     * Removes all relationships between the leftValue of synonym left
     * and the rightValue of synonym right.
     *
     * @param leftSyn The first synonym.
     * @param leftValue Value of the first synonym.
     * @param rightSyn The second synonym.
     * @param rightValue Value of the second synonym.
     */
    void disassociateRelationships(const Synonym& leftSyn, const String& leftValue, const Synonym& rightSyn,
                                   const String& rightValue);

    /**
     * Gets all registered relationships between two synonyms
     * in the relationships table. Note that if no such
     * relationships exist, there may be a runtime error!
     * Use hasRelationships to determine whether the
     * relationships exist before calling this method.
     *
     * @param leftSynonym Left synonym of the relationship.
     * @param rightSynonym Right synonym of the relationship.
     * @return Pairs of the possible (left, right) values in
     *         the relationships table.
     */
    PairedResult getRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym);

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
    ~ResultsTable() = default;
    ResultsTable(const ResultsTable&) = delete;
    ResultsTable& operator=(const ResultsTable&) = delete;
    ResultsTable(ResultsTable&&) = delete;
    ResultsTable& operator=(ResultsTable&&) = delete;

    /**
     * A method to compare two ResultsTable for testing purposes.
     */
    bool operator==(const ResultsTable& rt) const;

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
     * Adds a list of relationships between potential values
     * of certain synonyms. This method assumes the values are
     * new to the table and do not have any existing relations
     * in the relationships graph.
     *
     * If the references are not synonyms, do nothing.
     *
     * @param syn1 The reference that the first value
     *             corresponds to.
     * @param syn2 The reference that the second
     *             value corresponds to.
     * @param relationshipsPairs List of relationships between
     *                           potential values.
     */
    void associateRelationships(const Synonym& syn1, const Synonym& syn2,
                                const Vector<Pair<String, String>>& relationshipsPairs);

    /**
     * Disassociates a certain value from a synonym in
     * the results table, if that value exists.
     *
     * @param synonym The synonym in the query.
     * @param value The result to eliminate.
     */
    void eliminatePotentialValue(const Synonym& synonym, const String& value);

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
     * Checks if a synonym has been restricted to only match a
     * subset of all possible values due to another clause's
     * results that were previously stored in the table.
     *
     * @return True, if synonym has been restricted.
     */
    Boolean doesSynonymHaveConstraints(const Synonym& syn);

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
    Boolean hasRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym);

    /**
     * Forces the merging of the results queue.
     */
    Void getResultsZero();

    /**
     * Initiates merging of the results queue, unless
     * a certain result in the queue was empty. Afterwards,
     * returns the results for the synonym specified.
     *
     * @param syn The synonym to retrieve the results for.
     * @return The results for syn.
     */
    ClauseResult getResultsOne(const Synonym& syn);

    /**
     * Initiates merging of the results queue, unless a certain
     * result in the queue was empty. Afterwards, returns the
     * pairs of results for the two synonyms specified.
     *
     * @param syn1 First synonym to retrieve the results for.
     * @param syn2 Second synonym to retrieve the results for.
     * @return The result pairs for (syn1, syn2).
     */
    PairedResult getResultsTwo(const Synonym& syn1, const Synonym& syn2);

    /**
     * Adds the result for a single synonym into a queue.
     * The results in the queue will not be evaluated until
     * getResults is called.
     *
     * @param syn The synonym that the results belong to.
     * @param res Results for the synonym syn.
     */
    Void storeResultsOne(const Synonym& syn, const ClauseResult& res);

    /**
     * Adds the result for a single synonym into a queue.
     * The results in the queue will not be evaluated until
     * getResults is called.
     *
     * @param rfc The reference that the results were evaluated
     *            for. If this reference is not a synonym, the
     *            results will just be checked for emptiness.
     * @param res Results for the reference rfc.
     */
    Void storeResultsOne(const Reference& rfc, const ClauseResult& res);

    /**
     * Adds the result for two synonyms into a queue.
     * The results in the queue will not be evaluated until
     * getResults is called. If any of the references are
     * not synonyms, this method may call storeResultsOne
     * instead.
     *
     * @param rfc1 First reference in the clause.
     * @param res1 Results for the reference rfc1.
     * @param rfc2 Second reference in the clause.
     * @param res2 Results for the reference rfc2.
     * @param tuples Pairs of results for the first reference
     *               and the second reference.
     */
    Void storeResultsTwo(const Reference& rfc1, const ClauseResult& res1, const Reference& rfc2,
                         const ClauseResult& res2, const PairedResult& tuples);

    /**
     * Adds the result for two synonyms into a queue.
     * The results in the queue will not be evaluated until
     * getResults is called. If the reference is not of synonym
     * type, this method may call storeResultsOne instead.
     *
     * @param syn First synonym in the clause.
     * @param resSyn Results for the synonym syn.
     * @param ref Second reference in the clause.
     * @param tuples Pairs of results for the first synonym
     *               and the second reference.
     */
    Void storeResultsTwo(const Synonym& syn, const ClauseResult& resSyn, const Reference& ref,
                         const PairedResult& tuples);

    /**
     * Adds the result for two synonyms into a queue.
     * The results in the queue will not be evaluated until
     * getResults is called.
     *
     * @param syn1 First synonym in the clause.
     * @param syn2 Second synonym in the clause.
     * @param tuples Pairs of results for the first synonym
     *               and the second synonym.
     */
    Void storeResultsTwo(const Synonym& syn1, const Synonym& syn2, const PairedResult& tuples);
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

typedef Integer GraphEdge;
class TableUpdate;
class UpdatesQueue;

/**
 * A helper class for results table, the Relationships
 * Graph keeps track of relationships between possible
 * values of synonyms in the query.
 */
class RelationshipsGraph {
private:
    std::unordered_map<PotentialValue, std::unordered_set<GraphEdge>, PotentialValueHasher> valuesTable;
    std::unordered_map<GraphEdge, std::unordered_set<PotentialValue, PotentialValueHasher>> edgesTable;
    GraphEdge edgesIndex = 0;
    /**
     * If a relationship between two synonyms is found in the cache, then
     * it is certain that their potential values have some relationship
     * within relationshipsTable. If it is not found, a manual search of
     * relationshipsTable is required.
     */
    std::unordered_map<Synonym, std::unordered_set<Synonym>> synonymRelationshipsCache;
    // Allow ValuesTablesUpdates to access the valuesTable
    friend class ValuesTableDelete;
    friend class ValuesTableInsert;
    friend class ValuesTableInsertNewest;
    friend class ValuesTableForceInsertNewest;
    friend class ValuesTableNewSet;
    friend class EdgesTableDelete;
    friend class EdgesTableDeleteEdge;
    friend class EdgesTableInsert;
    friend class EdgesTableInsertToNewest;
    friend class EdgesTableNewSet;

    static bool associateTwoExisting(const RelationshipsGraph& graph, const PotentialValue& firstKey,
                                     const PotentialValue& secondKey, UpdatesQueue& updates);
    static bool associateOneExisting(const RelationshipsGraph& graph, const PotentialValue& existingKey,
                                     const PotentialValue& newKey, UpdatesQueue& updates);
    static bool associateOneExistingSwapped(const RelationshipsGraph& graph, const PotentialValue& newKey,
                                            const PotentialValue& existingKey, UpdatesQueue& updates);
    static bool associateZeroExisting(const RelationshipsGraph& graph, const PotentialValue& firstKey,
                                      const PotentialValue& secondKey, UpdatesQueue& updates);
    Boolean checkIfPotentialValueHasRelationships(const PotentialValue& pv);

public:
    RelationshipsGraph() = default;

    /**
     * Constructs RelationshipsGraph with a predefined set of edges,
     * and a value for the edgeIndex counter. Used for unit testing.
     */
    explicit RelationshipsGraph(const std::vector<std::pair<GraphEdge, std::vector<PotentialValue>>>& edges,
                                GraphEdge currentEdgeIndex);

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
     * of certain synonyms. This method assumes that the
     * synonyms are not related (checkIfRelated returns false).
     *
     * Doing this insertion may cause certain values to be discarded
     * from the ResultsTable.
     *
     * @param valueRelationships The pairs representing relationships
     *                           between values of the first synonym
     *                           and values of the second synonym.
     * @param firstSynonym The synonym whose results are the first
     *                     element of the pairs in valueRelationships.
     * @param firstIsNew Whether the first synonym has results in
     *                   the ResultsTable already.
     * @param secondSynonym The synonym whose results are the second
     *                      element of the pairs in valueRelationships.
     * @param secondIsNew Whether the second synonym has results in
     *                    the ResultsTable already.
     *
     * @return A pair of valid values for synonym 1 and synonym 2.
     */
    Pair<Vector<String>, Vector<String>> insertRelationships(const Vector<Pair<String, String>>& valueRelationships,
                                                             const Synonym& firstSynonym, bool firstIsNew,
                                                             const Synonym& secondSynonym, bool secondIsNew);

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
    void deleteOne(const PotentialValue& pv, ResultsTable* resultsTable);

    /**
     * Deletes any relations stored that involves both
     * the firstKey and secondKey. This will also attempt
     * to clear related values from a results table.
     *
     * @param firstKey First potential value in the relation.
     * @param secondKey Second potential value in the relation.
     * @param resultsTable The results table to update, if related
     *                     synonyms can be removed as well
     */
    void deleteTwo(const PotentialValue& firstKey, const PotentialValue& secondKey, ResultsTable* resultsTable);

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
     * Checks if a synonym has any potential values
     * in the relationships graph.
     *
     * @param syn The synonym to be checked.
     * @return True, if synonym has some potential values.
     *         Otherwise, false.
     */
    Boolean checkIfSynonymInRelationshipsGraph(const Synonym& syn);

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
