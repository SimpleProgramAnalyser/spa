/**
 * Class that represents a Reference in an Abstract Query,
 * which are used by clauses to represent the relationships
 * between synonyms, or the constraints of a single synonym.
 */

#ifndef SPA_PQL_PREPROCESSOR_REFERENCE_H
#define SPA_PQL_PREPROCESSOR_REFERENCE_H

#include "AqTypesUtils.h"
#include "Attribute.h"
#include "DeclarationTable.h"
#include "DesignEntity.h"

// All the types of References.
enum class ReferenceType : char {
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
    /**
     * Creates a Reference using the given ref String. It will
     * determine the ReferenceType based on the ref given.
     * If the ref is a synonym, the design entity of the synonym
     * will be stored in the Reference object.
     *
     * If the ref is an invalid form of a Reference, an invalid
     * Reference will be returned.
     *
     * @param ref   String of the reference to be constructed.
     * @return      A Reference based on ref.
     */
    static Reference createReference(String ref, DeclarationTable& declarationTable);

    Reference();

    // Instantiate an erroneous Reference with the given QueryErrorType
    // and ErrorMessage.
    Reference(QueryErrorType queryErrorType, ErrorMessage errorMessage);

    // Instantiate a Reference with the given ReferenceType and ReferenceValue.
    Reference(ReferenceType refType, ReferenceValue refValue);

    // Instantiate a Reference with the given ReferenceType, ReferenceValue
    // and DesignEntity.
    Reference(ReferenceType refType, ReferenceValue refValue, DesignEntity designEnt);

    // Instantiate a Reference of SynonymRefType with the given ReferenceValue,
    // DesignEntity and Attribute.
    Reference(ReferenceValue refValue, DesignEntity designEnt, Attribute attr);

    // Retrieves the ReferenceType of the Reference.
    ReferenceType getReferenceType() const;

    // Retrieves the DesignEntity of the Reference.
    DesignEntity getDesignEntity() const;

    // Retrieves the ReferenceValue of the Reference.
    ReferenceValue getValue() const;

    // Retrieves the Attribute of the Reference.
    Attribute getAttribute() const;

    // Retrieves the AttributeValueType of the Reference.
    AttributeValueType getAttributeValueType();

    // Checks if the Reference is a Synonym of ProcedureType.
    Boolean isProcedure();

    // Checks if the Reference is a Wildcard.
    Boolean isWildCard() const;

    Boolean operator==(const Reference& reference) const;
};

#endif // SPA_PQL_PREPROCESSOR_REFERENCE_H
