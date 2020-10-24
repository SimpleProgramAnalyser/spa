#ifndef SPA_CLAUSEVECTOR_H
#define SPA_CLAUSEVECTOR_H

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

    // Retrieves the number of clauses.
    Integer count() const;

    // Retrieves the QueryErrorType.
    QueryErrorType getErrorType() const;

    // Retrieves the ErrorMessage.
    ErrorMessage getErrorMessage() const;

    Boolean operator==(const ClauseVector& clauseVector);
    ~ClauseVector() = default;
    ClauseVector(const ClauseVector&) = delete;
    ClauseVector operator=(const ClauseVector&) = delete;
    ClauseVector(ClauseVector&&) = default;
    ClauseVector& operator=(ClauseVector&&) = default;
};

#endif // SPA_CLAUSEVECTOR_H
