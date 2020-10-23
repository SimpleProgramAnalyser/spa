#ifndef SPA_DESIGNENTITY_H
#define SPA_DESIGNENTITY_H

#include "DesignEntityType.h"
#include "Errorable.h"

class DesignEntity {
private:
    static std::unordered_map<String, DesignEntityType> designEntityMap;

    DesignEntityType type;

public:
    static const ErrorMessage INVALID_DESIGN_ENTITY;

    DesignEntity();
    explicit DesignEntity(DesignEntityType designEntityType);
    explicit DesignEntity(const String& stringType);
    DesignEntityType getType();
    Boolean operator==(const DesignEntity& designEntity) const;
};

#endif // SPA_DESIGNENTITY_H
