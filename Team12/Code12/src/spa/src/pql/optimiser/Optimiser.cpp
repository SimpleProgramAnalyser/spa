/**
 * Implementation of PQL query optimiser.
 */

#include "Optimiser.h"

#include <map>
#include <queue>
#include <set>

#include "ClauseGroupSorter.h"
#include "GroupedClauses.h"
#include "OptimiserUtils.h"

/**
 * Sorts the clauses in an AbstractQuery for faster evaluation.
 *
 * @param query The PQL query to sort.
 */
Void optimiseQuery(AbstractQuery& abstractQuery)
{
    // preprocessing
    substituteWithValues(abstractQuery);
    deleteDuplicateClauses(abstractQuery);

    // group and sort
    GroupedClauses groupedClauses = groupQueryClauses(abstractQuery);
    sortWithinEachGroup(groupedClauses);
    sortGroups(groupedClauses);

    // write changes into query
    sortClauses(groupedClauses, abstractQuery);
}

Void substituteSuchThat(SuchThatClause* suchThatClause, const Reference& target, const Reference& value)
{
    // target is either synonym or attribute. In any case, search for synonym/attribute
    // whose synonym matches target, and replace it with value.
    auto leftRef = suchThatClause->getRelationship().getLeftRef();
    auto rightRef = suchThatClause->getRelationship().getRightRef();
    if (leftRef.getValue() == target.getValue()) {
        suchThatClause->getRelationship().setLeftRef(value);
    }
    if (rightRef.getValue() == target.getValue()) {
        suchThatClause->getRelationship().setRightRef(value);
    }
}

Void substituteWith(WithClause* withClause, const Reference& target, const Reference& value)
{
    auto leftRef = withClause->getLeftReference();
    auto rightRef = withClause->getRightReference();
    if (leftRef.getValue() == target.getValue()) {
        withClause->setLeftReference(value);
    }
    if (rightRef.getValue() == target.getValue()) {
        withClause->setRightReference(value);
    }
}

Void substituteClauseList(List<Clause>& clauseList, const Reference& target, const Reference& value)
{
    for (auto& clause : clauseList) {
        switch (clause->getType()) {
        case SuchThatClauseType: {
            // NOLINTNEXTLINE
            SuchThatClause* suchThatClause = static_cast<SuchThatClause*>(clause.get());
            substituteSuchThat(suchThatClause, target, value);
            break;
        }
        case WithClauseType: {
            // NOLINTNEXTLINE
            WithClause* withClause = static_cast<WithClause*>(clause.get());
            substituteWith(withClause, target, value);
            break;
        }
        case PatternClauseType:
        case NonExistentClauseType:
            // Well, cannot substitute.
            break;
        }
    }
}

/**
 * For with clauses such as s.stmt# = 5, substitute all occurences of the synonym "s" with the value 5.
 *
 * @param abstractQuery The PQL query to substitute in.
 */
Void substituteWithValues(AbstractQuery& abstractQuery)
{
    /**
     * Scenarios for substitution:
     * 1. One side is integer, eg s.stmt# = 5
     * 2. One side is identifier, eg v.varName = "hello"
     */
    List<Clause>& clauseList = abstractQuery.getClausesUnsafe().getAllUnsafe();
    for (auto& clause : clauseList) {
        switch (clause->getType()) {
        case WithClauseType: {
            // NOLINTNEXTLINE
            WithClause* withClause = static_cast<WithClause*>(clause.get());
            auto leftRef = withClause->getLeftReference();
            auto rightRef = withClause->getRightReference();
            if (isValue(leftRef) && hasSynonym(rightRef)) {
                substituteClauseList(clauseList, rightRef, leftRef);
            } else if (isValue(rightRef) && hasSynonym(leftRef)) {
                substituteClauseList(clauseList, leftRef, rightRef);
            }
            break;
        }
        default:
            break;
        }
    }
}

/**
 * Remove clauses that are exactly the same. Eg. Next*(s1, s2) and Next*(s1, s2).
 *
 * @param abstractQuery
 * @return
 */
Void deleteDuplicateClauses(AbstractQuery& abstractQuery)
{
    // create a set, push all clauses in
    Vector<Clause*> clauses;
    for (int i = 0; i < abstractQuery.getClauses().count(); i++) {
        clauses.push_back(abstractQuery.getClausesUnsafe().remove(i));
    }

    // O(n^2) remove duplicate
    for (auto it = clauses.begin(); it != clauses.end(); it++) {
        for (auto it2 = it; it2 != clauses.end(); it2++) {
            if (**it == **it2) {
                clauses.erase(it2);
            }
        }
    }

    // create a new ClauseVector objectj
    ClauseVector newClauseVector;
    for (Clause* clause : clauses) {
        newClauseVector.add(clause);
    }

    // replace old ClauseVector object
    abstractQuery.setClauses(newClauseVector);
}

/**
 * Requirement: groupedClauses must have only one group only.
 *
 * @param groupedClauses
 * @return
 */
Void groupNoSynonym(GroupedClauses& groupedClauses)
{
    int originalGroup = 0;
    // add all the non-synonym clauses to a group.
    int noSynonymGroup = groupedClauses.addGroup();
    // check the main group for non-synonym clauses, move it into the noSynonymGroup (usually 1).
    int clauseIndex = 0;
    while (clauseIndex < groupedClauses.groupSize(originalGroup)) {
        // if has synonym, remove and don't increment
        if (hasSynonym(groupedClauses.getClause(originalGroup, clauseIndex))) {
            groupedClauses.moveClauseAcrossGroup(originalGroup, clauseIndex, noSynonymGroup,
                                                 groupedClauses.groupSize(noSynonymGroup) - 1);
        } else {
            clauseIndex++;
        }
    }
}

/**
 * Given a GroupedClauses and a group inside it, create an adjacency list where
 *  vertices: each clause, represented by its clauseNumber
 *  edges: each pair of vertices have an edge iff they share at least one synonym
 *
 * @param groupedClauses
 * @param groupIndex
 * @return
 */
std::map<Integer, std::set<Integer>> createAdjacencyList(GroupedClauses& groupedClauses, int groupIndex)
{
    std::map<Integer, std::set<Integer>> adjList;
    for (int i = 0; i < groupedClauses.groupSize(groupIndex); i++) {
        for (int j = 0; j < groupedClauses.groupSize(groupIndex); j++) {
            Clause* clause1 = groupedClauses.getClause(groupIndex, i);
            Clause* clause2 = groupedClauses.getClause(groupIndex, j);
            int clauseNumber1 = groupedClauses.getClauseNumber(groupIndex, i);
            int clauseNumber2 = groupedClauses.getClauseNumber(groupIndex, j);
            if (shareSynonym(clause1, clause2)) { // construct a bidirectional edge
                adjList[clauseNumber1].insert(clauseNumber2);
                adjList[clauseNumber2].insert(clauseNumber1);
            }
        }
    }
    return adjList;
}

/**
 * Run BFS on given graph, as described by a node list and an adjacency list.
 *
 * @param nodes
 * @param adjList
 *
 * @return A hash map, where keys are the nodes (represented by int), and the values are the components in the graph,
 * labelled from 0 onwards.
 */
std::unordered_map<int, int> BFS(Vector<int> nodes, std::map<Integer, std::set<Integer>> adjList)
{
    const int UNVISITED = -1;
    // BFS on the graph.
    // keep a running counter counting connected components, and a "visited" hashmap.
    int componentCount = 0;
    std::unordered_map<Integer, Integer> visited;
    for (int node : nodes)
        visited[node] = UNVISITED;
    std::queue<Integer> toVisit;
    int seed = nodes[0];
    while (seed < nodes.size()) {
        if (visited[seed] != UNVISITED) { // visited, skip
            seed++;
            continue;
        }
        toVisit.push(seed);
        while (!toVisit.empty()) {
            // visit the node in the queue
            int curr = toVisit.front();
            toVisit.pop();
            visited[curr] = componentCount;
            // visit all its unvisited neighbours
            for (auto neighbour : adjList[curr]) {
                if (visited[neighbour] == UNVISITED)
                    toVisit.push(neighbour);
            }
        }
        // done with one component once exited the while loop.
        componentCount++;
    }
    return visited;
}

/**
 * Requirement: group at index 0 will be grouped.
 *
 * @param groupedClauses
 * @return
 */
Void groupRest(GroupedClauses& groupedClauses)
{
    // create a graph (adjacency list).
    // vertices: each clause, represented by its clauseNumber
    // edges: each pair of vertices have an edge iff they share at least one synonym
    const int groupIndex = 0;
    auto nodes = groupedClauses.getGroup(groupIndex);
    std::map<Integer, std::set<Integer>> adjList = createAdjacencyList(groupedClauses, groupIndex);

    // BFS
    std::unordered_map<Integer, Integer> visited = BFS(nodes, adjList);

    // assert that all nodes should have been visited
    int componentCount = 0;
    for (auto node_component : visited) {
        // NOLINTNEXTLINE
        assert(node_component.second != -1 && "Should have been visited");
        componentCount = std::max(node_component.second, componentCount);
    }

    // count number of new groups needed, is the range of values.
    // remember the # of groups, this is the offset
    const int offset = groupedClauses.size();
    // create as many new groups as needed
    for (int i = 0; i < componentCount; i++)
        groupedClauses.addGroup();
    // iterate through visited and push the key into the group with index (offset + value)
    for (auto node_component : visited) {
        int node = node_component.first;
        int component = node_component.second;
        groupedClauses.moveClauseAcrossGroup(groupIndex, offset + component, node);
    }
    // finally first group should be empty. merge with any group (how about group 1)
    // NOLINTNEXTLINE
    assert(groupedClauses.groupSize(groupIndex) == 0 && "Group is not empty, BFS might have failed");
    groupedClauses.mergeAndRemoveGroup(groupIndex, groupIndex + 1); // merge with any group
}

/**
 * Group the clauses into a few groups, using the following principles:
 * 1. Clauses without any synonym in one group.
 * Consider the the remaining clauses as vertices in a graph, and add an edge between
 * two vertices if and only if the clauses share at least one synonym.
 * 2. Every connected component should belong to one and only one group.
 *
 * @param groupedClauses The abstract data type of grouped clauses.
 */
GroupedClauses groupQueryClauses(AbstractQuery& abstractQuery)
{
    GroupedClauses groupedClauses(abstractQuery);
    groupNoSynonym(groupedClauses);
    groupRest(groupedClauses);
    return groupedClauses;
}

/**
 * Start with clauses without synonyms, and prioritize groups with synonyms that do not return result.
 *
 * @param groupedClauses
 * @return
 */
Void sortGroups(GroupedClauses& groupedClauses)
{
    groupedClauses.sortGroups();
}

/**
 * Sort the clauses in abstractQuery based on the order in GroupedClauses.
 *
 * @param groupedClauses
 * @param abstractQuery
 * @return
 */
Void sortClauses(GroupedClauses& groupedClauses, AbstractQuery& abstractQuery)
{
    ClauseVector newClauseVector;
    for (int group = 0; group < groupedClauses.size(); group++) {
        for (int clauseIndex = 0; clauseIndex < groupedClauses.groupSize(group); clauseIndex++) {
            newClauseVector.add(
                abstractQuery.getClausesUnsafe().remove(groupedClauses.getClauseNumber(group, clauseIndex)));
        }
    }
    abstractQuery.setClauses(newClauseVector);
}
