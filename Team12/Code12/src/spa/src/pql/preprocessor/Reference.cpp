/**
 * Implementation of a Reference for AbstractQuery Clauses.
 */

#include "Reference.h"

#include <utility>

/************************/
/** Constructors        */
/************************/

Reference::Reference():
    Errorable(), referenceType{InvalidRefType}, referenceValue(), designEntity{}, attribute{NoAttributeType}
{}

Reference::Reference(QueryErrorType queryErrorType, ErrorMessage errorMessage):
    Errorable(queryErrorType, std::move(errorMessage)), referenceType{InvalidRefType}, referenceValue(),
    designEntity(), attribute{NoAttributeType}
{}

Reference::Reference(ReferenceType refType, ReferenceValue refValue):
    Errorable(), referenceType{refType}, referenceValue{std::move(refValue)}, designEntity{}, attribute{NoAttributeType}
{}

Reference::Reference(ReferenceType refType, ReferenceValue refValue, DesignEntity designEnt):
    Errorable(), referenceType{refType}, referenceValue{std::move(refValue)},
    designEntity{designEnt}, attribute{NoAttributeType}
{}

Reference::Reference(ReferenceValue refValue, DesignEntity designEnt, Attribute attr):
    Errorable(), referenceType{AttributeRefType}, referenceValue{std::move(refValue)},
    designEntity{designEnt}, attribute{attr}
{}

/************************/
/** Static Methods      */
/************************/

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

    if (util::isLiteral(ref)) {
        if (!util::isLiteralIdent(ref)) {
            return Reference(QuerySyntaxError, "Literal is not a valid Identifier: " + ref);
        }
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

Attribute Reference::getAttribute() const
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

Boolean Reference::operator==(const Reference& reference) const
{
    return this->referenceType == reference.referenceType && this->referenceValue == reference.referenceValue
           && this->designEntity == reference.designEntity;
}
