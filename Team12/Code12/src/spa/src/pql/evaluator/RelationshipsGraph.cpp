/**
 * Implementation of Relationships Graph for
 * Query Evaluator's Result Table.
 */
#include <algorithm>
#include <stdexcept>
#include <utility>

#include "ResultsTable.h"

//========================================================================
// HELPER CLASSES FOR RELATIONSHIPS GRAPH
//========================================================================
/**
 * A class that holds a instruction to update the values table
 * or edges table. This is used by the associate methods because
 * in-place updating could result in edges that are incorrect.
 */
class TableUpdate {
protected:
    PotentialValue value;
    GraphEdge edge;

    TableUpdate(PotentialValue valueToUpdate, GraphEdge edgeToUpdate):
        value(std::move(valueToUpdate)), edge(edgeToUpdate)
    {}

public:
    virtual ~TableUpdate() = default;
    TableUpdate(const TableUpdate&) = default;
    TableUpdate& operator=(const TableUpdate&) = default;
    TableUpdate(TableUpdate&&) = default;
    TableUpdate& operator=(TableUpdate&&) = default;

    /**
     * Executes the update to valuesTable or edgesTable of the relationships graph.
     */
    virtual void operator()(RelationshipsGraph& graph) const = 0;
};

class ValuesTableDelete: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to delete an edge from valuesTable
     * for a given potential value.
     *
     * This deletion can be performed on non-existent
     * edges and it will not cause any runtime errors.
     * However, the value must exist.
     *
     * @param valueToUpdate The PotentialValue to update.
     * @param edgeToUpdate The edge to delete from valuesTable.
     */
    ValuesTableDelete(PotentialValue valueToUpdate, GraphEdge edgeToUpdate):
        TableUpdate(std::move(valueToUpdate), edgeToUpdate)
    {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.valuesTable[value].erase(edge);
    }
};

class ValuesTableInsert: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to insert an edge into valuesTable
     * for a given potential value.
     *
     * @param valueToUpdate The PotentialValue to update.
     * @param edgeToUpdate The edge to insert into valuesTable.
     */
    ValuesTableInsert(PotentialValue valueToUpdate, GraphEdge edgeToUpdate):
        TableUpdate(std::move(valueToUpdate), edgeToUpdate)
    {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.valuesTable[value].insert(edge);
    }
};

class ValuesTableInsertNewest: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to insert the latest edge into
     * valuesTable for a given potential value.
     * The latest edge created is retrieved by taking
     * the current edgeIndex and decreasing it by one.
     *
     * @param valueToUpdate The PotentialValue to update.
     */
    explicit ValuesTableInsertNewest(PotentialValue valueToUpdate): TableUpdate(std::move(valueToUpdate), -1) {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.valuesTable[value].insert(graph.edgesIndex - 1);
    }
};

class ValuesTableForceInsertNewest: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to insert the latest edge into
     * valuesTable for a given potential value.
     * The latest edge created is retrieved by taking
     * the current edgeIndex and decreasing it by one.
     *
     * If the value does not already exist in the valuesTable,
     * a new set is created first (equivalent to ValuesTableNewSet
     * followed by ValuesTableInsertNewest).
     *
     * @param valueToUpdate The PotentialValue to update.
     */
    explicit ValuesTableForceInsertNewest(PotentialValue valueToUpdate): TableUpdate(std::move(valueToUpdate), -1) {}

    void operator()(RelationshipsGraph& graph) const override
    {
        if (graph.valuesTable.find(value) == graph.valuesTable.end()) {
            graph.valuesTable.insert(
                std::pair<PotentialValue, std::unordered_set<GraphEdge>>(value, std::unordered_set<GraphEdge>()));
        }
        graph.valuesTable[value].insert(graph.edgesIndex - 1);
    }
};

class ValuesTableNewSet: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to create a new set in valuesTable
     * for a given potential value.
     *
     * @param valueToUpdate The PotentialValue to update.
     */
    explicit ValuesTableNewSet(PotentialValue valueToUpdate): TableUpdate(std::move(valueToUpdate), -1) {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.valuesTable.insert(
            std::pair<PotentialValue, std::unordered_set<GraphEdge>>(value, std::unordered_set<GraphEdge>()));
    }
};

class EdgesTableDelete: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to delete a value from edgesTable
     * for a given edge number.
     *
     * This deletion can be performed on non-existent
     * values and it will not cause any runtime errors.
     * However, the edge must exist.
     *
     * @param edgeToUpdate The edge to update.
     * @param valueToUpdate The PotentialValue to
     *                      delete from edgesTable.
     */
    EdgesTableDelete(GraphEdge edgeToUpdate, PotentialValue valueToUpdate):
        TableUpdate(std::move(valueToUpdate), edgeToUpdate)
    {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.edgesTable[edge].erase(value.asSwv());
    }
};

class EdgesTableDeleteEdge: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to delete an entire edge from
     * edgesTable, given the edge number.
     *
     * This deletion can be performed on non-existent
     * edges and it will not cause any runtime errors.
     *
     * @param edgeToUpdate The edge to delete
     *                     from edgesTable
     */
    explicit EdgesTableDeleteEdge(GraphEdge edgeToUpdate): TableUpdate(PotentialValue("", ""), edgeToUpdate) {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.edgesTable.erase(edge);
    }
};

class EdgesTableInsert: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to insert a value into edgesTable
     * for a given edge number.
     *
     * @param edgeToUpdate The edge to update.
     * @param valueToUpdate The PotentialValue to
     *                      insert into edgesTable.
     */
    EdgesTableInsert(GraphEdge edgeToUpdate, PotentialValue valueToUpdate):
        TableUpdate(std::move(valueToUpdate), edgeToUpdate)
    {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.edgesTable[edge].insert(value.asSwv());
    }
};

class EdgesTableInsertToNewest: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to insert a value into edgesTable
     * for the latest edge created. The latest edge
     * created is retrieved by taking the current
     * edgeIndex and decreasing it by one.
     *
     * @param valueToUpdate The PotentialValue to
     *                      insert into edgesTable.
     */
    explicit EdgesTableInsertToNewest(PotentialValue valueToUpdate): TableUpdate(std::move(valueToUpdate), -1) {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.edgesTable[graph.edgesIndex - 1].insert(value.asSwv());
    }
};

class EdgesTableNewSet: public TableUpdate {
public:
    /**
     * Constructs a TableUpdate representing an
     * instruction to create a new set in edgesTable.
     * The number of the edge used will be the current
     * value of edgesIndex, and edgesIndex will be
     * incremented after the new set is added.
     *
     * @param valueToUpdate The PotentialValue to update.
     */
    EdgesTableNewSet(): TableUpdate(PotentialValue("", ""), -1) {}

    void operator()(RelationshipsGraph& graph) const override
    {
        graph.edgesTable.insert(std::pair<GraphEdge, std::unordered_set<SynonymWithValue, SynonymWithValueHasher>>(
            graph.edgesIndex, std::unordered_set<SynonymWithValue, SynonymWithValueHasher>()));
        graph.edgesIndex++;
    }
};

/**
 * A class that holds a queue of TableUpdate instructions.
 */
class UpdatesQueue {
public:
    std::queue<std::unique_ptr<TableUpdate>> queue;

    template <class T>
    void push(const T& instr)
    {
        queue.push(std::unique_ptr<T>(new T(instr)));
    }

    bool empty() const
    {
        return queue.empty();
    }

    const std::unique_ptr<TableUpdate>& front() const
    {
        return std::move(queue.front());
    }

    void pop()
    {
        queue.pop();
    }
};

//========================================================================
// RELATIONSHIPS GRAPH INTERNAL METHODS
//========================================================================

/**
 * Associates two values of synonyms that already exist
 * in the graph, but have not been linked with each other.
 * A cross join will be performed in this case.
 *
 * @param graph The RelationshipsGraph to look up.
 * @param firstKey The first value to be added.
 * @param secondKey The second value to be added.
 * @param updates Queue that stores the required
 *                updates to the valuesTable.
 *
 * @returns True, if the association was successful. Otherwise, false.
 */
bool RelationshipsGraph::associateTwoExisting(const RelationshipsGraph& graph, const PotentialValue& firstKey,
                                              const PotentialValue& secondKey, UpdatesQueue& updates)
{
    if (graph.valuesTable.find(firstKey) == graph.valuesTable.end()
        || graph.valuesTable.find(secondKey) == graph.valuesTable.end()) {
        // one of the values does not exist, or was deleted from the graph
        return false;
    }
    std::unordered_set<GraphEdge> edgesFirst = graph.valuesTable.at(firstKey);
    std::unordered_set<GraphEdge> edgesSecond = graph.valuesTable.at(secondKey);
    // form the Cartesian product of the edges and add each as a new edge
    for (GraphEdge e1 : edgesFirst) {
        for (GraphEdge e2 : edgesSecond) {
            updates.push(EdgesTableNewSet());
            std::unordered_set<PotentialValue, PotentialValueHasher> edgeValues;
            for (const SynonymWithValue& swv1 : graph.edgesTable.at(e1)) {
                updates.push(EdgesTableInsertToNewest(swv1.asPv()));
            }
            for (const SynonymWithValue& swv2 : graph.edgesTable.at(e2)) {
                updates.push(EdgesTableInsertToNewest(swv2.asPv()));
            }
            for (const PotentialValue& pv : edgeValues) {
                updates.push(ValuesTableInsertNewest(pv));
            }
        }
    }
    // remove the old edges
    for (GraphEdge e1 : edgesFirst) {
        for (const SynonymWithValue& swv1 : graph.edgesTable.at(e1)) {
            updates.push(ValuesTableDelete(swv1.asPv(), e1));
        }
        updates.push(EdgesTableDeleteEdge(e1));
    }
    for (GraphEdge e2 : edgesSecond) {
        for (const SynonymWithValue& swv2 : graph.edgesTable.at(e2)) {
            updates.push(ValuesTableDelete(swv2.asPv(), e2));
        }
        updates.push(EdgesTableDeleteEdge(e2));
    }
    return true;
}

/**
 * Associates two values where one's synonym exists in the graph
 * but the other does not. This corresponds to an (left) outer join.
 *
 * @param graph The RelationshipsGraph to look up.
 * @param existingKey The value whose synonym exists in the graph.
 * @param newKey The value whose synonym does not exist in the graph.
 * @param updates Queue that stores the required
 *                updates to the valuesTable.
 *
 * @returns True, if the association was successful. Otherwise, false.
 */
bool RelationshipsGraph::associateOneExisting(const RelationshipsGraph& graph, const PotentialValue& existingKey,
                                              const PotentialValue& newKey, UpdatesQueue& updates)
{
    if (graph.valuesTable.find(existingKey) != graph.valuesTable.end()) {
        // add new value to all the existing edges
        std::unordered_set<GraphEdge> existingEdges = graph.valuesTable.at(existingKey);
        updates.push(ValuesTableNewSet(newKey));
        for (GraphEdge e : existingEdges) {
            updates.push(EdgesTableNewSet());
            // duplicate the edge
            for (const SynonymWithValue& swv : graph.edgesTable.at(e)) {
                updates.push(EdgesTableInsertToNewest(swv.asPv()));
                updates.push(ValuesTableInsertNewest(swv.asPv()));
            }
            // add newKey to the edge
            updates.push(EdgesTableInsertToNewest(newKey));
            updates.push(ValuesTableInsertNewest(newKey));
        }
        // remove the old edges
        for (GraphEdge e : existingEdges) {
            for (const SynonymWithValue& swv : graph.edgesTable.at(e)) {
                updates.push(ValuesTableDelete(swv.asPv(), e));
            }
            updates.push(EdgesTableDeleteEdge(e));
        }
        return true;
    } else {
        return false;
    }
}

/**
 * Associates two values where one's synonym exists in the graph,
 * the other does not. The arguments are swapped as compared to
 * the similar function associateOneExisting, so this would
 * correspond to a right outer join.
 *
 * @param graph The RelationshipsGraph to look up.
 * @param newKey The value whose synonym does not exist in the graph.
 * @param existingKey The value whose synonym exists in the graph.
 * @param updates Queue that stores the required
 *                updates to the valuesTable.
 *
 * @returns True, if the association was successful. Otherwise, false.
 */
bool RelationshipsGraph::associateOneExistingSwapped(const RelationshipsGraph& graph, const PotentialValue& newKey,
                                                     const PotentialValue& existingKey, UpdatesQueue& updates)
{
    return associateOneExisting(graph, existingKey, newKey, updates);
}

/**
 * Associates two values of synonyms that do not
 * already exist in the graph.
 *
 * @param firstKey The first value to be added.
 * @param secondKey The second value to be added.
 * @param updates Queue that stores the required
 *                updates to the valuesTable.
 *
 * @returns Always returns true.
 */
bool RelationshipsGraph::associateZeroExisting(const RelationshipsGraph& /* unused */, const PotentialValue& firstKey,
                                               const PotentialValue& secondKey, UpdatesQueue& updates)
{
    // add edge -> (first, second)
    updates.push(EdgesTableNewSet());
    updates.push(EdgesTableInsertToNewest(firstKey));
    updates.push(EdgesTableInsertToNewest(secondKey));
    // add first -> edge, second -> edge
    updates.push(ValuesTableForceInsertNewest(firstKey));
    updates.push(ValuesTableForceInsertNewest(secondKey));
    return true;
}

Boolean RelationshipsGraph::checkIfPotentialValueHasRelationships(const PotentialValue& pv)
{
    if (valuesTable.find(pv) != valuesTable.end()) {
        return !valuesTable[pv].empty();
    } else {
        // synonym is not in table, it possibly exists independently of others
        return true;
    }
}

RelationshipsGraph::RelationshipsGraph(const std::vector<std::pair<GraphEdge, std::vector<PotentialValue>>>& edges,
                                       GraphEdge currentEdgeIndex):
    edgesIndex(currentEdgeIndex)
{
    for (const std::pair<GraphEdge, std::vector<PotentialValue>>& edge : edges) {
        edgesTable.insert(std::pair<GraphEdge, std::unordered_set<SynonymWithValue, SynonymWithValueHasher>>(
            edge.first, std::unordered_set<SynonymWithValue, SynonymWithValueHasher>()));
        for (const PotentialValue& pv : edge.second) {
            edgesTable[edge.first].insert(pv.asSwv());
            if (valuesTable.find(pv) == valuesTable.end()) {
                valuesTable.insert({pv, {}});
                synonymSet.insert(pv.synonym);
            }
            valuesTable[pv].insert(edge.first);
        }
    }
}

bool RelationshipsGraph::operator==(const RelationshipsGraph& rg) const
{
    return this->valuesTable == rg.valuesTable && this->edgesTable == rg.edgesTable && this->synonymSet == rg.synonymSet
           && this->edgesIndex == rg.edgesIndex;
}

bool RelationshipsGraph::compareStructure(const RelationshipsGraph& rg) const
{
    if (this->synonymSet != rg.synonymSet) {
        return false;
    }

    // compare values
    std::vector<PotentialValue> thisValues;
    std::vector<PotentialValue> rgValues;
    for (const std::pair<const PotentialValue, std::unordered_set<GraphEdge>>& value : this->valuesTable) {
        thisValues.push_back(value.first);
    }
    for (const std::pair<const PotentialValue, std::unordered_set<GraphEdge>>& value : rg.valuesTable) {
        rgValues.push_back(value.first);
    }
    std::sort(thisValues.begin(), thisValues.end());
    std::sort(rgValues.begin(), rgValues.end());
    if (thisValues != rgValues) {
        return false;
    }

    // compare edges
    std::vector<std::vector<PotentialValue>> thisEdges;
    std::vector<std::vector<PotentialValue>> rgEdges;
    for (const std::pair<const GraphEdge, std::unordered_set<SynonymWithValue, SynonymWithValueHasher>>& edge :
         this->edgesTable) {
        std::vector<PotentialValue> pvList;
        for (const SynonymWithValue& sve : edge.second) {
            pvList.push_back(sve.asPv());
        }
        std::sort(pvList.begin(), pvList.end());
        thisEdges.push_back(pvList);
    }
    for (const std::pair<const GraphEdge, std::unordered_set<SynonymWithValue, SynonymWithValueHasher>>& edge :
         rg.edgesTable) {
        std::vector<PotentialValue> pvList;
        for (const SynonymWithValue& sve : edge.second) {
            pvList.push_back(sve.asPv());
        }
        std::sort(pvList.begin(), pvList.end());
        rgEdges.push_back(pvList);
    }
    std::sort(thisEdges.begin(), thisEdges.end());
    std::sort(rgEdges.begin(), rgEdges.end());
    return thisEdges == rgEdges;
}

Pair<Vector<String>, Vector<String>>
RelationshipsGraph::insertRelationships(const Vector<Pair<String, String>>& valueRelationships,
                                        const Synonym& firstSynonym, bool firstIsNew, const Synonym& secondSynonym,
                                        bool secondIsNew)
{
    std::unordered_set<String> firstSynonymResults;
    std::unordered_set<String> secondSynonymResults;
    bool (*associate)(const RelationshipsGraph&, const PotentialValue&, const PotentialValue&, UpdatesQueue&)
        = firstIsNew ? (secondIsNew ? associateZeroExisting : associateOneExistingSwapped)
                     : (secondIsNew ? associateOneExisting : associateTwoExisting);
    UpdatesQueue updatesToValuesTable;
    for (const Pair<String, String>& value : valueRelationships) {
        PotentialValue firstKey(firstSynonym, value.first);
        PotentialValue secondKey(secondSynonym, value.second);
        if (associate(*this, firstKey, secondKey, updatesToValuesTable)) {
            // successful association
            firstSynonymResults.insert(value.first);
            secondSynonymResults.insert(value.second);
        }
    }
    // process the updates to actually apply the changes (this will mutate the table)
    while (!updatesToValuesTable.empty()) {
        (*updatesToValuesTable.front())(*this);
        updatesToValuesTable.pop();
    }
    // update synonym set
    if (firstIsNew) {
        synonymSet.insert(firstSynonym);
    }
    if (secondIsNew) {
        synonymSet.insert(secondSynonym);
    }
    return Pair<Vector<String>, Vector<String>>(
        Vector<String>(firstSynonymResults.begin(), firstSynonymResults.end()),
        Vector<String>(secondSynonymResults.begin(), secondSynonymResults.end()));
}

void RelationshipsGraph::deleteOne(const PotentialValue& pv, ResultsTable* resultsTable)
{
    if (valuesTable.find(pv) == valuesTable.end()) {
        // if not present, do nothing
        return;
    } else if (valuesTable[pv].empty()) {
        // if no relationships, trivial deletion
        valuesTable.erase(pv);
        return;
    }
    // synonym is present in table, delete all edges
    std::unordered_set<GraphEdge> affectedEdges = valuesTable[pv];
    std::unordered_set<PotentialValue, PotentialValueHasher> affectedValues;
    for (GraphEdge ge : affectedEdges) {
        std::unordered_set<SynonymWithValue, SynonymWithValueHasher> currentEdgeValues = edgesTable[ge];
        for (const SynonymWithValue& otherValue : currentEdgeValues) {
            valuesTable[otherValue.asPv()].erase(ge);
            affectedValues.insert(otherValue.asPv());
        }
        edgesTable.erase(ge);
    }
    // empty entry for pv, signifying no relationships
    affectedValues.erase(pv);
    valuesTable.erase(pv);
    // check related values to see if they are empty as well
    for (const PotentialValue& affected : affectedValues) {
        if (!checkIfPotentialValueHasRelationships(affected)) {
            // this related value no longer has any relationships
            // we update the results table
            resultsTable->eliminatePotentialValue(affected.synonym, affected.value);
            deleteOne(affected, resultsTable);
        }
    }
}

void RelationshipsGraph::deleteTwo(const PotentialValue& firstKey, const PotentialValue& secondKey,
                                   ResultsTable* resultsTable)
{
    if (valuesTable.find(firstKey) == valuesTable.end() || valuesTable.find(secondKey) == valuesTable.end()) {
        return;
    }
    // short-circuit if either values are empty
    bool doShortCircuit = false;
    if (valuesTable[firstKey].empty()) {
        doShortCircuit = true;
        valuesTable.erase(firstKey);
    }
    if (valuesTable[secondKey].empty()) {
        doShortCircuit = true;
        valuesTable.erase(secondKey);
    }
    if (doShortCircuit) {
        return;
    }

    // find all edges with secondKey
    std::unordered_set<GraphEdge> firstKeyEdges = valuesTable[firstKey];
    std::unordered_set<GraphEdge> edgesToDelete;
    for (GraphEdge potentialEdge : firstKeyEdges) {
        if (edgesTable[potentialEdge].find(secondKey.asSwv()) != edgesTable[potentialEdge].end()) {
            // transfer this edge to edgesToDelete
            valuesTable[firstKey].erase(potentialEdge);
            valuesTable[secondKey].erase(potentialEdge);
            edgesToDelete.insert(potentialEdge);
        }
    }
    // delete edges
    std::unordered_set<PotentialValue, PotentialValueHasher> affectedValues;
    for (GraphEdge edge : edgesToDelete) {
        std::unordered_set<SynonymWithValue, SynonymWithValueHasher>& currentEdgeValues = edgesTable[edge];
        for (const SynonymWithValue& otherValue : currentEdgeValues) {
            valuesTable[otherValue.asPv()].erase(edge);
            affectedValues.insert(otherValue.asPv());
        }
        edgesTable.erase(edge);
    }
    // check firstKey and secondKey whether empty or not
    if (!checkIfPotentialValueHasRelationships(firstKey)) {
        resultsTable->eliminatePotentialValue(firstKey.synonym, firstKey.value);
        valuesTable.erase(firstKey);
    }
    if (!checkIfPotentialValueHasRelationships(secondKey)) {
        resultsTable->eliminatePotentialValue(secondKey.synonym, secondKey.value);
        valuesTable.erase(secondKey);
    }
    // ignore firstKey and secondKey
    affectedValues.erase(firstKey);
    affectedValues.erase(secondKey);
    // check related values to see if they are empty as well
    for (const PotentialValue& affected : affectedValues) {
        if (!checkIfPotentialValueHasRelationships(affected)) {
            // this related value no longer has any relationships
            // we update the results table
            resultsTable->eliminatePotentialValue(affected.synonym, affected.value);
            // clear the value from relationships graph
            deleteOne(affected, resultsTable);
        }
    }
}

Boolean RelationshipsGraph::hasSeenBefore(const Synonym& syn)
{
    return synonymSet.find(syn) != synonymSet.end();
}

Boolean RelationshipsGraph::checkIfRelated(const PotentialValue& firstPv, const PotentialValue& secondPv)
{
    if (valuesTable.find(firstPv) != valuesTable.end()) {
        bool isRelated = false;
        for (GraphEdge ge : valuesTable[firstPv]) {
            auto element = edgesTable[ge].find(secondPv.asSwv());
            if (element != edgesTable[ge].end() && element->value == secondPv.value) {
                isRelated = true;
                break;
            }
        }
        return isRelated;
    } else {
        return false;
    }
}

Boolean RelationshipsGraph::isValueRelated(const PotentialValue& pv, const Synonym& syn)
{
    if (valuesTable.find(pv) == valuesTable.end() || valuesTable[pv].empty()) {
        return false;
    } else {
        std::unordered_set<SynonymWithValue, SynonymWithValueHasher>& valuesInEdge
            = edgesTable[*valuesTable[pv].begin()]; // just get first edge in unordered_set
                                                    // (all edges should have the same synonyms)
        SynonymWithValue placeholderValue(syn, "");
        return valuesInEdge.find(placeholderValue) != valuesInEdge.end();
    }
}

std::vector<PotentialValue> RelationshipsGraph::retrieveRelationships(const PotentialValue& value)
{
    if (valuesTable.find(value) != valuesTable.end()) {
        std::unordered_set<PotentialValue, PotentialValueHasher> resultsSet;
        for (GraphEdge e : valuesTable[value]) {
            std::unordered_set<SynonymWithValue, SynonymWithValueHasher>& edgeValues = edgesTable[e];
            for (const SynonymWithValue& edgeVal : edgeValues) {
                resultsSet.insert(static_cast<PotentialValue>(edgeVal));
            }
        }
        // remove the original potential value
        resultsSet.erase(value);
        return std::vector<PotentialValue>(resultsSet.begin(), resultsSet.end());
    } else {
        return std::vector<PotentialValue>();
    }
}
