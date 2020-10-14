/**
 * Implementation of the PQL query evaluator.
 */

#include "Evaluator.h"

#include <stdexcept>
#include <utility>

#include "attribute/WithQualifier.h"
#include "pattern/PatternMatcher.h"
#include "relationships/AffectsEvaluator.h"
#include "relationships/NextEvaluator.h"
#include "relationships/SuchThatEvaluator.h"

/**
 * Converts a paired result to a vector of strings, where
 * each string is a tuple with elements separated by spaces.
 *
 * Example: Select <a, p> ... returns {{"a", "b"}, {"c", "d"}}
 * This method will return {"a b", "c d"}.
 *
 * @param resultPairs The pairs of results to be converted.
 * @return A vector of strings with each tuple as a single string.
 */
Vector<String> convertToTupleString(const PairedResult& resultPairs)
{
    std::string delimiter = " ";
    Vector<String> tupleStrings;
    for (const std::pair<std::string, std::string>& result : resultPairs) {
        tupleStrings.push_back(result.first + delimiter + result.second);
    }
    return tupleStrings;
}

RawQueryResult evaluateQuery(const AbstractQuery& abstractQuery)
{
    Evaluator evaluator(abstractQuery);
    return evaluator.evaluateQuery();
}

Evaluator::Evaluator(const AbstractQuery& abstractQuery):
    query(abstractQuery), resultsTable{abstractQuery.getDeclarationTable()}
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
    // initiate Affects and Next evaluators
    resultsTable.manageEvaluator(new NextEvaluator(resultsTable));
    resultsTable.manageEvaluator(new AffectsEvaluator(resultsTable));
    const ClauseVector& clauses = query.getClauses();
    for (int i = 0; i < clauses.count(); i++) {
        Clause* clause = clauses.get(i);
        evaluateClause(clause);
        if (!resultsTable.hasResults()) {
            /*
             * If one clause yields no results, then we can conclude
             * immediately, that this query returns no results.
             *
             * As such, we don't have to continue processing
             * the rest of the query.
             */
            break;
        }
    }
    // call the result table to return the final result
    return evaluateSelectSynonym();
}

RawQueryResult Evaluator::evaluateSelectSynonym()
{
    Vector<ResultSynonym> selectedSynonyms = query.getSelectSynonym();
    switch (selectedSynonyms.size()) {
    case 0:
        return RawQueryResult(Vector<String>({resultsTable.getResultsZero() ? "TRUE" : "FALSE"}));
    case 1:
        return RawQueryResult(resultsTable.getResultsOne(selectedSynonyms[0].getSynonym()));
    case 2:
        return RawQueryResult(convertToTupleString(
            resultsTable.getResultsTwo(selectedSynonyms[0].getSynonym(), selectedSynonyms[1].getSynonym())));
    default:
        // TODO
        return RawQueryResult::getSyntaxError("TODO LOL");
    }
}

void evaluateAndCastSuchThat(Clause* cl, ResultsTable* resultsTable)
{
    // NOLINTNEXTLINE
    evaluateSuchThat(static_cast<SuchThatClause*>(cl), resultsTable);
}

void evaluateAndCastPattern(Clause* cl, ResultsTable* resultsTable)
{
    // NOLINTNEXTLINE
    evaluatePattern(static_cast<PatternClause*>(cl), resultsTable);
}

void evaluateAndCastWith(Clause* cl, ResultsTable* resultsTable)
{
    // NOLINTNEXTLINE
    evaluateWith(static_cast<WithClause*>(cl), resultsTable);
}

std::unordered_map<ClauseType, auto (*)(Clause*, ResultsTable*)->void> getClauseEvaluatorMap()
{
    return std::unordered_map<ClauseType, auto (*)(Clause*, ResultsTable*)->void>(
        {{SuchThatClauseType, evaluateAndCastSuchThat},
         {PatternClauseType, evaluateAndCastPattern},
         {WithClauseType, evaluateAndCastWith}});
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
    std::unordered_map<ClauseType, auto (*)(Clause*, ResultsTable*)->void> evaluatorMap = getClauseEvaluatorMap();
    auto mapEntry = evaluatorMap.find(type);
    if (mapEntry == evaluatorMap.end()) {
        throw std::runtime_error("Unknown clause type in evaluateClause");
    } else {
        mapEntry->second(clause, &resultsTable);
    }
}
