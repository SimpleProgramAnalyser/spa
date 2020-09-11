//
// Created by Chester Sim on 6/9/20.
//

#ifndef SPA_PREPROCESSOR_H
#define SPA_PREPROCESSOR_H

#include "../frontend/parser/Parser.h"
#include "../lexer/Lexer.h"
#include "AqTypes.h"

class Preprocessor {
private:
    DeclarationTable declarationTable;

public:
    AbstractQuery processQuery(String query);
    DeclarationTable processDeclarations(String declarationString);
    ClauseVector processClauses(String clausesString);
    Clause* processSuchThatClause(String clauseConstraint);
    Clause* processPatternClause(String clauseConstraint);
    Reference createReference(String ref);
    ExpressionSpec createExpressionSpec(String ref);
    Expression* createExpression(String literal);
    Boolean isSynonymOfProcedureType(Synonym s);
};

#endif // SPA_PREPROCESSOR_H
