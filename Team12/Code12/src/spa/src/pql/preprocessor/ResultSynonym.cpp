#include "AqTypes.h"

ResultSynonym::ResultSynonym(String syn): synonym(std::move(syn)), attribute(NoAttributeType) {}

ResultSynonym::ResultSynonym(Boolean hasError): attribute(NoAttributeType)
{
    setError(hasError);
}

ResultSynonym::ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity): synonym(std::move(syn))
{
    auto got = Attribute::attributeMap.find(attr);
    if (got == Attribute::attributeMap.end()) {
        setError(true);
    } else {
        if (!Attribute::validateDesignEntityAttributeSemantics(designEntity.getType(), got->second)) {
            setError(true);
        } else {
            attribute = Attribute(got->second);
        }
    }
}

Synonym ResultSynonym::getSynonym() const
{
    return synonym;
}

Attribute ResultSynonym::getAttribute()
{
    return attribute;
}

Boolean ResultSynonym::operator==(const ResultSynonym& resultSynonym)
{
    return this->synonym == resultSynonym.synonym && this->attribute == resultSynonym.attribute;
}

Boolean ResultSynonym::operator!=(const ResultSynonym& resultSynonym)
{
    return this->synonym != resultSynonym.synonym || this->attribute != resultSynonym.attribute;
}