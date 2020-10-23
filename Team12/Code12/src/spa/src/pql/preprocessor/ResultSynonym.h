#ifndef SPA_RESULTSYNONYM_H
#define SPA_RESULTSYNONYM_H

#include "Attribute.h"
#include "Errorable.h"

class ResultSynonym: public Errorable {
private:
    static std::unordered_map<AttributeType, DesignEntityTypeSet> attributeDesignEntityTypeValidationMap;

    Synonym synonym;
    Attribute attribute;

public:
    static const ErrorMessage INVALID_SYNONYM_MESSAGE;

    explicit ResultSynonym(QueryErrorType queryErrorType);
    explicit ResultSynonym(QueryErrorType queryErrorType, ErrorMessage errorMessage);
    explicit ResultSynonym(Synonym syn);
    ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity);
    Synonym getSynonym() const;
    Attribute getAttribute() const;
    QueryErrorType getErrorType() const;
    ErrorMessage getErrorMessage() const;
    Boolean operator==(const ResultSynonym& resultSynonym) const;
    Boolean operator!=(const ResultSynonym& resultSynonym) const;
};

#endif // SPA_RESULTSYNONYM_H
