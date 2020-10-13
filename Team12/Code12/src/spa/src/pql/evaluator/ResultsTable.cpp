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

// Helper class to merge pairs of strings
class ResultsRelation {
public:
    String value1;
    String value2;

    ResultsRelation(String v1, String v2): value1(std::move(v1)), value2(std::move(v2)) {}
    bool operator==(const ResultsRelation& rr) const
    {
        return this->value1 == rr.value1 && this->value2 == rr.value2;
    }
};

// Hash function for ResultsRelation
struct ResultsRelationHasher {
    std::size_t operator()(const ResultsRelation& rr) const
    {
        std::hash<std::string> stringHasher;
        std::size_t hashedA = stringHasher(rr.value1);
        return (hashedA ^ (stringHasher(rr.value2) + uint32_t(2654435769) + (hashedA * 64) + (hashedA / 4)));
    }
};

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

ClauseResult ResultsTable::get(const Synonym& syn)
{
    if (!hasResults()) {
        // table is marked as having no results
        return std::vector<String>();
    } else if (checkIfSynonymInMap(syn)) {
        ResultsSet resultsSet = resultsMap[syn];
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
    return affectsEvaluator.get();
}

NextEvaluator* ResultsTable::getNextEvaluator() const
{
    return nextEvaluator.get();
}

Void ResultsTable::manageEvaluator(AffectsEvaluator* affectsEval)
{
    affectsEvaluator = std::unique_ptr<AffectsEvaluator>(affectsEval);
}

Void ResultsTable::manageEvaluator(NextEvaluator* nextEval)
{
    nextEvaluator = std::unique_ptr<NextEvaluator>(nextEval);
}

Void ResultsTable::eliminatePotentialValue(const Synonym& synonym, const String& value)
{
    if (resultsMap.find(synonym) != resultsMap.end()) {
        resultsMap[synonym].erase(value);
    }
}

DesignEntityType ResultsTable::getTypeOfSynonym(const Synonym& syn)
{
    return declarations.getDesignEntityOfSynonym(syn).getType();
}

Boolean ResultsTable::doesSynonymHaveConstraints(const Synonym& syn)
{
    if (!hasResults()) {
        // there are no results, so by the specification of
        // this method, the synonym is restricted
        return true;
    } else {
        return checkIfSynonymInMap(syn);
    }
}

Boolean ResultsTable::hasRelationships(const Synonym& leftSynonym, const Synonym& rightSynonym)
{
    if (!relationships->hasSeenBefore(leftSynonym) || !relationships->hasSeenBefore(rightSynonym)) {
        return false;
    }
    ClauseResult resultsForLeft = get(leftSynonym);
    // just check one potential value, as the synonym results are guaranteed
    // to be in RelationshipsGraph if both synonyms are inside the graph
    return !resultsForLeft.empty()
           && relationships->isValueRelated(PotentialValue(leftSynonym, resultsForLeft[0]), rightSynonym);
}

Void ResultsTable::getResultsZero()
{
    mergeResults();
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
        return this->getRelationships(syn1, syn2);
    } else {
        std::vector<std::pair<String, String>> tuples;
        // do a Cartesian product of both result lists
        for (const String& res1 : this->get(syn1)) {
            for (const String& res2 : this->get(syn2)) {
                tuples.emplace_back(res1, res2);
            }
        }
        return tuples;
    }
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
        queue.push(createEvaluatorTwo(this, rfc1.getValue(), rfc2.getValue(), tuples));
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
