#include "GroupedClauses.h"

/**
 * Create a new GroupClauses instance based on an AbstractQuery.
 *
 * @param abstractQuery
 */
GroupedClauses::GroupedClauses(AbstractQuery& abstractQuery): clauseVector(abstractQuery.getClauses())
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
void GroupedClauses::addGroup()
{
    listOfGroups.emplace_back();
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
    auto originalGroup = listOfGroups[originalGroupIndex];
    auto destGroup = listOfGroups[destGroupIndex];
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
    // push all clauses into the target group
    listOfGroups[groupToMergeInto].insert(listOfGroups[groupToMergeInto].end(), listOfGroups[groupToRemove].begin(),
                                          listOfGroups[groupToRemove].end());
    // and remove the copied group
    listOfGroups.erase(listOfGroups.begin() + groupToRemove);
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
    return clauseVector.get(listOfGroups[groupIndex][clauseIndex]);
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
int GroupedClauses::size()
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
int GroupedClauses::groupSize(int groupIndex)
{
    return listOfGroups[groupIndex].size();
}
