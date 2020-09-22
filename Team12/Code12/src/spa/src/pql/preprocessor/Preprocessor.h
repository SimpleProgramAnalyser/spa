#ifndef SPA_PQL_PREPROCESSOR_H
#define SPA_PQL_PREPROCESSOR_H

#include "AqTypes.h"
#include "lexer/Lexer.h"

class Preprocessor {
private:
    DeclarationTable declarationTable;
    static DeclarationTable processDeclarations(const String& declarationString);
    ClauseVector processClauses(const String& clausesString);
    Clause* processSuchThatClause(String clauseConstraint);
    Clause* processPatternClause(String clauseConstraint);
    Reference createReference(String ref);
    ExpressionSpec createExpressionSpec(String exprSpecString);
    static Expression* createExpression(const String& literal);

public:
    AbstractQuery processQuery(String query);
};

#endif // SPA_PQL_PREPROCESSOR_H
