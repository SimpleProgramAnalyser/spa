#include "AqTypes.h"

Reference::Reference(): isProcedureType{false}, hasError{false}
{
    referenceType = SynonymRefType;
}

Reference::Reference(ReferenceType refType, ReferenceValue refValue):
    referenceType{refType}, referenceValue{refValue}, isProcedureType{false}, hasError{false}
{}

Reference::Reference(ReferenceType refType, ReferenceValue refValue, Boolean isProc):
    referenceType{refType}, referenceValue{refValue}, isProcedureType{isProc}, hasError{false}
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
    return isProcedureType;
}

Boolean Reference::isWildCard()
{
    return referenceType == WildcardRefType;
}

ReferenceType Reference::getReferenceType()
{
    return referenceType;
}

ReferenceValue Reference::getValue()
{
    return referenceValue;
}

Boolean Reference::operator==(const Reference& reference)
{
    return this->referenceType == reference.referenceType && this->referenceValue == referenceValue;
}
