#include "AbstractQueryBuilder.h"

#include <utility>

#include "frontend/parser/Parser.h"
#include "lexer/Lexer.h"

AbstractQueryBuilder AbstractQueryBuilder::create()
{
    return AbstractQueryBuilder();
}

AbstractQueryBuilder& AbstractQueryBuilder::addSelectSynonym(Synonym synonym)
{
    ResultSynonym resultSynonym(std::move(synonym));
    resultSynonyms.push_back(resultSynonym);
    return *this;
}

AbstractQueryBuilder& AbstractQueryBuilder::addSelectSynonym(Synonym synonym, String attribute, DesignEntityType type)
{
    DesignEntity de{type};
    ResultSynonym resultSynonym(std::move(synonym), attribute, de);
    resultSynonyms.push_back(resultSynonym);
    return *this;
}

AbstractQueryBuilder& AbstractQueryBuilder::addDeclaration(Synonym synonym, const String& designEntityType)
{
    DesignEntity designEntity{designEntityType};
    declarationTable.addDeclaration(std::move(synonym), designEntity);
    return *this;
}

AbstractQueryBuilder& AbstractQueryBuilder::addWithClause(ReferenceType leftRefType, ReferenceValue leftRefValue,
                                                          DesignEntityType leftDesignEntityType,
                                                          AttributeType leftAttributeType, ReferenceType rightRefType,
                                                          ReferenceValue rightRefValue,
                                                          DesignEntityType rightDesignEntityType,
                                                          AttributeType rightAttributeType)
{
    DesignEntity leftDesignEntity{leftDesignEntityType};
    Reference leftReference = leftRefType == AttributeRefType
                                  ? Reference(std::move(leftRefValue), leftDesignEntity, Attribute(leftAttributeType))
                                  : Reference(leftRefType, std::move(leftRefValue), leftDesignEntity);

    DesignEntity rightDesignEntity{rightDesignEntityType};
    Reference rightReference
        = rightRefType == AttributeRefType
              ? Reference(std::move(rightRefValue), rightDesignEntity, Attribute(rightAttributeType))
              : Reference(rightRefType, std::move(rightRefValue), rightDesignEntity);

    Clause* withClause = new WithClause(leftReference, rightReference);
    clauseVector.add(withClause);

    return *this;
}

AbstractQueryBuilder& AbstractQueryBuilder::addSuchThatClause(RelationshipReferenceType relRefType,
                                                              ReferenceType leftRefType, ReferenceValue leftRefValue,
                                                              DesignEntityType leftDesignEntityType,
                                                              ReferenceType rightRefType, ReferenceValue rightRefValue,
                                                              DesignEntityType rightDesignEntityType)
{
    DesignEntity leftDesignEntity{leftDesignEntityType};
    Reference leftReference{leftRefType, std::move(leftRefValue), leftDesignEntity};

    DesignEntity rightDesignEntity{rightDesignEntityType};
    Reference rightReference{rightRefType, std::move(rightRefValue), rightDesignEntity};

    Relationship relationship = Relationship::createRelationship(std::move(relRefType), leftReference, rightReference);

    Clause* suchThatClause = new SuchThatClause(relationship);
    clauseVector.add(suchThatClause);

    return *this;
}

AbstractQueryBuilder& AbstractQueryBuilder::addPatternClause(Synonym s, PatternStatementType patternStatementType,
                                                             ReferenceType refType, ReferenceValue refValue,
                                                             DesignEntityType designEntityType,
                                                             const String& exprString, ExpressionSpecType exprSpecType)
{
    Expression* expression = createExpression(exprString);
    ExpressionSpec expressionSpec{expression, exprSpecType};
    DesignEntity designEntity{designEntityType};
    Reference reference{refType, std::move(refValue), designEntity};

    Clause* patternClause = new PatternClause(std::move(s), patternStatementType, reference, std::move(expressionSpec));
    clauseVector.add(patternClause);

    return *this;
}

Expression* AbstractQueryBuilder::createExpression(const String& expressionString)
{
    StringVector splitString = splitProgram(expressionString);
    Expression* expression = parseExpression(splitString);
    return expression;
}

AbstractQuery AbstractQueryBuilder::build()
{
    return AbstractQuery(resultSynonyms, declarationTable, clauseVector);
}
