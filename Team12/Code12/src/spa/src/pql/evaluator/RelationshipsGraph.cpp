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

void RelationshipsGraph::deleteEdge(const PotentialValue& firstKey, const PotentialValue& secondKey)
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

Boolean RelationshipsGraph::checkIfPotentialValueHasRelationships(const PotentialValue& pv)
{
    if (relationshipsTable.find(pv) != relationshipsTable.end()) {
        return !relationshipsTable[pv].empty();
    } else {
        // synonym is not in table, it possibly exists independently of others
        return true;
    }
}

void RelationshipsGraph::insertRelationships(Vector<Pair<String, String>> valueRelationships,
                                             const Synonym& firstSynonym, const Synonym& secondSynonym)
{
    insert<String, String, stringId, stringId>(std::move(valueRelationships), firstSynonym, secondSynonym);
}

void RelationshipsGraph::insertRelationships(Vector<Pair<String, Integer>> valueRelationships,
                                             const Synonym& firstSynonym, const Synonym& secondSynonym)
{
    insert<String, Integer, stringId, std::to_string>(std::move(valueRelationships), firstSynonym, secondSynonym);
}

void RelationshipsGraph::insertRelationships(Vector<Pair<Integer, String>> valueRelationships,
                                             const Synonym& firstSynonym, const Synonym& secondSynonym)
{
    insert<Integer, String, std::to_string, stringId>(std::move(valueRelationships), firstSynonym, secondSynonym);
}

void RelationshipsGraph::insertRelationships(Vector<Pair<Integer, Integer>> valueRelationships,
                                             const Synonym& firstSynonym, const Synonym& secondSynonym)
{
    insert<Integer, Integer, std::to_string, std::to_string>(std::move(valueRelationships), firstSynonym,
                                                             secondSynonym);
}

void RelationshipsGraph::deleteFromGraph(const PotentialValue& pv, ResultsTable* resultsTable)
{
    std::unordered_set<PotentialValue, PotentialValueHasher> affectedValues;
    if (relationshipsTable.find(pv) != relationshipsTable.end()) {
        // synonym is present in table, delete all edges
        affectedValues = relationshipsTable[pv];
        for (const PotentialValue& av : affectedValues) {
            deleteEdge(pv, av);
        }
        // now, entry for pv is empty, signifying no relationships
    } else {
        // if not present, do nothing as synonym has no relationships
    }
    // check related values to see if they are empty as well
    for (const PotentialValue& affected : affectedValues) {
        if (!checkIfPotentialValueHasRelationships(affected)) {
            // this related value no longer has any relationships
            // we update the results table
            resultsTable->eliminatePotentialValue(affected.synonym, affected.value);
        }
    }
}
