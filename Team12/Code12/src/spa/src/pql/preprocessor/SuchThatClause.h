/**
 * This class represents a such that clause in the query.
 */

#ifndef SPA_PQL_PREPROCESSOR_SUCH_THAT_CLAUSE_H
#define SPA_PQL_PREPROCESSOR_SUCH_THAT_CLAUSE_H

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

    // Retrieves the Relationship of the SuchThatClause.
    Relationship& getRelationshipUnsafe();

    Boolean operator==(const SuchThatClause& suchThatClause);
};

#endif // SPA_PQL_PREPROCESSOR_SUCH_THAT_CLAUSE_H
