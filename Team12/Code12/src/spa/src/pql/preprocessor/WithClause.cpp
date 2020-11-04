/**
 * Implementation of an abstract representation of
 * a Program Query Language query "with" clause.
 */

#include "WithClause.h"

#include <utility>

/************************/
/** Constructors        */
/************************/

WithClause::WithClause(Reference& leftRef, Reference& rightRef):
    Clause{WithClauseType}, leftReference{leftRef}, rightReference{rightRef}
{}

/************************/
/** Static Methods      */
/************************/

Clause* WithClause::createWithClause(const String& clauseConstraint, DeclarationTable& declarationTable)
{
    StringVector splitStringVector = splitByDelimiter(clauseConstraint, "=");
    if (splitStringVector.size() != 2) {
        return new Clause(WithClauseType, QuerySyntaxError, "Invalid syntax in WithClause " + clauseConstraint);
    }

    Reference leftRef = Reference::createReference(splitStringVector.at(0), declarationTable);
    Reference rightRef = Reference::createReference(splitStringVector.at(1), declarationTable);

    if (leftRef.isInvalid()) {
        return new Clause(SuchThatClauseType, leftRef.getErrorType(), leftRef.getErrorMessage());
    } else if (rightRef.isInvalid()) {
        return new Clause(SuchThatClauseType, rightRef.getErrorType(), rightRef.getErrorMessage());
    }

    // Validate equality of AttributeValueType for both References
    AttributeValueType leftRefValueType = leftRef.getAttributeValueType();
    AttributeValueType rightRefValueType = rightRef.getAttributeValueType();

    if (leftRefValueType != rightRefValueType) {
        return new Clause(WithClauseType, QuerySemanticsError, "Left and Right value type of With Clause do not match");
    }

    return new WithClause(leftRef, rightRef);
}

/*************************/
/** Instance Methods    */
/************************/

Reference WithClause::getLeftReference()
{
    return leftReference;
}

Reference WithClause::getRightReference()
{
    return rightReference;
}

Boolean WithClause::operator==(const WithClause& withClause)
{
    return this->leftReference == withClause.leftReference && this->rightReference == withClause.rightReference;
}

Void WithClause::setLeftReference(Reference reference)
{
    leftReference = std::move(reference);
}

Void WithClause::setRightReference(Reference reference)
{
    rightReference = std::move(reference);
}
