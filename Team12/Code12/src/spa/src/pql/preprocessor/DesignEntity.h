/**
 * This class represents a Design Entity in the SIMPLE
 * program. When a synonym is declared in a query, a
 * certain Design Entity needs to be associated with it.
 */

#ifndef SPA_PQL_PREPROCESSOR_DESIGN_ENTITY_H
#define SPA_PQL_PREPROCESSOR_DESIGN_ENTITY_H

#include "DesignEntityType.h"
#include "Errorable.h"

class DesignEntity {
private:
    // Map of the design entity string to its corresponding DesignEntityType.
    static std::unordered_map<String, DesignEntityType> designEntityMap;

    DesignEntityType type;

public:
    static const ErrorMessage INVALID_DESIGN_ENTITY;

    DesignEntity();

    // Instantiate a DesignEntity with the given DesignEntityType.
    explicit DesignEntity(DesignEntityType designEntityType);

    // Instantiate a DesignEntity with the given string. If the
    // string is invalid, the DesignEntity will be instantiated
    // to a NonExistentType.
    explicit DesignEntity(const String& stringType);

    // Retrieves the DesignEntityType.
    DesignEntityType getType();

    Boolean operator==(const DesignEntity& designEntity) const;
};

#endif // SPA_PQL_PREPROCESSOR_DESIGN_ENTITY_H
