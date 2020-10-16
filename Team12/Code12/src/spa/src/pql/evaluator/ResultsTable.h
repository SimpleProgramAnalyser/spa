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
// Forward declaration of Evaluators
class AffectsEvaluator;
class NextEvaluator;

class ResultsTable {
private:
    std::unordered_map<Synonym, ResultsSet> resultsMap;
    DeclarationTable declarations;
    std::unique_ptr<RelationshipsGraph> relationships;
    EvaluationQueue queue;
    Boolean hasResult;
    Boolean hasEvaluated;
    // cache results for Next, Affects
    AffectsEvaluator* affectsEvaluator;
    NextEvaluator* nextEvaluator;

    Boolean checkIfSynonymInMap(const Synonym& syn) const;
    void filterAfterVerification(const Synonym& syn, const ClauseResult& results);
    ResultsSet findCommonElements(const ClauseResult& newResults, const Synonym& synonym);
    NtupledResult joinAllSynonyms(const Vector<Synonym>& syns);

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
    ClauseResult get(const Synonym& syn) const;

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
    ~ResultsTable();
    ResultsTable(const ResultsTable&) = delete;
    ResultsTable& operator=(const ResultsTable&) = delete;
    ResultsTable(ResultsTable&&) = delete;
    ResultsTable& operator=(ResultsTable&&) = delete;

    /**
     * A method to compare two ResultsTable for testing purposes.
     */
    bool operator==(const ResultsTable& rt) const;

    /**
     * A method to get the RelationshipsGraph stored inside
     * the ResultsTable, for testing purposes.
     *
     * @return The RelationshipsGraph stored inside.
     */
    RelationshipsGraph getRelationshipsGraph() const;

    /**
     * Returns the AffectsEvaluator stored within this ResultsTable.
     * This method may return a nullptr, if no AffectsEvaluator exists.
     *
     * @return The AffectsEvaluator storing cached results for
     *         Affects and Affects* specifically for the query
     *         that this ResultsTable is storing results for.
     */
    AffectsEvaluator* getAffectsEvaluator() const;

    /**
     * Returns the NextEvaluator stored within this ResultsTable.
     * This method may return a nullptr, if no NextEvaluator exists.
     *
     * @return The NextEvaluator storing cached results for
     *         Next* specifically for the query that this
     *         ResultsTable is storing results for.
     */
    NextEvaluator* getNextEvaluator() const;

    /**
     * Associates an AffectsEvaluator with this ResultsTable.
     * AffectsEvaluator will cache results for Affects, Affects* and
     * provide methods for the evaluation of Affects or Affects*.
     *
     * This evaluator should only persist for a single query,
     * similar to the ResultsTable, in order to make the SPA
     * scalable and compute Affects, Affects* on demand.
     *
     * The ResultsTable will handle deletion of the
     * AffectsEvaluator, once this method is called.
     *
     * @param affectsEval The AffectsEvaluator to manage.
     */
    Void manageEvaluator(AffectsEvaluator* affectsEval);

    /**
     * Associates a NextEvaluator with this ResultsTable.
     * NextEvaluator will cache results for Next* and
     * provide methods for the evaluation of Next, Next*.
     *
     * This evaluator should only persist for a single query,
     * similar to the ResultsTable, in order to make the SPA
     * scalable and compute Next* on demand.
     *
     * The ResultsTable will handle deletion of the
     * NextEvaluator, once this method is called.
     *
     * @param nextEval The NextEvaluator to manage.
     */
    Void manageEvaluator(NextEvaluator* nextEval);

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
     * Disassociates a certain value from a synonym in
     * the results table, if that value exists.
     *
     * @param synonym The synonym in the query.
     * @param value The result to eliminate.
     */
    Void eliminatePotentialValue(const Synonym& synonym, const String& value);

    /**
     * Retrieves the type of synonym from the
     * declarations table.
     *
     * @param syn Synonym to look up.
     * @return The type of the synonym. If the synonym is
     *         not in the table, return NonExistentType.
     */
    DesignEntityType getTypeOfSynonym(const Synonym& syn) const;

    /**
     * Checks if a synonym has been restricted to only match a
     * subset of all possible values due to another clause's
     * results that were previously stored in the table.
     *
     * @return True, if synonym has been restricted.
     */
    Boolean doesSynonymHaveConstraints(const Synonym& syn) const;

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
    Boolean hasRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym) const;

    /**
     * Forces the merging of the results queue.
     *
     * @return Whether this results table contains any results.
     */
    Boolean getResultsZero();

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
     * Initiates merging of the results queue, unless a certain
     * result in the queue was empty. Afterwards, returns the
     * results for all synonyms in the vector, as a vector
     * of n-tuples for each synonym. The order of the results
     * depends on the order of the synonyms.
     *
     * Note that this method assumes the synonyms vector to have
     * at least two synonyms (size > 1).
     *
     * @param syns The synonyms to get results for.
     *
     * @return The result n-tuples for (syns[0], syns[1], ..., syns[n]).
     */
    NtupledResult getResultsN(const Vector<Synonym>& syns);

    /**
     * Stores the result for a clause with no synonyms.
     * If true, nothing happens. But if false, the entire
     * results table is invalidated.
     *
     * @param hasResults Whether a clause has results.
     */
    Void storeResultsZero(Boolean hasResults);

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
    std::unordered_map<GraphEdge, std::unordered_set<SynonymWithValue, SynonymWithValueHasher>> edgesTable;
    GraphEdge edgesIndex = 0;
    /**
     * A set that keeps track of synonyms that were put into the graph.
     * This set will never decrease in size for the entire lifetime of
     * the RelationshipsGraph (synonyms will never be removed).
     */
    std::unordered_set<Synonym> synonymSet;
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
    bool hasRelationships(const PotentialValue& pv);

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
     */
    bool operator==(const RelationshipsGraph& rg) const;

    /**
     * A method to compare the structure of two RelationshipsGraphs.
     * This method ignores specific edge numbers.
     */
    bool compareStructure(const RelationshipsGraph& rg) const;

    /**
     * A method to add a synonym to the synonyms set, that keeps
     * track of which synonyms have already been encountered by
     * the RelationshipsGraph. Used for unit testing.
     */
    void addToSynonymSet(const Synonym& syn);

    /**
     * Adds a list of relationships between potential values
     * of certain synonyms. This method assumes that the
     * synonyms are not related (areValuesRelated returns false).
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
     * is deleted from the results table, to automatically
     * delete related values as well (if possible). Because
     * of this, deleteOne assumes pv no longer exists in
     * the resultsTable, and will not attempt to delete it.
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
     * Checks if a synonym has existed in the relationship
     * graph before (may not exist currently, however).
     *
     * @param syn The synonym to be checked.
     * @return True, if synonym has been entered into the
     *         graph before. Otherwise, false.
     */
    Boolean hasSeenBefore(const Synonym& syn);

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
    Boolean areValuesRelated(const PotentialValue& firstPv, const PotentialValue& secondPv);

    /**
     * For one potential value, checks whether it has an edge
     * with any potential values of a synonym.
     *
     * @param pv The potential value.
     * @param syn The synonym to be checked.
     * @return True, if pv has an edge to some
     *         potential value of synonym syn.
     */
    Boolean isValueRelated(const PotentialValue& pv, const Synonym& syn);

    /**
     * Retrieves all values related to a potential value
     * as stored in relationships table.
     *
     * @param value The potential value to look up.
     * @return List of all other potential values that are
     *         related to it.
     */
    std::vector<PotentialValue> retrieveRelationships(const PotentialValue& value) const;

    /**
     * Given a vector of synonym, retrieve all entries in the
     * RelationshipsGraph matching these synonyms, and return
     * them in the same order within a vector of rows.
     *
     * @param synonyms The synonyms to retrieve the rows of.
     * @return The result n-tuples for (syns[0], syns[1], ..., syns[n]).
     */
    NtupledResult retrieveUniqueRowsMatching(const Vector<Synonym>& synonyms) const;
};

#endif // SPA_PQL_RESULTS_TABLE_H
