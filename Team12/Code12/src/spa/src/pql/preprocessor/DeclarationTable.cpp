//
// Created by Chester Sim on 13/9/20.
//

#include "AqTypes.h"

Void DeclarationTable::addDeclaration(Synonym s, DesignEntity& designEntity)
{
    table.insert({s, designEntity});
}

Boolean DeclarationTable::isInvalid()
{
    return hasError;
}
Boolean DeclarationTable::hasSynonym(Synonym s)
{
    std::unordered_map<Synonym, DesignEntity>::const_iterator got = table.find(s);
    if (got == table.end()) {
        return false;
    }

    return true;
}

DesignEntity DeclarationTable::getDesignEntityOfSynonym(Synonym s) const
{
    std::unordered_map<Synonym, DesignEntity>::const_iterator got = table.find(s);
    if (got == table.end()) {
        DesignEntity nonExistentType("NonExistentType");
        return nonExistentType;
    } else {
        return got->second;
    }
}

DeclarationTable DeclarationTable::invalidDeclarationTable()
{
    DeclarationTable dT;
    dT.hasError = true;
    return dT;
}

Boolean DeclarationTable::operator==(const DeclarationTable& declarationTable)
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