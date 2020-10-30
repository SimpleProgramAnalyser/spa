//
// Created by titanx on 30/10/20.
//

#ifndef SPA_GROUPEDCLAUSES_H
#define SPA_GROUPEDCLAUSES_H

#include <Types.h>
#include <algorithm>
#include <pql/preprocessor/AqTypes.h>

class GroupedClauses {
public:
    /**
     * Given an abstract query, create a GroupClauses object, with an only group containing all clauses in the
     * abstractQuery.
     *
     * @param abstractQuery The AbstractQuery containing the clauses to be grouped
     */
    GroupedClauses(AbstractQuery& abstractQuery);

    // information retrieval
    int size();
    int groupSize(int groupIndex);
    Clause* getClause(int groupIndex, int clauseIndex);
    bool hasSynonym(int groupIndex);
    bool synonymIsReturned(int groupIndex);

    // group/clause operations
    void addGroup();
    void mergeAndRemoveGroup(int groupToRemove, int groupToMergeInto);
    void swapGroups(int groupIndex1, int groupIndex2);
    void swapClauseWithinGroup(int groupIndex, int clause1, int clause2);
    void moveClauseAcrossGroup(int originalGroupIndex, int originalIndex, int destGroupIndex, int destIndex);
    void swapClausesAcrossGroup(int groupIndex1, int clause1, int groupIndex2, int clause2);
    GroupedClauses() = delete;

    // compare functions
    static bool compareGroups(int group1, int group2, GroupedClauses* groupedClauses);
    void sortGroups();

private:
    // each clause is represented as a integer, its position in the List<Clause> from the original
    Vector<Vector<Integer>> listOfGroups;
    const AbstractQuery& abstractQuery;
};
#endif // SPA_GROUPEDCLAUSES_H
