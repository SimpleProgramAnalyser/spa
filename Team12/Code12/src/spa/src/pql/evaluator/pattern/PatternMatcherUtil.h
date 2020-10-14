/**
 * Utility functions and classes for pattern matching.
 */

#ifndef SPA_PQL_PATTERN_MATCHER_UTIL_H
#define SPA_PQL_PATTERN_MATCHER_UTIL_H

#include <unordered_set>

#include "Types.h"

/**
 * A class to hold result lists for matching
 * patterns and variables.
 */
class PatternMatcherTuple {
private:
    std::vector<String> targetStatementResults;
    std::unordered_set<String> variableResults;
    std::vector<std::pair<Integer, String>> relationshipsResults;

public:
    PatternMatcherTuple() = default;

    /**
     * Adds a matching target statement to the result lists.
     */
    Void addTargetStatement(Integer targetStatementNumber);

    /**
     * Adds a matching target statement and its variable
     * to the result lists.
     */
    Void addTargetStatement(Integer targetStatementNumber, const String& variable);

    /**
     * Adds a matching target statement and accompany set
     * of variables to the result lists.
     */
    Void addTargetStatement(Integer targetStatementNumber, const std::unordered_set<String>& variables);

    /**
     * Adds entries from another PatternMatcherTuple to
     * this PatternMatcherTuple.
     */
    Void concatTuple(const PatternMatcherTuple& pmt);

    /**
     * Gets the list of results for matching statements.
     */
    std::vector<String> getTargetStatements() const;

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
