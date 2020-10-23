#ifndef SPA_REFERENCE_H
#define SPA_REFERENCE_H

#include "AqTypesUtils.h"
#include "Attribute.h"
#include "DeclarationTable.h"
#include "DesignEntity.h"

enum ReferenceType : char {
    SynonymRefType,
    WildcardRefType,
    LiteralRefType,
    IntegerRefType,
    AttributeRefType,
    InvalidRefType
};

typedef std::unordered_set<ReferenceType> ReferenceTypeSet;

typedef String ReferenceValue;

class Reference: public Errorable {
protected:
    ReferenceType referenceType;
    ReferenceValue referenceValue;
    DesignEntity designEntity;
    Attribute attribute;

public:
    static Reference createReference(String ref, DeclarationTable& declarationTable);

    Reference();
    Reference(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    Reference(ReferenceType refType, ReferenceValue refValue);
    Reference(ReferenceType refType, ReferenceValue refValue,
              DesignEntity
                  designEnt); // TODO: Refactor to remove refType parameter (not needed, will always be SynonymRefType)
    Reference(ReferenceValue refValue, DesignEntity designEnt, Attribute attr);
    ReferenceType getReferenceType() const;
    DesignEntity getDesignEntity() const;
    ReferenceValue getValue() const;
    Attribute getAttribute() const;
    AttributeValueType getAttributeValueType();
    Boolean isProcedure();
    Boolean isWildCard() const;
    Boolean operator==(const Reference& reference) const;
};

#endif // SPA_REFERENCE_H
