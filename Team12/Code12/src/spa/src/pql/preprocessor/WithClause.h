#ifndef SPA_WITHCLAUSE_H
#define SPA_WITHCLAUSE_H

#include "Clause.h"
#include "Reference.h"

class WithClause: public Clause {
private:
    Reference leftReference;
    Reference rightReference;

public:
    static Clause* createWithClause(const String& clauseConstraint, DeclarationTable& declarationTable);

    WithClause(Reference& leftRef, Reference& rightRef);
    Reference getLeftReference();
    Reference getRightReference();
    Boolean operator==(const WithClause& withClause);
};

#endif // SPA_WITHCLAUSE_H
