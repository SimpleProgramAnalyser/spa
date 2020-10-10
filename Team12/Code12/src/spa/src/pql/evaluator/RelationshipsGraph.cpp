/**
 * Implementation of Relationships Graph for
 * Query Evaluator's Result Table.
 */
#include <utility>

#include "ResultsTable.h"

/**
 * Associates two values of synonyms that already exist
 * in the graph, but have not been linked with each other.
 *
 * @param graph The RelationshipsGraph.
 * @param firstKey The first value to be added.
 * @param secondKey The second value to be added.
 */
void RelationshipsGraph::associateTwoExisting(RelationshipsGraph* graph, const PotentialValue& firstKey,
                                              const PotentialValue& secondKey)
{
    if (graph->valuesTable.find(firstKey) == graph->valuesTable.end()
        || graph->valuesTable.find(secondKey) == graph->valuesTable.end()) {
        // one of the values do not exist, or have been deleted from the graph
        return;
    }
    std::unordered_set<GraphEdge> edgesFirst = graph->valuesTable[firstKey];
    std::unordered_set<GraphEdge> edgesSecond = graph->valuesTable[secondKey];
    // form the Cartesian product of the edges and add each as a new edge
    for (GraphEdge e1 : edgesFirst) {
        for (GraphEdge e2 : edgesSecond) {
            GraphEdge newEdge = graph->edgesIndex;
            std::unordered_set<PotentialValue, PotentialValueHasher> edgeValues;
            edgeValues.insert(graph->edgesTable[e1].begin(), graph->edgesTable[e1].end());
            edgeValues.insert(graph->edgesTable[e2].begin(), graph->edgesTable[e2].end());
            graph->edgesTable.insert(
                std::pair<GraphEdge, std::unordered_set<PotentialValue, PotentialValueHasher>>(newEdge, edgeValues));
            for (const PotentialValue& pv : edgeValues) {
                graph->valuesTable[pv].insert(newEdge);
            }
            graph->edgesIndex++;
        }
    }
    // remove the old edges
    for (GraphEdge e1 : edgesFirst) {
        for (const PotentialValue& pv1 : graph->edgesTable[e1]) {
            graph->valuesTable[pv1].erase(e1);
        }
        graph->edgesTable.erase(e1);
    }
    for (GraphEdge e2 : edgesSecond) {
        for (const PotentialValue& pv2 : graph->edgesTable[e2]) {
            graph->valuesTable[pv2].erase(e2);
        }
        graph->edgesTable.erase(e2);
    }
}

/**
 * Associates two values where one's synonym
 * exists in the graph, the other does not.
 *
 * @param graph The RelationshipsGraph.
 * @param existingKey The value whose synonym exists in the graph.
 * @param newKey The value whose synonym does not exist in the graph.
 */
void RelationshipsGraph::associateOneExisting(RelationshipsGraph* graph, const PotentialValue& existingKey,
                                              const PotentialValue& newKey)
{
    if (graph->valuesTable.find(existingKey) != graph->valuesTable.end()) {
        // add new value to all the existing edges
        std::unordered_set<GraphEdge> existingEdges = graph->valuesTable[existingKey];
        graph->valuesTable.insert(
            std::pair<PotentialValue, std::unordered_set<GraphEdge>>(newKey, std::unordered_set<GraphEdge>()));
        for (GraphEdge e : existingEdges) {
            graph->edgesTable[e].insert(newKey);
            graph->valuesTable[newKey].insert(e);
        }
    }
}

/**
 * Associates two values where one's synonym exists in the graph,
 * the other does not. The arguments are swapped as compared to
 * the similar function associateOneExisting.
 *
 * @param graph The RelationshipsGraph.
 * @param newKey The value whose synonym does not exist in the graph.
 * @param existingKey The value whose synonym exists in the graph.
 */
void RelationshipsGraph::associateOneExistingSwapped(RelationshipsGraph* graph, const PotentialValue& newKey,
                                                     const PotentialValue& existingKey)
{
    associateOneExisting(graph, existingKey, newKey);
}

/**
 * Associates two values of synonyms that do not
 * already exist in the graph.
 *
 * @param graph The RelationshipsGraph.
 * @param firstKey The first value to be added.
 * @param secondKey The second value to be added.
 */
void RelationshipsGraph::associateZeroExisting(RelationshipsGraph* graph, const PotentialValue& firstKey,
                                               const PotentialValue& secondKey)
{
    GraphEdge currentEdge = graph->edgesIndex;
    // add first -> edge, second -> edge
    if (graph->valuesTable.find(firstKey) == graph->valuesTable.end()) {
        graph->valuesTable.insert(
            std::pair<PotentialValue, std::unordered_set<GraphEdge>>(firstKey, std::unordered_set<GraphEdge>()));
    }
    if (graph->valuesTable.find(secondKey) == graph->valuesTable.end()) {
        graph->valuesTable.insert(
            std::pair<PotentialValue, std::unordered_set<GraphEdge>>(secondKey, std::unordered_set<GraphEdge>()));
    }
    graph->valuesTable[firstKey].insert(currentEdge);
    graph->valuesTable[secondKey].insert(currentEdge);

    // add edge -> (first, second)
    graph->edgesTable.insert(std::pair<GraphEdge, std::unordered_set<PotentialValue, PotentialValueHasher>>(
        currentEdge, std::unordered_set<PotentialValue, PotentialValueHasher>()));
    graph->edgesTable[currentEdge].insert(firstKey);
    graph->edgesTable[currentEdge].insert(secondKey);

    // increment edges index for next use
    graph->edgesIndex++;
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

bool RelationshipsGraph::operator==(const RelationshipsGraph& rg) const
{
    return this->valuesTable == rg.valuesTable && this->edgesTable == rg.edgesTable;
    // cache is not checked because the exact values may be different,
    // depending on order of addition to the relationships graph
}

bool RelationshipsGraph::checkEqualIncludingCache(const RelationshipsGraph& rg) const
{
    return *this == rg && this->synonymRelationshipsCache == rg.synonymRelationshipsCache;
}

void RelationshipsGraph::insertRelationships(const Vector<Pair<String, String>>& valueRelationships,
                                             const Synonym& firstSynonym, bool firstIsNew, const Synonym& secondSynonym,
                                             bool secondIsNew)
{
    void (*associate)(RelationshipsGraph*, const PotentialValue&, const PotentialValue&)
        = firstIsNew ? (secondIsNew ? associateZeroExisting : associateOneExistingSwapped)
                     : (secondIsNew ? associateOneExisting : associateTwoExisting);
    for (const Pair<String, String>& value : valueRelationships) {
        PotentialValue firstKey(firstSynonym, value.first);
        PotentialValue secondKey(secondSynonym, value.second);
        associate(this, firstKey, secondKey);
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

void RelationshipsGraph::deleteOne(const PotentialValue& pv, ResultsTable* resultsTable)
{
    if (valuesTable.find(pv) == valuesTable.end() || valuesTable[pv].empty()) {
        // if not present, do nothing as potential value has no relationships
        return;
    }
    // synonym is present in table, delete all edges
    std::unordered_set<GraphEdge> affectedEdges = valuesTable[pv];
    // empty entry for pv, signifying no relationships
    valuesTable[pv].clear();
    std::unordered_set<PotentialValue, PotentialValueHasher> affectedValues;
    for (GraphEdge ge : affectedEdges) {
        std::unordered_set<PotentialValue, PotentialValueHasher> currentEdgeValues = edgesTable[ge];
        for (const PotentialValue& otherValue : currentEdgeValues) {
            valuesTable[otherValue].erase(ge);
            affectedValues.insert(otherValue);
        }
        edgesTable.erase(ge);
    }
    // ignore pv
    affectedValues.erase(pv);
    // check related values to see if they are empty as well
    for (const PotentialValue& affected : affectedValues) {
        if (!checkIfPotentialValueHasRelationships(affected)) {
            // this related value no longer has any relationships
            // we update the results table
            resultsTable->eliminatePotentialValue(affected.synonym, affected.value);
            deleteOne(affected, resultsTable);
        }
        // clear synonyms relationship from cache
        if (synonymRelationshipsCache.find(affected.synonym) != synonymRelationshipsCache.end()) {
            // delete the cached relationship between synonyms, even if there could
            // still be relationships between them for other potential values
            synonymRelationshipsCache[affected.synonym].erase(pv.synonym);
        }
    }
    // clear synonym from cache, even though the synonym may still
    // have other potential values with relationships
    synonymRelationshipsCache.erase(pv.synonym);
}

void RelationshipsGraph::deleteTwo(const PotentialValue& firstKey, const PotentialValue& secondKey,
                                   ResultsTable* resultsTable)
{
    if (valuesTable.find(firstKey) == valuesTable.end() || valuesTable[firstKey].empty()
        || valuesTable.find(secondKey) == valuesTable.end() || valuesTable[secondKey].empty()) {
        return;
    }
    // find all edges with secondKey
    std::unordered_set<GraphEdge> firstKeyEdges = valuesTable[firstKey];
    std::unordered_set<GraphEdge> edgesToDelete;
    for (GraphEdge potentialEdge : firstKeyEdges) {
        if (edgesTable[potentialEdge].find(secondKey) != edgesTable[potentialEdge].end()) {
            // transfer this edge to edgesToDelete
            valuesTable[firstKey].erase(potentialEdge);
            valuesTable[secondKey].erase(potentialEdge);
            edgesToDelete.insert(potentialEdge);
        }
    }
    // delete edges
    std::unordered_set<PotentialValue, PotentialValueHasher> affectedValues;
    for (GraphEdge edge : edgesToDelete) {
        std::unordered_set<PotentialValue, PotentialValueHasher> currentEdgeValues = edgesTable[edge];
        for (const PotentialValue& otherValue : currentEdgeValues) {
            valuesTable[otherValue].erase(edge);
            affectedValues.insert(otherValue);
        }
        edgesTable.erase(edge);
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
        // clear synonyms relationship from cache
        if (synonymRelationshipsCache.find(affected.synonym) != synonymRelationshipsCache.end()) {
            // clear cache for affected synonym -> first synonym and second synonym
            synonymRelationshipsCache[affected.synonym].erase(firstKey.synonym);
            synonymRelationshipsCache[affected.synonym].erase(secondKey.synonym);
        }
    }
    // clear cache for first synonym and second synonym
    synonymRelationshipsCache.erase(firstKey.synonym);
    synonymRelationshipsCache.erase(secondKey.synonym);
}

Boolean RelationshipsGraph::checkCachedRelationships(const Synonym& firstSynonym, const Synonym& secondSynonym)
{
    if (synonymRelationshipsCache.find(firstSynonym) != synonymRelationshipsCache.end()) {
        return synonymRelationshipsCache[firstSynonym].find(secondSynonym)
               != synonymRelationshipsCache[firstSynonym].end();
    } else if (synonymRelationshipsCache.find(secondSynonym) != synonymRelationshipsCache.end()) {
        return synonymRelationshipsCache[secondSynonym].find(firstSynonym)
               != synonymRelationshipsCache[secondSynonym].end();
    } else {
        return false;
    }
}

Boolean RelationshipsGraph::checkIfRelated(const PotentialValue& firstPv, const PotentialValue& secondPv)
{
    if (valuesTable.find(firstPv) != valuesTable.end()) {
        bool isRelated = false;
        for (GraphEdge ge : valuesTable[firstPv]) {
            if (edgesTable[ge].find(secondPv) != edgesTable[ge].end()) {
                isRelated = true;
                break;
            }
        }
        return isRelated;
    } else {
        return false;
    }
}

std::vector<PotentialValue> RelationshipsGraph::retrieveRelationships(const PotentialValue& value)
{
    if (valuesTable.find(value) != valuesTable.end()) {
        std::unordered_set<PotentialValue, PotentialValueHasher> resultsSet;
        for (GraphEdge e : valuesTable[value]) {
            std::unordered_set<PotentialValue, PotentialValueHasher> edgeValues = edgesTable[e];
            resultsSet.insert(edgeValues.begin(), edgeValues.end());
        }
        return std::vector<PotentialValue>(resultsSet.begin(), resultsSet.end());
    } else {
        return std::vector<PotentialValue>();
    }
}
