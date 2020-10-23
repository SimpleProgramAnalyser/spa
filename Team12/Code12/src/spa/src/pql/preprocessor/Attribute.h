#ifndef SPA_ATTRIBUTE_H
#define SPA_ATTRIBUTE_H

#include "AqTypesUtils.h"
#include "DesignEntity.h"

enum AttributeType : char {
    ProcNameType,
    VarNameType,
    ValueType,
    StmtNumberType,
    NoAttributeType,
    InvalidAttributeType
};
enum AttributeValueType : char { IntegerValueType, NameValueType, InvalidValueType };

// Hash function for Attribute
template <>
struct std::hash<AttributeType> {
    std::size_t operator()(const AttributeType& attributeType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(attributeType));
    }
};

class Attribute {
private:
    AttributeType type;

public:
    static std::unordered_map<String, AttributeType> attributeMap;
    static std::unordered_map<AttributeType, DesignEntityTypeSet> attributeDesignEntityMap;

    static Boolean validateDesignEntityAttributeSemantics(DesignEntityType designEntityType,
                                                          AttributeType attributeType);

    Attribute();
    explicit Attribute(AttributeType attributeType);
    explicit Attribute(String attributeTypeString);
    AttributeType getType();
    Boolean operator==(const Attribute& attribute) const;
    Boolean operator!=(const Attribute& attribute) const;
};

#endif // SPA_ATTRIBUTE_H
