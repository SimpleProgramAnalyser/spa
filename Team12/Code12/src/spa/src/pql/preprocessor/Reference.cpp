#include <utility>

#include "AqTypes.h"

/************************/
/** Constructors        */
/************************/

Reference::Reference(): referenceType{InvalidRefType}, designEntity{}, attribute{NoAttributeType} {}

Reference::Reference(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    referenceType{InvalidRefType}, attribute{NoAttributeType}
{
    this->setError(queryErrorType, std::move(errorMessage));
}

Reference::Reference(ReferenceType refType, ReferenceValue refValue):
    referenceType{refType}, referenceValue{std::move(refValue)}, designEntity{}, attribute{NoAttributeType}
{}

Reference::Reference(ReferenceType refType, ReferenceValue refValue, DesignEntity designEnt):
    referenceType{refType}, referenceValue{std::move(refValue)}, designEntity{designEnt}, attribute{NoAttributeType}
{}

Reference::Reference(ReferenceValue refValue, DesignEntity designEnt, Attribute attr):
    referenceType{AttributeRefType}, referenceValue{std::move(refValue)}, designEntity{designEnt}, attribute{attr}
{}

/************************/
/** Static Methods      */
/************************/

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
Reference Reference::createReference(String ref, DeclarationTable& declarationTable)
{
    if (ref == "_") {
        Reference reference(WildcardRefType, ref);
        return reference;
    }

    if (util::isPossibleConstant(ref)) {
        // An Integer as a reference will always be of StmtType
        Reference reference(IntegerRefType, ref);
        return reference;
    }

    if (util::isLiteralIdent(ref)) {
        // unquote the string literal
        Reference reference(LiteralRefType, util::removeCharFromBothEnds(ref));
        return reference;
    }

    size_t fullStopPosition = ref.find('.');

    // Synonym with no Attribute
    if (fullStopPosition == String::npos) {
        DesignEntity designEnt = declarationTable.getDesignEntityOfSynonym(ref);
        if (designEnt.getType() == NonExistentType) {
            return Reference(QuerySemanticsError, "Reference Synonym was not declared: " + ref);
        }

        Reference reference(SynonymRefType, ref, designEnt);
        return reference;
    }

    // Synonym Attribute
    StringVector splitRefString = splitByDelimiter(ref, ".");
    if (splitRefString.size() != 2) {
        return Reference(QuerySyntaxError, "Invalid Attribute Reference syntax: " + ref);
    }

    Synonym synonym = splitRefString.at(0);
    String attributeString = splitRefString.at(1);
    DesignEntity designEnt = declarationTable.getDesignEntityOfSynonym(synonym);
    if (designEnt.getType() == NonExistentType) {
        return Reference(QuerySemanticsError, "Reference Synonym was not declared: " + ref);
    }

    Attribute attribute(attributeString);
    if (attribute.getType() == InvalidAttributeType) {
        return Reference(QuerySyntaxError, "Invalid Attribute: " + attributeString);
    }

    if (!Attribute::validateDesignEntityAttributeSemantics(designEnt.getType(), attribute.getType())) {
        return Reference(QuerySemanticsError, "Attribute type does not match Design Entity: " + ref);
    }

    Reference reference(synonym, designEnt, attribute);
    return reference;
}

/************************/
/** Instance Methods    */
/************************/

Boolean Reference::isProcedure()
{
    return designEntity.getType() == ProcedureType;
}

Boolean Reference::isWildCard() const
{
    return referenceType == WildcardRefType;
}

ReferenceType Reference::getReferenceType() const
{
    return referenceType;
}

ReferenceValue Reference::getValue() const
{
    return referenceValue;
}

DesignEntity Reference::getDesignEntity() const
{
    return designEntity;
}

Attribute Reference::getAttribute()
{
    return attribute;
}

AttributeValueType Reference::getAttributeValueType()
{
    if (referenceType == IntegerRefType || designEntity.getType() == Prog_LineType) {
        return IntegerValueType;
    }

    if (referenceType == LiteralRefType) {
        return NameValueType;
    }

    if (referenceType != AttributeRefType) {
        return InvalidValueType;
    }

    if (attribute.getType() == StmtNumberType || attribute.getType() == ValueType) {
        return IntegerValueType;
    }

    return NameValueType;
}

Boolean Reference::operator==(const Reference& reference)
{
    return this->referenceType == reference.referenceType && this->referenceValue == reference.referenceValue
           && this->designEntity == reference.designEntity;
}
