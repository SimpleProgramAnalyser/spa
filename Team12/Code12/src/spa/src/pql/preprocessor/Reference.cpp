#include "AqTypes.h"

Reference::Reference(): designEntity{}, hasError{false}
{
    referenceType = SynonymRefType;
}

Reference::Reference(ReferenceType refType, ReferenceValue refValue):
    referenceType{refType}, referenceValue{refValue}, designEntity{}, hasError{false}
{}

Reference::Reference(ReferenceType refType, ReferenceValue refValue, DesignEntity designEnt):
    referenceType{refType}, referenceValue{refValue}, designEntity{designEnt}, hasError{false}
{}

Reference Reference::invalidReference()
{
    Reference r;
    r.hasError = true;
    return r;
}

Boolean Reference::isInvalid()
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

Boolean Reference::isWildCard() // TODO: phase out
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
