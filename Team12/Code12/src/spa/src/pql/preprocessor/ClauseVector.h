/**
 * A wrapper class for a list of clauses, this class
 * helps to handle the resources used for each clause
 * (namely, it will delete the clauses automatically).
 */

#ifndef SPA_PQL_PREPROCESSOR_CLAUSE_VECTOR_H
#define SPA_PQL_PREPROCESSOR_CLAUSE_VECTOR_H

#include "Clause.h"

class ClauseVector: public Errorable {
private:
    List<Clause> clauses;

public:
    // Instantiate an empty ClauseVector.
    ClauseVector();

    // Instantiate an erroneous ClauseVector with the given
    // QueryErrorType and ErrorMessage.
    ClauseVector(QueryErrorType queryErrorType, ErrorMessage);

    // Adds a Clause* to the list of clauses.
    Void add(Clause* clause);

    // Retrieves a Clause* at the given index from the list.
    Clause* get(Integer index) const;

    // Retrieves a Clause* at the given index from the list.
    List<Clause>& getAllUnsafe();

    // Retrieves a Clause* at the given index from the list, and move the unique pointer.
    Clause* remove(Integer index);

    // Retrieves the number of clauses.
    Integer count() const;

    // Retrieves the QueryErrorType.
    QueryErrorType getErrorType() const;

    // Retrieves the ErrorMessage.
    ErrorMessage getErrorMessage() const;

    Boolean operator==(const ClauseVector& clauseVector) const;
    ~ClauseVector() = default;
    ClauseVector(const ClauseVector&) = delete;
    ClauseVector operator=(const ClauseVector&) = delete;
    ClauseVector(ClauseVector&&) = default;
    ClauseVector& operator=(ClauseVector&&) = default;
};

#endif // SPA_PQL_PREPROCESSOR_CLAUSE_VECTOR_H
