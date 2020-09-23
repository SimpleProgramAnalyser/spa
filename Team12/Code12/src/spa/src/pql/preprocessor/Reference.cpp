#include "AqTypes.h"

Reference::Reference(): designEntity{}, hasError{false}
{
    referenceType = InvalidRefType;
}

Reference::Reference(Boolean hasError): referenceType{InvalidRefType}, hasError{hasError} {}

Reference::Reference(ReferenceType refType, ReferenceValue refValue):
    referenceType{refType}, referenceValue{std::move(refValue)}, designEntity{}, hasError{false}
{}

Reference::Reference(ReferenceType refType, ReferenceValue refValue, DesignEntity designEnt):
    referenceType{refType}, referenceValue{refValue}, designEntity{designEnt}, hasError{false}
{}

Boolean Reference::isInvalid() const
{
    return hasError;
}

Boolean Reference::isValidEntityRef()
{
    return referenceType == LiteralRefType || referenceType == SynonymRefType || referenceType == WildcardRefType;
}

Boolean Reference::isValidStatementRef()
{
    return referenceType == IntegerRefType || referenceType == SynonymRefType || referenceType == WildcardRefType;
}

Boolean Reference::isProcedure()
{
    return designEntity.getType() == ProcedureType;
}

Boolean Reference::isNonStatementSynonym()
{
    DesignEntityType designEntityType = designEntity.getType();
    return designEntityType == ProcedureType || designEntityType == VariableType || designEntityType == ConstantType;
}

Boolean Reference::isWildCard() const // TODO: phase out
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

DesignEntity Reference::getDesignEntity()
{
    return designEntity;
}

Boolean Reference::operator==(const Reference& reference)
{
    return this->referenceType == reference.referenceType && this->referenceValue == reference.referenceValue
           && this->designEntity == reference.designEntity;
}

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
        Reference reference(IntegerRefType, ref, DesignEntity(StmtType));
        return reference;
    }

    if (util::isLiteralIdent(ref)) {
        // unquote the string literal
        Reference reference(LiteralRefType, util::removeCharFromBothEnds(ref));
        return reference;
    }

    if (declarationTable.hasSynonym(ref)) {
        Reference reference(SynonymRefType, ref, declarationTable.getDesignEntityOfSynonym(ref));
        return reference;
    }

    return Reference(true);
}