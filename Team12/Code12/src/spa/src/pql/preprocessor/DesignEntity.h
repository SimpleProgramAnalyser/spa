#ifndef SPA_DESIGNENTITY_H
#define SPA_DESIGNENTITY_H

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

#endif // SPA_DESIGNENTITY_H
