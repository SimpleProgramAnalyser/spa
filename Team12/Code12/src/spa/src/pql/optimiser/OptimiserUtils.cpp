#include "OptimiserUtils.h"

bool hasSynonym(Clause* clause)
{
    switch (clause->getType()) {
    case SuchThatClauseType: { // has synyonum if left or right is synonym.
        // NOLINTNEXTLINE
        SuchThatClause* suchThatClause = static_cast<SuchThatClause*>(clause);
        auto leftRef = suchThatClause->getRelationship().getLeftRef();
        auto rightRef = suchThatClause->getRelationship().getRightRef();
        if (hasSynonym(leftRef) || hasSynonym(rightRef))
            return true;
        break;
    }
    case PatternClauseType: { // patterns always have synonym.
        return true;
    }
    case WithClauseType: {
        // NOLINTNEXTLINE
        WithClause* withClause = static_cast<WithClause*>(clause);
        auto leftRef = withClause->getLeftReference();
        auto rightRef = withClause->getRightReference();
        if (hasSynonym(leftRef) || hasSynonym(rightRef))
            return true;
        break;
    }
    default:
        return false;
    }
    return false;
}

uint countSynonym(Clause* clause)
{
    switch (clause->getType()) {
    case SuchThatClauseType: {
        // NOLINTNEXTLINE
        SuchThatClause* suchThatClause = static_cast<SuchThatClause*>(clause);
        auto leftRef = suchThatClause->getRelationship().getLeftRef();
        auto rightRef = suchThatClause->getRelationship().getRightRef();
        return hasSynonym(leftRef) + hasSynonym(rightRef);
    }
    case PatternClauseType: {
        // NOLINTNEXTLINE
        PatternClause* patternClause = static_cast<PatternClause*>(clause);
        // pattern has one fixed synonym, and might have at most one more in entRef
        return 1 + hasSynonym(patternClause->getEntRef());
    }
    case WithClauseType: {
        // NOLINTNEXTLINE
        WithClause* withClause = static_cast<WithClause*>(clause);
        auto leftRef = withClause->getLeftReference();
        auto rightRef = withClause->getRightReference();
        return hasSynonym(leftRef) + hasSynonym(rightRef);
    }
    default:
        return false;
    }
}

std::set<Synonym> getSynonyms(Clause* clause)
{
    std::set<Synonym> toReturn;
    switch (clause->getType()) {
    case SuchThatClauseType: {
        // NOLINTNEXTLINE
        SuchThatClause* suchThatClause = static_cast<SuchThatClause*>(clause);
        auto leftRef = suchThatClause->getRelationship().getLeftRef();
        auto rightRef = suchThatClause->getRelationship().getRightRef();
        if (hasSynonym(leftRef)) {
            toReturn.insert(leftRef.getValue());
        }
        if (hasSynonym(rightRef))
            toReturn.insert(rightRef.getValue());
        break;
    }
    case WithClauseType: {
        // NOLINTNEXTLINE
        WithClause* withClause = static_cast<WithClause*>(clause);
        auto leftRef = withClause->getLeftReference();
        auto rightRef = withClause->getRightReference();
        if (hasSynonym(leftRef)) {
            toReturn.insert(leftRef.getValue());
        }
        if (hasSynonym(rightRef))
            toReturn.insert(rightRef.getValue());
        break;
    }
    case PatternClauseType: {
        // NOLINTNEXTLINE
        PatternClause* patternClause = static_cast<PatternClause*>(clause);
        // pattern clauses have pattern synonym and entity ref, ie "pattern a(a1,_)"
        // for example "a" is a pattern synonym, and "a1" is an entity ref (which can be int or ident too)
        toReturn.insert(patternClause->getPatternSynonym());
        if (hasSynonym(patternClause->getEntRef()))
            toReturn.insert(patternClause->getEntRef().getValue());
        break;
    }
    case NonExistentClauseType:
        break;
    }
    return toReturn;
}

bool shareSynonym(Clause* clause1, Clause* clause2)
{
    // push all synonyms
    std::set<String> synonyms1 = getSynonyms(clause1), synonyms2 = getSynonyms(clause2);
    std::set<String> synonyms;
    synonyms.insert(synonyms1.begin(), synonyms1.end());
    synonyms.insert(synonyms2.begin(), synonyms2.end());

    // there are shared synonym between two clauses iff the combined set is smaller than the
    // original - intersection is non-empty.
    return synonyms.size() < synonyms1.size() + synonyms2.size();
}
