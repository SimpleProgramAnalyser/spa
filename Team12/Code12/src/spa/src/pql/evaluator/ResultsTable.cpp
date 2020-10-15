/**
 * Implementation of a results table, to hold
 * the results of evaluating each clause in
 * a SIMPLE program query.
 */

#include "ResultsTable.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "pkb/PKB.h"
#include "relationships/AffectsEvaluator.h"
#include "relationships/NextEvaluator.h"

/**
 * A method to compare two vectors, to see whether
 * they have the same elements regardless of order.
 *
 * @tparam T Type of elements in the vector. Elements
 *           must implement the method
 *           bool cmp(const T&, const T&).
 */
template <typename T>
bool doVectorsHaveSameElements(std::vector<T> vector1, std::vector<T> vector2)
{
    std::sort(vector1.begin(), vector1.end());
    std::sort(vector2.begin(), vector2.end());
    return vector1 == vector2;
}

/**
 * Given two result lists, generate all possible pairs between
 * the two result lists by doing a Cartesian product.
 *
 * @param results1 The first result list.
 * @param results2 The second result list.
 * @return A list of pairs (first, second).
 */
PairedResult generateCartesianProduct(const ClauseResult& results1, const ClauseResult& results2)
{
    std::vector<std::pair<String, String>> tuples;
    for (const String& res1 : results1) {
        for (const String& res2 : results2) {
            tuples.emplace_back(res1, res2);
        }
    }
    return tuples;
}

/**
 * Checks if a synonym exists in the results table.
 *
 * @param syn The synonym to be checked.
 * @return True, if the synonym exists in the table.
 *         Otherwise, false.
 */
inline Boolean ResultsTable::checkIfSynonymInMap(const Synonym& syn) const
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
        std::unordered_set<String> resultsSet;
        std::copy(results.begin(), results.end(), std::inserter(resultsSet, resultsSet.end()));
        hasResult = !resultsSet.empty();
        resultsMap.insert(std::make_pair(syn, resultsSet));
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
 * @return The final set of common results.
 */
ResultsSet ResultsTable::findCommonElements(const ClauseResult& newResults, const Synonym& synonym)
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
            relationships->deleteOne(PotentialValue(synonym, str), this);
        } else {
            // element is found in newResults!
            resultsFoundInBoth.insert(str);
            // remove this element from the newResultsSet
            newResultsSet.erase(str);
        }
    }
    // elements left in newResultsSet will not be in the final results
    // ensure that relationships do not exist for them
    for (const String& rejectedNewResult : newResultsSet) {
        relationships->deleteOne(PotentialValue(synonym, rejectedNewResult), this);
    }
    return resultsFoundInBoth;
}

/**
 * For a list of synonyms, check the relationships between
 * every synonym to ensure that rows across every synonym
 * exists in the table, and then return these rows.
 *
 * This method will modify the RelationshipsGraph to join
 * two tables together (cross-join), if both synonyms are
 * in the vector but are not within the same table.
 *
 * @param syns The vector of synonyms to be processed.
 * @return All edges that match the vector of synonyms.
 */
NtupledResult ResultsTable::joinAllSynonyms(const Vector<Synonym>& syns)
{
    size_t length = syns.size();
    for (size_t i = 0; i < length - 1; i++) {
        Synonym firstSyn = syns[i];
        Synonym secondSyn = syns[i + 1];
        if (!hasRelationships(firstSyn, secondSyn)) {
            bool firstSynNewInGraph = !relationships->hasSeenBefore(firstSyn);
            bool secondSynNewInGraph = !relationships->hasSeenBefore(secondSyn);
            Vector<Pair<String, String>> tuples
                = generateCartesianProduct(getResultsOne(firstSyn), getResultsOne(secondSyn));
            // do joining to combine the tables
            relationships->insertRelationships(tuples, firstSyn, firstSynNewInGraph, secondSyn, secondSynNewInGraph);
        }
    }
    return relationships->retrieveRowsMatching(syns);
}

std::function<void()> ResultsTable::createEvaluatorOne(ResultsTable* table, const Synonym& syn,
                                                       const ClauseResult& results)
{
    return [table, syn, results]() {
        mergeOneSynonym(table, syn, results);
    };
}

std::function<void()> ResultsTable::createEvaluatorTwo(ResultsTable* table, const Synonym& s1, const Synonym& s2,
                                                       const PairedResult& tuples)
{
    return [table, s1, s2, tuples]() {
        mergeTwoSynonyms(table, s1, s2, tuples);
    };
}

void ResultsTable::mergeOneSynonym(ResultsTable* table, const Synonym& syn, const ClauseResult& results)
{
    table->filterAfterVerification(syn, results);
}

void ResultsTable::mergeTwoSynonyms(ResultsTable* table, const Synonym& s1, const Synonym& s2,
                                    const PairedResult& tuples)
{
    ClauseResult syn1Results;
    ClauseResult syn2Results;
    if (table->hasRelationships(s1, s2)) {
        // past relations exist for s1 and s2 (inner join)
        std::unordered_set<ResultsRelation, ResultsRelationHasher> newRelationsSet;
        for (const std::pair<String, String>& newRelation : tuples) {
            newRelationsSet.insert(ResultsRelation(newRelation.first, newRelation.second));
        }
        std::vector<std::pair<String, String>> pastRelationsList = table->getRelationships(s1, s2);
        for (const std::pair<String, String>& pastRelationPair : pastRelationsList) {
            ResultsRelation pastRelation(pastRelationPair.first, pastRelationPair.second);
            // check if pastRelation is in newRelationsSet
            if (newRelationsSet.find(pastRelation) == newRelationsSet.end()) {
                // past relation does not exist, remove it from graph
                table->disassociateRelationships(s1, pastRelation.value1, s2, pastRelation.value2);
            } else {
                syn1Results.push_back(pastRelationPair.first);
                syn2Results.push_back(pastRelationPair.second);
            }
        }
    } else {
        Boolean s1IsNew = !table->relationships->hasSeenBefore(s1);
        Boolean s2IsNew = !table->relationships->hasSeenBefore(s2);
        // load relationships first, to see which relationships were successfully added
        Pair<Vector<String>, Vector<String>> successfulValues
            = table->relationships->insertRelationships(tuples, s1, s1IsNew, s2, s2IsNew);
        syn1Results = successfulValues.first;
        syn2Results = successfulValues.second;
    }
    if (syn1Results.empty() || syn2Results.empty()) {
        table->hasResult = false;
    } else {
        table->filterAfterVerification(s1, syn1Results);
        table->filterAfterVerification(s2, syn2Results);
    }
}

void ResultsTable::mergeResults()
{
    while (!queue.empty() && !hasEvaluated && hasResult) {
        queue.front()();
        queue.pop();
    }
    hasEvaluated = true;
}

ClauseResult ResultsTable::get(const Synonym& syn) const
{
    if (!hasResults()) {
        // table is marked as having no results
        return std::vector<String>();
    } else if (checkIfSynonymInMap(syn)) {
        ResultsSet resultsSet = resultsMap.at(syn);
        return std::vector<String>(resultsSet.begin(), resultsSet.end());
    } else {
        return retrieveAllMatching(getTypeOfSynonym(syn));
    }
}

void ResultsTable::disassociateRelationships(const Synonym& leftSyn, const String& leftValue, const Synonym& rightSyn,
                                             const String& rightValue)
{
    relationships->deleteTwo(PotentialValue(leftSyn, leftValue), PotentialValue(rightSyn, rightValue), this);
}

PairedResult ResultsTable::getRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym)
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

// set hasResult to true at the start, since no clauses have been evaluated
ResultsTable::ResultsTable(DeclarationTable decls):
    declarations(std::move(decls)), relationships(std::unique_ptr<RelationshipsGraph>(new RelationshipsGraph())),
    hasResult(true), hasEvaluated(false), affectsEvaluator(nullptr), nextEvaluator(nullptr)
{}

ResultsTable::~ResultsTable()
{
    delete affectsEvaluator;
    delete nextEvaluator;
}

std::vector<std::pair<std::string, std::vector<std::string>>>
getVectorFromResultsMap(const std::unordered_map<Synonym, ResultsSet>& resultsMap)
{
    std::vector<std::pair<std::string, std::vector<std::string>>> resultsVector;
    for (const std::pair<Synonym, ResultsSet> entry : resultsMap) {
        std::vector<std::string> resultForEntry = std::vector<String>(entry.second.begin(), entry.second.end());
        resultsVector.emplace_back(entry.first, resultForEntry);
    }
    return resultsVector;
}

bool ResultsTable::operator==(const ResultsTable& rt) const
{
    if (this->resultsMap.size() != rt.resultsMap.size()) {
        return false;
    }

    std::function<bool(std::pair<std::string, std::vector<std::string>>,
                       std::pair<std::string, std::vector<std::string>>)>
        comparator = [](const std::pair<std::string, std::vector<std::string>>& pair1,
                        const std::pair<std::string, std::vector<std::string>>& pair2) {
            return pair1.first < pair2.first;
        };

    std::vector<std::pair<std::string, std::vector<std::string>>> thisResultsList
        = getVectorFromResultsMap(this->resultsMap);
    std::vector<std::pair<std::string, std::vector<std::string>>> otherResultsList
        = getVectorFromResultsMap(rt.resultsMap);

    bool isResultsTheSame = true;
    size_t length = this->resultsMap.size();
    for (size_t i = 0; i < length; i++) {
        if (thisResultsList.at(i).first != otherResultsList.at(i).first
            || !doVectorsHaveSameElements(thisResultsList.at(i).second, otherResultsList.at(i).second)) {
            // either key or value is different
            isResultsTheSame = false;
            break;
        }
    }
    return isResultsTheSame && this->declarations == rt.declarations && *(this->relationships) == *(rt.relationships)
           && this->hasResult == rt.hasResult && this->hasEvaluated == rt.hasEvaluated;
}

RelationshipsGraph ResultsTable::getRelationshipsGraph() const
{
    return *relationships;
}

Boolean ResultsTable::hasResults() const
{
    return hasResult;
}

AffectsEvaluator* ResultsTable::getAffectsEvaluator() const
{
    return affectsEvaluator;
}

NextEvaluator* ResultsTable::getNextEvaluator() const
{
    return nextEvaluator;
}

Void ResultsTable::manageEvaluator(AffectsEvaluator* affectsEval)
{
    affectsEvaluator = affectsEval;
}

Void ResultsTable::manageEvaluator(NextEvaluator* nextEval)
{
    nextEvaluator = nextEval;
}

Void ResultsTable::eliminatePotentialValue(const Synonym& synonym, const String& value)
{
    if (resultsMap.find(synonym) != resultsMap.end()) {
        resultsMap[synonym].erase(value);
    }
}

DesignEntityType ResultsTable::getTypeOfSynonym(const Synonym& syn) const
{
    return declarations.getDesignEntityOfSynonym(syn).getType();
}

Boolean ResultsTable::doesSynonymHaveConstraints(const Synonym& syn) const
{
    if (!hasResults()) {
        // there are no results, so by the specification of
        // this method, the synonym is restricted
        return true;
    } else {
        return checkIfSynonymInMap(syn);
    }
}

Boolean ResultsTable::hasRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym) const
{
    if (!relationships->hasSeenBefore(leftSynonym) || !relationships->hasSeenBefore(rightSynonym)
        || leftSynonym == rightSynonym) {
        return false;
    }
    ClauseResult resultsForLeft = get(leftSynonym);
    // just check one potential value, as the synonym results are guaranteed
    // to be in RelationshipsGraph if both synonyms are inside the graph
    return !resultsForLeft.empty()
           && relationships->isValueRelated(PotentialValue(leftSynonym, resultsForLeft[0]), rightSynonym);
}

Boolean ResultsTable::getResultsZero()
{
    mergeResults();
    return hasResults();
}

ClauseResult ResultsTable::getResultsOne(const Synonym& syn)
{
    mergeResults();
    return this->get(syn);
}

PairedResult ResultsTable::getResultsTwo(const Synonym& syn1, const Synonym& syn2)
{
    mergeResults();
    if (!hasResults()) {
        // table is marked as having no results
        return std::vector<std::pair<String, String>>();
    } else if (hasRelationships(syn1, syn2)) {
        return getRelationships(syn1, syn2);
    } else {
        // do a Cartesian product of both result lists
        return generateCartesianProduct(this->get(syn1), this->get(syn2));
    }
}

NtupledResult ResultsTable::getResultsN(const Vector<Synonym>& syns)
{
    assert(syns.size() > 1); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    mergeResults();
    return joinAllSynonyms(syns);
}

Void ResultsTable::storeResultsZero(Boolean hasResults)
{
    hasResult = hasResults;
}

Void ResultsTable::storeResultsOne(const Synonym& syn, const ClauseResult& res)
{
    if (res.empty()) {
        // if results are empty, invalidate the entire results table
        hasResult = false;
    } else {
        // store the synonym in the evaluator queue
        queue.push(createEvaluatorOne(this, syn, res));
    }
}

Void ResultsTable::storeResultsOne(const Reference& rfc, const ClauseResult& res)
{
    // check if reference is a synonym or not
    if (rfc.getReferenceType() == SynonymRefType) {
        // we get the synonym
        storeResultsOne(rfc.getValue(), res);
    } else if (res.empty()) {
        // if not synonym, just check empty or not
        hasResult = false;
    }
}

Void ResultsTable::storeResultsTwo(const Reference& rfc1, const ClauseResult& res1, const Reference& rfc2,
                                   const ClauseResult& res2, const PairedResult& tuples)
{
    if (tuples.empty()) {
        // short-circuit if tuples are empty
        hasResult = false;
    } else if (rfc1.getReferenceType() != SynonymRefType) {
        // ignore reference 1
        storeResultsOne(rfc2, res2);
    } else if (rfc2.getReferenceType() != SynonymRefType) {
        // ignore reference 2
        storeResultsOne(rfc1, res1);
    } else {
        Synonym s1 = rfc1.getValue();
        Synonym s2 = rfc2.getValue();
        if (s1 == s2) {
            // pairs between same synonym, so we just store one result
            storeResultsOne(s1, res1);
        } else {
            queue.push(createEvaluatorTwo(this, s1, s2, tuples));
        }
    }
}

Void ResultsTable::storeResultsTwo(const Synonym& syn, const ClauseResult& resSyn, const Reference& ref,
                                   const PairedResult& tuples)
{
    if (tuples.empty()) {
        // short-circuit if tuples are empty
        hasResult = false;
    } else if (ref.getReferenceType() != SynonymRefType) {
        // ignore the reference
        storeResultsOne(syn, resSyn);
    } else {
        queue.push(createEvaluatorTwo(this, syn, ref.getValue(), tuples));
    }
}

Void ResultsTable::storeResultsTwo(const Synonym& syn1, const Synonym& syn2, const PairedResult& tuples)
{
    if (tuples.empty()) {
        // short-circuit if tuples are empty
        hasResult = false;
    } else {
        queue.push(createEvaluatorTwo(this, syn1, syn2, tuples));
    }
}

// TODO: Hash table
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
