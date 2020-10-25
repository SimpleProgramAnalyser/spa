#include "ResultSynonym.h"

#include <utility>

/************************/
/** Static Members      */
/************************/

const ErrorMessage ResultSynonym::INVALID_SYNONYM_MESSAGE = "Invalid naming for Synonym ";

/************************/
/** Constructors        */
/************************/

ResultSynonym::ResultSynonym(String syn): synonym(std::move(syn)), attribute(NoAttributeType), Errorable() {}

ResultSynonym::ResultSynonym(QueryErrorType queryErrorType): Errorable(queryErrorType) {}

ResultSynonym::ResultSynonym(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, std::move(errorMessage))
{}

ResultSynonym::ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity): synonym(std::move(syn))
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