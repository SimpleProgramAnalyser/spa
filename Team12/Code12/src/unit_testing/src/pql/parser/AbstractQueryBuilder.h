#ifndef SPA_ABSTRACTQUERYBUILDER_H
#define SPA_ABSTRACTQUERYBUILDER_H

#include "pql/preprocessor/AqTypes.h"

class AbstractQueryBuilder {
private:
    AbstractQuery abstractQuery;
    DeclarationTable declarationTable;
    ClauseVector clauseVector;
    Synonym selectSynonym;

    static Expression* createExpression(String literal);
    AbstractQueryBuilder();

public:
    static AbstractQueryBuilder create();
    AbstractQueryBuilder addSelectSynonym(Synonym synonym);
    AbstractQueryBuilder addDeclaration(Synonym synonym, String designEntityType);
    AbstractQueryBuilder addSuchThatClause(String relRefType, ReferenceType leftRefType, ReferenceValue leftRefValue,
                                           DesignEntityType leftDesignEntityType, ReferenceType rightRefType,
                                           ReferenceValue rightRefValue, DesignEntityType rightDesignEntityType);
    AbstractQueryBuilder addPatternClause(PatternStatementType patternStatementType, ReferenceType refType,
                                          ReferenceValue refValue, DesignEntityType designEntityType, String exprString,
                                          ExpressionSpecType exprSpecType);
    AbstractQuery build();
};

#endif // SPA_ABSTRACTQUERYBUILDER_H
