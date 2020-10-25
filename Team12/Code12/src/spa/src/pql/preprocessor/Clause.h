#ifndef SPA_CLAUSE_H
#define SPA_CLAUSE_H

#include "AqTypesUtils.h"

// All the types of Clauses.
enum ClauseType : char { SuchThatClauseType, PatternClauseType, WithClauseType, NonExistentClauseType };

// Hash function for ClauseType
template <>
struct std::hash<ClauseType> {
    std::size_t operator()(const ClauseType& clauseType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(clauseType));
    }
};

class Clause: public Errorable {
protected:
    ClauseType type;

public:
    // Instantiate a Clause of ClauseType.
    explicit Clause(ClauseType clauseType);

    /**
     * Instantiate an erroneous Clause of ClauseType,
     * with the given QueryErrorType and ErrorMessage.
     */
    Clause(ClauseType clauseType, QueryErrorType queryErrorType, ErrorMessage errorMessage);

    // Retrieves the underlying ClauseType.
    ClauseType getType();

    // Retrieves the QueryErrorType.
    QueryErrorType getErrorType() const;

    // Retrieves the ErrorMessage.
    ErrorMessage getErrorMessage() const;

    // Rest of the methods are required to allow virtual operator==
    Clause(const Clause&) = default;
    Clause operator=(const Clause&) = delete;
    Clause(Clause&&) = default;
    Clause& operator=(Clause&&) = delete;
    virtual ~Clause() = default;
    virtual Boolean operator==(Clause& clause);
};

#endif // SPA_CLAUSE_H
