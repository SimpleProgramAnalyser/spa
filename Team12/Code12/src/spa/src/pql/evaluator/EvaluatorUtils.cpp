/**
 * Implementation of common methods used by the Query Evaluator.
 */
#include "EvaluatorUtils.h"

#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

std::size_t NtupleHasher::operator()(const Vector<String>& tuple) const
{
    if (tuple.empty()) {
        return 0;
    } else {
        std::hash<std::string> stringHasher;
        std::size_t hashedValues = stringHasher(tuple[0]);
        std::size_t length = tuple.size();
        for (std::size_t i = 1; i < length; i++) {
            const std::string& value = tuple[i];
            hashedValues = (hashedValues
                            ^ (stringHasher(value) + uint32_t(2654435769) + (hashedValues * 64) + (hashedValues / 4)));
        }
        return hashedValues;
    }
}

PotentialValue::PotentialValue(Synonym synonym, String value): synonym(std::move(synonym)), value(std::move(value)) {}

PotentialValue::PotentialValue(const SynonymWithValue& swv): synonym(swv.synonym), value(swv.value) {}

PotentialValue::operator SynonymWithValue() const
{
    return SynonymWithValue(synonym, value);
}

bool PotentialValue::operator==(const PotentialValue& pv) const
{
    return this->synonym == pv.synonym && this->value == pv.value;
}

bool PotentialValue::operator<(const PotentialValue& pv) const
{
    if (this->synonym == pv.synonym) {
        return this->value < pv.value;
    }
    return this->synonym < pv.synonym;
}

SynonymWithValue PotentialValue::asSwv() const
{
    return SynonymWithValue(*this);
}

std::size_t PotentialValueHasher::operator()(const PotentialValue& pv) const
{
    std::hash<std::string> stringHasher;
    std::size_t hashedSynonym = stringHasher(pv.synonym);
    return (hashedSynonym
            ^ (stringHasher(pv.value) + uint32_t(2654435769) + (hashedSynonym * 64) + (hashedSynonym / 4)));
}

SynonymWithValue::SynonymWithValue(Synonym synonym, String value): synonym(std::move(synonym)), value(std::move(value))
{}

SynonymWithValue::operator PotentialValue() const
{
    return PotentialValue(synonym, value);
}

bool SynonymWithValue::operator==(const SynonymWithValue& swv) const
{
    return this->synonym == swv.synonym;
}

PotentialValue SynonymWithValue::asPv() const
{
    return PotentialValue(*this);
}

std::size_t SynonymWithValueHasher::operator()(const SynonymWithValue& swv) const
{
    return std::hash<std::string>()(swv.synonym);
}

ResultsRelation::ResultsRelation(String v1, String v2): value1(std::move(v1)), value2(std::move(v2)) {}

bool ResultsRelation::operator==(const ResultsRelation& rr) const
{
    return this->value1 == rr.value1 && this->value2 == rr.value2;
}

std::size_t ResultsRelationHasher::operator()(const ResultsRelation& rr) const
{
    std::hash<std::string> stringHasher;
    std::size_t hashedA = stringHasher(rr.value1);
    return (hashedA ^ (stringHasher(rr.value2) + uint32_t(2654435769) + (hashedA * 64) + (hashedA / 4)));
}

std::size_t IntegerPairHasher::operator()(const Pair<Integer, Integer>& intPair) const
{
    std::hash<Integer> intHasher;
    std::size_t hashedFirst = intHasher(intPair.first);
    return (hashedFirst ^ (intHasher(intPair.second) + uint32_t(2654435769) + (hashedFirst * 64) + (hashedFirst / 4)));
}

ClauseResult convertToClauseResult(const Vector<Integer>& intList)
{
    ClauseResult strList;
    for (Integer i : intList) {
        strList.push_back(std::to_string(i));
    }
    return strList;
}

PairedResult convertToPairedResult(const Vector<Pair<Integer, Integer>>& intPairsList)
{
    PairedResult strPairsList;
    for (const Pair<Integer, Integer>& p : intPairsList) {
        strPairsList.push_back(std::make_pair(std::to_string(p.first), std::to_string(p.second)));
    }
    return strPairsList;
}

PairedResult convertToPairedResult(const Vector<Pair<String, Integer>>& intPairsList)
{
    PairedResult strPairsList;
    for (const Pair<String, Integer>& p : intPairsList) {
        strPairsList.push_back(std::make_pair(p.first, std::to_string(p.second)));
    }
    return strPairsList;
}

PairedResult convertToPairedResult(const Vector<Pair<Integer, String>>& intPairsList)
{
    PairedResult strPairsList;
    for (const Pair<Integer, String>& p : intPairsList) {
        strPairsList.push_back(std::make_pair(std::to_string(p.first), p.second));
    }
    return strPairsList;
}

std::unordered_map<DesignEntityType, StatementType> getStatementTypesMap()
{
    std::unordered_map<DesignEntityType, StatementType> queryPkbTypesMap
        = {{StmtType, AnyStatement},          {ReadType, ReadStatement},    {PrintType, PrintStatement},
           {CallType, CallStatement},         {WhileType, WhileStatement},  {IfType, IfStatement},
           {AssignType, AssignmentStatement}, {Prog_LineType, AnyStatement}};
    return queryPkbTypesMap;
}

StatementType mapToStatementType(DesignEntityType entType)
{
    std::unordered_map<DesignEntityType, StatementType> queryPkbTypesMap = getStatementTypesMap();
    if (queryPkbTypesMap.find(entType) != queryPkbTypesMap.end()) {
        return queryPkbTypesMap[entType];
    } else {
        throw std::runtime_error("Wrong entity type in mapToStatementType");
    }
}

ClauseResult removeDuplicates(const ClauseResult& list)
{
    std::unordered_set<String> resultsFromList;
    std::copy(list.begin(), list.end(), std::inserter(resultsFromList, resultsFromList.end()));
    return std::vector<String>(resultsFromList.begin(), resultsFromList.end());
}
