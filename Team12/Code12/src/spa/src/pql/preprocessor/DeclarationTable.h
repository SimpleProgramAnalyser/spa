/**
 * DeclarationTable is a class that provides a way
 * to map synonym names to their types.
 */

#ifndef SPA_PQL_PREPROCESSOR_DECLARATION_TABLE_H
#define SPA_PQL_PREPROCESSOR_DECLARATION_TABLE_H

#include "AqTypesUtils.h"
#include "DesignEntity.h"

class DeclarationTable: public Errorable {
private:
    std::unordered_map<Synonym, DesignEntity> table;

public:
    static const ErrorMessage INVALID_DECLARATION_SYNTAX;

    // Instantiate an empty DeclarationTable.
    DeclarationTable();

    // Instantiate an erroneous DeclarationTable with the given QueryErrorType and ErrorMessage.
    DeclarationTable(QueryErrorType queryErrorType, ErrorMessage errorMessage);

    // Add a declaration of Synonym with the given DesignEntity into the table.
    Void addDeclaration(const Synonym& s, DesignEntity& designEntity);

    // Retrieves the DesignEntity of the given Synonym.
    // If the Synonym does not exist in the table,
    // a NonExistentType DesignEntity will be returned.
    DesignEntity getDesignEntityOfSynonym(const Synonym& s) const;

    // Checks if the table has the given Synonym.
    Boolean hasSynonym(const Synonym& s) const;

    // Retrieves the ErrorMessage.
    ErrorMessage getErrorMessage() const;

    Boolean operator==(const DeclarationTable& declarationTable) const;
};

#endif // SPA_PQL_PREPROCESSOR_DECLARATION_TABLE_H
