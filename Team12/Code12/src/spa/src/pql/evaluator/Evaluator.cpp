/**
 * Implementation of the PQL query evaluator.
 */

#include "Evaluator.h"

#include <sstream>
#include <stdexcept>
#include <utility>

#include "attribute/AttributeMap.h"
#include "attribute/WithUnifier.h"
#include "pattern/PatternMatcher.h"
#include "relationships/NextEvaluator.h"
#include "relationships/SuchThatEvaluator.h"
#include "relationships/affects/AffectsEvaluator.h"

Vector<String> convertToTupleString(const PairedResult& resultPairs)
{
    std::string delimiter = " ";
    Vector<String> tupleStrings;
    for (const std::pair<std::string, std::string>& result : resultPairs) {
        tupleStrings.push_back(result.first + delimiter + result.second);
    }
    return tupleStrings;
}

Vector<String> convertToTupleString(const NtupledResult& resultTuples)
{
    std::string delimiter = " ";
    Vector<String> tupleStrings;
    for (const std::vector<std::string>& tuple : resultTuples) {
        std::ostringstream stringStream;
        size_t length = tuple.size();
        for (size_t i = 0; i + 1 < length; i++) {
            stringStream << tuple[i] << delimiter;
        }
        if (length > 0) {
            stringStream << tuple[length - 1];
        }
        tupleStrings.emplace_back(stringStream.str());
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
     * If invalid and Select BOOLEAN, we return FALSE.
     * If invalid and not Select BOOLEAN, stop evaluating query.
     */
    if (query.toReturnFalseResult()) {
        return RawQueryResult::getFalseResultWithSemanticError(query.getErrorMessage());
    } else if (query.isSyntacticallyInvalid()) {
        return RawQueryResult::getSyntaxError(query.getErrorMessage());
    } else if (query.isSemanticallyInvalid()) {
        return RawQueryResult::getSemanticError(query.getErrorMessage());
    }
    return evaluateValidQuery();
}

/*
 * Processes a PQL query and interacts with PKB if needed,
 * to obtain the results to a query that was determined
 * to be syntactically and semantically valid.
 *
 * @return The list of query results for the Select synonym.
 */
RawQueryResult Evaluator::evaluateValidQuery()
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
    Vector<ResultSynonym> selectedSynonyms = query.getSelectedSynonyms();
    Vector<String> resultsWithAttributes;
    switch (selectedSynonyms.size()) {
    case 0: {
        resultsWithAttributes = Vector<String>({resultsTable.getResultsZero() ? "TRUE" : "FALSE"});
        break;
    }
    case 1: {
        ClauseResult resultsForSynonym = resultsTable.getResultsOne(selectedSynonyms[0].getSynonym());
        resultsWithAttributes = mapAttributesOne(resultsTable, resultsForSynonym, selectedSynonyms[0]);
        break;
    }
    case 2: {
        PairedResult resultsForSynonym
            = resultsTable.getResultsTwo(selectedSynonyms[0].getSynonym(), selectedSynonyms[1].getSynonym());
        resultsWithAttributes = convertToTupleString(
            mapAttributesTwo(resultsTable, resultsForSynonym, selectedSynonyms[0], selectedSynonyms[1]));
        break;
    }
    default: {
        Vector<Synonym> synonymsList;
        for (const ResultSynonym& rs : selectedSynonyms) {
            synonymsList.push_back(rs.getSynonym());
        }
        NtupledResult resultsForSynonym = resultsTable.getResultsN(synonymsList);
        resultsWithAttributes = convertToTupleString(mapAttributesN(resultsTable, resultsForSynonym, selectedSynonyms));
    }
    }
    return RawQueryResult(std::move(resultsWithAttributes));
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
