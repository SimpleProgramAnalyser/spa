/**
 * Representation of an Attribute (e.g. stmt#, varName)
 * in a Program Query Language query.
 */

#ifndef SPA_PQL_PREPROCESSOR_ATTRIBUTE_H
#define SPA_PQL_PREPROCESSOR_ATTRIBUTE_H

#include "AqTypesUtils.h"
#include "DesignEntity.h"

// All the different types of Attributes.
enum class AttributeType : char {
    ProcNameType,
    VarNameType,
    ValueType,
    StmtNumberType,
    NoAttributeType,
    InvalidAttributeType
};

// All the underlying types of AttributeTypes.
enum AttributeValueType : char { IntegerValueType, NameValueType, InvalidValueType };

// Hash function for AttributeType
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
    // Map of attribute string to their AttributeType.
    static std::unordered_map<String, AttributeType> attributeMap;
    // Map of AttributeType to their valid DesignEntityTypes
    static std::unordered_map<AttributeType, DesignEntityTypeSet> attributeDesignEntityMap;
    // Checks if the DesignEntityType corresponds to the AttributeType.
    static Boolean validateDesignEntityAttributeSemantics(DesignEntityType designEntityType,
                                                          AttributeType attributeType);

    Attribute();
    explicit Attribute(AttributeType attributeType);
    explicit Attribute(const String& attributeTypeString);

    // Retrieves the underlying AttributeType
    AttributeType getType();

    Boolean operator==(const Attribute& attribute) const;
    Boolean operator!=(const Attribute& attribute) const;
};

#endif // SPA_PQL_PREPROCESSOR_ATTRIBUTE_H
