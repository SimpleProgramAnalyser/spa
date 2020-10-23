#include "SuchThatClause.h"

/************************/
/** Constructors        */
/************************/

SuchThatClause::SuchThatClause(Relationship& r): Clause(SuchThatClauseType), relationship{r} {}

/************************/
/** Static Methods      */
/************************/

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

    RelationshipReferenceType relRefType = Relationship::getRelRefType(relRef);
    if (relRefType == InvalidRelationshipType) {
        return new Clause(SuchThatClauseType, QuerySyntaxError, "Invalid Relationship type " + relRef);
    }

    String references
        = pair.second.substr(0, pair.second.size() - 1); // remove the last char which is a close parentheses
    StringPair refPair = util::splitByFirstDelimiter(references, ',');
    String leftRefString = refPair.first;
    String rightRefString = refPair.second;

    Reference leftReference = Reference::createReference(leftRefString, declarationTable);
    Reference rightReference = Reference::createReference(rightRefString, declarationTable);

    if (leftReference.isInvalid()) {
        return new Clause(SuchThatClauseType, leftReference.getErrorType(), leftReference.getErrorMessage());
    } else if (rightReference.isInvalid()) {
        return new Clause(SuchThatClauseType, rightReference.getErrorType(), rightReference.getErrorMessage());
    }

    Relationship relationship = Relationship::createRelationship(relRefType, leftReference, rightReference);
    if (relationship.isInvalid()) {
        return new Clause(SuchThatClauseType, QuerySemanticsError, relationship.getErrorMessage());
    }

    return new SuchThatClause(relationship);
}

/************************/
/** Instance Methods    */
/************************/

Relationship SuchThatClause::getRelationship()
{
    return relationship;
}

Boolean SuchThatClause::operator==(const SuchThatClause& suchThatClause)
{
    return this->relationship == suchThatClause.relationship;
}