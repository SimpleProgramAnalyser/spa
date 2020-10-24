#ifndef SPA_SUCHTHATCLAUSE_H
#define SPA_SUCHTHATCLAUSE_H

#include "Clause.h"
#include "Relationship.h"

class SuchThatClause: public Clause {
private:
    Relationship relationship;

public:
    /**
     * Processes the clause constraint string for a
     * SuchThatClause, by abstracting it into its relevant
     * Relationship and References.
     *
     * @param clauseConstraint  String of the clause constraint with all
     *                          whitespaces removed.
     * @return                  Clause* of the SuchThatClause
     *                          that was constructed.
     */
    static Clause* createSuchThatClause(const String& clauseConstraint, DeclarationTable& declarationTable);

    // Instantiates a SuchThatClause with the given Relationship.
    explicit SuchThatClause(Relationship& r);

    // Retrieves the Relationship of the SuchThatClause.
    Relationship getRelationship();

    Boolean operator==(const SuchThatClause& suchThatClause);
};

#endif // SPA_SUCHTHATCLAUSE_H
