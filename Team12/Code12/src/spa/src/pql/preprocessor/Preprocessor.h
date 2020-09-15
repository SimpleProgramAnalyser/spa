#ifndef SPA_PQL_PREPROCESSOR_H
#define SPA_PQL_PREPROCESSOR_H

#include "AqTypes.h"
#include "lexer/Lexer.h"

class Preprocessor {
private:
    DeclarationTable declarationTable;
    DeclarationTable processDeclarations(String declarationString);
    ClauseVector processClauses(String clausesString);
    Clause* processSuchThatClause(String clauseConstraint);
    Clause* processPatternClause(String clauseConstraint);
    Reference createReference(String ref);
    ExpressionSpec createExpressionSpec(String ref);
    Expression* createExpression(String literal);

public:
    AbstractQuery processQuery(String query);
};

#endif // SPA_PQL_PREPROCESSOR_H
