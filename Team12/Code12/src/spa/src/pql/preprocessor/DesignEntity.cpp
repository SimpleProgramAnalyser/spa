/**
 * Implementation of DesignEntity of a Synonym in
 * an AbstractQuery.
 */

#include "DesignEntity.h"

/************************/
/** Static Members      */
/************************/

const ErrorMessage DesignEntity::INVALID_DESIGN_ENTITY = "Invalid Design Entity ";

std::unordered_map<String, DesignEntityType> DesignEntity::designEntityMap{
    {"stmt", StmtType},           {"read", ReadType},          {"print", PrintType},
    {"call", CallType},           {"while", WhileType},        {"if", IfType},
    {"assign", AssignType},       {"variable", VariableType},  {"constant", ConstantType},
    {"procedure", ProcedureType}, {"prog_line", Prog_LineType}};

/************************/
/** Constructors        */
/************************/

DesignEntity::DesignEntity(): type(NonExistentType) {}

DesignEntity::DesignEntity(DesignEntityType designEntityType): type(designEntityType) {}

DesignEntity::DesignEntity(const String& stringType): type(NonExistentType)
{
    auto got = DesignEntity::designEntityMap.find(stringType);
    if (got == DesignEntity::designEntityMap.end()) {
        type = NonExistentType;
    } else {
        type = got->second;
    }
}

/************************/
/** Instance Members    */
/************************/

DesignEntityType DesignEntity::getType()
{
    return type;
}

Boolean DesignEntity::operator==(const DesignEntity& designEntity) const
{
    return this->type == designEntity.type;
}
