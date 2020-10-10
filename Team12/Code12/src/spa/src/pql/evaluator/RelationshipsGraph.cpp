/**
 * Implementation of Relationships Graph for
 * Query Evaluator's Result Table.
 */
#include <utility>

#include "ResultsTable.h"

void RelationshipsGraph::associate(const PotentialValue& firstKey, const PotentialValue& secondKey)
{
    // add first -> second
    if (relationshipsTable.find(firstKey) == relationshipsTable.end()) {
        relationshipsTable.insert(std::pair<PotentialValue, std::unordered_set<PotentialValue, PotentialValueHasher>>(
            firstKey, std::unordered_set<PotentialValue, PotentialValueHasher>()));
    }
    relationshipsTable[firstKey].insert(secondKey);

    // add second -> first
    if (relationshipsTable.find(secondKey) == relationshipsTable.end()) {
        relationshipsTable.insert(std::pair<PotentialValue, std::unordered_set<PotentialValue, PotentialValueHasher>>(
            secondKey, std::unordered_set<PotentialValue, PotentialValueHasher>()));
    }
    relationshipsTable[secondKey].insert(firstKey);
}

Boolean RelationshipsGraph::checkIfPotentialValueHasRelationships(const PotentialValue& pv)
{
    if (relationshipsTable.find(pv) != relationshipsTable.end()) {
        return !relationshipsTable[pv].empty();
    } else {
        // synonym is not in table, it possibly exists independently of others
        return true;
    }
}

bool RelationshipsGraph::operator==(const RelationshipsGraph& rg) const
{
    return this->relationshipsTable == rg.relationshipsTable;
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

void RelationshipsGraph::deleteFromGraph(const PotentialValue& pv, ResultsTable* resultsTable)
{
    if (relationshipsTable.find(pv) == relationshipsTable.end()) {
        // if not present, do nothing as potential value has no relationships
        return;
    }
    // synonym is present in table, delete all edges
    std::unordered_set<PotentialValue, PotentialValueHasher> affectedValues = relationshipsTable[pv];
    for (const PotentialValue& av : affectedValues) {
        deleteEdges(pv, av);
    }
    // now, entry for pv is empty, signifying no relationships
    // check related values to see if they are empty as well
    for (const PotentialValue& affected : affectedValues) {
        if (!checkIfPotentialValueHasRelationships(affected)) {
            // this related value no longer has any relationships
            // we update the results table
            resultsTable->eliminatePotentialValue(affected.synonym, affected.value);
        }
        // clear synonyms relationship from cache
        if (synonymRelationshipsCache.find(affected.synonym) != synonymRelationshipsCache.end()
            && synonymRelationshipsCache[affected.synonym].find(pv.synonym)
                   != synonymRelationshipsCache[affected.synonym].end()) {
            // delete the cached relationship between synonyms, even if there could
            // still be relationships between them for other potential values
            synonymRelationshipsCache[affected.synonym].erase(pv.synonym);
        }
    }
    // clear synonym from cache, even though the synonym may still
    // have other potential values with relationships
    if (synonymRelationshipsCache.find(pv.synonym) != synonymRelationshipsCache.end()) {
        synonymRelationshipsCache.erase(pv.synonym);
    }
}

void RelationshipsGraph::deleteEdges(const PotentialValue& firstKey, const PotentialValue& secondKey)
{
    // delete first -> second
    if (relationshipsTable.find(firstKey) != relationshipsTable.end()) {
        relationshipsTable[firstKey].erase(secondKey);
    }
    // delete second -> first
    if (relationshipsTable.find(secondKey) != relationshipsTable.end()) {
        relationshipsTable[secondKey].erase(firstKey);
    }
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
    if (relationshipsTable.find(firstPv) != relationshipsTable.end()) {
        return relationshipsTable[firstPv].find(secondPv) != relationshipsTable[firstPv].end();
    } else {
        return false;
    }
}

std::vector<PotentialValue> RelationshipsGraph::retrieveRelationships(const PotentialValue& value)
{
    std::vector<PotentialValue> resultsList;
    if (relationshipsTable.find(value) != relationshipsTable.end()) {
        resultsList.insert(resultsList.end(), relationshipsTable[value].begin(), relationshipsTable[value].end());
    }
    return resultsList;
}
