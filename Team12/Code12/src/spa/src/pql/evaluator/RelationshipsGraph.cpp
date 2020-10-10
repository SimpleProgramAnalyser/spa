/**
 * Implementation of Relationships Graph for
 * Query Evaluator's Result Table.
 */
#include <utility>

#include "ResultsTable.h"

void RelationshipsGraph::associate(const PotentialValue& firstKey, const PotentialValue& secondKey)
{
    GraphEdge currentEdge = edgesIndex;
    // add first -> edge, second -> edge
    if (valuesTable.find(firstKey) == valuesTable.end()) {
        valuesTable.insert(
            std::pair<PotentialValue, std::unordered_set<GraphEdge>>(firstKey, std::unordered_set<GraphEdge>()));
    }
    if (valuesTable.find(secondKey) == valuesTable.end()) {
        valuesTable.insert(
            std::pair<PotentialValue, std::unordered_set<GraphEdge>>(secondKey, std::unordered_set<GraphEdge>()));
    }
    valuesTable[firstKey].insert(currentEdge);
    valuesTable[secondKey].insert(currentEdge);

    // add edge -> (first, second)
    if (edgesTable.find(currentEdge) == edgesTable.end()) {
        edgesTable.insert(std::pair<GraphEdge, std::unordered_set<PotentialValue, PotentialValueHasher>>(
            currentEdge, std::unordered_set<PotentialValue, PotentialValueHasher>()));
    }
    edgesTable[currentEdge].insert(firstKey);
    edgesTable[currentEdge].insert(secondKey);

    // increment edges index for next use
    edgesIndex++;
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
                                             const Synonym& firstSynonym, const Synonym& secondSynonym)
{
    for (const Pair<String, String>& value : valueRelationships) {
        PotentialValue firstKey(firstSynonym, value.first);
        PotentialValue secondKey(secondSynonym, value.second);
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
