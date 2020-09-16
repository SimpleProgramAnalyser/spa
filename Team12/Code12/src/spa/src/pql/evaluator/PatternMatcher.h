/**
 * PatternMatcher holds logic required for Query Evaluator
 * to match a pattern clause in a Program Query Language query.
 */

#ifndef SPA_PATTERN_MATCHER_H
#define SPA_PATTERN_MATCHER_H

#include <Types.h>
#include <pql/preprocessor/AqTypes.h>
#include <vector>

std::vector<String> evaluatePattern(const Synonym& synonym, PatternClause* pnClause,
                                    const DeclarationTable& declarations);

#endif // SPA_PATTERN_MATCHER_H
