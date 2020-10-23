#ifndef SPA_AQTYPESUTILS_H
#define SPA_AQTYPESUTILS_H

#include <assert.h>
#include <cstdint>
#include <unordered_set>
#include <utility>

#include "Errorable.h"
#include "Types.h"
#include "Util.h"
#include "frontend/parser/Parser.h"
#include "lexer/Lexer.h"
#include "DesignEntityType.h"

typedef String Synonym;

// Utils

/**
 * Returns an Expression pointer after constructing the
 * Expression object using the given literal.
 * @param literal   String to be converted into Expression
 * @return          Pointer of the constructed Expression
 */
Expression* createExpression(const String& literal);

Boolean isValidSynonym(String s);

/**
 * Check if the DesignEntityType represents
 * a SIMPLE statement of some sort.
 *
 * @param type DesignEntityType to be checked.
 * @return True, if DesignEntityType is one of
 *         the 6 SIMPLE statement types. False,
 *         if DesignEntityType is other entities
 *         like variable or procedure.
 */
Boolean isStatementDesignEntity(DesignEntityType type);

#endif // SPA_AQTYPESUTILS_H
