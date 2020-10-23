#ifndef SPA_CLAUSE_H
#define SPA_CLAUSE_H

#include "AqTypesUtils.h"

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
    explicit Clause(ClauseType clauseType);
    Clause(ClauseType clauseType, QueryErrorType queryErrorType, ErrorMessage errorMessage);

    ClauseType getType();
    QueryErrorType getErrorType() const;
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
