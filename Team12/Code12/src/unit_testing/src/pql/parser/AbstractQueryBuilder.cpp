#include "AbstractQueryBuilder.h"

#include "frontend/parser/Parser.h"
#include "lexer/Lexer.h"

AbstractQueryBuilder::AbstractQueryBuilder()
{
    Synonym s = "";
    DeclarationTable dT;
    ClauseVector cV;

    declarationTable = dT;
    clauseVector = cV;
    selectSynonym = s;
}

AbstractQueryBuilder AbstractQueryBuilder::create()
{
    return *(new AbstractQueryBuilder());
}

AbstractQueryBuilder AbstractQueryBuilder::addSelectSynonym(Synonym synonym)
{
    selectSynonym = synonym;

    return *this;
}

AbstractQueryBuilder AbstractQueryBuilder::addDeclaration(Synonym synonym, String designEntityType)
{
    DesignEntity designEntity{designEntityType};
    declarationTable.addDeclaration(synonym, designEntity);

    return *this;
}

AbstractQueryBuilder AbstractQueryBuilder::addSuchThatClause(String relRefType, ReferenceType leftRefType,
                                                             ReferenceValue leftRefValue,
                                                             DesignEntityType leftDesignEntityType,
                                                             ReferenceType rightRefType, ReferenceValue rightRefValue,
                                                             DesignEntityType rightDesignEntityType)
{
    DesignEntity leftDesignEntity{leftDesignEntityType};
    Reference leftReference{leftRefType, leftRefValue, leftDesignEntity};
    DesignEntity rightDesignEntity{rightDesignEntityType};
    Reference rightReference{rightRefType, rightRefValue, rightDesignEntity};
    Relationship relationship{relRefType, leftReference, rightReference};

    Clause* suchThatClause = new SuchThatClause(relationship);
    clauseVector.add(suchThatClause);

    return *this;
}

AbstractQueryBuilder AbstractQueryBuilder::addPatternClause(Synonym s, PatternStatementType patternStatementType,
                                                            ReferenceType refType, ReferenceValue refValue,
                                                            DesignEntityType designEntityType, String exprString,
                                                            ExpressionSpecType exprSpecType)
{
    Expression* expression = createExpression(exprString);
    ExpressionSpec expressionSpec{expression, exprSpecType};
    DesignEntity designEntity{designEntityType};
    Reference reference{refType, refValue, designEntity};

    Clause* patternClause = new PatternClause(s, patternStatementType, reference, expressionSpec);
    clauseVector.add(patternClause);

    return *this;
}

Expression* AbstractQueryBuilder::createExpression(String expressionString)
{
    StringList* splitString = splitProgram(expressionString);
    Expression* expression = parseExpression(splitString);
    return expression;
}

AbstractQuery AbstractQueryBuilder::build()
{
    AbstractQuery abstractQuery(selectSynonym, declarationTable, clauseVector);
    return abstractQuery;
}
