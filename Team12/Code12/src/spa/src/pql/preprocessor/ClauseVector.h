#ifndef SPA_CLAUSEVECTOR_H
#define SPA_CLAUSEVECTOR_H

#include "Clause.h"

class ClauseVector: public Errorable {
private:
    List<Clause> clauses;

public:
    ClauseVector();
    ClauseVector(QueryErrorType queryErrorType, ErrorMessage);
    Void add(Clause* clause);
    Clause* get(Integer index) const;
    Integer count() const;
    QueryErrorType getErrorType() const;
    ErrorMessage getErrorMessage() const;
    Boolean operator==(const ClauseVector& clauseVector);
    ~ClauseVector() = default;
    ClauseVector(const ClauseVector&) = delete;
    ClauseVector operator=(const ClauseVector&) = delete;
    ClauseVector(ClauseVector&&) = default;
    ClauseVector& operator=(ClauseVector&&) = default;
};

#endif // SPA_CLAUSEVECTOR_H
