/**
 * An enumeration of the different Design Entity types.
 */

#ifndef SPA_PQL_PREPROCESSOR_DESIGN_ENTITY_TYPE_H
#define SPA_PQL_PREPROCESSOR_DESIGN_ENTITY_TYPE_H

#include "Types.h"

/**
 * All the valid DesignEntity types.
 * The enums are assigned special values
 * such that those whose smallest two bits
 * is 01, are Statements.
 */
// t
enum class DesignEntityType : unsigned char {
    // statement types: smallest bits are 01
    StmtType = 1,       // 0000 0001
    ReadType = 5,       // 0000 0101
    PrintType = 9,      // 0000 1001
    CallType = 13,      // 0000 1101
    WhileType = 17,     // 0001 0001
    IfType = 21,        // 0001 0101
    AssignType = 25,    // 0001 1001
    Prog_LineType = 29, // 0001 1101
    // other types: smallest bits are 00
    VariableType = 0,    // 0000 0000
    ConstantType = 4,    // 0000 0100
    ProcedureType = 8,   // 0000 1000
    NonExistentType = 12 // 0000 1100
};

typedef std::unordered_set<DesignEntityType> DesignEntityTypeSet;

// Hash function for DesignEntityType
template <>
struct std::hash<DesignEntityType> {
    std::size_t operator()(const DesignEntityType& de) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(de));
    }
};

#endif // SPA_PQL_PREPROCESSOR_DESIGN_ENTITY_TYPE_H
