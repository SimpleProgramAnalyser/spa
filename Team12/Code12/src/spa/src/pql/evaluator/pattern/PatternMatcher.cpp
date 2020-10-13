/**
 * Implementation of Pattern Matcher for Query Evaluator,
 * used to evaluate Pattern clauses in queries
 * given to SIMPLE Program Analyser.
 */

#include "PatternMatcher.h"

#include <stdexcept>

#include "AssignMatcher.h"
#include "IfMatcher.h"
#include "WhileMatcher.h"

std::unordered_map<PatternStatementType, auto (*)(PatternClause*, ResultsTable*)->void> getPatternMatcherMap()
{
    return std::unordered_map<PatternStatementType, auto (*)(PatternClause*, ResultsTable*)->void>(
        {{AssignPatternType, evaluateAssignPattern},
         {IfPatternType, evaluateIfPattern},
         {WhilePatternType, evaluateWhilePattern}});
}

Void evaluatePattern(PatternClause* pnClause, ResultsTable* resultsTable)
{
    PatternStatementType pnClauseType = pnClause->getStatementType();
    std::unordered_map<PatternStatementType, auto (*)(PatternClause*, ResultsTable*)->void> matcherMap
        = getPatternMatcherMap();
    auto mapEntry = matcherMap.find(pnClauseType);
    if (mapEntry == matcherMap.end()) {
        throw std::runtime_error("Unknown PatternClause type in evaluatePattern");
    } else {
        mapEntry->second(pnClause, resultsTable);
    }
}
