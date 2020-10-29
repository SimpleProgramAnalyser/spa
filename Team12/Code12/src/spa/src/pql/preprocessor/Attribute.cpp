/**
 * Implementation of a class representing an Attribute
 * (e.g. stmt#, varName) in a Program Query Language query.
 */

#include "Attribute.h"

/************************/
/** Static Members      */
/************************/

std::unordered_map<String, AttributeType> Attribute::attributeMap{
    {"procName", ProcNameType},
    {"varName", VarNameType},
    {"value", ValueType},
    {"stmt#", StmtNumberType},
};

std::unordered_map<AttributeType, DesignEntityTypeSet> Attribute::attributeDesignEntityMap{
    {ProcNameType, {ProcedureType, CallType}},
    {VarNameType, {ReadType, VariableType, PrintType}},
    {ValueType, {ConstantType}},
    {StmtNumberType, {StmtType, ReadType, PrintType, CallType, WhileType, IfType, AssignType}}};

/************************/
/** Constructors        */
/************************/

Attribute::Attribute(): type{NoAttributeType} {}

Attribute::Attribute(AttributeType attributeType): type{attributeType} {}

Attribute::Attribute(const String& attributeTypeString): type(NoAttributeType)
{
    auto got = attributeMap.find(attributeTypeString);
    if (got == attributeMap.end()) {
        type = InvalidAttributeType;
    } else {
        type = got->second;
    }
}

/************************/
/** Static Methods      */
/************************/

Boolean Attribute::validateDesignEntityAttributeSemantics(DesignEntityType designEntityType,
                                                          AttributeType attributeType)
{
    auto gotAttr = Attribute::attributeDesignEntityMap.find(attributeType);
    if (gotAttr == Attribute::attributeDesignEntityMap.end()) {
        return false;
    }

    auto gotDesignEntity = gotAttr->second.find(designEntityType);

    return gotDesignEntity != gotAttr->second.end();
}

/************************/
/** Instance Methods    */
/************************/

AttributeType Attribute::getType()
{
    return type;
}

Boolean Attribute::operator==(const Attribute& attribute) const
{
    return this->type == attribute.type;
}

Boolean Attribute::operator!=(const Attribute& attribute) const
{
    return this->type != attribute.type;
}
