#include "AqTypesUtils.h"

#include "DesignEntityType.h"

Expression* createExpression(const String& literal)
{
    StringVector splitString = splitProgram(literal);
    Expression* expression = parseExpression(splitString);
    return expression;
}

Boolean isStatementDesignEntity(DesignEntityType type)
{
    return static_cast<unsigned char>(type << 6u) == 0x40;
}
