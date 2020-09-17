/**
 * PatternMatcher holds logic required for Query Evaluator
 * to match a pattern clause in a Program Query Language query.
 */

#ifndef SPA_PATTERN_MATCHER_H
#define SPA_PATTERN_MATCHER_H

#include <Types.h>
#include <pql/preprocessor/AqTypes.h>
#include <vector>

/**
 * Evaluates a pattern clause in the query with
 * respect to the specified synonym in the query.
 *
 * @param synonym The Select synonym in the query.
 * @param pnClause A pattern clause in the query.
 * @param declarations Declarations table for the query.
 *
 * @return Returns a list of results found.
 */
std::vector<String> evaluatePattern(const Synonym& synonym, PatternClause* pnClause);

#endif // SPA_PATTERN_MATCHER_H
