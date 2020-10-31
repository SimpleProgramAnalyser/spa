/**
 * Implementation of PQL query optimiser.
 */

#include "Optimiser.h"

#include <set>

#include "GroupedClauses.h"

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
    GroupedClauses groupedClauses = GroupedClauses(abstractQuery);
    groupQueryClauses(groupedClauses);
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

inline bool isValue(const Reference& reference)
{
    return reference.getReferenceType() == LiteralRefType || reference.getReferenceType() == IntegerRefType;
}

inline bool canSubstitute(const Reference& reference)
{
    return reference.getReferenceType() == AttributeRefType || reference.getReferenceType() == SynonymRefType;
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
    List<Clause> clauseList = abstractQuery.getClausesUnsafe().getAllUnsafe();
    for (auto& clause : clauseList) {
        switch (clause->getType()) {
        case WithClauseType: {
            // NOLINTNEXTLINE
            WithClause* withClause = static_cast<WithClause*>(clause.get());
            auto leftRef = withClause->getLeftReference();
            auto rightRef = withClause->getRightReference();
            if (isValue(leftRef) && canSubstitute(rightRef)) {
                substituteClauseList(clauseList, rightRef, leftRef);
            } else if (isValue(rightRef) && canSubstitute(leftRef)) {
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
 * Group the clauses into a few groups, using the following principles:
 * 1. Clauses without any synonym in one group.
 * Consider the the remaining clauses as vertices in a graph, and add an edge between
 * two vertices if and only if the clauses share at least one synonym.
 * 2. Every connected component should belong to one and only one group.
 *
 * @param groupedClauses The abstract data type of grouped clauses.
 */
Void groupQueryClauses(GroupedClauses& groupedClauses)
{
    // TODO
}

/**
 * (Subjective) Sort the groups of clauses in the GroupedClauses object using the following principles, copied from
 * lecture notes:
 *
 * Prioritize clauses with one constant and one synonym
 * Prioritize clauses with less number of results: Follows, Modifies, etc.
 * Sort clauses such that at least one synonym has been computed in a previous clause
 * Prioritize with-clauses – more restrictive than such that clauses
 * Evaluating pattern-clauses – similar to any such that clause
 * Push Affects(*) clauses on the last positions in a group
 *
 * @param groupedClauses
 */
Void sortWithinEachGroup(GroupedClauses& groupedClauses)
{
    // TODO
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
