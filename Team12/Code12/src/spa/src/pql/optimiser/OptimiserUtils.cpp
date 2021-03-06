#include "OptimiserUtils.h"

#include <set>

#include "GroupedClauses.h"
#include "Optimiser.h"

/**
 * Checks if a clause contains synonym, which can be either just synonyms or with attributes.
 *
 * @param clause
 * @return
 */
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

unsigned int countSynonym(Clause* clause)
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

/**
 * Returns the set of synonyms found in clause, either from synonyms or attributes.
 *
 * @param clause
 * @return
 */
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

/**
 * Checks if two clauses save any synonym in constant time.
 *
 * @param clause1
 * @param clause2
 * @return
 */
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

/**
 * A reference in a clause can initiate substitution if
 * 1. it is a ProgLine synonym
 * 2. it is an attribute but not read.varName, print.varName, call.procName.
 *
 * @param reference
 * @return
 */
bool canInitiateSubstitution(Reference& reference)
{
    std::unordered_map<DesignEntityType, AttributeType> bannedTarget{
        {ReadType, VarNameType}, {PrintType, VarNameType}, {CallType, ProcNameType}};

    auto designEntityType = reference.getDesignEntity().getType();
    auto referenceType = reference.getReferenceType();
    bool isProgLine = referenceType == SynonymRefType && designEntityType == Prog_LineType;
    bool isAllowedTarget = referenceType == AttributeRefType
                           && !(bannedTarget.count(designEntityType)
                                && bannedTarget.at(designEntityType) == reference.getAttribute().getType());
    return isProgLine || isAllowedTarget;
}

/**
 * A reference in a clause can accept substitution if
 * 1. It is a synonym
 * 2. it is an attribute but not read.varName, print.varName, call.procName.
 *
 * @param reference
 * @return
 */
bool canBeSubstituted(Reference& reference)
{
    std::unordered_map<DesignEntityType, AttributeType> bannedTarget{
        {ReadType, VarNameType}, {PrintType, VarNameType}, {CallType, ProcNameType}};

    auto designEntityType = reference.getDesignEntity().getType();
    auto referenceType = reference.getReferenceType();
    bool isAllowedTarget = referenceType == AttributeRefType
                           && !(bannedTarget.count(designEntityType)
                                && bannedTarget.at(designEntityType) == reference.getAttribute().getType());
    return referenceType == SynonymRefType || isAllowedTarget;
}
