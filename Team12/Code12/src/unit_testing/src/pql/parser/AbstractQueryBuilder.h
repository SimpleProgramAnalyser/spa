#ifndef UNIT_TESTING_ABSTRACT_QUERY_BUILDER_H
#define UNIT_TESTING_ABSTRACT_QUERY_BUILDER_H

#include "pql/preprocessor/AqTypes.h"

class AbstractQueryBuilder {
private:
    DeclarationTable declarationTable;
    ClauseVector clauseVector;
    Vector<ResultSynonym> resultSynonyms = Vector<ResultSynonym>();

    static Expression* createExpression(const String& literal);

public:
    static AbstractQueryBuilder create();
    AbstractQueryBuilder& addSelectSynonym(Synonym synonym);
    AbstractQueryBuilder& addDeclaration(Synonym synonym, const String& designEntityType);
    AbstractQueryBuilder& addSuchThatClause(String relRefType, ReferenceType leftRefType,
                                                  ReferenceValue leftRefValue, DesignEntityType leftDesignEntityType,
                                                  ReferenceType rightRefType, ReferenceValue rightRefValue,
                                                  DesignEntityType rightDesignEntityType);
    AbstractQueryBuilder& addPatternClause(Synonym s, PatternStatementType patternStatementType,
                                                 ReferenceType refType, ReferenceValue refValue,
                                                 DesignEntityType designEntityType, const String& exprString,
                                                 ExpressionSpecType exprSpecType);
    AbstractQuery build();
};

#endif // UNIT_TESTING_ABSTRACT_QUERY_BUILDER_H
