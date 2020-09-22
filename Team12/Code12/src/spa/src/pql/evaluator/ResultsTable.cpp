/**
 * Implementation of a results table, to hold
 * the results of evaluating each clause in
 * a SIMPLE program query.
 */

#include "ResultsTable.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "pkb/PKB.h"

/**
 * Checks if a synonym exists in the results table.
 *
 * @param syn The synonym to be checked.
 * @return True, if the synonym exists in the table.
 *         Otherwise, false.
 */
inline Boolean ResultsTable::checkIfSynonymInMap(const Synonym& syn)
{
    return resultsMap.find(syn) != resultsMap.end();
}

/**
 * Associates a synonym with a set of results
 * in the table, assuming results are non-empty.
 *
 * @param syn The synonym.
 * @param results The results to associate with.
 */
void ResultsTable::filterAfterVerification(const Synonym& syn, const ClauseResult& results)
{
    assert(!results.empty()); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    if (checkIfSynonymInMap(syn)) {
        resultsMap[syn] = findCommonElements(results, syn);
    } else {
        // synonym is not found in table, associate results with synonym
        resultsMap.insert(std::make_pair(syn, removeDuplicates(results)));
    }
}

/**
 * Given a new clause results, find common elements with
 * the new clause results and the old results for the
 * synonym already registered in the table.
 *
 * This method assumes the synonym is already in
 * the table. If the synonym is not, there may
 * be a runtime error!
 *
 * @param newResults The first clause result.
 * @param syn The synonym for the result.
 * @return A single list of results.
 */
ClauseResult ResultsTable::findCommonElements(const ClauseResult& newResults, const Synonym& synonym)
{
    // initiate set of elements from first list
    std::unordered_set<String> newResultsSet;
    std::copy(newResults.begin(), newResults.end(), std::inserter(newResultsSet, newResultsSet.end()));
    // initiate set to contain elements found in both
    std::unordered_set<String> resultsFoundInBoth;
    // loop through elements from previous results
    for (const String& str : resultsMap[synonym]) {
        if (newResultsSet.find(str) == newResultsSet.end()) {
            // element from old results is not in newResults
            // we try to remove relationships for this element
            relationships->deleteFromGraph(PotentialValue(synonym, str), this);
        } else {
            // element is found in newResults!
            resultsFoundInBoth.insert(str);
        }
    }
    return std::vector<String>(resultsFoundInBoth.begin(), resultsFoundInBoth.end());
}

// set hasResult to true at the start, since no clauses have been evaluated
ResultsTable::ResultsTable(DeclarationTable decls):
    declarations(std::move(decls)), relationships(std::unique_ptr<RelationshipsGraph>(new RelationshipsGraph())),
    hasResult(true)
{}

bool ResultsTable::operator==(const ResultsTable& rt)
{
    return this->resultsMap == rt.resultsMap && this->declarations == rt.declarations
           && this->relationships == rt.relationships && this->hasResult == rt.hasResult;
}

void ResultsTable::filterTable(const Reference& ref, const ClauseResult& results)
{
    // if results are empty, invalidate the entire results table
    if (results.empty()) {
        hasResult = false;
        return;
    }
    // check if reference is a synonym or not
    if (ref.getReferenceType() != SynonymRefType) {
        return;
    }
    // else we get the synonym and store it in the table
    filterAfterVerification(ref.getValue(), results);
}

void ResultsTable::filterTable(const Synonym& syn, const ClauseResult& results)
{
    // if results are empty, invalidate the entire results table
    if (results.empty()) {
        hasResult = false;
        return;
    }
    filterAfterVerification(syn, results);
}

ClauseResult ResultsTable::get(const Synonym& syn)
{
    if (!hasResults()) {
        // table is marked as having no results
        return std::vector<String>();
    } else if (checkIfSynonymInMap(syn)) {
        return resultsMap[syn];
    } else {
        return retrieveAllMatching(getTypeOfSynonym(syn));
    }
}

DesignEntityType ResultsTable::getTypeOfSynonym(const Synonym& syn)
{
    return declarations.getDesignEntityOfSynonym(syn).getType();
}

Boolean ResultsTable::hasResults() const
{
    return hasResult;
}

Boolean ResultsTable::checkIfSynonymHasConstraints(const Synonym& syn)
{
    if (!hasResults()) {
        // there are no results, so by the specification of
        // this method, the synonym is restricted
        return true;
    } else {
        return checkIfSynonymInMap(syn);
    }
}

void ResultsTable::eliminatePotentialValue(const Synonym& synonym, const String& value)
{
    if (resultsMap.find(synonym) != resultsMap.end()) {
        ClauseResult& rawResultsForSynonym = resultsMap[synonym];
        auto position = std::find(rawResultsForSynonym.begin(), rawResultsForSynonym.end(), value);
        if (position != rawResultsForSynonym.end()) {
            rawResultsForSynonym.erase(position);
        }
    }
}

void ResultsTable::associateRelationships(Vector<Pair<String, String>> valueRelationships, const Reference& leftRef,
                                          const Reference& rightRef)
{
    // short-circuit if relationships are empty, or refs are not synonyms
    if (valueRelationships.empty() || leftRef.getReferenceType() != SynonymRefType
        || rightRef.getReferenceType() != SynonymRefType) {
        return;
    }
    this->relationships->insertRelationships(std::move(valueRelationships), leftRef.getValue(), rightRef.getValue());
}

void ResultsTable::associateRelationships(Vector<Pair<String, Integer>> valueRelationships, const Reference& leftRef,
                                          const Reference& rightRef)
{
    // short-circuit if relationships are empty, or refs are not synonyms
    if (valueRelationships.empty() || leftRef.getReferenceType() != SynonymRefType
        || rightRef.getReferenceType() != SynonymRefType) {
        return;
    }
    this->relationships->insertRelationships(std::move(valueRelationships), leftRef.getValue(), rightRef.getValue());
}

void ResultsTable::associateRelationships(Vector<Pair<Integer, String>> valueRelationships, const Reference& leftRef,
                                          const Reference& rightRef)
{
    // short-circuit if relationships are empty, or refs are not synonyms
    if (valueRelationships.empty() || leftRef.getReferenceType() != SynonymRefType
        || rightRef.getReferenceType() != SynonymRefType) {
        return;
    }
    this->relationships->insertRelationships(std::move(valueRelationships), leftRef.getValue(), rightRef.getValue());
}

void ResultsTable::associateRelationships(Vector<Pair<Integer, Integer>> valueRelationships, const Reference& leftRef,
                                          const Reference& rightRef)
{
    // short-circuit if relationships are empty, or refs are not synonyms
    if (valueRelationships.empty() || leftRef.getReferenceType() != SynonymRefType
        || rightRef.getReferenceType() != SynonymRefType) {
        return;
    }
    this->relationships->insertRelationships(std::move(valueRelationships), leftRef.getValue(), rightRef.getValue());
}

Boolean ResultsTable::checkIfHaveRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym)
{
    if (relationships->checkCachedRelationships(leftSynonym, rightSynonym)) {
        return true;
    }
    ClauseResult resultsForLeft = get(leftSynonym);
    ClauseResult resultsForRight = get(rightSynonym);
    for (const String& leftResult : resultsForLeft) {
        for (const String& rightResult : resultsForRight) {
            if (relationships->checkIfRelated(PotentialValue(leftSynonym, leftResult),
                                              PotentialValue(rightSynonym, rightResult))) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::pair<String, String>> ResultsTable::getRelationships(const Synonym& leftSynonym,
                                                                      const Synonym& rightSynonym)
{
    ClauseResult resultsForLeft = get(leftSynonym);
    std::vector<std::pair<String, String>> relationshipsList;
    for (const String& value : resultsForLeft) {
        std::vector<PotentialValue> relatedValues
            = relationships->retrieveRelationships(PotentialValue(leftSynonym, value));
        for (const PotentialValue& pv : relatedValues) {
            if (pv.synonym == rightSynonym) {
                relationshipsList.emplace_back(value, pv.value);
            }
            // if pv is not for right synonym, ignore
        }
    }
    return relationshipsList;
}

ClauseResult retrieveAllMatching(DesignEntityType entTypeOfSynonym)
{
    ClauseResult results;
    if (isStatementDesignEntity(entTypeOfSynonym)) {
        results = convertToClauseResult(getAllStatements(mapToStatementType(entTypeOfSynonym)));
    } else if (entTypeOfSynonym == VariableType) {
        results = getAllVariables();
    } else if (entTypeOfSynonym == ProcedureType) {
        results = getAllProcedures();
    } else if (entTypeOfSynonym == ConstantType) {
        results = convertToClauseResult(getAllConstants());
    } else {
        throw std::runtime_error("Unknown DesignEntityType in retrieveAllMatching");
    }
    return results;
}
