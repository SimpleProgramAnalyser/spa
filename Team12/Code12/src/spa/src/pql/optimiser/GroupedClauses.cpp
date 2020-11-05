#include "GroupedClauses.h"

#include <functional>
#include <numeric>

#include "OptimiserUtils.h"

/**
 * Create a new GroupClauses instance based on an AbstractQuery.
 *
 * @param abstractQuery
 */
GroupedClauses::GroupedClauses(AbstractQuery& abstractQuery): abstractQuery(abstractQuery)
{
    Vector<Integer> newGroup;
    for (int i = 0; i < abstractQuery.getClauses().count(); i++) {
        newGroup.push_back(i);
    }
    listOfGroups.push_back(newGroup);
}

/**
 * Add an empty group to the end of the list of GroupedClauses.
 */
int GroupedClauses::addGroup()
{
    listOfGroups.emplace_back();
    return listOfGroups.size() - 1;
}

/**
 * Add an empty group to the end of the list of GroupedClauses, and mark it as the noSynonymGroup
 */
int GroupedClauses::getNoSynonymGroupIndex()
{
    if (noSynonymGroup == -1) {
        listOfGroups.emplace_back();
        noSynonymGroup = listOfGroups.size() - 1;
    }
    return noSynonymGroup;
}

/**
 * Move a clause from a group to another.
 *
 * @param originalGroupIndex
 * @param originalIndex
 * @param destGroupIndex
 * @param destIndex
 */
void GroupedClauses::moveClauseAcrossGroup(int originalGroupIndex, int originalIndex, int destGroupIndex, int destIndex)
{
    auto& originalGroup = listOfGroups[originalGroupIndex];
    auto& destGroup = listOfGroups[destGroupIndex];
    // copy to destGroup, then delete from originalGroup
    destGroup.insert(destGroup.begin() + destIndex, originalGroup[originalIndex]);
    originalGroup.erase(originalGroup.begin() + originalIndex);
}

/**
 * Safe deletion of a group of clauses, by specifying which group to put its clauses into, if any.
 *
 * @param groupToRemove
 * @param groupToMergeInto
 */
void GroupedClauses::mergeAndRemoveGroup(int groupToRemove, int groupToMergeInto)
{
    // merging a group into itself has no effects whatsoever.
    if (groupToMergeInto == groupToRemove) {
        return;
    }

    // push all clauses into the target group
    listOfGroups[groupToMergeInto].insert(listOfGroups[groupToMergeInto].end(), listOfGroups[groupToRemove].begin(),
                                          listOfGroups[groupToRemove].end());
    // and remove the copied group
    listOfGroups.erase(listOfGroups.begin() + groupToRemove);

    // Adjust noSynonymGroup if it is merged/deleted
    if (groupToRemove == noSynonymGroup || groupToMergeInto == noSynonymGroup) {
        // naively noSynonymGroup should end up groupToMergeInto, but is actually
        // groupToMergeInto - 1 if groupToRemove is BEFORE groupToMergeInto
        noSynonymGroup = groupToMergeInto;
        if (groupToRemove < groupToMergeInto) {
            noSynonymGroup--;
        }
    } else {
        // need to shift back by one if a group is removed before it.
        if (groupToRemove < noSynonymGroup) {
            noSynonymGroup--;
        }
    }
}

/**
 * Returns a pointer to the clause given the group it is in, and its index in the group.
 *
 * @param groupIndex
 * @param clauseIndex
 * @return
 */
Clause* GroupedClauses::getClause(int groupIndex, int clauseIndex)
{
    return abstractQuery.getClauses().get(getClauseNumber(groupIndex, clauseIndex));
}

/**
 * Returns the position the clause given the group it is in, and its index in the group.
 *
 * @param groupIndex
 * @param clauseIndex
 * @return
 */
int GroupedClauses::getClauseNumber(int groupIndex, int clauseIndex) const
{
    return listOfGroups[groupIndex][clauseIndex];
}

/**
 * Allow two clauses to swap indices within a given group. This facilitates sorting and rearranging of clauses
 * within the groups.
 *
 * @param groupIndex
 * @param clause1
 * @param clause2
 */
void GroupedClauses::swapClauseWithinGroup(int groupIndex, int clause1, int clause2)
{
    Vector<Integer> currentGroup = listOfGroups[groupIndex];
    std::iter_swap(currentGroup.begin() + clause1, currentGroup.begin() + clause2);
}

/**
 * Combined with creating a new group using addGroup(), this allows splitting up groups into smaller groups.
 *
 * @param groupIndex1
 * @param clause1
 * @param groupIndex2
 * @param clause2
 */
void GroupedClauses::swapClausesAcrossGroup(int groupIndex1, int clause1, int groupIndex2, int clause2)
{
    auto group1 = listOfGroups[groupIndex1];
    auto group2 = listOfGroups[groupIndex2];

    std::iter_swap(group1.begin() + clause1, group2.begin() + clause2);
}

/**
 * Return the number of groups.
 *
 * @return
 */
int GroupedClauses::size() const
{
    return listOfGroups.size();
}

/**
 * Swap the position of two groups given their indices. This allows sorting of groups.
 *
 * @param groupIndex1
 * @param groupIndex2
 */
void GroupedClauses::swapGroups(int groupIndex1, int groupIndex2)
{
    std::iter_swap(listOfGroups.begin() + groupIndex1, listOfGroups.begin() + groupIndex2);
}

/**
 * Return the size of a group given its index.
 *
 * @param groupIndex
 * @return
 */
int GroupedClauses::groupSize(int groupIndex) const
{
    return listOfGroups[groupIndex].size();
}

/**
 * If any of the clauses in the group has a synonym, return true, otherwise false.
 *
 * @param groupIndex
 * @return
 */
bool GroupedClauses::groupHasSynonym(int groupIndex)
{
    for (int clauseIndex = 0; clauseIndex < groupSize(groupIndex); clauseIndex++) {
        Clause* clause = getClause(groupIndex, clauseIndex);
        if (hasSynonym(clause))
            return true;
    }
    return false;
}

/**
 * If any synonym in any clause is returned as a result of the PQL query, return true.
 * Return false otherwise.
 *
 * @param groupIndex
 * @return
 */
bool GroupedClauses::synonymIsReturned(int groupIndex)
{
    // get all returned synonyms
    std::unordered_set<Synonym> returnedSynonyms;
    auto resultSynonyms = abstractQuery.getSelectedSynonyms();
    for (auto& resultSynonym : resultSynonyms) {
        returnedSynonyms.insert(resultSynonym.getSynonym());
    }

    // match it clause by clause, return true whenever a returned synonym is found.
    for (int clauseIndex = 0; clauseIndex < groupSize(groupIndex); clauseIndex++) {
        auto clause = getClause(groupIndex, clauseIndex);
        switch (clause->getType()) {
        case SuchThatClauseType: {
            // NOLINTNEXTLINE
            SuchThatClause* suchThatClause = static_cast<SuchThatClause*>(clause);
            auto leftRef = suchThatClause->getRelationship().getLeftRef();
            auto rightRef = suchThatClause->getRelationship().getRightRef();
            if (leftRef.getReferenceType() == SynonymRefType && returnedSynonyms.count(leftRef.getValue())) {
                return true;
            }
            if (rightRef.getReferenceType() == SynonymRefType && returnedSynonyms.count(rightRef.getValue())) {
                return true;
            }
            break;
        }
        case PatternClauseType: {
            // NOLINTNEXTLINE
            PatternClause* patternClause = static_cast<PatternClause*>(clause);
            auto synonym = patternClause->getPatternSynonym();
            if (returnedSynonyms.count(synonym)) {
                return true;
            }
            break;
        }
        case WithClauseType: {
            // NOLINTNEXTLINE
            WithClause* withClause = static_cast<WithClause*>(clause);
            auto leftRef = withClause->getLeftReference();
            auto rightRef = withClause->getRightReference();
            if ((leftRef.getReferenceType() == SynonymRefType || leftRef.getReferenceType() == AttributeRefType)
                && returnedSynonyms.count(leftRef.getValue())) {
                return true;
            }
            if ((rightRef.getReferenceType() == SynonymRefType || rightRef.getReferenceType() == AttributeRefType)
                && returnedSynonyms.count(rightRef.getValue())) {
                return true;
            }
            break;
        }
        case NonExistentClauseType:
            break;
        }
    }
    return false;
}

/**
 * For use in sorting of groups.
 *
 * @param group1
 * @param group2
 * @return
 */
bool GroupedClauses::compareGroups(int group1, int group2, GroupedClauses* groupedClauses)
{
    /**
     * clause1 is (strictly) before clause2 iff
     * 1. clause1 has no synonyms while clause 2 has.
     * 2. clause1's synonyms are not returned while clause 2's synonyms are.
     * Otherwise sort them by length, shortest goes first
     */
    if (!groupedClauses->groupHasSynonym(group1) && groupedClauses->groupHasSynonym(group2)) {
        return true;
    } else if (groupedClauses->groupHasSynonym(group1) && !groupedClauses->groupHasSynonym(group2)) {
        return false;
    } else if (!groupedClauses->synonymIsReturned(group1) && groupedClauses->synonymIsReturned(group2)) {
        return true;
    } else if (groupedClauses->synonymIsReturned(group1) && !groupedClauses->synonymIsReturned(group2)) {
        return false;
    } else {
        return groupedClauses->groupSize(group1) < groupedClauses->groupSize(group2);
    }
}

bool compare(int group1, int group2, const Vector<bool>& groupHasSynonym, const Vector<bool>& synonymIsReturned,
             const Vector<int>& groupSize)
{
    if (!groupHasSynonym[group1] && groupHasSynonym[group2]) {
        return true;
    } else if (groupHasSynonym[group1] && !groupHasSynonym[group2]) {
        return false;
    } else if (!synonymIsReturned[group1] && synonymIsReturned[group2]) {
        return true;
    } else if (synonymIsReturned[group1] && !synonymIsReturned[group2]) {
        return false;
    } else {
        return groupSize[group1] < groupSize[group2];
    }
}

/**
 * Sort the list of groups of clauses according to the default order.
 */
void GroupedClauses::sortGroups()
{
    Vector<Integer> indexList(listOfGroups.size());
    std::iota(indexList.begin(), indexList.end(), 0);

    // Keep the curried version, just in case
    //    std::sort(indexList.begin(), indexList.end(),
    //              std::bind(compareGroups, std::placeholders::_1, std::placeholders::_2, this));
    Vector<bool> hasSynonym, isReturned;
    Vector<int> groupSize;
    std::transform(indexList.begin(), indexList.end(), std::back_inserter(hasSynonym),
                   [this](int a) { return this->groupHasSynonym(a); });
    std::transform(indexList.begin(), indexList.end(), std::back_inserter(isReturned),
                   [this](int a) { return this->synonymIsReturned(a); });
    std::transform(indexList.begin(), indexList.end(), std::back_inserter(groupSize),
                   [this](int a) { return this->groupSize(a); });
    std::sort(indexList.begin(), indexList.end(), [hasSynonym, isReturned, groupSize](int a, int b) {
        return compare(a, b, hasSynonym, isReturned, groupSize);
    });

    // apply permutation
    Vector<Vector<Integer>> newListOfGroups;
    for (int i = 0; i < listOfGroups.size(); i++) {
        newListOfGroups.push_back(listOfGroups[indexList[i]]);
    }
    listOfGroups = newListOfGroups;
}
void GroupedClauses::moveClauseAcrossGroup(int originalGroupIndex, int destGroupIndex, int clauseNumber)
{
    auto originalGroup = listOfGroups[originalGroupIndex];
    auto it = std::find(originalGroup.begin(), originalGroup.end(), clauseNumber);
    moveClauseAcrossGroup(originalGroupIndex, it - originalGroup.begin(), destGroupIndex,
                          listOfGroups[destGroupIndex].size());
}

const Vector<Integer>& GroupedClauses::getGroup(int groupIndex) const
{
    return listOfGroups[groupIndex];
}

void GroupedClauses::applyArrangementToGroup(std::queue<uint> arrangement, int groupIndex)
{
    Vector<int> newGroup;
    while (!arrangement.empty()) {
        int clauseIndex = arrangement.front();
        arrangement.pop();
        newGroup.push_back(getClauseNumber(groupIndex, clauseIndex));
    }
    listOfGroups[groupIndex] = newGroup;
}

void GroupedClauses::cleanUpEmptyGroups()
{
    if (listOfGroups.size() == 1)
        return;

    int index = 0;
    while (index < listOfGroups.size()) {
        if (listOfGroups.at(index).empty()) {
            if (index + 1 != listOfGroups.size() - 1) { // not the last element
                mergeAndRemoveGroup(index, index + 1);
                continue;
            } else if (index != 0) {
                mergeAndRemoveGroup(index, index - 1);
                continue;
            } else { // the only empty group
                return;
            }
        }
        index++;
    }
}
