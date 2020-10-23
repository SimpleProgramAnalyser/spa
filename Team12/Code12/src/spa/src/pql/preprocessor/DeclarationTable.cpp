#include "DeclarationTable.h"

#include <utility>

/************************/
/** Static Methods      */
/************************/

const String DeclarationTable::INVALID_DECLARATION_SYNTAX = "Invalid declaration syntax";

/************************/
/** Constructors        */
/************************/

DeclarationTable::DeclarationTable(): table(), Errorable() {}

DeclarationTable::DeclarationTable(QueryErrorType queryErrorType, String errorMessage):
    Errorable(queryErrorType, std::move(errorMessage))
{}

/************************/
/** Instance Methods    */
/************************/

Void DeclarationTable::addDeclaration(const Synonym& s, DesignEntity& designEntity)
{
    table.insert({s, designEntity});
}

Boolean DeclarationTable::hasSynonym(const Synonym& s) const
{
    auto got = table.find(s);
    if (got == table.end()) {
        return false;
    }

    return true;
}

DesignEntity DeclarationTable::getDesignEntityOfSynonym(const Synonym& s) const
{
    auto got = table.find(s);
    if (got == table.end()) {
        DesignEntity nonExistentType("NonExistentType");
        return nonExistentType;
    } else {
        return got->second;
    }
}

ErrorMessage DeclarationTable::getErrorMessage() const
{
    return errorMessage;
}

Boolean DeclarationTable::operator==(const DeclarationTable& declarationTable) const
{
    if (this->table.size() != declarationTable.table.size()) {
        return false;
    }

    for (const auto& entry : this->table) {
        auto key = entry.first;
        DesignEntity designEntityThis = this->getDesignEntityOfSynonym(key);
        DesignEntity designEntity = declarationTable.getDesignEntityOfSynonym(key);
        if (designEntityThis == designEntity) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}
