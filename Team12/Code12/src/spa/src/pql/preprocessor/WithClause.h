/**
 * This class represents a "with" clause in the query.
 */

#ifndef SPA_PQL_PREPROCESSOR_WITH_CLAUSE_H
#define SPA_PQL_PREPROCESSOR_WITH_CLAUSE_H

#include "Clause.h"
#include "Reference.h"

class WithClause: public Clause {
private:
    Reference leftReference;
    Reference rightReference;

public:
    /**
     * Processes the clause constraint string for a
     * WithClause, by abstracting it into its relevant
     * References.
     *
     * @param clauseConstraint  String of the clause constraint with all
     *                          whitespaces removed.
     * @return                  Clause* of the WithClause that was
     *                          constructed.
     */
    static Clause* createWithClause(const String& clauseConstraint, DeclarationTable& declarationTable);

    // Instantiates a WithClause with the given left and right References.
    WithClause(Reference& leftRef, Reference& rightRef);

    // Retrieves the left Reference of the WithClause.
    Reference getLeftReference();

    // Retrieves the right Reference of the WithClause.
    Reference getRightReference();

    Boolean operator==(const WithClause& withClause);
};

#endif // SPA_PQL_PREPROCESSOR_WITH_CLAUSE_H
