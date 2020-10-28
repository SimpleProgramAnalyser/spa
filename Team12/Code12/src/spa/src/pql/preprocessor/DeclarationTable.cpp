#include "DeclarationTable.h"

#include <algorithm>
#include <utility>

/************************/
/** Static Methods      */
/************************/

const ErrorMessage DeclarationTable::INVALID_DECLARATION_SYNTAX = "Invalid declaration syntax";

/************************/
/** Constructors        */
/************************/

DeclarationTable::DeclarationTable(): Errorable(), table() {}

DeclarationTable::DeclarationTable(QueryErrorType queryErrorType, ErrorMessage errorMessage):
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

    return std::all_of(
        this->table.begin(), this->table.end(), [this, declarationTable](const std::pair<String, DesignEntity>& key) {
            return this->getDesignEntityOfSynonym(key.first) == declarationTable.getDesignEntityOfSynonym(key.first);
        });
}
