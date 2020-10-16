#include <utility>

#include "AqTypes.h"

/************************/
/** Static Members      */
/************************/

const ErrorMessage ResultSynonym::INVALID_SYNONYM_MESSAGE = "Invalid naming for Synonym ";

/************************/
/** Constructors        */
/************************/

ResultSynonym::ResultSynonym(String syn): synonym(std::move(syn)), attribute(NoAttributeType) {}

ResultSynonym::ResultSynonym(QueryErrorType queryErrorType)
{
    setError(queryErrorType);
}

ResultSynonym::ResultSynonym(QueryErrorType queryErrorType, ErrorMessage errorMessage)
{
    setError(queryErrorType, std::move(errorMessage));
}

ResultSynonym::ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity): synonym(std::move(syn))
{
    auto got = Attribute::attributeMap.find(attr);
    if (got == Attribute::attributeMap.end()) {
        setError(QuerySyntaxError, "Attribute " + attr + " does not exist for Synonym " + syn);
    } else {
        if (!Attribute::validateDesignEntityAttributeSemantics(designEntity.getType(), got->second)) {
            setError(QuerySemanticsError, "Attribute " + attr + " cannot be used for Synonym " + syn);
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

Boolean ResultSynonym::operator==(const ResultSynonym& resultSynonym) const
{
    return this->synonym == resultSynonym.synonym && this->attribute == resultSynonym.attribute;
}

Boolean ResultSynonym::operator!=(const ResultSynonym& resultSynonym) const
{
    return this->synonym != resultSynonym.synonym || this->attribute != resultSynonym.attribute;
}