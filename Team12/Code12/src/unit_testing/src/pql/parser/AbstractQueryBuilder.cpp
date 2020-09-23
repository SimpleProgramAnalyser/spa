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
    selectSynonym = std::move(synonym);
    return *this;
}

AbstractQueryBuilder& AbstractQueryBuilder::addDeclaration(Synonym synonym, const String& designEntityType)
{
    DesignEntity designEntity{designEntityType};
    declarationTable.addDeclaration(std::move(synonym), designEntity);
    return *this;
}

AbstractQueryBuilder& AbstractQueryBuilder::addSuchThatClause(String relRefType, ReferenceType leftRefType,
                                                              ReferenceValue leftRefValue,
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
    return AbstractQuery(selectSynonym, declarationTable, clauseVector);
}
