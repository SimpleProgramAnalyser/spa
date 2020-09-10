//
// Created by Chester Sim on 6/9/20.
//

#ifndef SPA_PREPROCESSOR_H
#define SPA_PREPROCESSOR_H

#include "AqTypes.h"

class PreProcessor {
private:
    DeclarationTable declarationTable;

public:
    AbstractQuery processQuery(String query);
    DeclarationTable processDeclarations(String declarationString);
    ClauseVector processClauses(String clausesString);
    Clause* processSuchThatClause(String clauseConstraint);
    Clause* processPatternClause(String clauseConstraint);
    Boolean isStatementRef(String ref);
    Boolean isEntityRef(String ref);
    Reference createReference(String ref);
    Boolean isSynonymOfProcedureType(Synonym s);
};

#endif // SPA_PREPROCESSOR_H
