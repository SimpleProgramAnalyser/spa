/**
 * Utility functions and classes for pattern matching.
 */

#ifndef SPA_PQL_PATTERN_MATCHER_UTIL_H
#define SPA_PQL_PATTERN_MATCHER_UTIL_H

#include <unordered_set>

#include "Types.h"

/**
 * A class to hold result lists for matching
 * assign patterns and variables.
 */
class PatternMatcherTuple {
private:
    std::vector<String> assignStatementResults;
    std::unordered_set<String> variableResults;
    std::vector<std::pair<Integer, String>> relationshipsResults;

public:
    PatternMatcherTuple() = default;

    /**
     * Adds a matching assign statement to the result lists.
     */
    Void addAssignStatement(Integer assignStatementNumber, const String& variable);

    /**
     * Adds entries from another PatternMatcherTuple to
     * this PatternMatcherTuple.
     */
    Void concatTuple(const PatternMatcherTuple& pmt);

    /**
     * Gets the list of results for matching statements.
     */
    std::vector<String> getAssignStatements() const;

    /**
     * Gets the list of results for matching variables.
     */
    std::vector<String> getVariables() const;

    /**
     * Gets the list of results for matching relationships.
     */
    std::vector<std::pair<Integer, String>> getRelationships() const;
};

#endif // SPA_PQL_PATTERN_MATCHER_UTIL_H
