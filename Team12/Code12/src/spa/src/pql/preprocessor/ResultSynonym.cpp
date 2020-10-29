/**
 * Implementation of a ResultSynonym, or "Select"-ed
 * synonym in a Program Query Language query.
 */

#include "ResultSynonym.h"

#include <utility>

/************************/
/** Static Members      */
/************************/

const ErrorMessage ResultSynonym::INVALID_SYNONYM_MESSAGE = "Invalid naming for Synonym ";

/************************/
/** Constructors        */
/************************/

ResultSynonym::ResultSynonym(String syn): Errorable(), synonym(std::move(syn)), attribute() {}

ResultSynonym::ResultSynonym(QueryErrorType queryErrorType): Errorable(queryErrorType), synonym(), attribute() {}

ResultSynonym::ResultSynonym(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, std::move(errorMessage)), synonym(), attribute()
{}

ResultSynonym::ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity):
    Errorable(), synonym(std::move(syn)), attribute()
{
    auto got = Attribute::attributeMap.find(attr);
    if (got == Attribute::attributeMap.end()) {
        this->errorType = QuerySyntaxError;
        this->errorMessage = "Attribute " + attr + " does not exist for Synonym " + syn;
    } else {
        if (!Attribute::validateDesignEntityAttributeSemantics(designEntity.getType(), got->second)) {
            this->errorType = QuerySemanticsError;
            this->errorMessage = "Attribute " + attr + " cannot be used for Synonym " + syn;
        } else {
            attribute = Attribute(got->second);
        }
    }
}

/************************/
/** Instance Methods    */
/************************/

Synonym ResultSynonym::getSynonym() const
{
    return synonym;
}

Attribute ResultSynonym::getAttribute() const
{
    return attribute;
}

QueryErrorType ResultSynonym::getErrorType() const
{
    return errorType;
}

ErrorMessage ResultSynonym::getErrorMessage() const
{
    return errorMessage;
}

Boolean ResultSynonym::operator==(const ResultSynonym& resultSynonym) const
{
    return this->synonym == resultSynonym.synonym && this->attribute == resultSynonym.attribute;
}

Boolean ResultSynonym::operator!=(const ResultSynonym& resultSynonym) const
{
    return this->synonym != resultSynonym.synonym || this->attribute != resultSynonym.attribute;
}