//
// Created by Chester Sim on 12/9/20.
//

#include "AqTypes.h"

Reference::Reference(ReferenceType refType, ReferenceValue refValue):
    referenceType{refType}, referenceValue{refValue}, isProcedureType{false}
{}

Reference::Reference(ReferenceType refType, ReferenceValue refValue, Boolean isProc):
    referenceType{refType}, referenceValue{refValue}, isProcedureType{isProc}
{}

Reference Reference::invalidReference()
{
    Reference r(InvalidRefType, "");
    return r;
}

Boolean Reference::isInvalid()
{
    return referenceType == InvalidRefType;
}

Boolean Reference::isValidEntityRef()
{
    return util::isLiteralIdent(referenceValue) || util::isPossibleIdentifier(referenceValue) || referenceValue == "_";
}

Boolean Reference::isValidStatementRef()
{
    return util::isPossibleConstant(referenceValue) || util::isPossibleIdentifier(referenceValue)
           || referenceValue == "_";
}

Boolean Reference::isWildCard()
{
    return referenceValue == "_";
}

Boolean Reference::isProcedure()
{
    return isProcedureType;
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
