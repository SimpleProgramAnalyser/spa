/**
 * Implementation of the PQL query evaluator.
 */

#include "Evaluator.h"

#include <stdexcept>

#include "PatternMatcher.h"
#include "SuchThatEvaluator.h"

RawQueryResult evaluateQuery(const AbstractQuery& abstractQuery)
{
    Evaluator evaluator(abstractQuery);
    return evaluator.evaluateQuery();
}

Evaluator::Evaluator(const AbstractQuery& abstractQuery):
    query(abstractQuery), resultsTable(ResultsTable(abstractQuery.getDeclarationTable()))
{}

RawQueryResult Evaluator::evaluateQuery()
{
    /*
     * First check if PQL query is semantically valid.
     * If invalid, don't continue evaluating query.
     */
    if (query.isInvalid()) {
        return RawQueryResult::getSyntaxError(
            "ERROR CODE 3735929054: PQL was not parsed. SIGSYNTAX obtained. This incident will be reported.");
    }
    return evaluateSyntacticallyValidQuery();
}

/*
 * Processes a PQL query and interacts with PKB if needed,
 * to obtain the results to a query that was determined
 * to be syntactically valid.
 *
 * @return The list of query results for the Select synonym.
 */
RawQueryResult Evaluator::evaluateSyntacticallyValidQuery()
{
    ClauseVector clauses = query.getClauses();
    for (int i = 0; i < clauses.count(); i) {
        Clause* clause = clauses.get(i);
        evaluateClause(clause);
    }
    // call the result table to return the final result
    return RawQueryResult(resultsTable.get(query.getSelectSynonym()));
}

/*
 * Processes a single clause in a PQL query, with respect to
 * a given synonym. All results obtained from the clauses will be
 * stored in the results table of Evaluator.
 *
 * @param clause The clause to evaluate.
 */
Void Evaluator::evaluateClause(Clause* clause)
{
    ClauseType type = clause->getType();
    if (type == SuchThatClauseType) {
        // NOLINTNEXTLINE
        return evaluateSuchThat(static_cast<SuchThatClause*>(clause), &resultsTable);
    } else if (type == PatternClauseType) {
        // NOLINTNEXTLINE
        return evaluatePattern(static_cast<PatternClause*>(clause), &resultsTable);
    } else {
        throw std::runtime_error("Unknown clause type in evaluateClause");
    }
}
