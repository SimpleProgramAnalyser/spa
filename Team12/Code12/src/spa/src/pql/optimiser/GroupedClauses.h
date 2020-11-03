/**
 * A class handling the grouping of clauses
 * to speed up the query evaluation.
 */

#ifndef SPA_PQL_GROUPED_CLAUSES_H
#define SPA_PQL_GROUPED_CLAUSES_H

#include <Types.h>
#include <algorithm>
#include <pql/preprocessor/AqTypes.h>
#include <queue>

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
    int size() const;
    int groupSize(int groupIndex) const;
    int getClauseNumber(int groupIndex, int clauseIndex) const;
    const Vector<Integer>& getGroup(int groupIndex) const;
    Clause* getClause(int groupIndex, int clauseIndex);
    bool groupHasSynonym(int groupIndex);
    bool synonymIsReturned(int groupIndex);

    // group/clause operations
    int addGroup();
    void mergeAndRemoveGroup(int groupToRemove, int groupToMergeInto);
    void swapGroups(int groupIndex1, int groupIndex2);
    void swapClauseWithinGroup(int groupIndex, int clause1, int clause2);
    void moveClauseAcrossGroup(int originalGroupIndex, int originalIndex, int destGroupIndex, int destIndex);
    void moveClauseAcrossGroup(int originalGroupIndex, int destGroupIndex, int clauseNumber);
    void swapClausesAcrossGroup(int groupIndex1, int clause1, int groupIndex2, int clause2);
    void applyArrangementToGroup(std::queue<uint> arrangement, int groupIndex);
    GroupedClauses() = delete;

    // compare functions
    static bool compareGroups(int group1, int group2, GroupedClauses* groupedClauses);
    void sortGroups();

private:
    // each clause is represented as a integer, its position in the List<Clause> from the original
    Vector<Vector<Integer>> listOfGroups;
    const AbstractQuery& abstractQuery;
};

#endif // SPA_PQL_GROUPED_CLAUSES_H
