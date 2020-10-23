#ifndef SPA_SUCHTHATCLAUSE_H
#define SPA_SUCHTHATCLAUSE_H

#include "Clause.h"
#include "Relationship.h"

class SuchThatClause: public Clause {
private:
    Relationship relationship;

public:
    static Clause* createSuchThatClause(const String& clauseConstraint, DeclarationTable& declarationTable);
    explicit SuchThatClause(Relationship& r);
    Relationship getRelationship();
    Boolean operator==(const SuchThatClause& suchThatClause);
};

#endif // SPA_SUCHTHATCLAUSE_H
