#include "AqTypes.h"

/************************/
/** Constructors        */
/************************/

WithClause::WithClause(Reference& leftRef, Reference& rightRef):
    Clause{WithClauseType}, leftReference{leftRef}, rightReference{rightRef}
{}

/*&&&********************/
/** Static Methods      */
/************************/

Clause* WithClause::createWithClause(const String& clauseConstraint, DeclarationTable& declarationTable) {
    StringVector splitStringVector = splitByDelimiter(clauseConstraint, "=");
    if (splitStringVector.size() != 2) {
        return Clause::invalidClause(WithClauseType);
    }

    Reference leftRef = Reference::createReference(splitStringVector.at(0), declarationTable);
    Reference rightRef = Reference::createReference(splitStringVector.at(1), declarationTable);

    if (leftRef.isInvalid() || rightRef.isInvalid()) {
        return Clause::invalidClause(WithClauseType);
    }

    // Validate equality of AttributeValueType for both References
    AttributeValueType leftRefValueType = leftRef.getAttributeValueType();
    AttributeValueType rightRefValueType = rightRef.getAttributeValueType();

    if (leftRefValueType != rightRefValueType) {
        return Clause::invalidClause(WithClauseType);
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