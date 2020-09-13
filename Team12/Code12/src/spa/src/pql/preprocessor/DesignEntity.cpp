#include "AqTypes.h"

DesignEntity::DesignEntity(DesignEntityType designEntityType)
{
    type = designEntityType;
}

DesignEntity::DesignEntity(const String& stringType)
{
    /*
     * Suggestion: Declare some variables on top, to hold
     * these values, don't use 'magic' values.
     */
    if (stringType == "stmt") {
        type = StmtType;
    } else if (stringType == "read") {
        type = ReadType;
    } else if (stringType == "print") {
        type = PrintType;
    } else if (stringType == "call") {
        type = CallType;
    } else if (stringType == "while") {
        type = WhileType;
    } else if (stringType == "if") {
        type = IfType;
    } else if (stringType == "assign") {
        type = AssignType;
    } else if (stringType == "variable") {
        type = VariableType;
    } else if (stringType == "constant") {
        type = ConstantType;
    } else if (stringType == "procedure") {
        type = ProcedureType;
    } else {
        type = NonExistentType;
    }
}

DesignEntityType DesignEntity::getType()
{
    return type;
}

Boolean DesignEntity::operator==(const DesignEntity& designEntity)
{
    return this->type == designEntity.type;
}
