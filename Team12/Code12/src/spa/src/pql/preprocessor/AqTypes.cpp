/**
 * Implementation of the AbstractQuery, and
 * supporting substructures and their public methods.
 */

#include "AqTypes.h"

Expression* createExpression(const String& literal)
{
    StringVector splitString = splitProgram(literal);
    Expression* expression = parseExpression(splitString);
    return expression;
}

Boolean isValidSynonym(String testString)
{
    return util::isPossibleIdentifier(testString);
}

Boolean isStatementDesignEntity(DesignEntityType type)
{
    return static_cast<unsigned char>(type << 6u) == 0x40;
}
