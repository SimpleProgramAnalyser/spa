#ifndef SPA_DECLARATIONTABLE_H
#define SPA_DECLARATIONTABLE_H

#include "AqTypesUtils.h"
#include "DesignEntity.h"

class DeclarationTable: public Errorable {
private:
    std::unordered_map<Synonym, DesignEntity> table;

public:
    static const String INVALID_DECLARATION_SYNTAX;

    DeclarationTable();
    DeclarationTable(QueryErrorType queryErrorType, String errorMessage);
    Void addDeclaration(const Synonym& s, DesignEntity& designEntity);
    DesignEntity getDesignEntityOfSynonym(const Synonym& s) const;
    Boolean hasSynonym(const Synonym& s) const;
    ErrorMessage getErrorMessage() const;
    Boolean operator==(const DeclarationTable& declarationTable) const;
};

#endif // SPA_DECLARATIONTABLE_H
