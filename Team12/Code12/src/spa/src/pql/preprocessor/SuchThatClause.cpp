#include "AqTypes.h"

SuchThatClause::SuchThatClause(Relationship& r): Clause(SuchThatClauseType), relationship{r} {}

Relationship SuchThatClause::getRelationship()
{
    return relationship;
}

Boolean SuchThatClause::operator==(const SuchThatClause& suchThatClause)
{
    return this->relationship == suchThatClause.relationship;
}

/**
 * Processes the clause constraint string for a
 * SuchThatClause, by abstracting it into its relevant
 * Relationship and References.
 *
 * @param clauseConstraint  String of the clause constraint with all
 *                          whitespaces removed.
 * @return                  A Clause pointer of the SuchThatClause
 *                          that was constructed.
 */
Clause* SuchThatClause::createSuchThatClause(const String& clauseConstraint, DeclarationTable& declarationTable)
{
    StringPair pair = util::splitByFirstDelimiter(clauseConstraint, '(');

    String relRef = pair.first;
    if (!util::isRelationshipReference(relRef)) {
        return Clause::invalidClause(SuchThatClauseType);
    }

    String references
        = pair.second.substr(0, pair.second.size() - 1); // remove the last char which is a close parentheses
    StringPair refPair = util::splitByFirstDelimiter(references, ',');
    String leftRefString = refPair.first;
    String rightRefString = refPair.second;

    Reference leftReference = Reference::createReference(leftRefString, declarationTable);
    Reference rightReference = Reference::createReference(rightRefString, declarationTable);

    if (leftReference.isInvalid() || rightReference.isInvalid()) {
        return Clause::invalidClause(SuchThatClauseType);
    }

    Relationship relationship = Relationship::createRelationship(relRef, leftReference, rightReference);
    if (relationship.isInvalid()) {
        return Clause::invalidClause(SuchThatClauseType);
    }

    return new SuchThatClause(relationship);
}