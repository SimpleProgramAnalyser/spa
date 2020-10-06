#include "AqTypes.h"

std::unordered_map<String, Attribute> ResultSynonym::attributeMap{
    {"procName", ProcNameType},
    {"varName", VarNameType},
    {"value", ValueType},
    {"stmt#", ProcNameType},
};

ResultSynonym::ResultSynonym(String syn): synonym(std::move(syn)), attribute(NoAttribute){};

ResultSynonym::ResultSynonym(Boolean hasError): attribute(NoAttribute)
{
    setError(hasError);
};

ResultSynonym::ResultSynonym(Synonym syn, const String& attr): synonym(std::move(syn))
{
    auto got = attributeMap.find(attr);
    if (got == attributeMap.end()) {
        setError(true);
    } else {
        attribute = got->second;
    }
};

Synonym ResultSynonym::getSynonym() const
{
    return synonym;
}

Attribute ResultSynonym::getAttribute()
{
    return attribute;
};

Boolean ResultSynonym::operator==(const ResultSynonym& resultSynonym)
{
    return this->synonym == resultSynonym.synonym && this->attribute == resultSynonym.attribute;
}

Boolean ResultSynonym::operator!=(const ResultSynonym& resultSynonym)
{
    return this->synonym != resultSynonym.synonym || this->attribute != resultSynonym.attribute;
}