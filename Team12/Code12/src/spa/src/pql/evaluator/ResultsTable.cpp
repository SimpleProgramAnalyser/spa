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
#include "relationships/NextEvaluator.h"
#include "relationships/affects/AffectsEvaluator.h"

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
    ResultsSet resultsSet;
    if (checkIfSynonymInMap(syn)) {
        resultsSet = findCommonElements(results, syn);
        resultsMap[syn] = resultsSet;
    } else {
        // synonym is not found in table, associate results with synonym
        std::copy(results.begin(), results.end(), std::inserter(resultsSet, resultsSet.end()));
        resultsMap.insert(std::make_pair(syn, resultsSet));
    }
    hasResult = !resultsSet.empty();
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
 * Given a vector of synonym which may contain duplicates,
 * iterate through the possible results and based on the
 * edges in RelationshipsGraph, create tuples that match
 * the list of (non-unique) synonyms.
 *
 * This method is more general than RelationshipsGraph::
 * retrieveUniqueRowsMatching, but it is less efficient,
 * as retrieveUniqueRowsMatching has an asymptotic time
 * complexity linear to the number of edges in the graph,
 * while this method calculateMatchingTuples potentially
 * runs in exponential time in the worst case.
 *
 * The worst case is when all synonyms are the same, in which
 * calculateMatchingTuples will generate multiple Cartesian
 * products and be O((number of results) ^ n) where n is the
 * total number of synonyms in the vector of synonyms.
 *
 * @param synonyms The synonyms to retrieve the rows of.
 * @return The result n-tuples for (syns[0], syns[1], ..., syns[n]).
 */
NtupledResult ResultsTable::calculateMatchingTuples(const Vector<Synonym>& synonyms)
{
    if (synonyms.size() < 2) {
        return NtupledResult();
    }

    // a hash map to store the tuples
    std::unordered_map<Integer, Vector<String>> tuples;
    Integer tupleIndex = 0;
    // map to store the previous synonym results, and which tuple it relates to
    auto* previousResultsMap = new std::unordered_map<String, std::vector<Integer>>();
    // map to store the current synonym results, and which tuple it relates to
    auto* currentResultsMap = new std::unordered_map<String, std::vector<Integer>>();

    // first run, get the relationships for the first two synonyms
    Vector<Pair<String, String>> firstAndSecondPairs = getResultsTwo(synonyms[0], synonyms[1]);
    for (const Pair<String, String>& pair : firstAndSecondPairs) {
        Vector<String> newTuple;
        newTuple.push_back(pair.first);
        newTuple.push_back(pair.second);
        tuples.insert(std::pair<Integer, Vector<String>>(tupleIndex, newTuple));
        if (previousResultsMap->find(pair.second) == previousResultsMap->end()) {
            previousResultsMap->insert(std::pair<String, std::vector<Integer>>(pair.second, std::vector<Integer>()));
        }
        (*previousResultsMap)[pair.second].push_back(tupleIndex);
        tupleIndex++;
    }

    size_t length = synonyms.size();
    for (size_t i = 1; i < length - 1; i++) {
        Vector<Pair<String, String>> pairs = getResultsTwo(synonyms[i], synonyms[i + 1]);
        // keep track of results that we never see,
        // so we can remove them at the end
        std::unordered_set<String> prevResultsNotYetSeen;
        // indexes to delete at the end
        std::unordered_set<Integer> tuplesToDelete;
        for (const std::pair<String, std::vector<Integer>> prevResult : *previousResultsMap) {
            prevResultsNotYetSeen.insert(prevResult.first);
        }
        for (const Pair<String, String>& p : pairs) {
            auto prevResultPosition = previousResultsMap->find(p.first);
            if (prevResultPosition != previousResultsMap->end()) {
                // add p.second to p.first's tuples
                for (Integer index : prevResultPosition->second) {
                    // copy over old tuple
                    tuplesToDelete.insert(index);
                    tuples[tupleIndex] = tuples[index];
                    tuples[tupleIndex].push_back(p.second);
                    // store the new tuple for current synonym
                    if (currentResultsMap->find(p.second) == currentResultsMap->end()) {
                        currentResultsMap->insert(
                            std::pair<String, std::vector<Integer>>(p.second, std::vector<Integer>()));
                    }
                    (*currentResultsMap)[p.second].push_back(tupleIndex);
                    tupleIndex++;
                }
                prevResultsNotYetSeen.erase(p.first);
            }
        }
        // remove tuples for values that were not seen in pairs
        for (const String& unseenValue : prevResultsNotYetSeen) {
            for (Integer index : (*previousResultsMap)[unseenValue]) {
                tuples.erase(index);
            }
        }
        // remove old tuples with one less synonym result
        for (Integer index : tuplesToDelete) {
            tuples.erase(index);
        }
        // swap previousResultMap and currentResultMap for next pair
        std::unordered_map<String, std::vector<Integer>>* tempMapPtr = previousResultsMap;
        previousResultsMap = currentResultsMap;
        currentResultsMap = tempMapPtr;
        currentResultsMap->clear();
    }
    delete previousResultsMap;
    delete currentResultsMap;
    NtupledResult tuplesVector;
    for (const std::pair<Integer, Vector<String>> indexTuplePair : tuples) {
        tuplesVector.push_back(indexTuplePair.second);
    }
    return tuplesVector;
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
    bool allDifferent = true;
    std::unordered_set<std::string> seenSynonyms;
    for (size_t i = 0; i < length - 1; i++) {
        Synonym firstSyn = syns[i];
        Synonym secondSyn = syns[i + 1];
        if (!hasRelationships(firstSyn, secondSyn) && firstSyn != secondSyn) {
            bool firstSynNewInGraph = !relationships->hasSeenBefore(firstSyn);
            bool secondSynNewInGraph = !relationships->hasSeenBefore(secondSyn);
            Vector<Pair<String, String>> tuples
                = generateCartesianProduct(getResultsOne(firstSyn), getResultsOne(secondSyn));
            // do joining to combine the tables
            relationships->insertRelationships(tuples, firstSyn, firstSynNewInGraph, secondSyn, secondSynNewInGraph);
        } else if (firstSyn == secondSyn || seenSynonyms.find(secondSyn) != seenSynonyms.end()) {
            allDifferent = false;
        }
        seenSynonyms.insert(firstSyn);
        seenSynonyms.insert(secondSyn);
    }
    return allDifferent ? relationships->retrieveUniqueRowsMatching(syns) : calculateMatchingTuples(syns);
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
    Boolean filterS2First = false;
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
        // check for cases where one synonym is singular in results
        // table, but no relationships in the relationships graph
        Boolean s1IsSingular = table->checkIfSynonymInMap(s1) && s1IsNew;
        Boolean s2IsSingular = table->checkIfSynonymInMap(s2) && s2IsNew;
        // put the one that does not exist in the results table in first
        filterS2First = s1IsSingular && !s2IsSingular;
    }
    if (syn1Results.empty() || syn2Results.empty()) {
        table->hasResult = false;
    } else if (filterS2First) {
        table->filterAfterVerification(s2, syn2Results);
        table->filterAfterVerification(s1, syn1Results);
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
    if (!hasResults()) {
        return std::vector<std::vector<String>>();
    } else {
        return joinAllSynonyms(syns);
    }
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
