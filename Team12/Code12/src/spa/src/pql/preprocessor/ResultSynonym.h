#ifndef SPA_RESULTSYNONYM_H
#define SPA_RESULTSYNONYM_H

#include "Attribute.h"
#include "Errorable.h"

class ResultSynonym: public Errorable {
private:
    // Map of AttributeType and their corresponding valid DesignEntityTypes.
    static std::unordered_map<AttributeType, DesignEntityTypeSet> attributeDesignEntityTypeValidationMap;

    Synonym synonym;
    Attribute attribute;

public:
    static const ErrorMessage INVALID_SYNONYM_MESSAGE;

    // Instantiate an erroneous ResultSynonym with the given QueryErrorType.
    explicit ResultSynonym(QueryErrorType queryErrorType);

    // Instantiate an erroneous ResultSynonym with the given QueryErrorType
    // and ErrorMessage.
    explicit ResultSynonym(QueryErrorType queryErrorType, ErrorMessage errorMessage);

    // Instantiate a ResultSynonym with the given Synonym.
    explicit ResultSynonym(Synonym syn);

    // Validates and instantiate a ResultSynonym with the given Synonym,
    // attribute and DesignEntity.
    ResultSynonym(Synonym syn, const String& attr, DesignEntity& designEntity);

    // Retrieves the Synonym of the ResultSynonym.
    Synonym getSynonym() const;

    // Retrieves the Attribute of the ResultsSynonym.
    Attribute getAttribute() const;

    // Retrieves the QueryErrorType of the ResultSynonym.
    QueryErrorType getErrorType() const;

    // Retrieves the ErrorMessage of the ResultSynonym.
    ErrorMessage getErrorMessage() const;

    Boolean operator==(const ResultSynonym& resultSynonym) const;
    Boolean operator!=(const ResultSynonym& resultSynonym) const;
};

#endif // SPA_RESULTSYNONYM_H
